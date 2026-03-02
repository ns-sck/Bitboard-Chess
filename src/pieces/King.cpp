#include "King.h"
#include "BitUtil.h"

std::vector<Move> King::generate_moves(uint64_t position, uint64_t team, uint64_t enemy, bool color) {
    std::vector<Move> moves;
    uint64_t p = position;
    
    int sq = get_lsb_idx(p);
    
    uint64_t possible_moves = king_moves[sq];
    uint64_t occupancy = possible_moves & ~team;
    add_moves(moves, sq, occupancy, enemy);
    return moves;
}

bool King::is_safe(uint64_t position, uint64_t team, uint64_t enemy, uint64_t bitboard[], bool color) {
    uint64_t p = position;
    int sq = get_lsb_idx(p);
    uint64_t occupancy = team | enemy;
    
    uint64_t bishop_occ = occupancy & bishop_masks[sq];
    bishop_occ *= bishop_magic[sq];
    bishop_occ >>= 64 - bishop_bit_counts[sq];
    uint64_t bishop_attacks = bishop_moves[sq][bishop_occ];
    
    uint64_t rook_occ = occupancy & rook_masks[sq];
    rook_occ *= rook_magic[sq];
    rook_occ >>= 64 - rook_bit_counts[sq];
    uint64_t rook_attacks = rook_moves[sq][rook_occ];

    uint64_t knight_attacks = knight_moves[sq];
    uint64_t pawn_attacks = pawn_captures[color][sq];
    uint64_t king_attacks = king_moves[sq];
    
    int j = color ? 6 : 0;
    if (bitboard[7 - j] & pawn_attacks) return 0;
    if (bitboard[8 - j] & knight_attacks) return 0;
    if (bitboard[9 - j] & bishop_attacks) return 0;
    if (bitboard[10 - j] & rook_attacks) return 0;
    if (bitboard[11 - j] & (bishop_attacks | rook_attacks)) return 0;
    if (bitboard[12 - j] & king_attacks) return 0;
    return 1;
}

char King::get_symbol() {
    return 'K';
}

int King::get_value() {
    return 0;
}