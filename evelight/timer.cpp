#include "timer.h"

namespace evl {

Timer::Timer(uint32_t ticks)
{
    mPrevTicks = ticks;
}

uint32_t Timer::Update(uint32_t ticks)
{
    uint32_t diffTicks = ticks - mPrevTicks;
    mPrevTicks = ticks;

    return diffTicks;
}
}
