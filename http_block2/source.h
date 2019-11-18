#pragma once
#include <string>
#include <vector>

bool load_text(std::string in_file, std::vector<std::string> *arr);
bool save_text(std::string out_file, std::vector<std::string> *arr);
bool create_md5(std::string in_file, std::string out_file);
bool create_md5(const std::vector<std::string> *src_arr, std::vector<std::string> *dst_arr);
