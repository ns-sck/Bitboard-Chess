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
    resetBoard();
}

void Game::resetBoard() {
    for (int i = 0; i < PIECE_TYPE_COUNT; i++) {
        bitboard[i] = 0ULL;
    }
    
    white_pieces = 0ULL;
    black_pieces = 0ULL;
    
    bitboard[WHITE_ROOK] = (1ULL << 0) | (1ULL << 7);     // a1, h1
    bitboard[WHITE_KNIGHT] = (1ULL << 1) | (1ULL << 6);   // b1, g1
    bitboard[WHITE_BISHOP] = (1ULL << 2) | (1ULL << 5);   // c1, f1
    bitboard[WHITE_QUEEN] = (1ULL << 3);                  // d1
    bitboard[WHITE_KING] = (1ULL << 4);                   // e1
    
    bitboard[WHITE_PAWN] = 0ULL;
    for (int i = 8; i < 16; i++) {
        bitboard[WHITE_PAWN] |= (1ULL << i);
    }
    
    bitboard[BLACK_ROOK] = (1ULL << 56) | (1ULL << 63);   // a8, h8
    bitboard[BLACK_KNIGHT] = (1ULL << 57) | (1ULL << 62); // b8, g8
    bitboard[BLACK_BISHOP] = (1ULL << 58) | (1ULL << 61); // c8, f8
    bitboard[BLACK_QUEEN] = (1ULL << 59);                 // d8
    bitboard[BLACK_KING] = (1ULL << 60);                  // e8
    
    bitboard[BLACK_PAWN] = 0ULL;
    for (int i = 48; i < 56; i++) {
        bitboard[BLACK_PAWN] |= (1ULL << i);
    }
    
    white_pieces = bitboard[WHITE_PAWN] | bitboard[WHITE_KNIGHT] | 
                  bitboard[WHITE_BISHOP] | bitboard[WHITE_ROOK] | 
                  bitboard[WHITE_QUEEN] | bitboard[WHITE_KING];
    
    black_pieces = bitboard[BLACK_PAWN] | bitboard[BLACK_KNIGHT] | 
                  bitboard[BLACK_BISHOP] | bitboard[BLACK_ROOK] | 
                  bitboard[BLACK_QUEEN] | bitboard[BLACK_KING];
    
    pieces[WHITE_PAWN]      =new WhitePawn(bitboard[WHITE_PAWN]);
    pieces[WHITE_BISHOP]    =new WhiteBishop(bitboard[WHITE_BISHOP]);
    pieces[WHITE_KNIGHT]    =new WhiteKnight(bitboard[WHITE_KNIGHT]);
    pieces[WHITE_ROOK]      =new WhiteRook(bitboard[WHITE_ROOK]);
    pieces[WHITE_QUEEN]     =new WhiteQueen(bitboard[WHITE_QUEEN]);
    pieces[WHITE_KING]      =new WhiteKing(bitboard[WHITE_KING]);
    pieces[BLACK_PAWN]      =new BlackPawn(bitboard[BLACK_PAWN]);
    pieces[BLACK_BISHOP]    =new BlackBishop(bitboard[BLACK_BISHOP]);
    pieces[BLACK_KNIGHT]    =new BlackKnight(bitboard[BLACK_KNIGHT]);
    pieces[BLACK_ROOK]      =new BlackRook(bitboard[BLACK_ROOK]);
    pieces[BLACK_QUEEN]     =new BlackQueen(bitboard[BLACK_QUEEN]);
    pieces[BLACK_KING]      =new BlackKing(bitboard[BLACK_KING]);

    white_to_move = true;
    state = GameState::ACTIVE;
    halfMoveClock = 0;
    fullMoveNumber = 1;
    
    white_occupancy = 0;
    black_occupancy = 0;

    whiteKingsideCastle = true;
    whiteQueensideCastle = true;
    blackKingsideCastle = true;
    blackQueensideCastle = true;
    
    white_en_passant = 0;
    black_en_passant = 0;
    
    positionHistory.clear();
    positionHistory.push_back(toFEN());
    
    evaluation = 0.0f;
}

