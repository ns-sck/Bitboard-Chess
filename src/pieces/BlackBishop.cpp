#include "BlackBishop.h"
#include "BitUtil.h"
using namespace std;

BlackBishop::BlackBishop(uint64_t position) 
    : Piece(position, false) {}

std::vector<Move> BlackBishop::generate_moves(uint64_t position, uint64_t team, uint64_t enemy) {
    std::vector<Move> moves;
    uint64_t p = position;
    while (p) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);
        uint64_t occupancy = team | enemy;
        occupancy &= bishop_masks[sq];
        occupancy *= bishop_magic[sq];
        occupancy >>= 64 - bishop_bit_counts[sq];
        occupancy = bishop_moves[sq][occupancy];
        occupancy &= ~team;
        this->add_moves(moves, sq, occupancy, enemy);
    }
    return moves;
}

uint64_t BlackBishop::get_attacked_pos(uint64_t position, uint64_t team, uint64_t enemy) {
    uint64_t attack = 0;
    uint64_t p = position;
    while (p) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);
        uint64_t occupancy = team | enemy;
        occupancy &= bishop_masks[sq];
        occupancy *= bishop_magic[sq];
        occupancy >>= 64 - bishop_bit_counts[sq];
        occupancy = bishop_moves[sq][occupancy];
        occupancy &= ~team;
        attack |= occupancy;
    }
    return attack;
}

char BlackBishop::get_symbol() const {
    return 'b';
}

int BlackBishop::get_value() const {
    return 3;
} 