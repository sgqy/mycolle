#ifndef __GETBE_HPP__
#define __GETBE_HPP__

#include <cstdint>

class getbe {
	bool _le = false;
public:
	template<class T>
	uint8_t *conv(T *val, const int l = sizeof(T)) {
		uint8_t *ret = reinterpret_cast<uint8_t*>(val);
		const int sl = l / 2;

		if(!_le) {
			ret += (sizeof(T) - l);
			goto swap_done;
		}

		for(int i = 0; i < sl; ++i) {
			uint8_t tmp = ret[i];
			ret[i] = ret[(l-1)-i];
			ret[(l-1)-i] = tmp;
		}

	swap_done:
		return ret;
	}

	getbe() {
		uint16_t v = 0x0001;
		uint8_t *t = (uint8_t*)&v;
		if(*t == 0x01) {
			_le = true;
		} else {
			_le = false;
		}
	}
};
#endif // __GETBE_HPP__
