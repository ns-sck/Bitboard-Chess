#ifndef MOVE_H
#define MOVE_H

#include <cstdint>
#include <string>

typedef uint64_t Info;

class Move {
private:
    // promoted type = 0 - 3
    // check         = 4 - 4
    // capture       = 5 - 5
    // en_passant    = 6 - 6
    // from square   = 7 - 12
    // to square     = 13 - 18
    // from type     = 19 - 22
    // to type       = 23 - 26
    // kingsd castle = 27 - 27
    // quensd castle = 28 - 28
    uint64_t info;
public:
    Move(uint64_t info);
    uint64_t get_info();
    void set_info(uint64_t info);
};

#endif