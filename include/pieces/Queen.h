#pragma once
#include "Piece.h"

namespace Queen {

    std::vector<Move> generate_moves(uint64_t position, uint64_t team, uint64_t enemy, bool color);

    char get_symbol();

    int get_value();
};