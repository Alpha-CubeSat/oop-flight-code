#include <stdint.h>
class Fault
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