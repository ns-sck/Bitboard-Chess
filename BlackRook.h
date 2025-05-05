#ifndef BLACK_ROOK_H
#define BLACK_ROOK_H

#include "Piece.h"

class BlackRook : public Piece {
private:
    bool hasMoved;

public:
    BlackRook(uint64_t position);
    
    std::vector<Move> generateMoves(uint64_t position, uint64_t team, uint64_t enemy);
    char getSymbol() const override;
    int getValue() const override;
    
    bool getHasMoved() const;
    void setHasMoved(bool moved);
};

#endif