#ifndef TIMER_H
#define TIMER_H

#include <cstdint>

class Timer
{
  public:
    Timer(uint32_t currTicks);
    ~Timer() {}
    uint32_t Update(uint32_t currTicks);

  private:
    uint32_t mPrevTicks;
    uint32_t mNumUpdates;
};

#endif
