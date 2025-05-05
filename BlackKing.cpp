#include "BlackKing.h"
#include "BitUtil.h"
using namespace std;

BlackKing::BlackKing(uint64_t position) 
    : Piece(position, false), hasMoved(false) {}

vector<Move> BlackKing::generateMoves(uint64_t position, uint64_t team, uint64_t enemy) {
    vector<Move> moves;
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

char BlackKing::getSymbol() const {
    return 'k';
}

int BlackKing::getValue() const {
    return 0;
}

bool BlackKing::getHasMoved() const {
    return hasMoved;
}

void BlackKing::setHasMoved(bool moved) {
    hasMoved = moved;
} 