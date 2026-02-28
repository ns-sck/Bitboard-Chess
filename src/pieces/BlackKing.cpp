#include "BlackKing.h"
#include "BitUtil.h"
using namespace std;

BlackKing::BlackKing(uint64_t position) 
    : Piece(position, false), has_moved(false) {}

vector<Move> BlackKing::generate_moves(uint64_t position, uint64_t team, uint64_t enemy) {
    vector<Move> moves;
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

bool BlackKing::is_king_safe(uint64_t position, uint64_t team, uint64_t enemy, uint64_t bitboard[]) {
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
    uint64_t pawn_attacks = black_pawn_captures[sq];
    uint64_t king_attacks = king_moves[sq];
    
    if (bitboard[1] & pawn_attacks) return 0;
    if (bitboard[2] & knight_attacks) return 0;
    if (bitboard[3] & bishop_attacks) return 0;
    if (bitboard[4] & rook_attacks) return 0;
    if (bitboard[5] & (bishop_attacks | rook_attacks)) return 0;
    if (bitboard[6] & king_attacks) return 0;
    return 1;
}

char BlackKing::get_symbol() const {
    return 'k';
}

int BlackKing::get_value() const {
    return 0;
}
