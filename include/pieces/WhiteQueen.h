#ifndef WHITE_QUEEN_H
#define WHITE_QUEEN_H

#include "Piece.h"

class WhiteQueen : public Piece {
public:
    WhiteQueen(uint64_t position);
    
    std::vector<Move> generate_moves(uint64_t position, uint64_t team, uint64_t enemy);
    uint64_t get_attacked_pos(uint64_t position, uint64_t team, uint64_t enemy);
    char get_symbol() const override;
    int get_value() const override;
};

#endif