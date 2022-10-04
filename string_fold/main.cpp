#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "util.h"
#include "block.h"
#include "milp.h"

int main()
{
    auto current_time = std::chrono::system_clock::now();
    std::ifstream f_in;
    f_in.open("D:/Workspace/sequence_folding/example", std::ios_base::in);
    
    std::string seq;
    std::vector<std::string> all_sequence;

    std::getline(f_in, seq);

    size_t total_sequences_length = 0;
    while (std::getline(f_in, seq) /*&& all_sequence.size() < 15*/)
    {
        total_sequences_length += seq.size();
        all_sequence.push_back(seq);
    }
    f_in.close();
    std::cout << "reading time: " << std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::system_clock::now() - current_time).count() << std::endl;
    std::cout << "The number of sequences is " << all_sequence.size() << std::endl;
    std::cout << "The total length of all sequences is " << total_sequences_length << std::endl;


	current_time = std::chrono::system_clock::now();
    size_t n_sequences = all_sequence.size();
    std::unordered_set<Block,Block::BlockHasher> maximal_blocks;

	for (size_t i = 0; i < n_sequences - 1; i++)
	{
		for (size_t j = i + 1; j < n_sequences; j++)
		{
            find_all_common_maximal_blocks(all_sequence, i, j, 20, maximal_blocks);
		}
	}
	std::cout << "find all maximal blocks use time : " << std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::system_clock::now() - current_time).count() << std::endl;
    std::cout << "The number of maximal blocks is " << maximal_blocks.size() << std::endl;


	//current_time = std::chrono::system_clock::now();
 //   std::unordered_set<Block, Block::BlockHasher> elementry_blocks;
 //   for (const auto& block : maximal_blocks) {
 //       generate_elementary_blocks(block, 20, elementry_blocks);
 //   }

	//std::cout << "find all elementary blocks use time : " << std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::system_clock::now() - current_time).count() << std::endl;
	//std::cout << elementry_blocks.size() << std::endl;


    current_time = std::chrono::system_clock::now();
    std::unordered_set<Block, Block::BlockHasher> lattice_blocks;
    find_all_lattice_blocks(all_sequence, maximal_blocks, 20, lattice_blocks);

    std::cout << "find all lattice blocks use time : " << std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::system_clock::now() - current_time).count() << std::endl;
    std::cout << "The number of lattice blocks is " << lattice_blocks.size() << std::endl;



    std::unordered_map<std::string, std::vector<Block>> pattern_block_map;

	for (const auto& block : lattice_blocks) {
        pattern_block_map[block.get_substring()].push_back(block);
	}

    std::cout << "The number of lattice pattern is " << pattern_block_map.size() << std::endl;

    std::unordered_map<std::string, std::vector<Block>> selected_pattern_block_map;
    solve_by_integrate_model(pattern_block_map, selected_pattern_block_map);
    std::cout << "The number of patterns is " << selected_pattern_block_map.size() << std::endl;

	write_solution(selected_pattern_block_map, "D:/Workspace/sequence_folding/solution.txt");
	write_solution(pattern_block_map, "D:/Workspace/sequence_folding/pattern_block.txt");


    pattern_block_map.clear();
	for (const auto& block : maximal_blocks) {
		pattern_block_map[block.get_substring()].push_back(block);
	}
	write_solution(pattern_block_map, "D:/Workspace/sequence_folding/maximal_block.txt");


    return 0;
}
