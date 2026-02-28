#ifndef BLACK_KING_H
#define BLACK_KING_H

#include "Piece.h"

class BlackKing : public Piece {
private:
    bool has_moved;

public:
    BlackKing(uint64_t position);
    
    std::vector<Move> generate_moves(uint64_t position, uint64_t team, uint64_t enemy);
    bool is_king_safe(uint64_t position, uint64_t team, uint64_t enemy, uint64_t bitboard[]);
    char get_symbol() const override;
    int get_value() const override;
};

#endif