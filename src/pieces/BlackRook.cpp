#include "BlackRook.h"
#include "BitUtil.h"
using namespace std;

BlackRook::BlackRook(uint64_t position) 
    : Piece(position, false) {}

std::vector<Move> BlackRook::generate_moves(uint64_t position, uint64_t team, uint64_t enemy) {
    std::vector<Move> moves;
    uint64_t p = position;
    while (p) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);
        uint64_t occupancy = team | enemy;
        occupancy &= rook_masks[sq];
        occupancy *= rook_magic[sq];
        occupancy >>= 64 - rook_bit_counts[sq];
        uint64_t attacks = rook_moves[sq][occupancy];
        attacks &= ~team; 
        this->add_moves(moves, sq, attacks, enemy);
    }
    return moves;
}

uint64_t BlackRook::get_attacked_pos(uint64_t position, uint64_t team, uint64_t enemy) {
    uint64_t attack = 0;
    uint64_t p = position;
    while (p) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);
        uint64_t occupancy = team | enemy;
        occupancy &= rook_masks[sq];
        occupancy *= rook_magic[sq];
        occupancy >>= 64 - rook_bit_counts[sq];
        uint64_t attacks = rook_moves[sq][occupancy];
        attacks &= ~team;
        attack |= attacks;
    }
    return attack;
}

char BlackRook::get_symbol() const {
    return 'r';
}

int BlackRook::get_value() const {
    return 5;
}

bool BlackRook::getHasMoved() const {
    return false;
}

void BlackRook::setHasMoved(bool moved) {
} 