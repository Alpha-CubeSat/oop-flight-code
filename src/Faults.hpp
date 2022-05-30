#ifndef _FAULTS_HPP_
#define _FAULTS_HPP_

// NOTE: This namespace needs to be separated from the SFR because including sfr.hpp in SensorReading creates a circular dependency

namespace faults {
    extern unsigned char fault_1;
    extern unsigned char fault_2;
    extern unsigned char fault_3;
} // namespace faults

#endif