#ifndef MISSION_MODE_HPP_
#define MISSION_MODE_HPP_

#include "Modes/acs_mode_type.enum"
#include "Modes/mode_type.enum"
#include "Modes/sensor_power_mode_type.enum"
#include <string>

class MissionMode
{
public:
    virtual void transition_to();
    virtual void dispatch();
    float start_time;
    void set_start_time(float new_start_time)
    {
        start_time = new_start_time;
    }
    virtual int get_id();
    virtual std::string get_name();
    virtual mode_type get_type();
};

class Boot : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 0;
    }
    std::string get_name()
    {
        return "Boot";
    }
    mode_type get_type()
    {
        return mode_type::OTHER;
    }
};

class AliveSignal : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 1;
    }
    std::string get_name()
    {
        return "Alive Signal";
    }
    mode_type get_type()
    {
        return mode_type::TRANSMIT;
    }
};

class DetumbleSpin : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 2;
    }
    std::string get_name()
    {
        return "Detumble Spin";
    }
    mode_type get_type()
    {
        return mode_type::NORMAL;
    }
};

class LowPowerDetumbleSpin : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 3;
    }
    std::string get_name()
    {
        return "Low Power Detumble Spin";
    }
    mode_type get_type()
    {
        return mode_type::OTHER;
    }
};

class Normal : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 4;
    }
    std::string get_name()
    {
        return "Normal";
    }
    mode_type get_type()
    {
        return mode_type::NORMAL;
    }
};

class Transmit : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 5;
    }
    std::string get_name()
    {
        return "Transmit";
    }
    mode_type get_type()
    {
        return mode_type::TRANSMIT;
    }
};

class LowPower : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 6;
    }
    std::string get_name()
    {
        return "Low Power";
    }
    mode_type get_type()
    {
        return mode_type::LP;
    }
};

class NormalDeployment : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 7;
    }
    std::string get_name()
    {
        return "Normal Deployment";
    }
    mode_type get_type()
    {
        return mode_type::NORMAL;
    }
};

class TransmitDeployment : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 8;
    }
    std::string get_name()
    {
        return "Transmit Deployment";
    }
    mode_type get_type()
    {
        return mode_type::TRANSMIT;
    }
};

class LowPowerDeployment : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 9;
    }
    std::string get_name()
    {
        return "Low Power Deployment";
    }
    mode_type get_type()
    {
        return mode_type::LP;
    }
};

class NormalArmed : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 10;
    }
    std::string get_name()
    {
        return "Normal Armed";
    }
    mode_type get_type()
    {
        return mode_type::NORMAL;
    }
};

class TransmitArmed : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 11;
    }
    std::string get_name()
    {
        return "Transmit Armed";
    }
    mode_type get_type()
    {
        return mode_type::TRANSMIT;
    }
};

class LowPowerArmed : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 12;
    }
    std::string get_name()
    {
        return "Low Power Armed";
    }
    mode_type get_type()
    {
        return mode_type::LP;
    }
};

class NormalInSun : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 13;
    }
    std::string get_name()
    {
        return "Normal In Sun";
    }
    mode_type get_type()
    {
        return mode_type::NORMAL;
    }
};

class TransmitInSun : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 14;
    }
    std::string get_name()
    {
        return "Transmit In Sun";
    }
    mode_type get_type()
    {
        return mode_type::TRANSMIT;
    }
};

class LowPowerInSun : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 15;
    }
    std::string get_name()
    {
        return "Low Power In Sun";
    }
    mode_type get_type()
    {
        return mode_type::OTHER;
    }
};

class VoltageFailureInSun : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 16;
    }
    std::string get_name()
    {
        return "Voltage Failure In Sun";
    }
    mode_type get_type()
    {
        return mode_type::LP;
    }
};

class BootIMU : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 17;
    }
    std::string get_name()
    {
        return "Boot IMU";
    }
    mode_type get_type()
    {
        return mode_type::OTHER;
    }
};

class BootCamera : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 18;
    }
    std::string get_name()
    {
        return "Boot Optical Sensor";
    }
    mode_type get_type()
    {
        return mode_type::OTHER;
    }
};

class MandatoryBurns : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 19;
    }
    std::string get_name()
    {
        return "Mandatory Burns";
    }
    mode_type get_type()
    {
        return mode_type::BURN;
    }
};

class RegularBurns : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 20;
    }
    std::string get_name()
    {
        return "Regular Burns";
    }
    mode_type get_type()
    {
        return mode_type::BURN;
    }
};

class Photo : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 21;
    }
    std::string get_name()
    {
        return "Deployment Verification";
    }
    mode_type get_type()
    {
        return mode_type::OTHER;
    }
};

void exit_signal_phase(MissionMode *mode);
void exit_detumble_phase(MissionMode *mode);
void exit_insun_phase(MissionMode *mode);
void enter_lp(MissionMode *lp_mode);
void exit_lp(MissionMode *reg_mode);
void timed_out(MissionMode *next_mode, float max_time);
void boot_initialization();
void check_previous(MissionMode *normal_mode, MissionMode *transmit_mode);
void enter_lp_insun();
void settings(bool rockblock_sleep_mode, sensor_power_mode_type camera_power_setting, bool acs_off, uint32_t downlink_period);

#endif