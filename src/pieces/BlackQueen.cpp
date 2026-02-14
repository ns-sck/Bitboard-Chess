#include "BlackQueen.h"
#include "BitUtil.h"
using namespace std;

BlackQueen::BlackQueen(uint64_t position) 
    : Piece(position, false) {}

std::vector<Move> BlackQueen::generate_moves(uint64_t position, uint64_t team, uint64_t enemy) {
    std::vector<Move> moves;
    uint64_t p = position;
    while (p) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);
        uint64_t occupancy = team | enemy;
        
        uint64_t bishopOcc = occupancy & bishop_masks[sq];
        bishopOcc *= bishop_magic[sq];
        bishopOcc >>= 64 - bishop_bit_counts[sq];
        uint64_t bishop_attacks = bishop_moves[sq][bishopOcc];
        
        uint64_t rookOcc = occupancy & rook_masks[sq];
        rookOcc *= rook_magic[sq];
        rookOcc >>= 64 - rook_bit_counts[sq];
        uint64_t rook_attacks = rook_moves[sq][rookOcc];
        
        uint64_t attacks = (bishop_attacks | rook_attacks) & ~team;
        
        this->add_moves(moves, sq, attacks, enemy);
    }
    return moves;
}

uint64_t BlackQueen::get_attacked_pos(uint64_t position, uint64_t team, uint64_t enemy) {
    uint64_t attack = 0;
    uint64_t p = position;
    while (p) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);
        uint64_t occupancy = team | enemy;
        
        uint64_t bishopOcc = occupancy & bishop_masks[sq];
        bishopOcc *= bishop_magic[sq];
        bishopOcc >>= 64 - bishop_bit_counts[sq];
        uint64_t bishop_attacks = bishop_moves[sq][bishopOcc];
        
        uint64_t rookOcc = occupancy & rook_masks[sq];
        rookOcc *= rook_magic[sq];
        rookOcc >>= 64 - rook_bit_counts[sq];
        uint64_t rook_attacks = rook_moves[sq][rookOcc];
        
        uint64_t attacks = (bishop_attacks | rook_attacks) & ~team;
        attack |= attacks;
    }
    return attack;
}

char BlackQueen::get_symbol() const {
    return 'q';
}

int BlackQueen::get_value() const {
    return 9;
} 