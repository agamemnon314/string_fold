#include "Block.h"
#include <iostream>

bool Block::operator==(const Block& otherBlock) const
{
	return _sequence_id == otherBlock._sequence_id && _start_pos == otherBlock._start_pos && _block_size == otherBlock._block_size;
}

void Block::print() const
{
	std::cout << _substring << std::endl;
	std::cout << _substring.size() << ", [" << _start_pos << ", " << _start_pos + _block_size << "]" << std::endl;
}

bool Block::is_overlap(const Block& other_block) const
{
	if (_sequence_id != other_block._sequence_id) {
		return false;
	}
	else if (_start_pos + _block_size <= other_block._start_pos || other_block._start_pos + other_block._block_size <= _start_pos) {
		return false;
	}
	return true;
}
