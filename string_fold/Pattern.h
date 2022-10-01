#pragma once
#include <string>
#include <map>
class Pattern
{
	std::string _substring;
	std::map<int, std::map<int, int>> _pos_map; //<seq_id <start_pos, end pos>>
public:
	Pattern(const std::string s) { _substring = s; };
	std::string get_substring() { return _substring; }
	int get_count();
	int get_count(int seq_id);
	void update_pos_map(int seq_id, const std::map<int, int>& new_pos_map);
	void print();
	void print_all();
};

