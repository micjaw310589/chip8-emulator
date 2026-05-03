#include "TimeAccumulatorNS.hpp"
#include <SDL3/SDL_timer.h>

TimeAccumulatorNS::TimeAccumulatorNS(const uint64_t cycle_time_ns)
    : CYCLE_TIME_NS(cycle_time_ns)
    , start_ref_point(SDL_GetTicksNS())
    , accumulator(0u)
{
}


void TimeAccumulatorNS::update()
{
    accumulator = SDL_GetTicksNS() - start_ref_point;
}

void TimeAccumulatorNS::reset()
{
    start_ref_point += accumulator;
}

bool TimeAccumulatorNS::isTimeUp() const
{
    return accumulator >= CYCLE_TIME_NS;
}
