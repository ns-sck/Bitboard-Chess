#include "BlackKnight.h"
#include "BitUtil.h"
using namespace std;

BlackKnight::BlackKnight(uint64_t position) 
    : Piece(position, false) {}

std::vector<Move> BlackKnight::generate_moves(uint64_t position, uint64_t team, uint64_t enemy) {
    std::vector<Move> moves;
    uint64_t p = position;
    
    while (p > 0) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);
        
        uint64_t possible_moves = knight_moves[sq];
        uint64_t occupancy = possible_moves & ~team;
        this->add_moves(moves, sq, occupancy);
    }
    return moves;
}

char BlackKnight::get_symbol() const {
    return 'n';
}

int BlackKnight::get_value() const {
    return 3;
} 