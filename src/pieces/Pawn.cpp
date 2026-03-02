#include "Pawn.h"
#include "BitUtil.h"

#include "Piece.h"

std::vector<Move> Pawn::generate_moves(uint64_t position, uint64_t team, uint64_t enemy, bool color) {
    std::vector<Move> moves;
    uint64_t p = position;
    
    while (p > 0) {
        int sq = get_lsb_idx(p);
        p = clear_lsb(p);
        uint64_t slide = pawn_moves[color][sq] & ~enemy;
        if (sq <= 55 && sq >= 8 && (enemy & (1ull << (sq + 8 - 16 * color)))) slide = 0;
        uint64_t attack = (slide | (pawn_captures[color][sq] & (enemy))) & ~team;
        while (attack) {
            int to = get_lsb_idx(attack);
            attack = clear_lsb(attack);
            uint64_t info = (sq | (to << 6)) << 7;
            if (to < 8 || to > 55) {
                for (int i = 2; i <= 5; ++i) {
                    int j = color ? i + 6 : i;
                    moves.push_back(Move(info | j));
                }
            } else {
                moves.push_back(Move(info)); 
            }
        }
    }
    
    return moves;
}

char Pawn::get_symbol() {
    return 'P';
}

int Pawn::get_value() {
    return 1;
}