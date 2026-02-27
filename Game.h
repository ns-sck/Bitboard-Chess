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
    EMPTY        = 0,
    WHITE_PAWN   = 1,
    WHITE_KNIGHT = 2,
    WHITE_BISHOP = 3,
    WHITE_ROOK   = 4,
    WHITE_QUEEN  = 5,
    WHITE_KING   = 6,
    BLACK_PAWN   = 7,
    BLACK_KNIGHT = 8,
    BLACK_BISHOP = 9,
    BLACK_ROOK   = 10,
    BLACK_QUEEN  = 11,
    BLACK_KING   = 12,
    TYPE_COUNT   = 13
};

class Game {
private:
    uint64_t bitboard[TYPE_COUNT];
    Piece* pieces[TYPE_COUNT];
    uint64_t white_pieces;
    uint64_t black_pieces;
    uint64_t white_occupancy;
    uint64_t black_occupancy;
    
    std::string pieceSymbols[TYPE_COUNT] = {" ", "♟", "♞", "♝", "♜", "♛", "♚", "♙", "♘", "♗", "♖", "♕", "♔"};
    
    bool white_to_move;
    GameState state;

    // TODO: one or two integer will hold the state of the game
    // 0-5 from
    // 6-11 to 
    // 12-15 from type
    // 16-19 to type
    // 20-20 capture,
    // 21-21 en passant capture,
    // 22-27 en passant square
    // 28-28 white king side castle
    // 29-29 white queen side castle
    // 30-30 black king side castle
    // 31-31 black queen side castle
    // 32-32 white to move

    uint64_t STATE, PREV;
    uint16_t types[64];

    int half_move_clock;
    int full_move_number;
    
    uint64_t en_passant_sq[2];
    
    float evaluation;

    std::vector<Move> move_stack; 
    
    public:
    Game();
    
    void reset_board();
    
    std::vector<Move> generate_legal_moves();
    bool make_move(Move& move);
    bool unmake_move();
    bool check_move(Move& move);
    void handle_en_passant(int from, int to, uint64_t& info);
    void save_state();
    void rollback_state();
    
    Move parse_move_string(std::string move_str);
    bool is_game_over() const;
    GameState get_state() const;
    bool is_white_to_move() const;
    
    void print_board() const;
};

#endif