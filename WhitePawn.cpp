#include "WhitePawn.h"
#include "BitUtil.h"
using namespace std;

WhitePawn::WhitePawn(uint64_t position) 
    : Piece(position, true) {}

std::vector<Move> WhitePawn::generateMoves(uint64_t position, uint64_t team, uint64_t enemy) {
    std::vector<Move> moves;
    uint64_t p = position;
    
    while (p > 0) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);
        uint64_t occupancy = ((white_pawn_moves[sq] & ~enemy) | (white_pawn_captures[sq] & (enemy))) & ~team;
        this->addMoves(moves, sq, occupancy);
    }
    
    return moves;
}

char WhitePawn::getSymbol() const {
    return 'P';
}

int WhitePawn::getValue() const {
    return 1;
}