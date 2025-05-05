#include "WhiteKing.h"
#include "BitUtil.h"
using namespace std;

WhiteKing::WhiteKing(uint64_t position) 
    : Piece(position, true) {}

std::vector<Move> WhiteKing::generateMoves(uint64_t position, uint64_t team, uint64_t enemy) {
    std::vector<Move> moves;
    uint64_t p = position;
    
    while (p > 0) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);
        
        uint64_t possibleMoves = king_moves[sq];
        uint64_t occupancy = possibleMoves & ~team;
        this->addMoves(moves, sq, occupancy);
    }
    return moves;
}

char WhiteKing::getSymbol() const {
    return 'K';
}

int WhiteKing::getValue() const {
    return 0;
}

bool WhiteKing::getHasMoved() const {
    return false;
}

void WhiteKing::setHasMoved(bool moved) {
} 