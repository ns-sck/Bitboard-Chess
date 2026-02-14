#include "WhitePawn.h"
#include "BitUtil.h"
using namespace std;

WhitePawn::WhitePawn(uint64_t position) 
    : Piece(position, true) {}

std::vector<Move> WhitePawn::generate_moves(uint64_t position, uint64_t team, uint64_t enemy) {
    std::vector<Move> moves;
    uint64_t p = position;
    
    while (p > 0) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);
        uint64_t occupancy = ((white_pawn_moves[sq] & ~enemy) | (white_pawn_captures[sq] & (enemy))) & ~team;
        this->add_moves(moves, sq, occupancy, enemy);
    }
    
    return moves;
}

uint64_t WhitePawn::get_attacked_pos(uint64_t position, uint64_t team, uint64_t enemy) {
    uint64_t attack = 0;
    uint64_t p = position;
    
    while (p > 0) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);
        uint64_t occupancy = white_pawn_captures[sq];
        attack |= occupancy;
    }
    
    return attack;
}

char WhitePawn::get_symbol() const {
    return 'P';
}

int WhitePawn::get_value() const {
    return 1;
}