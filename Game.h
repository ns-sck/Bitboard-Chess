#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>
#include <string>
#include "Move.h"
#include "Piece.h"

typedef uint16_t State;

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
    WP   = 1,
    WN = 2,
    WB = 3,
    WR   = 4,
    WQ  = 5,
    WK   = 6,
    BP   = 7,
    BN = 8,
    BB = 9,
    BR   = 10,
    BQ  = 11,
    BK   = 12,
    TYPE_COUNT   = 13
};

class Game {
private:
    Piece* pieces[TYPE_COUNT];
    uint64_t bitboard[TYPE_COUNT];
    uint16_t types[64];
    uint64_t white_occ;
    uint64_t black_occ;
    
    std::string pieceSymbols[TYPE_COUNT] = {" ", "♟", "♞", "♝", "♜", "♛", "♚", "♙", "♘", "♗", "♖", "♕", "♔"};
    
    bool white_to_move;
    GameState game_state;

    // w_castle_k   = 0 - 0
    // w_castle_q   = 1 - 1
    // b_castle_k   = 2 - 2
    // b_castle_q   = 3 - 3
    // move_cntr    = 4 - 9
    // en_pssnt_sqr = 10 - 15 
    State state;
    State new_state;
    
    int full_move_number;
    
    std::vector<Move> move_stack; 
    std::vector<State> state_stack;

    float evaluation;

    public:
    Game();
    
    void reset_board();
    
    std::vector<Move> generate_legal_moves();
    bool make_move(Move& move);
    bool unmake_move();
    bool check_move(Move& move);
    void handle_en_passant(int from, int to, uint64_t& info);
    void handle_castling(int from, int to, int src, uint64_t&info);
    void generate_castlings(std::vector<Move>& moves);
    
    Move parse_move_string(std::string move_str);
    bool is_game_over() const;
    GameState get_state() const;
    bool is_white_to_move() const;
    
    void print_board() const;
};

#endif