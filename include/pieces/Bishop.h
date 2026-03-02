#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"

namespace Bishop {

    std::vector<Move> generate_moves(uint64_t position, uint64_t team, uint64_t enemy, bool color);

    char get_symbol();

    int get_value();
};

#endif