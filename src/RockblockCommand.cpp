#include "RockblockCommand.hpp"
#include "sfr.hpp"

void CameraFragmentCommand::execute()
{
    sfr::camera::fragment_requested = true;
    sfr::camera::serial_requested = f_arg_1;
    sfr::camera::fragment_number_requested = f_arg_2;
}

bool CameraFragmentCommand::isValid()
{
    return sfr::mission::deployed;
}

void IMUFragmentCommand::execute()
{
    sfr::imu::fragment_requested = true;
    sfr::imu::fragment_number_requested = f_arg_1;
}

bool IMUFragmentCommand::isValid()
{
    return sfr::mission::deployed;
}