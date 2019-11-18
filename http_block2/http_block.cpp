#include <cstdint>		// uintN_t
#include <string>		// string
#include <vector>		// vector
#include "MD5.h"
#include "http_block.h"

#include <iostream>

// string_md5 -> uint128_md5 and
uint16_t make_md5(std::string md5_url, uint128_t *value, int size, bool flag) {
	char* end;

	if (flag) md5_url = md5(md5_url);
	(*value) = { strtoull(md5_url.substr(16, 16).c_str(), &end, 16),
		     strtoull(md5_url.substr(00, 16).c_str(), &end, 16) };
	return ((*value).back() >> 48) % size;
}

bool binarysearch(std::vector<uint128_t> value, uint128_t doubt_val, uint64_t size) {
	uint64_t max_p = size;
	uint64_t min_p = 0;
	uint64_t now_p = half(max_p, min_p);

	if (!size) return false; // Unmatched
	while (1) {
		int res = ((value)[now_p] == doubt_val);
		if (res == 0) {				// Matched
			return true;
		}
		else if (max_p - min_p == 1) {		// Unmatched
			return false;
		}
		else if (res > 0) {
			max_p = now_p;
			now_p = half(max_p, min_p);
		}
		else if (res < 0) {
			min_p = now_p;
			now_p = half(max_p, min_p);
		}
		else { fprintf(stderr, "error: occurred during search\n"); exit(1); }
	}
}
