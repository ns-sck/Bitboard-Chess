#include "BlackKing.h"
#include "BitUtil.h"
using namespace std;

BlackKing::BlackKing(uint64_t position) 
    : Piece(position, false), hasMoved(false) {}

vector<Move> BlackKing::generate_moves(uint64_t position, uint64_t team, uint64_t enemy) {
    vector<Move> moves;
    uint64_t p = position;
    
    while (p > 0) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);
        
        uint64_t possible_moves = king_moves[sq];
        uint64_t occupancy = possible_moves & ~team;
        this->add_moves(moves, sq, occupancy);
    }
    return moves;
}

char BlackKing::get_symbol() const {
    return 'k';
}

int BlackKing::get_value() const {
    return 0;
}

bool BlackKing::getHasMoved() const {
    return hasMoved;
}

void BlackKing::setHasMoved(bool moved) {
    hasMoved = moved;
} 