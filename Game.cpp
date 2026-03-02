#include "Game.h"
#include "BitUtil.h"
#include "Queen.h"
#include "Knight.h"
#include "Rook.h"
#include "Bishop.h"
#include "King.h"
#include "Pawn.h"
#include <iostream>

Game::Game() {
    reset_board();
}

void Game::reset_board() {

    bitboard[WP] = ((1ULL << 8) - 1) << 8;
    bitboard[WN] = (1ULL << 1) | (1ULL << 6);
    bitboard[WB] = (1ULL << 2) | (1ULL << 5);
    bitboard[WR] = (1ULL << 0) | (1ULL << 7);
    bitboard[WQ] = (1ULL << 3);
    bitboard[WK] = (1ULL << 4);  
    
    bitboard[BP] = ((1ULL << 8) - 1) << 48;
    bitboard[BN] = (1ULL << 57) | (1ULL << 62);
    bitboard[BB] = (1ULL << 58) | (1ULL << 61);
    bitboard[BR] = (1ULL << 56) | (1ULL << 63);
    bitboard[BQ] = (1ULL << 59);
    bitboard[BK] = (1ULL << 60);
    
    white_occ = bitboard[WP] | bitboard[WN] | 
                bitboard[WB] | bitboard[WR] | 
                bitboard[WQ] | bitboard[WK];
    
    black_occ = bitboard[BP] | bitboard[BN] | 
                bitboard[BB] | bitboard[BR] | 
                bitboard[BQ] | bitboard[BK];
    
    for (int i = 16; i < 48; ++i) {
        types[i] = 0;
    }
    for (int i = 1; i <= 12; ++i) {
        uint64_t p = bitboard[i];
        while (p) {
            int sq = get_lsb_idx(p);
            p = clear_lsb(p); 
            types[sq] = i;
        }
    } 

    white_to_move = true;
    game_state = GameState::ACTIVE;
    full_move_number = 1;

    state = 0;
    state_stack = {0};
    
    evaluation = 0.0f;
}

bool Game::is_game_over() const {
    return game_state != GameState::ACTIVE && game_state != GameState::CHECK;
}
GameState Game::get_state() const {
    return game_state;
}

bool Game::is_white_to_move() const {
    return white_to_move;
}

void Game::print_board() const {
    std::cout << "\n  +---+---+---+---+---+---+---+---+\n";
    
    std::string board[64];
    for (int i = 0; i < 64; i++) {
        board[i] = ' ';
    }
    
    for (int pieceType = 0; pieceType < TYPE_COUNT; pieceType++) {
        uint64_t bb = bitboard[pieceType];
        std::string symbol = pieceSymbols[pieceType];
        
        while (bb) {
            int square = get_lsb_idx(bb);
            board[square] = symbol;
            bb &= bb - 1;
        }
    }
    
    for (int rank = 7; rank >= 0; rank--) {
        std::cout << (rank + 1) << " |";
        
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            std::cout << " " << board[square] << " |";
        }
        
        std::cout << "\n  +---+---+---+---+---+---+---+---+\n";
    }
    
    std::cout << "    a   b   c   d   e   f   g   h  \n\n";
    
    std::cout << "Turn: " << (white_to_move ? "White" : "Black") << std::endl;
}

const move_gen_func move_generators[7] = {
    nullptr,
    Pawn::generate_moves,
    Knight::generate_moves,
    Bishop::generate_moves,
    Rook::generate_moves,
    Queen::generate_moves,
    King::generate_moves
};

std::vector<Move> Game::generate_legal_moves() {
    std::vector<Move> moves;
    uint64_t team = white_to_move ? white_occ : black_occ;
    uint64_t enemy = white_to_move ? black_occ : white_occ;
    uint64_t en_passant = 1ull << get_mask(state, 10, 15);

    std::cout << en_passant << '\n';
    
    for (int i = 1; i <= 6; ++i) {
        int j = i;
        if (!white_to_move) {
            j += 6;
        }
        std::vector<Move> pseudo_moves = move_generators[i](bitboard[j], team, enemy | (i == 1 ? en_passant : 0), !white_to_move);
        for (auto& m : pseudo_moves) {
            if (check_move(m)) {
                moves.push_back(m);
            }
        }
    }
    if (white_to_move) generate_castlings<true>(moves);
    else generate_castlings<false>(moves);
    return moves;
}

