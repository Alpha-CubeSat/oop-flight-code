#ifndef MISSION_MANAGER_HPP_
#define MISSION_MANAGER_HPP_

#include "Control Tasks/TimedControlTask.hpp"
#include "constants.hpp"
#include "sfr.hpp"

class MissionManager : public TimedControlTask<void>
{
public:
    MissionManager(unsigned int offset);
    void execute();
    void dispatch_boot();
    void dispatch_alive_signal();
    void dispatch_lp_alive_signal();

    void dispatch_detumble_spin();
    void dispatch_lp_detumble_spin();

    void dispatch_normal();
    void dispatch_transmit();
    void dispatch_lp();

    void dispatch_normal_deployment();
    void dispatch_transmit_deployment();
    void dispatch_lp_deployment();

    void dispatch_normal_armed();
    void dispatch_transmit_armed();
    void dispatch_lp_armed();

    void dispatch_normal_in_sun();
    void dispatch_transmit_in_sun();
    void dispatch_lp_in_sun();
    void dispatch_volt_fail_in_sun();

    void dispatch_boot_cam();
    void dispatch_mand_burns();
    void dispatch_reg_burns();
    void dispatch_photo();

    static void transition_to_boot();
    static void transition_to_alive_signal();
    static void transition_to_lp_alive_signal();

    static void transition_to_detumble_spin();
    static void transition_to_lp_detumble_spin();

    static void transition_to_normal();
    static void transition_to_transmit();
    static void transition_to_lp();

    static void transition_to_normal_deployment();
    static void transition_to_transmit_deployment();
    static void transition_to_lp_deployment();

    static void transition_to_normal_armed();
    static void transition_to_transmit_armed();
    static void transition_to_lp_armed();

    static void transition_to_normal_in_sun();
    static void transition_to_transmit_in_sun();
    static void transition_to_lp_in_sun();
    static void transition_to_volt_fail_in_sun();

    static void transition_to_boot_cam();
    static void transition_to_mand_burns();
    static void transition_to_reg_burns();
    static void transition_to_photo();
};

#endif