#pragma once
#include <string>
#include <map>
class Block
{
	std::string _substring;
	size_t _sequence_id;
	size_t _start_pos;
	size_t _block_size;

public:
	Block(const std::string str, int id, size_t s, size_t size) :_substring(str), _sequence_id(id), _start_pos(s), _block_size(size) {};
	bool operator==(const Block& otherBlock) const;
	struct BlockHasher
	{
		size_t operator()(const Block& block) const{
			return std::hash<int>()(block._sequence_id) ^ std::hash<int>()(block._start_pos) ^ std::hash<int>()(block._block_size);
		}
	};
	std::string get_substring() const { return _substring; }
	size_t get_sequence_id() const { return _sequence_id; }
	size_t get_start_pos() const { return _start_pos; }
	size_t get_end_pos() const { return _start_pos + _block_size; }
	size_t get_block_size() const { return _block_size; }
	void print() const;

	bool is_overlap(const Block& other_block) const;
};

