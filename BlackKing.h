#ifndef BLACK_KING_H
#define BLACK_KING_H

#include "Piece.h"

class BlackKing : public Piece {
private:
    bool hasMoved;

public:
    BlackKing(uint64_t position);
    
    std::vector<Move> generateMoves(uint64_t position, uint64_t team, uint64_t enemy);
    char getSymbol() const override;
    int getValue() const override;
    
    bool getHasMoved() const;
    void setHasMoved(bool moved);
};

#endif