#ifndef PHASE_HPP_
#define PHASE_HPP_

class Phase
{
public:
    float start_time;
    void set_start_time(float new_start_time)
    {
        start_time = new_start_time;
    }
    virtual int get_id();
};

class Initialization : public Phase
{
    int get_id()
    {
        return 0;
    }
};

class Signal : public Phase
{
    int get_id()
    {
        return 1;
    }
};

class Stabilization : public Phase
{
    int get_id()
    {
        return 2;
    }
};

class Standby : public Phase
{
    int get_id()
    {
        return 3;
    }
};

class Deployment : public Phase
{
    int get_id()
    {
        return 4;
    }
};

class Armed : public Phase
{
    int get_id()
    {
        return 5;
    }
};

class InSun : public Phase
{
    int get_id()
    {
        return 6;
    }
};

class Firing : public Phase
{
    int get_id()
    {
        return 7;
    }
};

#endif