uint64_t Game::getOccupiedSquares() const {
    return white_pieces | black_pieces;
}

uint64_t Game::getwhite_pieces() const {
    return white_pieces;
}

uint64_t Game::getblack_pieces() const {
    return black_pieces;
}

uint64_t Game::getPieceBitboard(PieceType type) const {
    return bitboard[type];
}

bool Game::isGameOver() const {
    return state != GameState::ACTIVE && state != GameState::CHECK;
}

GameState Game::getState() const {
    return state;
}

bool Game::isWhiteToMove() const {
    return white_to_move;
}

float Game::getEvaluation() const {
    return evaluation;
}

std::string Game::toFEN() const {
    return "";
}

void Game::printBoard() const {
    std::cout << "\n  +---+---+---+---+---+---+---+---+\n";
    
    std::string board[64];
    for (int i = 0; i < 64; i++) {
        board[i] = ' ';
    }
    
    for (int pieceType = 0; pieceType < PIECE_TYPE_COUNT; pieceType++) {
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

uint64_t Game::generate_pins() const {

}
std::vector<Move> Game::generate_legal_moves() const {
    uint64_t pin = generate_pins();
    std::vector<Move> moves;
    uint64_t occupied = 0;
    if (white_to_move) {
        for (int i = 0; i < 6; ++i) {
            for (auto& m : pieces[i]->generate_moves(bitboard[i], white_pieces, black_pieces | (i == 0 ? black_en_passant : 0))) {
                moves.push_back(m);
            }
        }
    } else {
        for (int i = 6; i < 12; ++i) {
            for (auto& m : pieces[i]->generate_moves(bitboard[i], black_pieces, white_pieces | (i == 6 ? white_en_passant : 0))) {
                moves.push_back(m);
            }
        }
    }
    return moves;
}

void Game::handle_en_passant(int from, int to) {
    // is called after pieces moved
    if (white_to_move) {
        if (((1ull << to) & bitboard[WHITE_PAWN]) && to == from + 16) {
            white_en_passant |= (1ull << from) + 8;
        } 
        if ((1ull << to) & black_en_passant) {
            bitboard[BLACK_PAWN] &= ~(black_en_passant >> 8);
        }
        black_en_passant = 0;
    } else {
        if (((1ull << to) & bitboard[BLACK_PAWN]) && to == from - 16) {
            black_en_passant |= (1ull << from) - 8;
        } 
        if ((1ull << to) & white_en_passant) {
            bitboard[WHITE_PAWN] &= ~(white_en_passant << 8);
        }
        white_en_passant = 0;
    }
}

void Game::handle_check() {

}

bool Game::make_move(Move& move) {
    // 0-5 from, 6-11 to, 12-15 from type, 16-19 to type
    uint64_t info = move.getInfo();
    int from = info & 63;
    int to = (info >> 6) & 63;
    int src = (info >> 12) & 15;
    int dst = (info >> 16) & 15;

    bitboard[src] &= ~(1ull << from);
    bitboard[src] |= (1ull << to);

    if (dst < 12) {
        bitboard[dst] &= ~(1ull << to);
    }

    handle_en_passant(from, to);
    handle_check();
    white_pieces = bitboard[0] | bitboard[1] | bitboard[2] | bitboard[3] | bitboard[4] | bitboard[5];
    black_pieces = bitboard[6] | bitboard[7] | bitboard[8] | bitboard[9] | bitboard[10] | bitboard[11];

    white_to_move ^= 1;
    return true;
}

bool Game::make_simple_move(Move& move) {
    uint64_t info = move.getInfo();
    int from = info & 63;
    int to = (info >> 6) & 63;
    info |= (15ull << 12);
    info |= (15ull << 16);
    for (int i = 0; i < 12; ++i) {
        if (bitboard[i] & (1ull << from)) {
            info &= ~(15ull << 12);
            info |= (i << 12);
        }
        if (bitboard[i] & (1ull << to)) {
            info &= ~(15ull << 16);
            info |= (i << 16);
        }
    }
    move.setInfo(info);
    make_move(move);
    return true;
}

bool Game::unmake_move(const Move& move) {
    return true;
} 