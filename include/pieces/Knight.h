#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

class Knight : public Piece {
public:
    Knight(uint64_t position);
    
    std::vector<Move> generate_moves(uint64_t position, uint64_t team, uint64_t enemy);
    char get_symbol() const override;
    int get_value() const override;
};

#endif