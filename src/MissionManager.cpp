#include "MissionManager.hpp"
#include <ctime>

MissionManager::MissionManager(unsigned int offset) : TimedControlTask<void>(offset)
{
    transition_to_boot();
}

void MissionManager::execute()
{
    mission_mode_type mode = sfr::mission::mode;

    switch (mode) {
    case mission_mode_type::boot:
        dispatch_boot();
        break;
    case mission_mode_type::alive_signal:
        dispatch_alive_signal();
        break;
    case mission_mode_type::lp_alive_signal:
        dispatch_lp_alive_signal();
        break;
    case mission_mode_type::detumble_spin:
        dispatch_detumble_spin();
        break;
    case mission_mode_type::lp_detumble_spin:
        dispatch_lp_detumble_spin();
        break;
    case mission_mode_type::normal:
        dispatch_normal();
        break;
    case mission_mode_type::transmit:
        dispatch_transmit();
        break;
    case mission_mode_type::lp:
        dispatch_lp();
        break;
    case mission_mode_type::normal_deployment:
        dispatch_normal_deployment();
        break;
    case mission_mode_type::transmit_deployment:
        dispatch_transmit_deployment();
        break;
    case mission_mode_type::lp_deployment:
        dispatch_lp_deployment();
        break;
    case mission_mode_type::normal_armed:
        dispatch_normal_armed();
        break;
    case mission_mode_type::transmit_armed:
        dispatch_transmit_armed();
        break;
    case mission_mode_type::lp_armed:
        dispatch_lp_armed();
        break;
    case mission_mode_type::normal_in_sun:
        dispatch_normal_in_sun();
        break;
    case mission_mode_type::transmit_in_sun:
        dispatch_transmit_in_sun();
        break;
    case mission_mode_type::lp_in_sun:
        dispatch_lp_in_sun();
        break;
    case mission_mode_type::volt_fail_in_sun:
        dispatch_volt_fail_in_sun();
        break;
    case mission_mode_type::boot_cam:
        dispatch_boot_cam();
        break;
    case mission_mode_type::mand_burns:
        dispatch_mand_burns();
        break;
    case mission_mode_type::reg_burns:
        dispatch_reg_burns();
        break;
    case mission_mode_type::photo:
        dispatch_photo();
        break;
    }
}

// Initialization

/*
 * Boot
 * Exit Conditions:
 *  -> X minutes since power on
 * Settings:
 */

void MissionManager::dispatch_boot()
{
    /*if (millis() - sfr::mission::boot_start >= constants::mission::max_boot_time) {
        sfr::mission::mode = mission_mode_type::alive_signal;
    }*/
}
void MissionManager::transition_to_boot() {}

/*
 * Alive Signal
 * Exit Conditions:
 *  -> Voltage sensor fault
 *  -> Voltage less than 3.75V
 *  -> Downlink fails X times
 *  -> Downlinked
 * Settings:
 */

void MissionManager::dispatch_alive_signal()
{
    /*if (sfr::battery::voltage_average <= sfr::battery::lower_bound) {
        sfr::mission::mode = mission_mode_type::lp_alive_signal;
    }*/
}
void MissionManager::transition_to_alive_signal() {}

/*
 * Low Power Alive Signal
 * Exit Conditions:
 *  -> Voltage greater than 3.9V
 *  -> Downlink fails X times
 *  -> Downlinked
 * Settings:
 */

void MissionManager::dispatch_lp_alive_signal() {}
void MissionManager::transition_to_lp_alive_signal() {}

// Stabilization

/*
 * Detumble/Spin
 * Exit Conditions:
 *  -> Voltage sensor fault
 *  -> Voltage less than 3.75V
 *  -> Detumble times out
 *  -> IMU determines stable
 * Settings:
 */

void MissionManager::dispatch_detumble_spin() {}
void MissionManager::transition_to_detumble_spin() {}

/*
 * Low Power Detumble/Spin
 * Exit Conditions:
 *  -> Voltage greater than 3.9V
 *  -> Detumble times out
 *  -> IMU determines stable
 * Settings:
 */

void MissionManager::dispatch_lp_detumble_spin() {}
void MissionManager::transition_to_lp_detumble_spin() {}

// Standby

/*
 * Normal
 * Exit Conditions:
 *  -> ACS Duty Cycle
 *  -> Voltage less than 3.75V
 *  -> Voltage sensor fault
 * Settings:
 */

void MissionManager::dispatch_normal() {}
void MissionManager::transition_to_normal() {}

/*
 * Transmit
 * Exit Conditions:
 * Settings:
 */

void MissionManager::dispatch_transmit() {}
void MissionManager::transition_to_transmit() {}

/*
 * Low Power
 * Exit Conditions:
 *  Voltage greater than 3.9V
 * Settings:
 */

