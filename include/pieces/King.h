#pragma once

#include "Piece.h"

namespace King {
    
    std::vector<Move> generate_moves(uint64_t position, uint64_t team, uint64_t enemy, bool color);

    bool is_safe(uint64_t position, uint64_t team, uint64_t enemy, uint64_t bitboard[], bool color);

    char get_symbol();

    int get_value();
};