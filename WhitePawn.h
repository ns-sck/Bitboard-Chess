#ifndef WHITE_PAWN_H
#define WHITE_PAWN_H

#include "Piece.h"

class WhitePawn : public Piece {
public:
    WhitePawn(uint64_t position);
    
    std::vector<Move> generateMoves(uint64_t position, uint64_t team, uint64_t enemy);
    char getSymbol() const override;
    int getValue() const override;
};

#endif