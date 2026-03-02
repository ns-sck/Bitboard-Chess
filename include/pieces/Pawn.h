#pragma once
#include "Move.h"
#include "BitUtil.h"

namespace Pawn {

    std::vector<Move> generate_moves(uint64_t position, uint64_t team, uint64_t enemy, bool color);

    char get_symbol();

    int get_value();
};