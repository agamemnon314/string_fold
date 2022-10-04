#include "util.h"
#include <iostream>
#include <algorithm>
#include <fstream>

void find_all_common_maximal_blocks(const std::vector<std::string>& sequences, size_t id_1, size_t id_2, int lb,
	std::unordered_set<Block, Block::BlockHasher>& block_set)
{
	const std::string& s1 = sequences[id_1];
	const std::string& s2 = sequences[id_2];
	const size_t n1 = s1.size();
	const size_t n2 = s2.size();

	int** mat = new int* [n1];
	for (size_t i = 0; i < n1; ++i)
	{
		mat[i] = new int[n2];
		for (size_t j = 0; j < n2; j++)
		{
			mat[i][j] = 0;
			if (s1[i] != s2[j]) {
				mat[i][j] = 0;
				if (i > 0 && j > 0 && mat[i - 1][j - 1] >= lb) {
					const int substr_length = mat[i - 1][j - 1];
					const std::string common_substr = s1.substr(i - substr_length, substr_length);
					//add maximal block of s1 
					block_set.insert(Block(common_substr, id_1, i - substr_length, substr_length));
					//add maximal block of s2 
					block_set.insert(Block(common_substr, id_2, j - substr_length, substr_length));
				}
			}
			else if (i == 0 || j == 0)
			{
				mat[i][j] = 1;
			}
			else
			{
				mat[i][j] = mat[i - 1][j - 1] + 1;
				if ((i == n1 - 1 || j == n2 - 1) && mat[i][j] >= lb) {
					const int substr_length = mat[i][j];
					const std::string common_substr = s1.substr(i - substr_length + 1, substr_length);
					//add maximal block of s1 
					block_set.insert(Block(common_substr, id_1, i - substr_length + 1, substr_length));
					//add maximal block of s2 
					block_set.insert(Block(common_substr, id_2, j - substr_length + 1, substr_length));
				}
			}
		}
	}
}

void find_all_lattice_blocks(const std::vector<std::string>& all_sequences,
	const std::unordered_set<Block, Block::BlockHasher>& maximal_block_set,
	size_t lb, std::unordered_set<Block, Block::BlockHasher>& lattice_block_set)
{
	std::map<size_t, std::vector<size_t>>seq_id_dots_map;
	for (const auto& maximal_block : maximal_block_set) {
		seq_id_dots_map[maximal_block.get_sequence_id()].push_back(maximal_block.get_start_pos());
		seq_id_dots_map[maximal_block.get_sequence_id()].push_back(maximal_block.get_end_pos());
	}

	for (auto& seq_id_dots : seq_id_dots_map) {
		auto& dots = seq_id_dots.second;
		std::stable_sort(dots.begin(), dots.end());
	}

	for (const auto& maximal_block : maximal_block_set) {
		const auto& dots = seq_id_dots_map.at(maximal_block.get_sequence_id());
		const auto n_dots = dots.size();
		for (size_t i = 0; i < n_dots - 1; i++)
		{
			const auto start_pos = dots[i];
			if (start_pos < maximal_block.get_start_pos()) {
				continue;
			}

			for (size_t j = i+1; j < n_dots; j++)
			{
				const auto end_pos = dots[j];
				if (end_pos - start_pos < lb) {
					continue;
				}

				if (end_pos > maximal_block.get_end_pos()) {
					break;
				}
				const auto& sub_str = all_sequences[maximal_block.get_sequence_id()].substr(start_pos, end_pos - start_pos);
				lattice_block_set.insert(Block(sub_str, maximal_block.get_sequence_id(), start_pos, end_pos - start_pos));
			}
		}
	}
}

void generate_elementary_blocks(const Block& block, size_t lb, std::unordered_set<Block, Block::BlockHasher>& sub_block_set)
{
	const size_t block_size = block.get_block_size();
	if (block_size <= lb)
	{
		return;
	}

	const size_t max_sub_block_size = std::min(2 * lb, block_size);

	for (size_t sub_block_size = lb; sub_block_size < max_sub_block_size; sub_block_size++)
	{
		for (size_t i = 0; i < block_size - sub_block_size; i++)
		{
			Block sub_block(block.get_substring().substr(i, sub_block_size), block.get_sequence_id(), block.get_start_pos() + i, sub_block_size);
			sub_block_set.insert(sub_block);
		}
	}
}

void write_solution(const std::unordered_map<std::string, std::vector<Block>>& pattern_block_map, const std::string& file_path)
{
	std::ofstream f_out;
	f_out.open(file_path, std::ios_base::out);

	size_t n_blocks = 0;

	for (const auto& pattern_blocks : pattern_block_map) {
		const auto& blocks = pattern_blocks.second;
		n_blocks += blocks.size();
	}

	f_out << "Select " << pattern_block_map.size() << " patterns with "<<n_blocks<<" blocks." << std::endl;

	for (const auto& pattern_blocks : pattern_block_map) {
		const auto& pattern = pattern_blocks.first;
		const auto& blocks = pattern_blocks.second;
		f_out << " Pattern: " << pattern << std::endl;
		for (size_t i = 0; i < blocks.size(); i++)
		{
			f_out << "[" << blocks[i].get_sequence_id() << ", "
				<< blocks[i].get_start_pos() << ", "
				<< blocks[i].get_end_pos() << "], ";
		}
		f_out << std::endl;
		f_out << " ============================================== " << std::endl;
	}

	f_out.close();
}

void find_all_maximal_cliques(const std::vector<const Block*>& blocks, std::vector<std::unordered_set<int>>& maximal_cliques)
{
	int start_type = 1;
	int end_type = -1;
	std::map<size_t, std::map<int, std::vector<int>>> pos_type_block_ids_map;
	size_t n_blocks = blocks.size();
	for (size_t i = 0; i < n_blocks; i++)
	{
		pos_type_block_ids_map[blocks[i]->get_start_pos()][start_type].push_back(i);
		pos_type_block_ids_map[blocks[i]->get_end_pos()][end_type].push_back(i);
	}

	std::unordered_set<int> current_maximal_clique;
	bool is_growing = true;

	for (const auto& pos_type_block_ids : pos_type_block_ids_map) {
		const auto pos = pos_type_block_ids.first;
		auto itr1 = pos_type_block_ids.second.find(end_type);
		if (itr1 != pos_type_block_ids.second.end() && !itr1->second.empty())
		{
			if (is_growing) {
				maximal_cliques.push_back(current_maximal_clique);
				is_growing = false;
			}
			for (auto block_id : itr1->second) {
				current_maximal_clique.erase(block_id);
			}
		}
		auto itr2 = pos_type_block_ids.second.find(start_type);
		if (itr2 != pos_type_block_ids.second.end() && !itr2->second.empty())
		{
			for (auto block_id : itr2->second) {
				current_maximal_clique.insert(block_id);
			}
			is_growing = true;
		}
	}
}
