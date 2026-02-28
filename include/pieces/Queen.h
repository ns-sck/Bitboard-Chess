#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

class Queen : public Piece {
public:
    Queen(uint64_t position);
    
    std::vector<Move> generate_moves(uint64_t position, uint64_t team, uint64_t enemy);
    char get_symbol() const override;
    int get_value() const override;
};

#endif