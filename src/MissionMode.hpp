#ifndef MISSION_MODE_HPP_
#define MISSION_MODE_HPP_

class MissionMode
{
public:
    virtual void transition_to();
    virtual void dispatch();
    virtual int id();
};

class Boot : public MissionMode
{
public:
    void transition_to();
    void dispatch();
    int id()
    {
        return 0;
    }
};

class AliveSignal : public MissionMode
{
public:
    void dispatch();
    void transition_to();
    int id()
    {
        return 1;
    }
};

class LowPowerAliveSignal : public MissionMode
{
public:
    void dispatch();
    void transition_to();
    int id()
    {
        return 2;
    }
};

class DetumbleSpin : public MissionMode
{
public:
    void dispatch();
    void transition_to();
    int id()
    {
        return 3;
    }
};

class LowPowerDetumbleSpin : public MissionMode
{
public:
    void dispatch();
    void transition_to();
    int id()
    {
        return 4;
    }
};

class Normal : public MissionMode
{
public:
    void dispatch();
    void transition_to();
    int id()
    {
        return 5;
    }
};

class Transmit : public MissionMode
{
public:
    void dispatch();
    void transition_to();
    int id()
    {
        return 6;
    }
};

class LowPower : public MissionMode
{
public:
    void dispatch();
    void transition_to();
    int id()
    {
        return 7;
    }
};

class NormalDeployment : public MissionMode
{
public:
    void dispatch();
    void transition_to();
    int id()
    {
        return 8;
    }
};

class TransmitDeployment : public MissionMode
{
public:
    void dispatch();
    void transition_to();
    int id()
    {
        return 9;
    }
};

class LowPowerDeployment : public MissionMode
{
public:
    void dispatch();
    void transition_to();
    int id()
    {
        return 10;
    }
};

class NormalArmed : public MissionMode
{
public:
    void dispatch();
    void transition_to();
    int id()
    {
        return 11;
    }
};

class TransmitArmed : public MissionMode
{
public:
    void dispatch();
    void transition_to();
    int id()
    {
        return 12;
    }
};

class LowPowerArmed : public MissionMode
{
public:
    void dispatch();
    void transition_to();
    int id()
    {
        return 13;
    }
};

class NormalInSun : public MissionMode
{
public:
    void dispatch();
    void transition_to();
    int id()
    {
        return 14;
    }
};

class TransmitInSun : public MissionMode
{
public:
    void dispatch();
    void transition_to();
    int id()
    {
        return 15;
    }
};

class LowPowerInSun : public MissionMode
{
public:
    void dispatch();
    void transition_to();
    int id()
    {
        return 16;
    }
};

class VoltageFailureInSun : public MissionMode
{
public:
    void dispatch();
    void transition_to();
    int id()
    {
        return 17;
    }
};

class BootCamera : public MissionMode
{
public:
    void dispatch();
    void transition_to();
    int id()
    {
        return 18;
    }
};

class MandatoryBurns : public MissionMode
{
public:
    void dispatch();
    void transition_to();
    int id()
    {
        return 19;
    }
};

class RegularBurns : public MissionMode
{
public:
    void dispatch();
    void transition_to();
    int id()
    {
        return 20;
    }
};

class Photo : public MissionMode
{
public:
    void dispatch();
    void transition_to();
    int id()
    {
        return 21;
    }
};

void exit_signal_phase(MissionMode *mode);
void exit_detumble_phase(MissionMode *mode);
void enter_lp(MissionMode *lp_mode);
void exit_lp(MissionMode *reg_mode);

#endif