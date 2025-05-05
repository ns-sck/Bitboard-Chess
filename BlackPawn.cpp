#include "BlackPawn.h"
#include "BitUtil.h"
using namespace std;

BlackPawn::BlackPawn(uint64_t position) 
    : Piece(position, false) {}

std::vector<Move> BlackPawn::generateMoves(uint64_t position, uint64_t team, uint64_t enemy) {
    std::vector<Move> moves;
    uint64_t p = position;
    
    while (p > 0) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);
        uint64_t occupancy = ((black_pawn_moves[sq] & ~enemy) | (black_pawn_captures[sq] & enemy)) & ~team;
        this->addMoves(moves, sq, occupancy);
    }
    
    return moves;
}

char BlackPawn::getSymbol() const {
    return 'p';
}

int BlackPawn::getValue() const {
    return 1;
}