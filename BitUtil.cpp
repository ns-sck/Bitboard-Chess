#include <cstdint>
#include <vector>
#include <string>
#include "BitUtil.h"
#include <iostream>

std::string square_to_algebraic[64] = {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
};

const uint64_t FILE_A = 0x0101010101010101ULL;
const uint64_t FILE_B = 0x0202020202020202ULL;
const uint64_t FILE_C = 0x0404040404040404ULL;
const uint64_t FILE_D = 0x0808080808080808ULL;
const uint64_t FILE_E = 0x1010101010101010ULL;
const uint64_t FILE_F = 0x2020202020202020ULL;
const uint64_t FILE_G = 0x4040404040404040ULL;
const uint64_t FILE_H = 0x8080808080808080ULL;

const uint64_t RANK_1 = 0x00000000000000FFULL;
const uint64_t RANK_2 = 0x000000000000FF00ULL;
const uint64_t RANK_3 = 0x0000000000FF0000ULL;
const uint64_t RANK_4 = 0x00000000FF000000ULL;
const uint64_t RANK_5 = 0x00000000FF000000ULL;
const uint64_t RANK_6 = 0x0000FF0000000000ULL;
const uint64_t RANK_7 = 0x00FF000000000000ULL;
const uint64_t RANK_8 = 0xFF00000000000000ULL;

int get_lsb_idx(uint64_t bitboard) {
    if (bitboard == 0) return -1;
    return __builtin_ctzll(bitboard);
}

int get_msb(uint64_t bitboard) {
    if (bitboard == 0) return -1;
    return 63 - __builtin_clzll(bitboard);
}

uint64_t get_lsb(uint64_t bitboard) {
    return bitboard & -bitboard;
}

uint64_t clear_lsb(uint64_t bitboard) {
    return bitboard & (bitboard - 1);
}

int popCount(uint64_t bitboard) {
    return __builtin_popcountll(bitboard);
}

// https://github.com/maksimKorzh/bbc/blob/master/src/bbc_nnue/bbc.c#L1219
const uint64_t rook_magic[64] = {
    0x8a80104000800020ULL,
    0x140002000100040ULL,
    0x2801880a0017001ULL,
    0x100081001000420ULL,
    0x200020010080420ULL,
    0x3001c0002010008ULL,
    0x8480008002000100ULL,
    0x2080088004402900ULL,
    0x800098204000ULL,
    0x2024401000200040ULL,
    0x100802000801000ULL,
    0x120800800801000ULL,
    0x208808088000400ULL,
    0x2802200800400ULL,
    0x2200800100020080ULL,
    0x801000060821100ULL,
    0x80044006422000ULL,
    0x100808020004000ULL,
    0x12108a0010204200ULL,
    0x140848010000802ULL,
    0x481828014002800ULL,
    0x8094004002004100ULL,
    0x4010040010010802ULL,
    0x20008806104ULL,
    0x100400080208000ULL,
    0x2040002120081000ULL,
    0x21200680100081ULL,
    0x20100080080080ULL,
    0x2000a00200410ULL,
    0x20080800400ULL,
    0x80088400100102ULL,
    0x80004600042881ULL,
    0x4040008040800020ULL,
    0x440003000200801ULL,
    0x4200011004500ULL,
    0x188020010100100ULL,
    0x14800401802800ULL,
    0x2080040080800200ULL,
    0x124080204001001ULL,
    0x200046502000484ULL,
    0x480400080088020ULL,
    0x1000422010034000ULL,
    0x30200100110040ULL,
    0x100021010009ULL,
    0x2002080100110004ULL,
    0x202008004008002ULL,
    0x20020004010100ULL,
    0x2048440040820001ULL,
    0x101002200408200ULL,
    0x40802000401080ULL,
    0x4008142004410100ULL,
    0x2060820c0120200ULL,
    0x1001004080100ULL,
    0x20c020080040080ULL,
    0x2935610830022400ULL,
    0x44440041009200ULL,
    0x280001040802101ULL,
    0x2100190040002085ULL,
    0x80c0084100102001ULL,
    0x4024081001000421ULL,
    0x20030a0244872ULL,
    0x12001008414402ULL,
    0x2006104900a0804ULL,
    0x1004081002402ULL
};

