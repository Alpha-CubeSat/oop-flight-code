#include "RockblockCommand.hpp"
#include <stdint.h>

class FaultInterface
{
private:
    bool suppressed;
    bool signaled;
    bool base;
    uint16_t opcode;

public:
    static std::map<int, FaultInterface *> opcode_lookup; // </brief OpCode Lookup Map

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
};

class FaultSurpressCommand : public RockblockCommand
{
public:
    FaultSurpressCommand(RawRockblockCommand raw) : RockblockCommand{raw}
    {
        if (FaultInterface::opcode_lookup.find(f_opcode) != FaultInterface::opcode_lookup.end()) {
            fault = FaultInterface::opcode_lookup[f_opcode];
        }
    };

    void suppress()
    {
        if (fault) {
            fault->suppress();
        }
    }

    bool isValid()
    {
        return fault->get_base();
    }

private:
    FaultInterface *fault;
};
