#include <cstdio>
#include <cstring>
/*

f4,ff,  0
ff,ff,  1
00,00,
00,00,
00,00,
00,00,
00,00,
00,00,
90,01,  8
00,00,
00,00,
00,01,  11
00,00,
05,00,  13
4d,00,  14 -- start
69,00,
63,00,
72,00,
6f,00,
73,00,
6f,00,
66,00,
74,00,
20,00,
59,00,
61,00,
48,00,
65,00,
69,00,
20,00,
55,00,
49,00,
00,00,
00,00,
00,00,
00,00,
00,00,
00,00,
00,00,
00,00,
00,00,
00,00,
00,00,
00,00,
00,00,  44 -- end -- max 31
00,00

*/

#define LEN (46)

int main()
{
	wchar_t str[LEN] = { 0 };
	str[0] = 0xFFF4; // may be 0xFFF1
	str[1] = 0xFFFF;
	str[8] = 0x0190;
	str[11] = 0x0100;
	str[13] = 0x0005;
	
	wchar_t *name = L"NSimSun";
	wcscpy(str+14, name);
	
	for(int i = 0; i < LEN*2; ++i) {
		printf("%02x,", ((unsigned char *)str)[i]);
	}
	
	return 0;
}