#ifndef FAULT_CHECK_HPP_
#define FAULT_CHECK_HPP_

#include "sfr.hpp"

class FaultCheck
{
public:
    static bool check_is_fault(fault_check_type fault_type);
    static bool check_is_flag();
    static void flip_flag(auto flag);
    static void flip_fault(auto fault);
};

#endif