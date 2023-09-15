#include <cstring>
#include <limits>
#include <map>
#include <stdint.h>
#include <type_traits>
#include <vector>

#ifndef _SFRFIELD_HPP_
#define _SFRFIELD_HPP_

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

    virtual ~SFRInterface(){};
    static void setFieldValByOpcode(int opcode, uint32_t arg1);

    virtual void setFieldValue(uint32_t arg1);
    virtual uint32_t getFieldValue();

    virtual uint32_t getDefaultValue();

    virtual void setRestoreOnBoot(bool restore_on_boot);
    virtual bool getRestoreOnBoot();
};

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class SFRField : public SFRInterface
{
private:
    T value;    // </brief Field Value, Only Arithmetic Types Allowed
    T max;      // </brief Inclusive Minium Value
    T min;      // </brief Inclusive Maximum Value
    int opcode; // </brief Uplink Op Code to set this field
    float resolution;
    bool restore; // </brief Restore on boot from EEPROM flag
    T initial;    // </brief Default field value

public:
    SFRField(T default_val, T min, T max, int opcode_val)
    {
        value = default_val;
        this->min = min;
        this->max = max;
        opcode = opcode_val;
        resolution = 1;
        restore = false;
        T initial = default_val;
        SFRInterface::opcode_lookup[opcode_val] = this;
    }

    SFRField(T default_val, int opcode_val)
    {
        value = default_val;
        min = std::numeric_limits<T>::min();
        max = std::numeric_limits<T>::max();
        opcode = opcode_val;
        resolution = 1;
        restore = false;
        T initial = default_val;
        SFRInterface::opcode_lookup[opcode_val] = this;
    }

    SFRField(float default_val, float min, float max, int opcode_val, float resolution)
    {
        value = default_val * resolution;
        this->min = min;
        this->max = max;
        opcode = opcode_val;
        this->resolution = resolution;
        restore = false;
        T initial = default_val * resolution;
        SFRInterface::opcode_lookup[opcode_val] = this;
    }

    SFRField(float default_val, int opcode_val, float resolution)
    {
        value = default_val * resolution;
        opcode = opcode_val;
        min = std::numeric_limits<T>::min();
        max = std::numeric_limits<T>::max();
        this->resolution = resolution;
        restore = false;
        T initial = default_val * resolution;
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

    void set(T input)
    {
        if (input <= max && input >= min) {
            value = input;
        }
    }

    void reset()
    {
        value = initial;
        restore = true;
    }

    void setFieldValue(uint32_t arg1)
    {
        // Convert 32bit word into Target Type
        static_assert(sizeof(T) <= sizeof arg1, "Templated Type is larger than input.");
        T casted;
        std::memcpy(&casted, &arg1, sizeof(T));
        set(casted);
    }

    uint32_t getFieldValue()
    {
        return (uint32_t)value;
    }

    uint32_t getDefaultValue()
    {
        return (uint32_t)initial;
    }

    void setRestoreOnBoot(bool restore_on_boot)
    {
        restore = restore_on_boot;
    }

    bool getRestoreOnBoot()
    {
        return restore;
    }

    // Postfix increment operator.
    void operator++(int)
    {
        set(value + 1);
    }

    // Assignment Operator
    void operator=(T val)
    {
        set(val);
    }

    void operator+=(T val)
    {
        set(value + val);
    }
};

#endif