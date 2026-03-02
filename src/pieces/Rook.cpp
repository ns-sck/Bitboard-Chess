#include "Rook.h"
#include "BitUtil.h"

std::vector<Move> Rook::generate_moves(uint64_t position, uint64_t team, uint64_t enemy, bool color) {
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
        add_moves(moves, sq, attacks, enemy);
    }
    return moves;
}

char Rook::get_symbol() {
    return 'R';
}

int Rook::get_value() {
    return 5;
} 