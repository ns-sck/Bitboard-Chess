#ifndef WHITE_BISHOP_H
#define WHITE_BISHOP_H

#include "Piece.h"

class WhiteBishop : public Piece {
public:
    WhiteBishop(uint64_t position);
    
    std::vector<Move> generateMoves(uint64_t position, uint64_t team, uint64_t enemy);
    char getSymbol() const override;
    int getValue() const override;
};

#endif