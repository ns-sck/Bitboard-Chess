#ifndef WHITE_KNIGHT_H
#define WHITE_KNIGHT_H

#include "Piece.h"

class WhiteKnight : public Piece {
public:
    WhiteKnight(uint64_t position);
    
    std::vector<Move> generateMoves(uint64_t position, uint64_t team, uint64_t enemy);
    char getSymbol() const override;
    int getValue() const override;
};

#endif