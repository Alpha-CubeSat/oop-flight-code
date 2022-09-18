#include "SFRField.hpp"
#include <cstring>
#include <map>

std::map<int, SFRInterface *> SFRInterface::opcode_lookup;

void SFRInterface::setFieldVal(int opcode, uint32_t arg1)
{
    if (opcode_lookup.count(opcode)) {
        // Valid Op Code
        SFRInterface *ptr = opcode_lookup[opcode];
        ptr->setValue(arg1);
    }
};