void MissionManager::dispatch_lp() {}
void MissionManager::transition_to_lp() {}

// Deployment

/*
 * Deployment Normal
 * Exit Conditions:
 * Settings:
 */

void MissionManager::dispatch_normal_deployment() {}
void MissionManager::transition_to_normal_deployment() {}

/*
 * Deployment Transmit
 * Exit Conditions:
 * Settings:
 */

void MissionManager::dispatch_transmit_deployment() {}
void MissionManager::transition_to_transmit_deployment() {}

/*
 * Deployment Low Power
 * Exit Conditions:
 *  Voltage greater than 3.9V
 * Settings:
 */

void MissionManager::dispatch_lp_deployment() {}
void MissionManager::transition_to_lp_deployment() {}

// Armed

/*
 * Armed Normal
 * Exit Conditions:
 * Settings:
 */

void MissionManager::dispatch_normal_armed() {}
void MissionManager::transition_to_normal_armed() {}

/*
 * Armed Transmit
 * Exit Conditions:
 * Settings:
 */

void MissionManager::dispatch_transmit_armed() {}
void MissionManager::transition_to_transmit_armed() {}

/*
 * Armed Low Power
 * Exit Conditions:
 *  Voltage greater than 3.9V
 * Settings:
 */

void MissionManager::dispatch_lp_armed() {}
void MissionManager::transition_to_lp_armed() {}

// In Sun

/*
 * In Sun Normal
 * Exit Conditions:
 * Settings:
 */

void MissionManager::dispatch_normal_in_sun() {}
void MissionManager::transition_to_normal_in_sun() {}

/*
 * In Sun Transmit
 * Exit Conditions:
 * Settings:
 */

void MissionManager::dispatch_transmit_in_sun() {}
void MissionManager::transition_to_transmit_in_sun() {}

/*
 * In Sun Low Power
 * Exit Conditions:
 *  Voltage greater than 3.9V
 * Settings:
 */

void MissionManager::dispatch_lp_in_sun() {}
void MissionManager::transition_to_lp_in_sun() {}

/*
 * In Sun Voltage Sensor Fault
 * Exit Conditions:
 * Settings:
 */

void MissionManager::dispatch_volt_fail_in_sun() {}
void MissionManager::transition_to_volt_fail_in_sun() {}

// Firing

/*
 * Turn on Camera
 * Exit Conditions:
 * Settings:
 */

void MissionManager::dispatch_boot_cam() {}
void MissionManager::transition_to_boot_cam() {}

/*
 * Mandatory Burns
 * Exit Conditions:
 * Settings:
 */

void MissionManager::dispatch_mand_burns() {}
void MissionManager::transition_to_mand_burns() {}

/*
 * Regular Burns
 * Exit Conditions:
 * Settings:
 */
void expand_buffer(int old_size, char *buffer)
{
    char *expanded_buffer = new char[old_size * 2];
    for (unsigned int i = 0; i < old_size; ++i) {
        expanded_buffer[i] = buffer[i];
    }
    delete[] buffer;
    buffer = expanded_buffer;
}
void MissionManager::dispatch_reg_burns()
{

    // const clock_t begin_time = clock();
    // float timespan =  float(clock() - begin_time) / CLOCKS_PER_SEC;
    // std::string timestamp;
    // std::stringstream ss;
    // ss << clock();
    // ss >> timestamp;

    // creating buffer
    int buffer_curr_size = 5 * 100;
    char *buffer = new char[buffer_curr_size];
    int buffer_idx_tracker = 0;
    //section to make sure transition_to_photo() has been called and excuted successfully
    boolean trans_to_photo = false;
    printf("%s\n", (transition_to_photo(), "transition_to_photo() was called."));
    trans_to_photo = true;
    // mission mode transitions to take photo
    if (trans_to_photo) {
        // create and downlink the imu data
        buffer[buffer_curr_size++] = clock(); //place holder for identifier
        buffer[buffer_curr_size++] = clock();
        buffer[buffer_curr_size++] = 'take photo'; //need to be changed to imu mode
        buffer[buffer_curr_size++] = sfr::imu::gyro_x;
        buffer[buffer_curr_size++] = sfr::imu::gyro_y;
        buffer[buffer_curr_size++] = sfr::imu::gyro_z;
        //if should be checked after every append, unless know the format of downlink report. e.g. how would identifier goes
        if (buffer_idx_tracker == buffer_curr_size - 1) {
            expand_buffer(buffer_curr_size, buffer);
            buffer_curr_size *= 2;
        }
    }
}
void MissionManager::transition_to_reg_burns() {}

/*
 * Take Photo
 * Exit Conditions:
 * Settings:
 */

void MissionManager::dispatch_photo() {}
void MissionManager::transition_to_photo() {}