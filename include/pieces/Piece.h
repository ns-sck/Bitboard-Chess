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

    virtual std::vector<Move> generate_moves(uint64_t position, uint64_t team, uint64_t enemy) = 0;
    virtual uint64_t get_attacked_pos(uint64_t position, uint64_t team, uint64_t enemy) = 0;
    virtual char get_symbol() const = 0;
    virtual int get_value() const = 0;

    uint64_t get_position() const;
    void set_position(uint64_t newPosition);
    bool is_white() const;
    void add_moves(std::vector<Move>& moves, int sq, uint64_t occupancy, uint64_t enemy) const;
};

#endif