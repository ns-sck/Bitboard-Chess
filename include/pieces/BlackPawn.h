#ifndef BLACK_PAWN_H
#define BLACK_PAWN_H

#include "Piece.h"

class BlackPawn : public Piece {
public:
    BlackPawn(uint64_t position);
    
    std::vector<Move> generate_moves(uint64_t position, uint64_t team, uint64_t enemy);
    char get_symbol() const override;
    int get_value() const override;
};

#endif