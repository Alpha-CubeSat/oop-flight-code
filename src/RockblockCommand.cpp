#include "RockBlockCommand.hpp"
#include "sfr.hpp"

void CameraFragmentCommand::execute()
{
    sfr::camera::fragment_requested = true;
    sfr::camera::serial_requested = f_arg_1;
    sfr::camera::fragment_number_requested = f_arg_2;
}

bool CameraFragmentCommand::isValid()
{
    for (int mission_mode : sfr::mission::mode_history) {
        if (mission_mode == 22) {
            return true;
        }
    }
    return false;
}

void IMUFragmentCommand::execute()
{
    // TODO: implement
}

bool IMUFragmentCommand::isValid()
{
    return true;
}
