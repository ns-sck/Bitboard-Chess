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
    
    std::vector<std::string> positionHistory;
    
    float evaluation;
    
    bool isSquareAttacked(int square, bool byWhite) const;
    bool isInCheck(bool white) const;
    void updateGameState();
    void handleEnPassant(int from, int to);
    uint64_t generatePins() const;
public:
    Game();
    
    void resetBoard();
    
    std::vector<Move> generateLegalMoves() const;
    bool makeMove(Move& move);
    bool makeSimpleMove(Move& move);
    bool unmakeMove(const Move& move);
    void handleCheck();
    
    uint64_t getOccupiedSquares() const;
    uint64_t getwhite_pieces() const;
    uint64_t getblack_pieces() const;
    uint64_t getPieceBitboard(PieceType type) const;
    bool isGameOver() const;
    GameState getState() const;
    bool isWhiteToMove() const;
    float getEvaluation() const;
    
    std::string toFEN() const;
    bool fromFEN(const std::string& fen);
    
    void printBoard() const;
};

#endif