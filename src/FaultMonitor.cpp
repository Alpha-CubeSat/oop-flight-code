#include "FaultMonitor.hpp"

FaultMonitor::FaultMonitor(){}

void FaultMonitor::execute(){
    sfr::fault::is_fault = false;
    sfr::fault::fault_1 = 0;
    sfr::fault::fault_2 = 0;

    //TODO set actual values

    //FAULT_1
    if((sfr::imu::mag_x < 0 || sfr::imu::mag_x > 0) && sfr::fault::check_mag_x){
        sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::mag_x;
    }
    if((sfr::imu::mag_y < 0 || sfr::imu::mag_y > 0) && sfr::fault::check_mag_y){
        sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::mag_y;
    }
    if(sfr::imu::mag_z < 0 || sfr::imu::mag_z > 0){
        sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::mag_z;
    }
    if(sfr::imu::gyro_x < 0 || sfr::imu::gyro_x > 0){
        sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::gyro_x;
    }
    if(sfr::imu::gyro_y < 0 || sfr::imu::gyro_y > 0){
        sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::gyro_y;
    }
    if(sfr::imu::gyro_z < 0 || sfr::imu::gyro_z > 0){
        sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::gyro_z;
    }
    if(sfr::imu::acc_x < 0 || sfr::imu::acc_x > 0){
        sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::acc_x;
    }
    if(sfr::imu::acc_y < 0 || sfr::imu::acc_y > 0){
        sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::acc_y;
    }

    //FAULT_2




    if(sfr::fault::fault_1 > 0 || sfr::fault::fault_2 > 0){
        sfr::fault::is_fault = true;
    }
    
}
