#include "WhiteKing.h"
#include "BitUtil.h"
using namespace std;

WhiteKing::WhiteKing(uint64_t position) 
    : Piece(position, true) {}

std::vector<Move> WhiteKing::generate_moves(uint64_t position, uint64_t team, uint64_t enemy) {
    std::vector<Move> moves;
    uint64_t p = position;
    
    while (p > 0) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);
        
        uint64_t possible_moves = king_moves[sq];
        uint64_t occupancy = possible_moves & ~team;
        this->add_moves(moves, sq, occupancy, enemy);
    }
    return moves;
}

uint64_t WhiteKing::get_attacked_pos(uint64_t position, uint64_t team, uint64_t enemy) {
    uint64_t attack = 0;
    uint64_t p = position;
    
    while (p > 0) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);
        
        uint64_t possible_moves = king_moves[sq];
        uint64_t occupancy = possible_moves & ~team;
        attack |= occupancy;
    }
    return attack;
}

bool WhiteKing::is_under_check(uint64_t position, uint64_t team, uint64_t enemy, uint64_t bitboard[]) {
    uint64_t p = position;
    int sq = get_lsb_idx(p);
    uint64_t occupancy = team | enemy;
    
    uint64_t bishopOcc = occupancy & bishop_masks[sq];
    bishopOcc *= bishop_magic[sq];
    bishopOcc >>= 64 - bishop_bit_counts[sq];
    uint64_t bishop_attacks = bishop_moves[sq][bishopOcc];
    
    uint64_t rookOcc = occupancy & rook_masks[sq];
    rookOcc *= rook_magic[sq];
    rookOcc >>= 64 - rook_bit_counts[sq];
    uint64_t rook_attacks = rook_moves[sq][rookOcc];

    uint64_t knight_attacks = knight_moves[sq];

    uint64_t pawn_attacks = white_pawn_captures[sq];

    uint64_t king_attacks = king_moves[sq];
    
    if (bitboard[6] & pawn_attacks) return 0;
    if (bitboard[7] & knight_attacks) return 0;
    if (bitboard[8] & bishop_attacks) return 0;
    if (bitboard[9] & rook_attacks) return 0;
    if (bitboard[10] & (bishop_attacks | rook_attacks)) return 0;
    if (bitboard[11] & king_attacks) return 0;
    return 1;
}

char WhiteKing::get_symbol() const {
    return 'K';
}

int WhiteKing::get_value() const {
    return 0;
}

bool WhiteKing::getHasMoved() const {
    return false;
}

void WhiteKing::setHasMoved(bool moved) {
} 