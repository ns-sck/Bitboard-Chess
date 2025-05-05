#include "Move.h"
#include "BitUtil.h"
#include <string>

Move::Move(int info)
    : info(info) {}

int Move::getInfo() {
    return info;
}

void Move::setInfo(uint64_t info) {
    this->info = info;
}
// bool Move::operator==(const Move& other) const {
//     return from == other.from && to == other.to && type == other.type;
// }

// bool Move::operator!=(const Move& other) const {
//     return !(*this == other);
// }

std::string Move::toUCI() const {
    std::string result;
    return result;
} 