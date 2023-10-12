#ifndef CLOCK_MANAGER_HPP_
#define CLOCK_MANAGER_HPP_

#include "sfr.hpp"

class ClockManager
{
public:
    ClockManager();
    void execute();

private:
    uint32_t cycle_time;
};

#endif