#include "SFRField.hpp"

std::map<int, SFRInterface *> SFRInterface::opcode_lookup;
std::vector<SFRInterface *> SFRInterface::sfr_fields_vector;

void SFRInterface::setFieldVal(int opcode, uint32_t arg1)
{
    if (opcode_lookup.count(opcode)) {
        // Valid Op Code
        SFRInterface *ptr = opcode_lookup[opcode];
        ptr->setValue(arg1);
    }
}

void SFRInterface::setFieldValue(int val)
{
    field_value = val;
}

int SFRInterface::getFieldValue()
{
    return field_value;
}

void SFRInterface::setDataType(int type)
{
    data_type = type;
}

int SFRInterface::getDataType()
{
    return data_type;
}

void SFRInterface::setAddressOffset(int offset)
{
    address_offset = offset;
}

int SFRInterface::getAddressOffset()
{
    return address_offset;
}

void SFRInterface::setRestore(bool res)
{
    restore = res;
}

bool SFRInterface::getRestore()
{
    return restore;
}