template <bool W>
void Game::generate_castlings(std::vector<Move>& moves) {
    uint64_t occ = white_occ | black_occ;

    bool ks_mask = occ & (3ull << (W ? 5 : 61));
    bool qs_mask = occ & (7ull << (W ? 1 : 57));

    uint64_t team = W ? white_occ : black_occ;
    uint64_t enemy = W ? black_occ : white_occ;

    constexpr int c = W ? c1 : c8;
    constexpr int d = W ? d1 : d8;
    constexpr int e = W ? e1 : e8;
    constexpr int f = W ? f1 : f8;
    constexpr int g = W ? g1 : g8;

    constexpr int ks_bit = W ? 0 : 2;
    constexpr int qs_bit = W ? 1 : 3;

    if (!get_mask(state, ks_bit, ks_bit) &&
        !ks_mask &&
        King::is_safe(1ull << e, team, enemy, bitboard, !W) &&
        King::is_safe(1ull << f, team, enemy, bitboard, !W) &&
        King::is_safe(1ull << g, team, enemy, bitboard, !W)) {
            moves.push_back((e | (g << 6)) << 7);
        }
    if (!get_mask(state, qs_bit, qs_bit) &&
            !qs_mask &&
            King::is_safe(1ull << e, team, enemy, bitboard, !W) &&
            King::is_safe(1ull << d, team, enemy, bitboard, !W) &&
            King::is_safe(1ull << c, team, enemy, bitboard, !W)) {
                moves.push_back((e | (c << 6)) << 7);
            }
}

void Game::handle_en_passant(int from, int to, uint64_t& info) {
    int en_passant_square = get_mask(state, 10, 15);

    int OP = white_to_move ? WP : BP;
    int EP = white_to_move ? BP : WP;
    int offset = white_to_move ? 8 : -8;

    if ((1ull << to) & bitboard[OP]) {
        if (to == from + offset * 2) {
            embed(new_state, 10, 15, from + offset);
        }
        if (to == en_passant_square) {
            bitboard[EP] &= ~(1ull << (en_passant_square - offset));
            types[en_passant_square - offset] = EMPTY;
            embed(info, 6, 6, 1);
        }
    }
}

void Game::unmake_en_passant(uint64_t& info, int to) {
    bool white_to_unmake = !white_to_move;

    int EP = white_to_unmake ? BP : WP;
    int offset = white_to_unmake ? 8 : -8;

    if (get_mask(info, 6, 6)) {
        bitboard[EP] |= 1ull << (to - offset);
        types[to - offset] = EP;
    } 
}

void Game::handle_castling(uint64_t& info, int from, int to, int src) {
    int K = white_to_move ? WK : BK;
    int R = white_to_move ? WR : BR;

    if (src ^ K && src ^ R) return;

    int a = white_to_move ? a1 : a8;
    int c = white_to_move ? c1 : c8;
    int d = white_to_move ? d1 : d8;
    int e = white_to_move ? e1 : e8;
    int f = white_to_move ? f1 : f8;
    int g = white_to_move ? g1 : g8;
    int h = white_to_move ? h1 : h8;

    int ks_bit = white_to_move ? 0 : 2;
    int qs_bit = white_to_move ? 1 : 3;
    
    if (src == K) {
        if (from == e && to == g) {
            bitboard[R] &= ~(1ull << h);   
            bitboard[R] |= (1ull << f);
            types[h] = EMPTY;
            types[f] = R;
            embed(info, 27, 27, 1); 
        } else if (from == e && to == c) {
            bitboard[R] &= ~(1ull << a);
            bitboard[R] |= 1ull << d;
            types[a] = EMPTY;
            types[d] = R;
            embed(info, 28, 28, 1);
        }
        embed(new_state, ks_bit, qs_bit, 3);
    } else if (src == R && from == h) {
        embed(new_state, ks_bit, ks_bit, 1);
    } else if (src == R && from == a) {
        embed(new_state, qs_bit, qs_bit, 1);
    }
}

