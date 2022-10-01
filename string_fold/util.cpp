#include "util.h"
#include <iostream>

void find_all_common_maximal_substring(const std::string& s1, const std::string& s2, int lb,
	std::unordered_map<std::string, std::map<int, int>>& substr_map_1,
	std::unordered_map<std::string, std::map<int, int>>& substr_map_2
)
{
	substr_map_1.clear();
	substr_map_2.clear();

	const size_t n1 = s1.size();
	const size_t n2 = s2.size();

	int** mat = new int* [n1];
	for (size_t i = 0; i < n1; ++i)
	{
		mat[i] = new int[n2];
		for (size_t j = 0; j < n2; j++)
		{
			if (s1[i] != s2[j]) {
				mat[i][j] = 0;
				if (i > 0 && j > 0 && mat[i - 1][j - 1] >= lb) {
					const int substr_length = mat[i - 1][j - 1];
					const std::string common_substr = s1.substr(i - substr_length, substr_length);
					//std::cout << substr_length << ", " << common_substr << std::endl;
					//record maximal substr of s1 
					auto itr1 = substr_map_1.find(common_substr);
					if (itr1 == substr_map_1.end())
					{
						std::map<int, int> tmp_map;
						tmp_map[i - substr_length] = i - 1;
						substr_map_1.emplace(common_substr, tmp_map);
					}
					else
					{
						itr1->second.emplace(i - substr_length, i - 1);
					}
					//record maximal substr of s2 
					auto itr2 = substr_map_2.find(common_substr);
					if (itr2 == substr_map_2.end())
					{
						std::map<int, int> tmp_map;
						tmp_map[j - substr_length] = j - 1;
						substr_map_2.emplace(common_substr, tmp_map);
					}
					else
					{
						itr2->second.emplace(j - substr_length, j - 1);
					}
				}
			}
			else if (i == 0 || j == 0)
			{
				mat[i][j] = 1;
			}
			else
			{
				mat[i][j] = mat[i - 1][j - 1] + 1;
			}
		}
	}
}
