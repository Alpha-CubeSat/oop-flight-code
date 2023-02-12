#include "Fault.hpp"

Fault::Fault(uint16_t opcode)
{
    this->opcode = opcode;
    this->base = false;
    this->signaled = false;
    this->suppressed = false;
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
    this->base = false;
}

void Fault::suppress()
{
    this->suppressed = true;
    this->base = false;
}

void Fault::unsuppress()
{
    this->suppressed = false;
    this->base = this->signaled;
}

bool Fault::get_base()
{
    return base;
}

bool Fault::get_signaled()
{
    return signal;
}

bool Fault::get_supressed()
{
    return suppressed;
}