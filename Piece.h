#ifndef PIECE_H
#define PIECE_H

#include <cstdint>
#include <vector>
#include "Move.h"

class Piece {
protected:
    uint64_t position;
    bool isWhite;

public:
    Piece(uint64_t position, bool isWhite);
    virtual ~Piece() = default;

    virtual std::vector<Move> generateMoves(uint64_t position, uint64_t team, uint64_t enemy) = 0;
    virtual char getSymbol() const = 0;
    virtual int getValue() const = 0;

    uint64_t getPosition() const;
    void setPosition(uint64_t newPosition);
    bool getIsWhite() const;
    void addMoves(std::vector<Move>& moves, int sq, uint64_t occupancy) const;
};

#endif