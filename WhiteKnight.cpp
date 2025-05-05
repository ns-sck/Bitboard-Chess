#include "WhiteKnight.h"
#include "BitUtil.h"
using namespace std;

WhiteKnight::WhiteKnight(uint64_t position) 
    : Piece(position, true) {}

std::vector<Move> WhiteKnight::generateMoves(uint64_t position, uint64_t team, uint64_t enemy) {
    std::vector<Move> moves;
    uint64_t p = position;
    
    while (p > 0) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);

        uint64_t possibleMoves = knight_moves[sq];
        uint64_t occupancy = possibleMoves & ~team;
        this->addMoves(moves, sq, occupancy);
    }
    return moves;
}

char WhiteKnight::getSymbol() const {
    return 'N';
}

int WhiteKnight::getValue() const {
    return 3;
} 