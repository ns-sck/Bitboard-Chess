#include "Game.h"
#include "BitUtil.h"
#include "Queen.h"
#include "Knight.h"
#include "Rook.h"
#include "Bishop.h"
#include "WhiteKing.h"
#include "BlackKing.h"
#include "WhitePawn.h"
#include "BlackPawn.h"
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
    
    pieces[WP] = new WhitePawn(bitboard[WP]);
    pieces[WN] = new Knight(bitboard[WN]);
    pieces[WB] = new Bishop(bitboard[WB]);
    pieces[WR] = new Rook(bitboard[WR]);
    pieces[WQ] = new Queen(bitboard[WQ]);
    pieces[WK] = new WhiteKing(bitboard[WK]);
    pieces[BP] = new BlackPawn(bitboard[BP]);
    pieces[BN] = new Knight(bitboard[BN]);
    pieces[BB] = new Bishop(bitboard[BB]);
    pieces[BR] = new Rook(bitboard[BR]);
    pieces[BQ] = new Queen(bitboard[BQ]);
    pieces[BK] = new BlackKing(bitboard[BK]);

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

Move Game::parse_move_string(std::string move_str) {
    int from_file = move_str[0] - 'a';
    int from_rank = move_str[1] - '1';
    int from = from_rank * 8 + from_file;

    int to_file = move_str[2] - 'a';
    int to_rank = move_str[3] - '1';
    int to = to_rank * 8 + to_file;

    uint64_t info = 0;
    embed(info, 7, 12, from);
    embed(info, 13, 18, to);

    return Move(static_cast<uint64_t>(info));
}

void Game::generate_castlings(std::vector<Move>& moves) {
    uint64_t occ = white_occ | black_occ;
    if (white_to_move) {
        WhiteKing* wk = static_cast<WhiteKing*>(pieces[WK]);
        if (!get_mask(state, 0, 0) &&
            !(occ & 96) &&
            wk->is_king_safe(1ull << e1, white_occ, black_occ, bitboard) &&
            wk->is_king_safe(1ull << f1, white_occ, black_occ, bitboard) &&
            wk->is_king_safe(1ull << g1, white_occ, black_occ, bitboard)) {
                moves.push_back((e1 | (g1 << 6)) << 7);
            }
        if (!get_mask(state, 1, 1) &&
            !(occ & 14) &&
            wk->is_king_safe(1ull << e1, white_occ, black_occ, bitboard) &&
            wk->is_king_safe(1ull << d1, white_occ, black_occ, bitboard) &&
            wk->is_king_safe(1ull << c1, white_occ, black_occ, bitboard)) {
                moves.push_back((e1 | (c1 << 6)) << 7);
            }
    } else {
        BlackKing* bk = static_cast<BlackKing*>(pieces[BK]);
        if (!get_mask(state, 2, 2) &&
            !(occ & (3ull << 61)) &&
            bk->is_king_safe(1ull << e8, black_occ, white_occ, bitboard) &&
            bk->is_king_safe(1ull << f8, black_occ, white_occ, bitboard) &&
            bk->is_king_safe(1ull << g8, black_occ, white_occ, bitboard)) {
                moves.push_back((e8 | (g8 << 6)) << 7);
            }
        if (!get_mask(state, 3, 3) &&
            !(occ & (7ull << 57)) &&
            bk->is_king_safe(1ull << e8, black_occ, white_occ, bitboard) &&
            bk->is_king_safe(1ull << d8, black_occ, white_occ, bitboard) &&
            bk->is_king_safe(1ull << c8, black_occ, white_occ, bitboard)) {
                moves.push_back((e8 | (c8 << 6)) << 7);
            }
    }
}

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
        for (auto& m : pieces[j]->generate_moves(bitboard[j], team, enemy | (i == 1 ? en_passant : 0))) {
            if (check_move(m)) {
                moves.push_back(m);
            }
        }
    }
    generate_castlings(moves);
    return moves;
}

void Game::handle_en_passant(int from, int to, uint64_t& info) {
    int en_passant_square = get_mask(state, 10, 15);
    if (white_to_move) {
        if (((1ull << to) & bitboard[WP]) && to == from + 16) {
            embed(new_state, 10, 15, from + 8);
        } 
        if (to == en_passant_square) {
            bitboard[BP] &= ~(1ull << (en_passant_square - 8));
            types[en_passant_square - 8] = 0;
            embed(info, 6, 6, 1);
        }
    } else {
        if (((1ull << to) & bitboard[BP]) && to == from - 16) {
            embed(new_state, 10, 15, from - 8);
        } 
        if (to == en_passant_square) {
            bitboard[WP] &= ~(1ull << (en_passant_square + 8));
            types[en_passant_square + 8] = 0;
            embed(info, 6, 6, 1);
        }
    }
}

