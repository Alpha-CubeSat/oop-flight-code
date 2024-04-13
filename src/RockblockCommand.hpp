#ifndef _ROCKBLOCKCOMMAND_HPP_
#define _ROCKBLOCKCOMMAND_HPP_

#include "Arduino.h"
#include "Fault.hpp"
#include "SFRField.hpp"
#include "constants.hpp"
#include <EEPROM.h>
#include <stdint.h>

class RawRockblockCommand
{
public:
    uint8_t opcode[2];
    uint8_t arg_1[4];
    uint8_t arg_2[4];

    uint16_t get_f_opcode()
    {
        return (this->opcode[0] << 8) | (this->opcode[1]);
    }

    uint32_t get_f_arg_1()
    {
        return (this->arg_1[0] << 24) | (this->arg_1[1] << 16) | (this->arg_1[2] << 8) | (this->arg_1[3]);
    }

    uint32_t get_f_arg_2()
    {
        return (this->arg_2[0] << 24) | (this->arg_2[1] << 16) | (this->arg_2[2] << 8) | (this->arg_2[3]);
    }
};

class RockblockCommand
{
public:
    uint16_t f_opcode;
    uint32_t f_arg_1;
    uint32_t f_arg_2;

    RockblockCommand(RawRockblockCommand raw)
    {
        this->f_opcode = raw.get_f_opcode();
        this->f_arg_1 = raw.get_f_arg_1();
        this->f_arg_2 = raw.get_f_arg_2();
    };

    virtual bool isValid()
    {
        return false;
    } ///< \brief Return true if a command arguments are valid for that op code, false otherwise

    virtual void execute(){}; ///< \brief Executes Command Functionality or SFR Edit
};

class SFROverrideCommand : public RockblockCommand
{
private:
    bool set_value = false;
    bool set_restore = false;
    bool restore_value = false;

public:
    SFROverrideCommand(RawRockblockCommand raw) : RockblockCommand{raw}
    {
        if (SFRInterface::opcode_lookup.find(f_opcode) != SFRInterface::opcode_lookup.end()) {
            field = SFRInterface::opcode_lookup[f_opcode];
            set_value = !!((constants::masks::uint32_byte1_mask & f_arg_2) >> 24);   // Whether to override SFR
            set_restore = !!((constants::masks::uint32_byte2_mask & f_arg_2) >> 16); // Whether to override restore boolean
            restore_value = !!(constants::masks::uint32_byte4_mask & f_arg_2);       // Restore boolean value
        } else {
            field = nullptr;
        }
    };

    void execute()
    {
        if (isValid() && set_value) {
            field->setFieldValue(f_arg_1);
        }
        if (isValid() && set_restore) {
            field->setRestoreOnBoot((bool)restore_value);
        }
    }

    bool isValid()
    {
        if (field != nullptr) {
            if (set_value && f_arg_1 >= field->getMin() && f_arg_1 <= field->getMax()) {
                return true;
            }
            if (!set_value) {
                return true;
            }
        }
        return false;
    }

private:
    SFRInterface *field;
};

class FaultOverrideCommand : public RockblockCommand
{
private:
    FaultInterface *fault;

public:
    FaultOverrideCommand(RawRockblockCommand raw) : RockblockCommand{raw}
    {
        if (FaultInterface::opcode_lookup.find(f_opcode) != FaultInterface::opcode_lookup.end()) {
            fault = FaultInterface::opcode_lookup[f_opcode];
        } else {
            fault = nullptr;
        }
    };

    void execute()
    {
        if (f_arg_1 && !f_arg_2) {
            fault->force();
        } else if (!f_arg_1 && f_arg_2) {
            fault->suppress();
        } else if (!f_arg_1 && !f_arg_2) {
            fault->restore();
        }
    }

    bool isValid()
    {
        return fault != nullptr;
    }
};

class CameraFragmentCommand : public RockblockCommand
{
public:
    CameraFragmentCommand(RawRockblockCommand raw) : RockblockCommand{raw} {};

    void execute();
    bool isValid();
};

class UnknownCommand : public RockblockCommand
{
public:
    UnknownCommand(RawRockblockCommand raw) : RockblockCommand{raw} {};
};

class DeployCommand : public RockblockCommand
{
public:
    DeployCommand(RawRockblockCommand raw) : RockblockCommand{raw} {};

    void execute();
    bool isValid();
};

class ArmCommand : public RockblockCommand
{
public:
    ArmCommand(RawRockblockCommand raw) : RockblockCommand{raw} {};

    void execute();
    bool isValid();
};

class FireCommand : public RockblockCommand
{
public:
    FireCommand(RawRockblockCommand raw) : RockblockCommand{raw} {};

    void execute();
    bool isValid();
};

class MissionModeOverrideCommand : public RockblockCommand
{
public:
    MissionModeOverrideCommand(RawRockblockCommand raw) : RockblockCommand{raw} {};

    void execute();
    bool isValid();
};

class EEPROMResetCommand : public RockblockCommand
{
public:
    EEPROMResetCommand(RawRockblockCommand raw) : RockblockCommand{raw} {};

    void execute();
    bool isValid();
};

#endif
