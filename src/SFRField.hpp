#include <limits>
#include <map>
#include <stdint.h>
#include <type_traits>

class SFRInterface
{

public:
    static std::map<int, SFRInterface *> opcode_lookup; // </brief Op Code Lookup Map For SFR Field Uplink Override
#ifdef DEBUG
    static void resetSFR()
    {
        for (auto const &kv : SFRInterface::opcode_lookup) {
            opcode_lookup[kv.first]->reset();
        }
    }

    virtual void reset();
#endif
};

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class SFRField : public SFRInterface
{
private:
    T value;      // </brief Field Value, Only Arithmetic Types Allowed
    T max;        // </brief Inclusive Minium Value
    T min;        // </brief Inclusive Maximum Value
    bool bounded; // </brief If max and min are bounded beyond data type
    int opcode;   // </brief Uplink Op Code to set this field
    int resolution;

#ifdef DEBUG
    T inital;
#endif

public:
    SFRField(T default_val, T min, T max, int opcode_val)
    {
        value = default_val;
        min = min;
        max = max;
        bounded = true;
        opcode = opcode_val;
        resolution = 1;
#ifdef DEBUG
        T inital = default_val;
#endif
        SFRInterface::opcode_lookup[opcode_val] = this;
    }

    SFRField(T default_val, int opcode_val)
    {
        value = default_val;
        bounded = false;
        opcode = opcode_val;
        resolution = 1;
#ifdef DEBUG
        T inital = default_val;
#endif
        SFRInterface::opcode_lookup[opcode_val] = this;
    }

    SFRField(float default_val, float min, float max, int opcode_val, float resolution)
    {
        value = default_val * resolution;
        min = min;
        max = max;
        bounded = true;
        opcode = opcode_val;
        resolution = resolution;
#ifdef DEBUG
        T inital = default_val;
#endif
        SFRInterface::opcode_lookup[opcode_val] = this;
    }

    operator T()
    {
        return value;
    }

    T get()
    {
        return value;
    }

    float get_float()
    {
        return value / resolution;
    }

    T set(T input)
    {
        if ((bounded && input <= max && input >= min) || (!bounded)) {
            value = input;
        }
    }

#ifdef DEBUG
    void reset()
    {
        value = initial;
    }
#endif
};