void Game::handle_castling(int from, int to, int src, uint64_t& info) {
    if (white_to_move) {
        if (src == WK) {
            if (from == e1 && to == g1) {
                bitboard[WR] &= ~(1ull << h1);
                bitboard[WR] |= 1ull << f1;
                types[h1] = EMPTY;
                types[f1] = WR;
                embed(info, 27, 27, 1);
            } else if (from == e1 && to == c1) {
                bitboard[WR] &= ~(1ull << a1);
                bitboard[WR] |= 1ull << d1;
                types[a1] = EMPTY;
                types[d1] = WR;
                embed(info, 28, 28, 1);
            }
            embed(new_state, 0, 1, 3);
        } else if (src == WR && from == h1) {
            embed(new_state, 0, 0, 1);
        } else if (src == WR && from == a1) {
            embed(new_state, 1, 1, 1);
        }
    } else {
        if (src == BK) {
            if (from == e8 && to == g8) {
                bitboard[BR] &= ~(1ull << h8);
                bitboard[BR] |= 1ull << f8;
                types[h8] = EMPTY;
                types[f8] = BR;
                embed(info, 27, 27, 1);
            } else if (from == e8 && to == c8) {
                bitboard[BR] &= ~(1ull << a8);
                bitboard[BR] |= 1ull << d8;
                types[a8] = EMPTY;
                types[d8] = BR;
                embed(info, 28, 28, 1);
            }
            embed(new_state, 2, 3, 3);
        } else if (src == BR && from == h8) {
            embed(new_state, 2, 2, 1);
        } else if (src == BR && from == a8) {
            embed(new_state, 3, 3, 1);
        }
    }
}

bool Game::unmake_move() {
    Info info = move_stack.back().get_info();
    
    int from = get_mask(info, 7, 12);
    int to = get_mask(info, 13, 18);
    int src = get_mask(info, 19, 22);
    int dst = get_mask(info, 23, 26);

    bitboard[src] &= ~(1ull << to);
    bitboard[src] |= (1ull << from);
    bool white_to_unmake = !white_to_move;

    // unmake en passant
    if (get_mask(info, 6, 6)) {
        if (white_to_unmake) {
            bitboard[BP] |= 1ull << (to - 8);
            types[to - 8] = BP;
        } else {
            bitboard[WP] |= 1ull << (to + 8);
            types[to + 8] = WP;
        }
    } 
    
    // unmake capture
    if (get_mask(info, 5, 5)) {
        bitboard[dst] |= (1ull << to);
    }

    // unmake castling
    if (white_to_unmake) {
        if (get_mask(info, 27, 27)) {
            bitboard[WR] |= 1ull << h1;
            bitboard[WR] &= ~(1ull << f1);
        } else if (get_mask(info, 28, 28)) {
            bitboard[WR] |= 1ull << a1;
            bitboard[WR] &= ~(1ull << d1);
        }
    } else {
        if (get_mask(info, 27, 27)) {
            bitboard[BR] |= 1ull << h8;
            bitboard[BR] &= ~(1ull << f8);
        } else if (get_mask(info, 28, 28)) {
            bitboard[BR] |= 1ull << a8;
            bitboard[BR] &= ~(1ull << d8);
        }
    }

    types[from] = src;
    types[to] = dst;

    move_stack.pop_back();
    state_stack.pop_back();
    state = state_stack.back();

    white_occ = bitboard[WP] | bitboard[WN] | bitboard[WB] | bitboard[WR] | bitboard[WQ] | bitboard[WK];
    black_occ = bitboard[BP] | bitboard[BN] | bitboard[BB] | bitboard[BR] | bitboard[BQ] | bitboard[BK];

    white_to_move ^= 1;
}

bool Game::make_move(Move& move) {
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
    
    handle_castling(from, to, src, info);
    handle_en_passant(from, to, info);
    
    move.set_info(info);
    move_stack.push_back(move);
    state_stack.push_back(new_state);
    state = new_state;

    white_to_move ^= 1;
    white_occ = bitboard[WP] | bitboard[WN] | bitboard[WB] | bitboard[WR] | bitboard[WQ] | bitboard[WK];
    black_occ = bitboard[BP] | bitboard[BN] | bitboard[BB] | bitboard[BR] | bitboard[BQ] | bitboard[BK];

    return true;
}

bool Game::check_move(Move& move) {
    bool white = white_to_move;
    make_move(move);
    
    bool safe = 1;
    if (white) {
        safe = static_cast<WhiteKing*>(pieces[WK])->is_king_safe(
            bitboard[WK], white_occ, black_occ, bitboard);
    } else {
        safe = static_cast<BlackKing*>(pieces[BK])->is_king_safe(
            bitboard[BK], black_occ, white_occ, bitboard);
    }
    unmake_move();
    return safe; 
}