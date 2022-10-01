#include "Pattern.h"
#include <iostream>

int Pattern::get_count()
{
	if (_pos_map.empty())
	{
		return 0;
	}
	int n = 0;
	for (const auto& x : _pos_map)
	{
		n = n + x.second.size();
	}
	return n;
}

int Pattern::get_count(int seq_id)
{
	auto itr = _pos_map.find(seq_id);
	if (itr == _pos_map.end()) {
		return 0;
	}
	else
	{
		return itr->second.size();
	}
}

void Pattern::update_pos_map(int seq_id, const std::map<int, int>& new_pos_map)
{
	auto itr = _pos_map.find(seq_id);
	if (itr == _pos_map.end()) {
		_pos_map.emplace(seq_id, new_pos_map);
	}
	else
	{
		for (const auto& x : new_pos_map)
		{
			itr->second.emplace(x.first, x.second);
		}
	}
}

void Pattern::print()
{
	//std::cout << _substring << std::endl;
	std::cout << _substring.size() << ", " << get_count() << std::endl;
}

void Pattern::print_all()
{
	std::cout << _substring << std::endl;
	std::cout << _substring.size() << ", " << get_count() << std::endl;
	for (const auto& x : _pos_map)
	{
		std::cout << x.first << ", " << x.second.size() << " =================" << std::endl;
		for (const auto& st_pair : x.second)
		{
			std::cout << "[" << st_pair.first << ", " << st_pair.second << "], ";
		}
		std::cout << std::endl;
	}
}