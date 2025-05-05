#ifndef BLACK_QUEEN_H
#define BLACK_QUEEN_H

#include "Piece.h"

class BlackQueen : public Piece {
public:
    BlackQueen(uint64_t position);
    
    std::vector<Move> generateMoves(uint64_t position, uint64_t team, uint64_t enemy);
    char getSymbol() const override;
    int getValue() const override;
};

#endif