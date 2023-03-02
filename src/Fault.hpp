#include "RockblockCommand.hpp"
#include <stdint.h>

#ifndef _FAULT_HPP_
#define _FAULT_HPP_
class FaultInterface
{
private:
    bool suppressed;
    bool signaled;
    bool base;
    uint16_t opcode;

public:
    static std::map<uint16_t, FaultInterface *> opcode_lookup; // </brief OpCode Lookup Map

    void signal();
    void release();
    void suppress();
    void unsuppress();
    bool get_base();
    bool get_signaled();
    bool get_supressed();
};

class Fault : public FaultInterface
{
private:
    bool suppressed;
    bool signaled;
    bool base;
    uint16_t opcode;

public:
    Fault(uint16_t opcode);
    void signal();
    void release();
    void suppress();
    void unsuppress();
    bool get_base();
    bool get_signaled();
    bool get_supressed();
    uint8_t serialize();
};
#endif