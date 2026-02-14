#ifndef BLACK_ROOK_H
#define BLACK_ROOK_H

#include "Piece.h"

class BlackRook : public Piece {
private:
    bool hasMoved;

public:
    BlackRook(uint64_t position);
    
    std::vector<Move> generate_moves(uint64_t position, uint64_t team, uint64_t enemy);
    uint64_t get_attacked_pos(uint64_t position, uint64_t team, uint64_t enemy);
    char get_symbol() const override;
    int get_value() const override;
    
    bool getHasMoved() const;
    void setHasMoved(bool moved);
};

#endif