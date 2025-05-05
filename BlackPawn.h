#ifndef BLACK_PAWN_H
#define BLACK_PAWN_H

#include "Piece.h"

class BlackPawn : public Piece {
public:
    BlackPawn(uint64_t position);
    
    std::vector<Move> generateMoves(uint64_t position, uint64_t team, uint64_t enemy);
    char getSymbol() const override;
    int getValue() const override;
};

#endif