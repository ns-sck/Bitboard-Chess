#include "Bishop.h"
#include "BitUtil.h"

std::vector<Move> Bishop::generate_moves(uint64_t position, uint64_t team, uint64_t enemy, bool color) {
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
        add_moves(moves, sq, occupancy, enemy);
    }
    return moves;
}

char Bishop::get_symbol() {
    return 'B';
}

int Bishop::get_value() {
    return 3;
} 