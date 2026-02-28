#include "WhiteKing.h"
#include "BitUtil.h"
using namespace std;

WhiteKing::WhiteKing(uint64_t position) 
    : Piece(position, true) {}

std::vector<Move> WhiteKing::generate_moves(uint64_t position, uint64_t team, uint64_t enemy) {
    std::vector<Move> moves;
    uint64_t p = position;
    
    int sq = get_lsb_idx(p);
    
    uint64_t possible_moves = king_moves[sq];
    uint64_t occupancy = possible_moves & ~team;
    this->add_moves(moves, sq, occupancy, enemy);
    return moves;
}

bool WhiteKing::is_king_safe(uint64_t position, uint64_t team, uint64_t enemy, uint64_t bitboard[]) {
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
    uint64_t pawn_attacks = white_pawn_captures[sq];
    uint64_t king_attacks = king_moves[sq];
    
    if (bitboard[7] & pawn_attacks) return 0;
    if (bitboard[8] & knight_attacks) return 0;
    if (bitboard[9] & bishop_attacks) return 0;
    if (bitboard[10] & rook_attacks) return 0;
    if (bitboard[11] & (bishop_attacks | rook_attacks)) return 0;
    if (bitboard[12] & king_attacks) return 0;
    return 1;
}

char WhiteKing::get_symbol() const {
    return 'K';
}

int WhiteKing::get_value() const {
    return 0;
}