#include <fstream>				// file in/out
#include <string>				// string
#include <vector>				// arr[]
#include <algorithm>				// sort
#include <vector>

#include "MD5.h"

bool load_text(std::string in_file, std::vector<std::string> *arr) {
	std::ifstream in(in_file);
	std::string url;
	bool flag = 1;
	
	puts("\nLoading text file...");
	if (in.is_open()) {
		while (!in.eof()) {
			std::getline(in, url);
			(*arr).push_back(url);
		}
		if (!memcmp(&(*arr).back(), "", 1)) (*arr).pop_back();
		flag = 0;
	}
	in.close();

	if (flag == 0)	puts("Loading complete.\n");
	else		fprintf(stderr, "error: File not found\n");
	return flag;
}

bool save_text(std::string out_file, std::vector<std::string> *arr) {
	std::ofstream out(out_file, std::ios::out);
	bool flag = 1;

	puts("\nMaking text file...");
	int i = 0;
	for (std::string url : (*arr)) {
		if (i++) out << '\n';
		out << url;
	}
	flag = 0;
	out.close();

	if (flag == 0)	puts("Making complete.\n");
	else		fprintf(stderr, "error: File not found\n");
	return flag;
}

bool create_md5(std::string in_file, std::string out_file) {
	std::ifstream in(in_file);
	std::ofstream out(out_file, std::ios::out);
	std::string url;
	bool flag = 1;

	puts("\nMaking md5 text file...");
	if (in.is_open()) {
		while (!in.eof()) {
			std::getline(in, url);
			out << md5(url) << '\n';
		}
		flag = 0;
	}
	in.close();
	out.close();

	if(flag == 0)	puts("Making complete.\n");
	else		fprintf(stderr, "error: File not found\n");
	return flag;
}

bool create_md5(const std::vector<std::string> *src_arr, std::vector<std::string> *dst_arr) {
	puts("\nConverting to md5...");
	for (std::string url : (*src_arr))
		(*dst_arr).push_back(md5(url));
	puts("Converting complete.\n");
	return 0;
}

/*
int main() {
	std::string urlfile("black_url.txt");
	std::string md5file("md5_url.txt");
	std::string sortmd5("md5_url_sort.txt");
	std::string sorturl("black_url_sort.txt");
	std::vector<std::string> black_url;
	std::vector<std::string> md5_url;

	// Save sorted URL value
//	load_text(urlfile, &black_url);
//	std::sort(black_url.begin(), black_url.end());
//	save_text(sorturl, &black_url);

	// Save sorted md5 value
	load_text(urlfile, &black_url);
	create_md5(&black_url, &md5_url);
	std::sort(md5_url.begin(), md5_url.end());
	save_text(sortmd5, &md5_url);

	return 0;
}
*/
