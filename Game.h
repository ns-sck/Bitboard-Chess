#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>
#include <string>
#include "Move.h"
#include "Piece.h"

enum class GameState {
    ACTIVE,
    CHECK,
    CHECKMATE,
    STALEMATE,
    DRAW_INSUFFICIENT_MATERIAL,
    DRAW_FIFTY_MOVE_RULE,
    DRAW_THREEFOLD_REPETITION
};

enum PieceType {
    WHITE_PAWN = 0,
    WHITE_KNIGHT = 1,
    WHITE_BISHOP = 2,
    WHITE_ROOK = 3,
    WHITE_QUEEN = 4,
    WHITE_KING = 5,
    BLACK_PAWN = 6,
    BLACK_KNIGHT = 7,
    BLACK_BISHOP = 8,
    BLACK_ROOK = 9,
    BLACK_QUEEN = 10,
    BLACK_KING = 11,
    PIECE_TYPE_COUNT = 12
};

class Game {
private:
    uint64_t bitboard[PIECE_TYPE_COUNT];
    Piece* pieces[PIECE_TYPE_COUNT];
    uint64_t white_pieces;
    uint64_t black_pieces;
    uint64_t white_occupancy;
    uint64_t black_occupancy;
    
    std::string pieceSymbols[PIECE_TYPE_COUNT] = {"♟", "♞", "♝", "♜", "♛", "♚", "♙", "♘", "♗", "♖", "♕", "♔"};
    
    bool white_to_move;
    GameState state;
    int halfMoveClock;
    int fullMoveNumber;
    
    bool whiteKingsideCastle;
    bool whiteQueensideCastle;
    bool blackKingsideCastle;
    bool blackQueensideCastle;
    
    uint64_t white_en_passant;
    uint64_t black_en_passant;

    int en_passant_target = -1;
    
    float evaluation;

    std::vector<Move> move_stack; 
    
    void handle_en_passant(int from, int to);
public:
    Game();
    
    void reset_board();
    
    std::vector<Move> generate_legal_moves();
    bool make_move(Move& move);
    bool make_simple_move(Move& move);
    bool unmake_move(const Move& move);
    bool check_move(Move& move);
    void rollback();
    
    Move parse_move_string(std::string move_str);
    uint64_t get_attacked_squares(bool white) const;
    uint64_t get_occupied_squares() const;
    uint64_t getwhite_pieces() const;
    uint64_t getblack_pieces() const;
    uint64_t get_piece_bitboard(PieceType type) const;
    bool is_game_over() const;
    GameState get_state() const;
    bool is_white_to_move() const;
    float getEvaluation() const;
    
    std::string toFEN() const;
    
    void print_board() const;
};

#endif