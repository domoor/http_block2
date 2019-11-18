#ifndef HTTP_URL_BLOCK_H
#define HTTP_URL_BLOCK_H
#define TABLESIZE		4096		// hash table size
#define half(x, y)		((x)+(y))/2

class uint128_t {
private:
	uint64_t _back;
	uint64_t _front;

public:
	uint128_t() {}
	uint128_t(uint64_t _back, uint64_t _front) : _back(_back), _front(_front) {}
	uint128_t(const uint128_t& p) { _back = p._back, _front = p._front; }
	uint64_t back() const { return _back; }
	uint64_t front() const { return _front; }

	int operator==(const uint128_t& p) {
		if (_front < p._front)
			return -1;
		else if (_front > p._front)
			return 1;

		if (_back < p._back)
			return -1;
		else if (_back > p._back)
			return 1;
		else
			return 0;
	}
};

uint16_t make_md5(std::string md5_url, uint128_t *value, int size, bool flag = false);
bool binarysearch(std::vector<uint128_t> value, uint128_t doubt_val, uint64_t size);
#endif
