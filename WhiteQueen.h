#ifndef WHITE_QUEEN_H
#define WHITE_QUEEN_H

#include "Piece.h"

class WhiteQueen : public Piece {
public:
    WhiteQueen(uint64_t position);
    
    std::vector<Move> generateMoves(uint64_t position, uint64_t team, uint64_t enemy);
    char getSymbol() const override;
    int getValue() const override;
};

#endif