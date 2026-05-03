#ifndef CHIP8_EMULATOR_TIMEACCUMULATOR_HPP
#define CHIP8_EMULATOR_TIMEACCUMULATOR_HPP

#include <cstdint>

class TimeAccumulatorNS
{
    const uint64_t CYCLE_TIME_NS;
    uint64_t start_ref_point;
    uint64_t accumulator;

public:
    explicit TimeAccumulatorNS(uint64_t cycle_time_ns);

    void update();
    void reset();
    bool isTimeUp() const;
};

#endif //CHIP8_EMULATOR_TIMEACCUMULATOR_HPP