#include "Move.h"
#include "BitUtil.h"
#include <string>

Move::Move(uint64_t info)
    : info(info) {}

uint64_t Move::get_info() {
    return info;
}

void Move::set_info(uint64_t info) {
    this->info = info;
}
// bool Move::operator==(const Move& other) const {
//     return from == other.from && to == other.to && type == other.type;
// }

// bool Move::operator!=(const Move& other) const {
//     return !(*this == other);
// }