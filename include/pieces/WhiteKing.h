#ifndef WHITE_KING_H
#define WHITE_KING_H

#include "Piece.h"

class WhiteKing : public Piece {
private:
    bool hasMoved;

public:
    WhiteKing(uint64_t position);
    
    std::vector<Move> generate_moves(uint64_t position, uint64_t team, uint64_t enemy);
    uint64_t get_attacked_pos(uint64_t position, uint64_t team, uint64_t enemy);
    bool is_under_check(uint64_t position, uint64_t team, uint64_t enemy, uint64_t bitboard[]);
    char get_symbol() const override;
    int get_value() const override;
    
    bool getHasMoved() const;
    void setHasMoved(bool moved);
};

#endif