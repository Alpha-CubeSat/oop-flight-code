#include "SFRField.hpp"

std::map<int, SFRInterface *> SFRInterface::opcode_lookup;

void SFRInterface::setFieldValByOpcode(int opcode, uint32_t arg1)
{
    if (opcode_lookup.count(opcode)) {
        // Valid Op Code
        SFRInterface *ptr = opcode_lookup[opcode];
        ptr->setFieldValue(arg1);
    }
}
