#pragma once
#include<string>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include "block.h"
void find_all_common_maximal_blocks(const std::vector<std::string>& sequences,
	size_t id_1, size_t id_2, int lb,
	std::unordered_set<Block, Block::BlockHasher>& block_set);
void find_all_lattice_blocks(const std::vector<std::string>& all_sequences,
	const std::unordered_set<Block, Block::BlockHasher>& maximal_block_set,
	size_t lb, std::unordered_set<Block, Block::BlockHasher>& lattice_block_set);
void generate_elementary_blocks(const Block& block, size_t lb, std::unordered_set<Block, Block::BlockHasher>& block_set);

void write_solution(const std::unordered_map<std::string, std::vector<Block>>& selected_pattern_block_map, const std::string& file_path);

void find_all_maximal_cliques(const std::vector<const Block*>& blocks, std::vector<std::unordered_set<int>>& maximal_cliques);
