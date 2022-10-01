#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "util.h"
#include "Pattern.h"

int main()
{
    std::ifstream f_in;
    f_in.open("D:/Workspace/sequence_folding/example", std::ios_base::in);
    
    std::string seq;
    std::vector<std::string> all_sequence;

    std::getline(f_in, seq);

    while (std::getline(f_in, seq))
    {
        all_sequence.push_back(seq);
        //std::cout << seq << std::endl;
    }
    f_in.close();
    std::cout << all_sequence.size() << std::endl;

    std::unordered_map<std::string, Pattern*> all_patterns;

    std::unordered_map<std::string, std::map<int, int>> substr_map_1;
    std::unordered_map<std::string, std::map<int, int>> substr_map_2;

	for (size_t i = 0; i < all_sequence.size() - 1; i++)
	{
		for (size_t j = i + 1; j < all_sequence.size(); j++)
		{
			find_all_common_maximal_substring(all_sequence[i], all_sequence[j], 20, substr_map_1, substr_map_2);
            for (auto& substr_pos_pair : substr_map_1) {
				auto& sub_str = substr_pos_pair.first;
				auto& pos_map = substr_pos_pair.second;
                auto itr = all_patterns.find(sub_str);
                if (itr == all_patterns.end()) {
                    Pattern* pattern = new Pattern(sub_str);
                    pattern->update_pos_map(i, pos_map);
                    all_patterns[sub_str] = pattern;
                }
                else
                {
                    itr->second->update_pos_map(i, pos_map);
                }
            }

			for (auto& substr_pos_pair : substr_map_2) {
				auto& sub_str = substr_pos_pair.first;
				auto& pos_map = substr_pos_pair.second;
				auto itr = all_patterns.find(sub_str);
				if (itr == all_patterns.end()) {
					Pattern* pattern = new Pattern(sub_str);
                    pattern->update_pos_map(i, pos_map);
					all_patterns[sub_str] = pattern;
				}
				else
				{
					itr->second->update_pos_map(j, pos_map);
				}
			}
		}
	}
    
    int total_pattern_length = 0;
    for (auto& x : all_patterns) {
        total_pattern_length = total_pattern_length + x.second->get_count();
    }

    int total_sequence_length = 0;
    for (auto& x : all_sequence) {
        total_sequence_length = total_sequence_length + x.size();
    }

    std::cout << "The number of all maximal pattern is " << all_patterns.size() << std::endl;
    std::cout << "The total length of all maximal patterns is " << total_pattern_length << std::endl;
    std::cout << "The total length of all sequences is " << total_sequence_length << std::endl;

    for (auto& x : all_patterns) {
        x.second->print_all();
    }
}
