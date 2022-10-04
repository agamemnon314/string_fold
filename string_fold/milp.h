#pragma once
#include "block.h"
#include <unordered_map>
#include <vector>
#include <ilcplex/ilocplex.h>

void solve_by_integrate_model(const std::unordered_map<std::string, std::vector<Block>>& pattern_block_map,
	std::unordered_map<std::string, std::vector<Block>>& selected_pattern_block_map);


