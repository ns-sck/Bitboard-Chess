#ifndef WHITE_ROOK_H
#define WHITE_ROOK_H

#include "Piece.h"

class WhiteRook : public Piece {
private:
    bool hasMoved;

public:
    WhiteRook(uint64_t position);
    
    std::vector<Move> generateMoves(uint64_t position, uint64_t team, uint64_t enemy);
    char getSymbol() const override;
    int getValue() const override;
    
    bool getHasMoved() const;
    void setHasMoved(bool moved);
};

#endif