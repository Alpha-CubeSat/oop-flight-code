#include "ACSMonitor.hpp"

ACSMonitor::ACSMonitor(){
    rtObj.initialize();
}

void ACSMonitor::execute(){
    rtObj.rtU.angularvelocity[0] = sfr::imu::gyro_x * 3.14159 / 180.0; 
    rtObj.rtU.angularvelocity[1] = sfr::imu::gyro_y * 3.14159 / 180.0;
    rtObj.rtU.angularvelocity[2] = sfr::imu::gyro_z * 3.14159 / 180.0;
    rtObj.rtU.Bfield_body[0] = sfr::imu::mag_x;
    rtObj.rtU.Bfield_body[1] = sfr::imu::mag_y;
    rtObj.rtU.Bfield_body[2] = sfr::imu::mag_z;
    rtObj.step();

    if(sfr::fault::check_acc_x && sfr::fault::check_acc_y && sfr::acs::mode != acs_mode_type::off){
        if(sfr::imu::gyro_x < 0 && sfr::imu::gyro_y < 0){
            sfr::acs::mode = acs_mode_type::point;
        }
        else{
            sfr::acs::mode = acs_mode_type::detumble;
        }
    }

    switch(sfr::acs::mode){
        case acs_mode_type::detumble:
            sfr::acs::current1 = rtObj.rtY.detuble[0];
            sfr::acs::current2 = rtObj.rtY.detuble[1];
            sfr::acs::current3 = rtObj.rtY.detuble[2];
            break;
        case acs_mode_type::point:
            sfr::acs::current1 = rtObj.rtY.point[0];
            sfr::acs::current2 = rtObj.rtY.point[1];
            sfr::acs::current3 = rtObj.rtY.point[2];
            break;
        case acs_mode_type::off:
            sfr::acs::current1 = 0;
            sfr::acs::current2 = 0;
            sfr::acs::current3 = 3;
            break;
    }

}
