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
protected:
    /* EEPROM saving and restoring uses a vector of SFRInterfaces, since C++ does not allow creating a vector
    using the SFRField<T> template class, which means SFRField members and functions cannot be accessed. So,
    we pass the SFRField data to the SFRInterface parent class cast to generic integers and booleans. */
    int field_value;   // The value of the field cast to an int
    int default_value; // The default value of the field cast to an int
    int data_type;     // An int representing the field's type T, 1 for bool, 2 for uint8_t, 3 for uint16_t, and 4 for uint32_t
    float min;
    float max;
    bool restore = false; // If the field should be restored or not

public:
    static std::map<int, SFRInterface *> opcode_lookup; // </brief Op Code Lookup Map For SFR Field Uplink Override
    static std::vector<SFRInterface *> sfr_fields_vector;

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
    virtual void setValue(uint32_t arg1);

    int getFieldValue();

    int getDefaultValue();

    int getDataType();

    void setRestore(bool restore_on_boot);
    bool getRestore();

    float getMin();
    float getMax();
};

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class SFRField : public SFRInterface
{
private:
    T value;    // </brief Field Value, Only Arithmetic Types Allowed
    int opcode; // </brief Uplink Op Code to set this field
    int resolution = 1;
#ifdef DEBUG
    T initial;
#endif

public:
    SFRField(T default_val, T min, T max, int opcode_val)
    {
        value = default_val;
        opcode = opcode_val;

        field_value = (int)value;
        default_value = (int)value;
        this->min = min;
        this->max = max;

        if (sizeof(T) == sizeof(uint32_t))
            data_type = 4;
        else if (sizeof(T) == sizeof(uint16_t))
            data_type = 3;
        else if (sizeof(T) == sizeof(uint8_t))
            data_type = 2;
        else if (sizeof(T) == sizeof(bool))
            data_type = 1;

#ifdef DEBUG
        T initial = default_val;
#endif

        SFRInterface::opcode_lookup[opcode_val] = this;
        SFRInterface::sfr_fields_vector.push_back(this);
    }

    SFRField(T default_val, int opcode_val)
    {
        value = default_val;
        opcode = opcode_val;

        field_value = (int)value;
        default_value = (int)value;
        min = std::numeric_limits<T>::min();
        max = std::numeric_limits<T>::max();

        if (sizeof(T) == sizeof(uint32_t))
            data_type = 4;
        else if (sizeof(T) == sizeof(uint16_t))
            data_type = 3;
        else if (sizeof(T) == sizeof(uint8_t))
            data_type = 2;
        else if (sizeof(T) == sizeof(bool))
            data_type = 1;

#ifdef DEBUG
        T initial = default_val;
#endif

        SFRInterface::opcode_lookup[opcode_val] = this;
        SFRInterface::sfr_fields_vector.push_back(this);
    }

    SFRField(float default_val, float min, float max, int opcode_val, float resolution)
    {
        value = default_val * resolution;
        opcode = opcode_val;
        resolution = resolution;

        field_value = (int)value;
        default_value = (int)value;
        this->min = min;
        this->max = max;

        if (sizeof(T) == sizeof(uint32_t))
            data_type = 4;
        else if (sizeof(T) == sizeof(uint16_t))
            data_type = 3;
        else if (sizeof(T) == sizeof(uint8_t))
            data_type = 2;
        else if (sizeof(T) == sizeof(bool))
            data_type = 1;

#ifdef DEBUG
        T initial = default_val * resolution; // Since value gets set to initial in reset(), initial should be default_val * resolution instead of default_val
#endif

        SFRInterface::opcode_lookup[opcode_val] = this;
        SFRInterface::sfr_fields_vector.push_back(this);
    }

    SFRField(float default_val, int opcode_val, float resolution)
    {
        value = default_val * resolution;
        opcode = opcode_val;
        resolution = resolution;

        field_value = (int)value;
        default_value = (int)value;
        this->min = min;
        this->max = max;

        if (sizeof(T) == sizeof(uint32_t))
            data_type = 4;
        else if (sizeof(T) == sizeof(uint16_t))
            data_type = 3;
        else if (sizeof(T) == sizeof(uint8_t))
            data_type = 2;
        else if (sizeof(T) == sizeof(bool))
            data_type = 1;

#ifdef DEBUG
        T initial = default_val * resolution; // Since value gets set to initial in reset(), initial should be default_val * resolution instead of default_val
#endif

        SFRInterface::opcode_lookup[opcode_val] = this;
        SFRInterface::sfr_fields_vector.push_back(this);
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
        field_value = (int)input;
    }

#ifdef DEBUG
    void reset()
    {
        value = initial;
        field_value = (int)initial;
    }
#endif
    void setValue(uint32_t arg1)
    {
        // Convert 32bit word into Target Type
        // TODO Joining Two uint16s FS-158
        static_assert(sizeof(T) <= sizeof arg1, "Templated Type is larger than input.");
        T casted;
        std::memcpy(&casted, &arg1, sizeof(T));
        set(casted);
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