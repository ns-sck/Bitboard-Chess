#include "Game.h"
#include "BitUtil.h"
#include "WhiteQueen.h"
#include "BlackQueen.h"
#include "WhiteKnight.h"
#include "BlackKnight.h"
#include "WhiteRook.h"
#include "BlackRook.h"
#include "WhiteBishop.h"
#include "BlackBishop.h"
#include "WhiteKing.h"
#include "BlackKing.h"
#include "WhitePawn.h"
#include "BlackPawn.h"
#include <iostream>

Game::Game() {
    reset_board();
}

void Game::reset_board() {

    bitboard[WHITE_PAWN]   = ((1ULL << 8) - 1) << 8;
    bitboard[WHITE_KNIGHT] = (1ULL << 1) | (1ULL << 6);
    bitboard[WHITE_BISHOP] = (1ULL << 2) | (1ULL << 5);
    bitboard[WHITE_ROOK]   = (1ULL << 0) | (1ULL << 7);
    bitboard[WHITE_QUEEN]  = (1ULL << 3);
    bitboard[WHITE_KING]   = (1ULL << 4);  
    
    bitboard[BLACK_PAWN]   = ((1ULL << 8) - 1) << 48;
    bitboard[BLACK_KNIGHT] = (1ULL << 57) | (1ULL << 62);
    bitboard[BLACK_BISHOP] = (1ULL << 58) | (1ULL << 61);
    bitboard[BLACK_ROOK]   = (1ULL << 56) | (1ULL << 63);
    bitboard[BLACK_QUEEN]  = (1ULL << 59);
    bitboard[BLACK_KING]   = (1ULL << 60);
    
    white_pieces = bitboard[WHITE_PAWN]   | bitboard[WHITE_KNIGHT] | 
                   bitboard[WHITE_BISHOP] | bitboard[WHITE_ROOK]   | 
                   bitboard[WHITE_QUEEN]  | bitboard[WHITE_KING];
    
    black_pieces = bitboard[BLACK_PAWN]   | bitboard[BLACK_KNIGHT] | 
                   bitboard[BLACK_BISHOP] | bitboard[BLACK_ROOK]   | 
                   bitboard[BLACK_QUEEN]  | bitboard[BLACK_KING];
    
    pieces[WHITE_PAWN]   = new WhitePawn(bitboard[WHITE_PAWN]);
    pieces[WHITE_BISHOP] = new WhiteBishop(bitboard[WHITE_BISHOP]);
    pieces[WHITE_KNIGHT] = new WhiteKnight(bitboard[WHITE_KNIGHT]);
    pieces[WHITE_ROOK]   = new WhiteRook(bitboard[WHITE_ROOK]);
    pieces[WHITE_QUEEN]  = new WhiteQueen(bitboard[WHITE_QUEEN]);
    pieces[WHITE_KING]   = new WhiteKing(bitboard[WHITE_KING]);
    pieces[BLACK_PAWN]   = new BlackPawn(bitboard[BLACK_PAWN]);
    pieces[BLACK_BISHOP] = new BlackBishop(bitboard[BLACK_BISHOP]);
    pieces[BLACK_KNIGHT] = new BlackKnight(bitboard[BLACK_KNIGHT]);
    pieces[BLACK_ROOK]   = new BlackRook(bitboard[BLACK_ROOK]);
    pieces[BLACK_QUEEN]  = new BlackQueen(bitboard[BLACK_QUEEN]);
    pieces[BLACK_KING]   = new BlackKing(bitboard[BLACK_KING]);

    for (int i = 1; i <= 12; ++i) {
        uint64_t p = bitboard[i];
        while (p) {
            int sq = get_lsb_idx(p);
            p = clear_lsb(p); 
            types[sq] = i;
        }
    } 

    white_to_move = true;
    state = GameState::ACTIVE;
    half_move_clock = 0;
    full_move_number = 1;

    STATE = 0;
    
    white_occupancy = 0;
    black_occupancy = 0;

    en_passant_sq[0] = 0;
    en_passant_sq[1] = 0;
    
    evaluation = 0.0f;
}

bool Game::is_game_over() const {
    return state != GameState::ACTIVE && state != GameState::CHECK;
}

GameState Game::get_state() const {
    return state;
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
    info |= (from & 63);
    info |= (to & 63) << 6;

    return Move(static_cast<uint64_t>(info));
}

