#include "WhiteBishop.h"
#include "BitUtil.h"
using namespace std;

WhiteBishop::WhiteBishop(uint64_t position) 
    : Piece(position, true) {}

std::vector<Move> WhiteBishop::generateMoves(uint64_t position, uint64_t team, uint64_t enemy) {
    std::vector<Move> moves;
    uint64_t p = position;
    while (p) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);
        uint64_t occupancy = team | enemy;
        occupancy &= bishop_masks[sq];
        occupancy *= bishop_magic[sq];
        occupancy >>= 64 - bishop_bit_counts[sq];
        occupancy = bishop_moves[sq][occupancy];
        occupancy &= ~team;
        this->addMoves(moves, sq, occupancy);
    }
    return moves;
}

char WhiteBishop::getSymbol() const {
    return 'B';
}

int WhiteBishop::getValue() const {
    return 3;
} 