// https://github.com/maksimKorzh/bbc/blob/master/src/bbc_nnue/bbc.c#L1219
const uint64_t bishop_magic[64] = {
    0x40040844404084ULL,
    0x2004208a004208ULL,
    0x10190041080202ULL,
    0x108060845042010ULL,
    0x581104180800210ULL,
    0x2112080446200010ULL,
    0x1080820820060210ULL,
    0x3c0808410220200ULL,
    0x4050404440404ULL,
    0x21001420088ULL,
    0x24d0080801082102ULL,
    0x1020a0a020400ULL,
    0x40308200402ULL,
    0x4011002100800ULL,
    0x401484104104005ULL,
    0x801010402020200ULL,
    0x400210c3880100ULL,
    0x404022024108200ULL,
    0x810018200204102ULL,
    0x4002801a02003ULL,
    0x85040820080400ULL,
    0x810102c808880400ULL,
    0xe900410884800ULL,
    0x8002020480840102ULL,
    0x220200865090201ULL,
    0x2010100a02021202ULL,
    0x152048408022401ULL,
    0x20080002081110ULL,
    0x4001001021004000ULL,
    0x800040400a011002ULL,
    0xe4004081011002ULL,
    0x1c004001012080ULL,
    0x8004200962a00220ULL,
    0x8422100208500202ULL,
    0x2000402200300c08ULL,
    0x8646020080080080ULL,
    0x80020a0200100808ULL,
    0x2010004880111000ULL,
    0x623000a080011400ULL,
    0x42008c0340209202ULL,
    0x209188240001000ULL,
    0x400408a884001800ULL,
    0x110400a6080400ULL,
    0x1840060a44020800ULL,
    0x90080104000041ULL,
    0x201011000808101ULL,
    0x1a2208080504f080ULL,
    0x8012020600211212ULL,
    0x500861011240000ULL,
    0x180806108200800ULL,
    0x4000020e01040044ULL,
    0x300000261044000aULL,
    0x802241102020002ULL,
    0x20906061210001ULL,
    0x5a84841004010310ULL,
    0x4010801011c04ULL,
    0xa010109502200ULL,
    0x4a02012000ULL,
    0x500201010098b028ULL,
    0x8040002811040900ULL,
    0x28000010020204ULL,
    0x6000020202d0240ULL,
    0x8918844842082200ULL,
    0x4010011029020020ULL
};

const int bishop_bit_counts[64] = {
    6, 5, 5, 5, 5, 5, 5, 6, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    6, 5, 5, 5, 5, 5, 5, 6
};

const int rook_bit_counts[64] = {
    12, 11, 11, 11, 11, 11, 11, 12, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    12, 11, 11, 11, 11, 11, 11, 12
};

uint64_t rook_moves[64][4096];
uint64_t bishop_moves[64][4096];
uint64_t rook_masks[64];
uint64_t bishop_masks[64];
uint64_t knight_moves[64];
uint64_t king_moves[64];
uint64_t white_pawn_moves[64];
uint64_t white_pawn_captures[64];
uint64_t black_pawn_moves[64];
uint64_t black_pawn_captures[64];

void init() {
    init_king_moves();
    init_knight_moves();
    init_pawn_moves();
    init_sliders_moves();
}

void init_knight_moves() {
    int dir[8][2] = {{-2, 1}, {-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1}};
    for (int i = 0; i < 64; ++i) {
        for (auto [x, y] : dir) {
            int r = (i / 8) + x;
            int c = (i % 8) + y;
            if (r >= 0 && r < 8 && c >= 0 && c < 8) {
                knight_moves[i] |= 1ull << (r * 8 + c);
            }
        }
    }
}

void init_pawn_moves() {
    for (int i = 8; i < 56; ++i) {
        white_pawn_moves[i] |= 1ull << (i + 8);
        if (i < 16) {
            white_pawn_moves[i] |= 1ull << (i + 16);
        }
        if (i % 8 != 0) {
            white_pawn_captures[i] |= 1ull << (i + 7);
        }
        if (i % 8 != 7) {
            white_pawn_captures[i] |= 1ull << (i + 9);
        }
    }
    for (int i = 8; i < 56; ++i) {
        black_pawn_moves[i] |= 1ull << (i - 8);
        if (i >= 48) {
            black_pawn_moves[i] |= 1ull << (i - 16);
        }
        if (i % 8 != 0) {
            black_pawn_captures[i] |= 1ull << (i - 9);
        }
        if (i % 8 != 7) {
            black_pawn_captures[i] |= 1ull << (i - 7);
        }
    }
}

