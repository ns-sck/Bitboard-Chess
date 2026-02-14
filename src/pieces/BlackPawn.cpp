#include "BlackPawn.h"
#include "BitUtil.h"
using namespace std;

BlackPawn::BlackPawn(uint64_t position) 
    : Piece(position, false) {}

std::vector<Move> BlackPawn::generate_moves(uint64_t position, uint64_t team, uint64_t enemy) {
    std::vector<Move> moves;
    uint64_t p = position;
    
    while (p > 0) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);
        uint64_t occupancy = ((black_pawn_moves[sq] & ~enemy) | (black_pawn_captures[sq] & enemy)) & ~team;
        this->add_moves(moves, sq, occupancy, enemy);
    }
    
    return moves;
}

uint64_t BlackPawn::get_attacked_pos(uint64_t position, uint64_t team, uint64_t enemy) {
    uint64_t attack = 0;
    uint64_t p = position;
    
    while (p > 0) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);
        uint64_t occupancy = black_pawn_captures[sq];
        attack |= occupancy;
    }
    
    return attack;
}

char BlackPawn::get_symbol() const {
    return 'p';
}

int BlackPawn::get_value() const {
    return 1;
}