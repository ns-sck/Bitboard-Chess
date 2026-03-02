#ifndef BIT_UTIL_H
#define BIT_UTIL_H

#include <cstdint>
#include <vector>

#define WHITE 0
#define BLACK 1

// File masks
extern const uint64_t FILE_A;
extern const uint64_t FILE_B;
extern const uint64_t FILE_C;
extern const uint64_t FILE_D;
extern const uint64_t FILE_E;
extern const uint64_t FILE_F;
extern const uint64_t FILE_G;
extern const uint64_t FILE_H;

// Rank masks
extern const uint64_t RANK_1;
extern const uint64_t RANK_2;
extern const uint64_t RANK_3;
extern const uint64_t RANK_4;
extern const uint64_t RANK_5;
extern const uint64_t RANK_6;
extern const uint64_t RANK_7;
extern const uint64_t RANK_8;

int get_lsb_idx(uint64_t bitboard);
int get_msb(uint64_t bitboard);
uint64_t get_lsb(uint64_t bitboard);
uint64_t clear_lsb(uint64_t bitboard);
int pop_count(uint64_t bitboard);

inline void embed(uint64_t& x, int l, int r, int y) {
    x &= ~(((1ull << (r - l + 1)) - 1) << l);
    x |= (y << l);
}

inline void embed(uint16_t& x, int l, int r, int y) {
    x &= ~(((1ull << (r - l + 1)) - 1) << l);
    x |= (y << l);
}

inline int get_mask(uint64_t x, int l, int r) {
    return (x >> l) & ((1ull << (r - l + 1)) - 1);
}

extern uint64_t knight_moves[64];
extern uint64_t king_moves[64];
extern uint64_t pawn_moves[2][64];
extern uint64_t pawn_captures[2][64];

extern const uint64_t rook_magic[64];
extern const uint64_t bishop_magic[64];
extern const int bishop_bit_counts[64];
extern const int rook_bit_counts[64];

extern uint64_t rook_moves[64][4096];
extern uint64_t bishop_moves[64][4096];
extern uint64_t rook_masks[64];
extern uint64_t bishop_masks[64];
extern std::string square_to_algebraic[64];

enum Square {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
};

uint64_t slide_the_rook(int sq);
uint64_t slide_the_bishop(int sq);
void init();
void init_king_moves();
void init_knight_moves();
void init_pawn_moves();
void init_sliders_moves();

#endif