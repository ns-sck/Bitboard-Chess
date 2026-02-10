#ifndef BLACK_KNIGHT_H
#define BLACK_KNIGHT_H

#include "Piece.h"

class BlackKnight : public Piece {
public:
    BlackKnight(uint64_t position);
    
    std::vector<Move> generate_moves(uint64_t position, uint64_t team, uint64_t enemy);
    char get_symbol() const override;
    int get_value() const override;
};

#endif