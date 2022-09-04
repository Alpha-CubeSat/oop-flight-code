#include "MissionManager.hpp"
#include "RockblockCommand.hpp"
#include "constants.hpp"
#include "sfr.hpp"

using namespace constants::rockblock;

void FaultCheckCommand::execute()
{
    switch (f_opcode) {
    case opcodes::fault_mode: // Fault Mode
        sfr::fault::mode = f_arg_1 ? fault_mode_type::active : fault_mode_type::inactive;
        break;
    case opcodes::fault_check_mag_x: // MAG Faults
        sfr::fault::check_mag_x = f_arg_1;
        break;
    case opcodes::fault_check_mag_y:
        sfr::fault::check_mag_y = f_arg_1;
        break;
    case opcodes::fault_check_mag_z:
        sfr::fault::check_mag_z = f_arg_1;
        break;
    case opcodes::fault_check_gyro_x: // GYRO Faults
        sfr::fault::check_gyro_x = f_arg_1;
        break;
    case opcodes::fault_check_gyro_y:
        sfr::fault::check_gyro_y = f_arg_1;
        break;
    case opcodes::fault_check_gyro_z:
        sfr::fault::check_gyro_z = f_arg_1;
        break;
    case opcodes::fault_check_temp_c: // Temperature Fault
        sfr::fault::check_temp_c = f_arg_1;
        break;
    case opcodes::fault_check_solar_current:
        sfr::fault::check_solar_current = f_arg_1; // Solar Current
        break;
    case opcodes::fault_check_voltage: // Voltage Fault
        sfr::fault::check_voltage = f_arg_1;
        break;
    // Missing, ACC, SD Card, and Camera On Failed @Lauren what is needed?
    default:
        break;
    }
}

bool FaultCheckCommand::isValid()
{
    return f_arg_1 == args::true_arg || args::false_arg;
}