#include "Queen.h"
#include "BitUtil.h"
using namespace std;

Queen::Queen(uint64_t position) 
    : Piece(position, true) {}

std::vector<Move> Queen::generate_moves(uint64_t position, uint64_t team, uint64_t enemy) {
    std::vector<Move> moves;
    uint64_t p = position;
    while (p) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);
        uint64_t occupancy = team | enemy;
        
        uint64_t bishop_occ = occupancy & bishop_masks[sq];
        bishop_occ *= bishop_magic[sq];
        bishop_occ >>= 64 - bishop_bit_counts[sq];
        uint64_t bishop_attacks = bishop_moves[sq][bishop_occ];
        
        uint64_t rook_occ = occupancy & rook_masks[sq];
        rook_occ *= rook_magic[sq];
        rook_occ >>= 64 - rook_bit_counts[sq];
        uint64_t rook_attacks = rook_moves[sq][rook_occ];
        
        uint64_t attacks = (bishop_attacks | rook_attacks) & ~team;
        this->add_moves(moves, sq, attacks, enemy);
    }
    return moves;
}

char Queen::get_symbol() const {
    return 'Q';
}

int Queen::get_value() const {
    return 9;
} 