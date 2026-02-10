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

void Piece::add_moves(std::vector<Move>& moves, int sq, uint64_t occupancy) const {
    while (occupancy) {
        int to = get_lsb_idx(occupancy);
        occupancy = clear_lsb(occupancy);
        int info = sq | (to << 6);
        moves.push_back(Move(info)); 
    }
}