#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <vector>
#include <string>
#include <algorithm>

#include <dirent.h>

std::vector<std::string> get_files(std::string name)
{
	DIR *dir = opendir(name.c_str());

	std::vector<std::string> files;
	if(dir == 0) {
		printf("*** %s error\n", name.c_str());
		return files;
	}

	dirent *entry;

	while((entry = readdir(dir)) != 0) {
		std::string en(entry->d_name);
		std::string fn = name + "/" + en;

		if(entry->d_type == DT_DIR) {
			if(en == "." || en == "..") {
				continue;
			}
			auto subfiles = get_files(fn);
			files.insert(files.end(), subfiles.begin(), subfiles.end());
		} else {
			files.push_back(fn);
		}
	}

	closedir(dir);
	return files;
}

std::vector<std::string> gen_cmd(std::vector<std::string> files)
{
	std::vector<std::string> ret;

	for(int i = 0; i < files.size(); ++i) {
		auto &f = files[i];

		auto pos = f.find_last_of('.');
		if(pos == std::string::npos) {
			continue;
		}
		auto type = f.substr(pos);

		std::string tmpf = "/var/tmp/actmp"; // mount tmpfs or ramfs first
		tmpf += std::to_string(i);
		tmpf += type;

		std::string cmd;
		cmd += "convert \'" + f + "\' -normalize \'" + tmpf + "\' && mv \'" + tmpf + "\' \'" + f + "\'";

		ret.push_back(cmd);
	}

	return ret;
}

int main(int argc, char **argv)
{
	if(argc != 2) {
		printf("%s <dir>\n", argv[0]);
		return 1;
	}
	printf("get file list...\n");
	auto files = get_files(argv[1]);
	printf("generate command...\n");
	auto cmds = gen_cmd(files);
	printf("processing command...\n");

	#pragma omp parallel for
	for(int i = 0; i < cmds.size(); ++i) {
		printf("[%d/%d] %s\n", i + 1, cmds.size(), cmds[i].c_str());
		system(cmds[i].c_str());
	}

	return 0;
}
