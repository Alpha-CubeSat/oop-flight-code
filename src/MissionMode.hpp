#ifndef MISSION_MODE_HPP_
#define MISSION_MODE_HPP_

#include "Modes/mode_type.enum"

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
    mode_type get_type()
    {
        return mode_type::TRANSMIT;
    }
};

class LowPowerAliveSignal : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 2;
    }
    mode_type get_type()
    {
        return mode_type::LP;
    }
};

class DetumbleSpin : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int get_id()
    {
        return 3;
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
        return 4;
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
        return 5;
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
        return 6;
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
        return 7;
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
        return 8;
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
        return 9;
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
        return 10;
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
        return 11;
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
        return 12;
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
        return 13;
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
        return 14;
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
        return 15;
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
        return 16;
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
        return 17;
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
        return 18;
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
        return 19;
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
        return 20;
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
        return 21;
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
        return 22;
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
void transmit_mode_settings();
void acs_mode_settings();

#endif