void Game::save_state() {
    PREV = STATE;
}

void Game::rollback_state() {
    STATE = PREV;
}

std::vector<Move> Game::generate_legal_moves() {
    std::vector<Move> moves;
    uint64_t team = white_to_move ? white_pieces : black_pieces;
    uint64_t enemy = white_to_move ? black_pieces : white_pieces;
    uint64_t en_passant = 1ull << get_mask(STATE, 22, 27);
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
    return moves;
}

void Game::handle_en_passant(int from, int to, uint64_t& info) {
    // is called after pieces moved
    int en_passant_square = get_mask(PREV, 22, 27);
    if (white_to_move) {
        if (((1ull << to) & bitboard[WHITE_PAWN]) && to == from + 16) {
            embed(STATE, 22, 27, from + 8);
        } 
        if (to == en_passant_square) {
            bitboard[BLACK_PAWN] &= ~(1ull << (en_passant_square - 8));
            embed(STATE, 21, 21, 1);
            embed(STATE, 16, 19, BLACK_PAWN);
        }
    } else {
        if (((1ull << to) & bitboard[BLACK_PAWN]) && to == from - 16) {
            embed(STATE, 22, 27, from - 8);
        } 
        if (to == en_passant_square) {
            bitboard[WHITE_PAWN] &= ~(1ull << (en_passant_square + 8));
            embed(STATE, 21, 21, 1);
            embed(STATE, 16, 19, WHITE_PAWN);
        }
    }
}

bool Game::unmake_move() {
    int from = STATE & 63;
    int to = (STATE >> 6) & 63;
    int src = (STATE >> 12) & 15;
    int dst = (STATE >> 16) & 15;

    bitboard[src] &= ~(1ull << to);
    bitboard[src] |= (1ull << from);

    if ((STATE >> 21) & 1) {
        bitboard[dst] |= (1ull << (to + (white_to_move ? 8 : -8)));
    } else if ((STATE >> 20) & 1) {
        bitboard[dst] |= (1ull << to);
    }

    rollback_state();

    white_pieces = bitboard[6] | bitboard[1] | bitboard[2] | bitboard[3] | bitboard[4] | bitboard[5];
    black_pieces = bitboard[12] | bitboard[7] | bitboard[8] | bitboard[9] | bitboard[10] | bitboard[11];

    white_to_move ^= 1;
}

bool Game::make_move(Move& move) {
    save_state();
    uint64_t info = move.get_info();
    int from = info & 63;
    int to = (info >> 6) & 63;

    STATE = info;

    for (int i = 1; i < TYPE_COUNT; ++i) {
        if ((bitboard[i] >> from) & 1) {
            embed(STATE, 12, 15, i);
        }
        if ((bitboard[i] >> to) & 1) {
            embed(STATE, 16, 19, i);
            embed(STATE, 20, 20, 1);
        }
    }
    
    int src = get_mask(STATE, 12, 15);
    int dst = get_mask(STATE, 16, 19);
    
    if (get_mask(STATE, 20, 20)) {
        bitboard[dst] &= ~(1ull << to);
    }

    bitboard[src] &= ~(1ull << from);
    bitboard[src] |= (1ull << to);

    handle_en_passant(from, to, info);
    move_stack.push_back(move);

    white_to_move ^= 1;
    white_pieces = bitboard[6] | bitboard[1] | bitboard[2] | bitboard[3] | bitboard[4] | bitboard[5];
    black_pieces = bitboard[12] | bitboard[7] | bitboard[8] | bitboard[9] | bitboard[10] | bitboard[11];

    return true;
}

bool Game::check_move(Move& move) {
    bool white = white_to_move;
    uint64_t white_enp = en_passant_sq[WHITE];
    uint64_t black_enp = en_passant_sq[BLACK];
    make_move(move);
    
    bool safe = 1;
    if (white) {
        safe = static_cast<WhiteKing*>(pieces[WHITE_KING])->is_under_check(
            bitboard[WHITE_KING], white_pieces, black_pieces, bitboard);
    } else {
        safe = static_cast<BlackKing*>(pieces[BLACK_KING])->is_under_check(
            bitboard[BLACK_KING], black_pieces, white_pieces, bitboard);
    }
    unmake_move();
    en_passant_sq[WHITE] = white_enp;
    en_passant_sq[BLACK] = black_enp;
    return safe; 
}