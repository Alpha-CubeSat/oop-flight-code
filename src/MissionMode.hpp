#ifndef MISSION_MODE_HPP_
#define MISSION_MODE_HPP_

class MissionMode
{
public:
    virtual void transition_to();
    virtual void dispatch();
    int id;
    float start_time;
    void set_start_time(float new_start_time)
    {
        start_time = new_start_time;
    }
    void set_id(int new_id)
    {
        id = new_id;
    }
};

class Boot : public MissionMode
{
public:
    void transition_to();
    void dispatch();
};

class AliveSignal : public MissionMode
{
public:
    void dispatch();
    void transition_to();
};

class LowPowerAliveSignal : public MissionMode
{
public:
    void dispatch();
    void transition_to();
};

class DetumbleSpin : public MissionMode
{
public:
    void dispatch();
    void transition_to();
};

class LowPowerDetumbleSpin : public MissionMode
{
public:
    void dispatch();
    void transition_to();
};

class Normal : public MissionMode
{
public:
    void dispatch();
    void transition_to();
};

class Transmit : public MissionMode
{
public:
    void dispatch();
    void transition_to();
};

class LowPower : public MissionMode
{
public:
    void dispatch();
    void transition_to();
};

class NormalDeployment : public MissionMode
{
public:
    void dispatch();
    void transition_to();
};

class TransmitDeployment : public MissionMode
{
public:
    void dispatch();
    void transition_to();
};

class LowPowerDeployment : public MissionMode
{
public:
    void dispatch();
    void transition_to();
};

class NormalArmed : public MissionMode
{
public:
    void dispatch();
    void transition_to();
};

class TransmitArmed : public MissionMode
{
public:
    void dispatch();
    void transition_to();
};

class LowPowerArmed : public MissionMode
{
public:
    void dispatch();
    void transition_to();
};

class NormalInSun : public MissionMode
{
public:
    void dispatch();
    void transition_to();
};

class TransmitInSun : public MissionMode
{
public:
    void dispatch();
    void transition_to();
};

class LowPowerInSun : public MissionMode
{
public:
    void dispatch();
    void transition_to();
};

class VoltageFailureInSun : public MissionMode
{
public:
    void dispatch();
    void transition_to();
};

class BootCamera : public MissionMode
{
public:
    void dispatch();
    void transition_to();
};

class MandatoryBurns : public MissionMode
{
public:
    void dispatch();
    void transition_to();
};

class RegularBurns : public MissionMode
{
public:
    void dispatch();
    void transition_to();
};

class Photo : public MissionMode
{
public:
    void dispatch();
    void transition_to();
};

void exit_signal_phase(MissionMode *mode);
void exit_detumble_phase(MissionMode *mode);
void enter_lp(MissionMode *lp_mode);
void exit_lp(MissionMode *reg_mode);
void timed_out(MissionMode *next_mode, float max_time);

void boot_initialization();

#endif