#include "Rook.h"
#include "BitUtil.h"
using namespace std;

Rook::Rook(uint64_t position) 
    : Piece(position, true) {}

std::vector<Move> Rook::generate_moves(uint64_t position, uint64_t team, uint64_t enemy) {
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

char Rook::get_symbol() const {
    return 'R';
}

int Rook::get_value() const {
    return 5;
} 