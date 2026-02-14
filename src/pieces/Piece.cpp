#include "Piece.h"
#include "BitUtil.h"

Piece::Piece(uint64_t position, bool isWhite) 
    : position(position), isWhite(isWhite) {}

uint64_t Piece::get_position() const {
    return position;
}

void Piece::set_position(uint64_t newPosition) {
    position = newPosition;
}

bool Piece::is_white() const {
    return isWhite;
}

void Piece::add_moves(std::vector<Move>& moves, int sq, uint64_t occupancy, uint64_t enemy) const {
    while (occupancy) {
        int to = get_lsb_idx(occupancy);
        occupancy = clear_lsb(occupancy);
        uint64_t info = sq | (to << 6);
        if ((enemy >> to) & 1) info |= (1ull << 20);
        moves.push_back(Move(info)); 
    }
}