#ifndef MOVE_H
#define MOVE_H

#include <cstdint>
#include <string>

class Move {
private:
    int info;
public:
    Move(int info);
    int getInfo();
    void setInfo(uint64_t info);
    std::string toUCI() const;
};

#endif