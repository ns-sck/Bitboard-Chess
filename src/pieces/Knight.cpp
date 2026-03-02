#include "Knight.h"
#include "BitUtil.h"

std::vector<Move> Knight::generate_moves(uint64_t position, uint64_t team, uint64_t enemy, bool color) {
    std::vector<Move> moves;
    uint64_t p = position;
    
    while (p > 0) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);

        uint64_t possible_moves = knight_moves[sq];
        uint64_t occupancy = possible_moves & ~team;
        add_moves(moves, sq, occupancy, enemy);
    }
    return moves;
}

char Knight::get_symbol() {
    return 'N';
}

int Knight::get_value() {
    return 3;
} 