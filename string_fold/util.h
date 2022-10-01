#pragma once
#include<string>
#include <map>
#include <unordered_map>
void find_all_common_maximal_substring(const std::string& s1, const std::string& s2, int lb,
	std::unordered_map<std::string, std::map<int, int>>& substr_map_1,
	std::unordered_map<std::string, std::map<int, int>>& substr_map_2
);