void init_king_moves() {
    int dir[8][2] = {{-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}};
    for (int i = 0; i < 64; ++i) {
        for (auto [x, y] : dir) {
            int r = (i / 8) + x;
            int c = (i % 8) + y;
            if (r >= 0 && r < 8 && c >= 0 && c < 8) {
                king_moves[i] |= 1ull << (r * 8 + c);
            }
        }
    }
}

uint64_t slide_rook(int sq, uint64_t block) {
    uint64_t occupancy = 0;
    int e = block == 0;
    int r = sq / 8, c = sq % 8;
    for (int i = r + 1; i < 8 - e; ++i) {
        occupancy |= 1ull << (i * 8 + c);
        if (block & (1ull << (i * 8 + c))) break;
    }
    for (int i = r - 1; i >= 0 + e; --i) {
        occupancy |= 1ull << (i * 8 + c);
        if (block & (1ull << (i * 8 + c))) break;
    }
    for (int j = c + 1; j < 8 - e; ++j) {
        occupancy |= 1ull << (r * 8 + j);
        if (block & (1ull << (r * 8 + j))) break;
    }
    for (int j = c - 1; j >= 0 + e; --j) {
        occupancy |= 1ull << (r * 8 + j);
        if (block & (1ull << (r * 8 + j))) break;
    }
    if (block == 0) {
        rook_masks[sq] = occupancy;
    }
    return occupancy;
}

uint64_t slide_bishop(int sq, uint64_t block) {
    uint64_t occupancy = 0;
    int e = block == 0;
    int r = sq / 8, c = sq % 8;
    for (int i = r + 1, j = c + 1; i < 8 - e && j < 8 - e; ++i, ++j) {
        occupancy |= 1ull << (i * 8 + j);
        if (block & (1ull << (i * 8 + j))) break;
    }
    for (int i = r + 1, j = c - 1; i < 8 - e && j >= 0 + e; ++i, --j) {
        occupancy |= 1ull << (i * 8 + j);
        if (block & (1ull << (i * 8 + j))) break;
    }
    for (int i = r - 1, j = c - 1; i >= 0 + e && j >= 0 + e; --i, --j) {
        occupancy |= 1ull << (i * 8 + j);
        if (block & (1ull << (i * 8 + j))) break;
    }
    for (int i = r - 1, j = c + 1; i >= 0 + e && j < 8 - e; --i, ++j) {
        occupancy |= 1ull << (i * 8 + j);
        if (block & (1ull << (i * 8 + j))) break;
    }
    if (block == 0) {
        bishop_masks[sq] = occupancy;
    }
    return occupancy;
}

void init_sliders_moves() {
    for (int i = 0; i < 64; ++i) {
        // rook
        {   
            int count = rook_bit_counts[i];
            uint64_t occupancy = slide_rook(i, 0);
            
            for (int j = 0; j < (1 << count); ++j) {
                uint64_t dummy = occupancy;
                uint64_t ans = 0;
                for (int k = 0; k < count; ++k) {
                    if ((j >> k) & 1) {
                        ans |= get_lsb(dummy);
                    }
                    dummy = clear_lsb(dummy);
                }
                int idx = (ans * rook_magic[i]) >> (64 - rook_bit_counts[i]);
                rook_moves[i][idx] = slide_rook(i, ans);
            }
        }
        // bishop
        {
            int count = bishop_bit_counts[i];
            uint64_t occupancy = slide_bishop(i, 0);
            for (int j = 0; j < (1 << count); ++j) {
                uint64_t dummy = occupancy;
                uint64_t ans = 0;
                for (int k = 0; k < count; ++k) {
                    if ((j >> k) & 1) {
                        ans |= get_lsb(dummy);
                    }
                    dummy = clear_lsb(dummy);
                }
                int idx = (ans * bishop_magic[i]) >> (64 - bishop_bit_counts[i]);
                bishop_moves[i][idx] = slide_bishop(i, ans);
            }
        }
    }
}