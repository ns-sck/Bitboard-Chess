#ifndef BIT_UTIL_H
#define BIT_UTIL_H

#include <cstdint>
#include <vector>

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
int popCount(uint64_t bitboard);

extern uint64_t knight_moves[64];
extern uint64_t king_moves[64];
extern uint64_t white_pawn_moves[64];
extern uint64_t white_pawn_captures[64];
extern uint64_t black_pawn_moves[64];
extern uint64_t black_pawn_captures[64];

extern const uint64_t rook_magic[64];
extern const uint64_t bishop_magic[64];
extern const int bishop_bit_counts[64];
extern const int rook_bit_counts[64];

extern uint64_t rook_moves[64][4096];
extern uint64_t bishop_moves[64][4096];
extern uint64_t rook_masks[64];
extern uint64_t bishop_masks[64];
extern std::string square_to_algebraic[64];

uint64_t slide_the_rook(int sq);
uint64_t slide_the_bishop(int sq);
void init();
void init_king_moves();
void init_knight_moves();
void init_pawn_moves();
void init_sliders_moves();

#endif
