#ifndef BLACK_BISHOP_H
#define BLACK_BISHOP_H

#include "Piece.h"

class BlackBishop : public Piece {
public:
    BlackBishop(uint64_t position);
    
    std::vector<Move> generate_moves(uint64_t position, uint64_t team, uint64_t enemy);
    char get_symbol() const override;
    int get_value() const override;
};

#endif