void Game::unmake_castling(uint64_t& info) {
    bool white_to_unmake = !white_to_move;

    int R = white_to_unmake ? WR : BR;
    int a = white_to_unmake ? a1 : a8;
    int d = white_to_unmake ? d1 : d8;
    int f = white_to_unmake ? f1 : f8;
    int h = white_to_unmake ? h1 : h8;

    if (get_mask(info, 27, 27)) {
        bitboard[R] |= 1ull << h;
        bitboard[R] &= ~(1ull << f);
        types[f] = EMPTY;
        types[h] = R;
    } else if (get_mask(info, 28, 28)) {
        bitboard[R] |= 1ull << a;
        bitboard[R] &= ~(1ull << d);
        types[d] = EMPTY;
        types[a] = R; 
    }
}

void Game::handle_promotion(uint64_t& info, int to) {
    int piece = get_mask(info, 0, 3);
    if (piece) {
        int P = white_to_move ? WP : BP;
        bitboard[P] &= ~(1ull << to);
        bitboard[piece] |= (1ull << to);
        types[to] = piece;
    }
}

void Game::unmake_promotion(uint64_t& info, int to) {
    int piece = get_mask(info, 0, 3);
    if (piece) {
        bitboard[piece] &= ~(1ull << to);
    }
}

void Game::make_move(Move& move) {
    uint64_t info = move.get_info();
    int from = get_mask(info, 7, 12);
    int to = get_mask(info, 13, 18);

    new_state = state_stack.back() & 1023;

    embed(info, 19, 22, types[from]);
    embed(info, 23, 26, types[to]);

    int src = types[from];
    int dst = types[to];

    if (dst) embed(info, 5, 5, 1);
    
    bitboard[dst] &= ~(1ull << to);
    bitboard[src] &= ~(1ull << from);
    bitboard[src] |= (1ull << to);
    types[to] = src;
    types[from] = EMPTY;
    
    handle_castling(info, from, to, src);
    handle_en_passant(from, to, info);
    handle_promotion(info, to);
    
    move.set_info(info);
    move_stack.push_back(move);
    state_stack.push_back(new_state);
    state = new_state;

    white_to_move ^= 1;
    white_occ = bitboard[WP] | bitboard[WN] | bitboard[WB] | bitboard[WR] | bitboard[WQ] | bitboard[WK];
    black_occ = bitboard[BP] | bitboard[BN] | bitboard[BB] | bitboard[BR] | bitboard[BQ] | bitboard[BK];
}

void Game::unmake_move() {
    Info info = move_stack.back().get_info();
    
    int from = get_mask(info, 7, 12);
    int to = get_mask(info, 13, 18);
    int src = get_mask(info, 19, 22);
    int dst = get_mask(info, 23, 26);

    bitboard[src] &= ~(1ull << to);
    bitboard[src] |= (1ull << from);

    types[from] = src;
    types[to] = dst;
    
    if (get_mask(info, 5, 5)) {
        bitboard[dst] |= (1ull << to);
    }
    
    unmake_en_passant(info, to);
    unmake_castling(info);
    unmake_promotion(info, to);

    move_stack.pop_back();
    state_stack.pop_back();
    state = state_stack.back();

    white_occ = bitboard[WP] | bitboard[WN] | bitboard[WB] | bitboard[WR] | bitboard[WQ] | bitboard[WK];
    black_occ = bitboard[BP] | bitboard[BN] | bitboard[BB] | bitboard[BR] | bitboard[BQ] | bitboard[BK];

    white_to_move ^= 1;
}

bool Game::check_move(Move& move) {
    bool white = white_to_move;
    make_move(move);
    
    bool safe = 1;
    if (white) {
        safe = King::is_safe(bitboard[WK], white_occ, black_occ, bitboard, 0);
    } else {
        safe = King::is_safe(bitboard[BK], black_occ, white_occ, bitboard, 1);
    }
    unmake_move();
    return safe; 
}