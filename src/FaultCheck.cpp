#include "FaultCheck.hpp"

bool check_is_fault(fault_check_type fault_type)
{
    bool is_fault = false;
    switch (fault_type) {
    case fault_check_type::mag_x:
        is_fault = sfr::fault::check_mag_x;
        break;
    case fault_check_type::mag_y:
        is_fault = sfr::fault::check_mag_y;
        break;
    case fault_check_type::mag_z:
        is_fault = sfr::fault::check_mag_z;
        break;
    case fault_check_type::gyro_x:
        is_fault = sfr::fault::check_gyro_x;
        break;
    case fault_check_type::gyro_y:
        is_fault = sfr::fault::check_gyro_y;
        break;
    case fault_check_type::gyro_z:
        is_fault = sfr::fault::check_gyro_z;
        break;
    case fault_check_type::acc_x:
        is_fault = sfr::fault::check_acc_x;
        break;
    case fault_check_type::acc_y:
        is_fault = sfr::fault::check_acc_y;
        break;
    case fault_check_type::acc_z:
        is_fault = sfr::fault::check_acc_z;
        break;
    case fault_check_type::temp_c:
        is_fault = sfr::fault::check_temp_c;
        break;
    case fault_check_type::solar_current:
        is_fault = sfr::fault::check_solar_current;
        break;
    case fault_check_type::voltage:
        is_fault = sfr::fault::check_voltage;
        break;
    }
    return is_fault;
}

// To Be Implemented
bool check_is_flag()
{
    return false;
}

void flip_flag(auto flag)
{
    flag = (unsigned char)flag;
    if (flag != 0) {
        for (int i = 0; i < 8; i++) {
            flag &= ~(1 << i);
        }
    } else {
        for (int i = 0; i < 8; i++) {
            flag = (1 << i) | flag;
        }
    }
}

void flip_fault(auto fault)
{
    fault = (unsigned char)fault;
    if (fault != 0) {
        for (int i = 0; i < 8; i++) {
            fault &= ~(1 << i);
        }
    } else {
        for (int i = 0; i < 8; i++) {
            fault = (1 << i) | fault;
        }
    }
}