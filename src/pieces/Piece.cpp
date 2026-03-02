#include "Piece.h"
#include "BitUtil.h"

void add_moves(std::vector<Move>& moves, int sq, uint64_t occupancy, uint64_t enemy) {
    while (occupancy) {
        int to = get_lsb_idx(occupancy);
        occupancy = clear_lsb(occupancy);
        uint64_t info = (sq | (to << 6)) << 7;
        moves.push_back(Move(info)); 
    }
}