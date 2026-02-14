#ifndef MOVE_H
#define MOVE_H

#include <cstdint>
#include <string>

class Move {
private:
    // 0-5 from, 6-11 to, 12-15 from type, 16-19 to type
    // 20-20 capture, 21-21 color, 22-23 castle, 
    uint64_t info;
public:
    Move(uint64_t info);
    uint64_t get_info();
    void set_info(uint64_t info);
};

#endif