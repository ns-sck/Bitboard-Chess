#include "WhiteKnight.h"
#include "BitUtil.h"
using namespace std;

WhiteKnight::WhiteKnight(uint64_t position) 
    : Piece(position, true) {}

std::vector<Move> WhiteKnight::generate_moves(uint64_t position, uint64_t team, uint64_t enemy) {
    std::vector<Move> moves;
    uint64_t p = position;
    
    while (p > 0) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);

        uint64_t possible_moves = knight_moves[sq];
        uint64_t occupancy = possible_moves & ~team;
        this->add_moves(moves, sq, occupancy, enemy);
    }
    return moves;
}

uint64_t WhiteKnight::get_attacked_pos(uint64_t position, uint64_t team, uint64_t enemy) {
    uint64_t attack = 0;
    uint64_t p = position;
    
    while (p > 0) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);

        uint64_t possible_moves = knight_moves[sq];
        uint64_t occupancy = possible_moves & ~team;
        attack |= occupancy;
    }
    return attack;
}

char WhiteKnight::get_symbol() const {
    return 'N';
}

int WhiteKnight::get_value() const {
    return 3;
} 