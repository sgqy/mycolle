#ifndef __I2CDEV_HPP__
#define __I2CDEV_HPP__

#include <cstdint>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <fcntl.h>

#include "getbe.hpp"

// not thread safe
class i2cdev {
	getbe   _getbe;
	int     _fd = -1;
	uint8_t _addr = 0;
	uint8_t _reg_len = 0;
public:
	i2cdev(const char* fn, const uint8_t addr, const uint8_t reg_len,
		int timeout = 1 /* n * 10ms */, int retry = 3) {
		_addr = addr;
		_reg_len = reg_len;
		_fd = open(fn, O_RDWR);
		if(_fd < 0) {
			fprintf(stderr, "i2c: cannot open %s (%s)\n", fn, strerror(errno));
			return;
		}
		if(ioctl(_fd, I2C_TIMEOUT, timeout) < 0) {
			fprintf(stderr, "i2c: set timeout fail (%s)\n", strerror(errno));
			return;
		}
		if(ioctl(_fd, I2C_RETRIES, retry) < 0) {
			fprintf(stderr, "i2c: set retry fail (%s)\n", strerror(errno));
			return;
		}
	}
	~i2cdev() {
		if(_fd >= 0) {
			close(_fd);
		}
	}
	int read(const int reg_addr, uint8_t *buf, const uint16_t buf_len) {
		if(_fd < 0) {
			fprintf(stderr, "i2c: not init (%s)\n", strerror(errno));
			return -1;
		}

		int ret = 0;
		int reg = reg_addr;
		i2c_rdwr_ioctl_data packet;
		i2c_msg msg[2];

		msg[0].addr  = _addr;
		msg[0].flags = 0;
		msg[0].len   = _reg_len;
		msg[0].buf   = _getbe.conv(&reg, _reg_len);

		msg[1].addr  = _addr;
		msg[1].flags = I2C_M_RD;
		msg[1].len   = buf_len;
		msg[1].buf   = buf;

		packet.msgs  = msg;
		packet.nmsgs = 2;

		ret = ioctl(_fd, I2C_RDWR, (unsigned long)&packet);

		if(ret < 0) {
			fprintf(stderr, "i2c: read dev [%x] from [%x] fail (%s)\n", _addr, reg_addr, strerror(errno));
		}
		return ret;
	}
	int write(const int reg_addr, const uint8_t *buf, const uint16_t buf_len) {
		if(_fd < 0) {
			fprintf(stderr, "i2c: not init (%s)\n", strerror(errno));
			return -1;
		}

		int ret = 0;
		int reg = reg_addr;
		i2c_rdwr_ioctl_data packet;
		i2c_msg msg;

		msg.addr  = _addr;
		msg.flags = 0;
		msg.len   = _reg_len + buf_len;
		msg.buf   = new uint8_t[msg.len];
		memcpy(msg.buf, _getbe.conv(&reg, _reg_len), _reg_len);
		memcpy(msg.buf + _reg_len, buf, buf_len);

		packet.msgs  = &msg;
		packet.nmsgs = 1;

		ret = ioctl(_fd, I2C_RDWR, (unsigned long)&packet);

		if(ret < 0) {
			fprintf(stderr, "i2c: write dev [%x] to [%x] fail (%s)\n", _addr, reg_addr, strerror(errno));
		}

		delete[] msg.buf;
		return ret;
	}

};

#endif // __I2CDEV_HPP__
