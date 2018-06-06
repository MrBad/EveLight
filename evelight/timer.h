#pragma once

#include <cstdint>

namespace evl {

class Timer {
public:
    Timer(uint32_t currTicks);
    ~Timer() {}
    uint32_t Update(uint32_t currTicks);

private:
    uint32_t mPrevTicks;
    uint32_t mNumUpdates;
};
}
