#include "Fault.hpp"

std::map<uint16_t, FaultInterface *> FaultInterface::opcode_lookup;

Fault::Fault(uint16_t opcode)
{
    this->opcode = opcode;
    this->base = false;
    this->signaled = false;
    this->forced = false;
    this->suppressed = false;
    opcode_lookup[opcode] = this;
}

void Fault::signal()
{
    this->signaled = true;
    if (!this->suppressed) {
        this->base = true;
    }
}

void Fault::release()
{
    this->signaled = false;
    if (!this->forced) {
        this->base = false;
    }
}

void Fault::suppress()
{
    this->forced = false;
    this->suppressed = true;
    this->base = false;
}

void Fault::force()
{
    this->forced = true;
    this->suppressed = false;
    this->base = true;
}

void Fault::restore()
{
    this->forced = false;
    this->suppressed = false;
    this->base = this->signaled;
}

bool Fault::get_base()
{
    return base;
}

bool Fault::get_signaled()
{
    return signaled;
}

bool Fault::get_forced()
{
    return forced;
}

bool Fault::get_supressed()
{
    return suppressed;
}

uint8_t Fault::serialize()
{
    uint8_t serialized = 0;
    serialized += (int8_t)base << 3;
    serialized += (int8_t)forced << 2;
    serialized += (int8_t)suppressed << 1;
    serialized += (int8_t)signaled;
    return serialized;
}