#include "FaultMonitor.hpp"

FaultMonitor::FaultMonitor(){}

void FaultMonitor::execute(){
    sfr::fault::is_fault = false;
    sfr::fault::imu_fault = 0;

    //TODO set actual values
    if(sfr::imu::mag_x < 0 || sfr::imu::mag_x > 0){
        sfr::fault::imu_fault = sfr::fault::imu_fault | constants::fault::mag_x;
        sfr::fault::is_fault = true;
    }
    if(sfr::imu::mag_y < 0 || sfr::imu::mag_y > 0){
        sfr::fault::imu_fault = sfr::fault::imu_fault | constants::fault::mag_y;
        sfr::fault::is_fault = true;
    }
    if(sfr::imu::mag_z < 0 || sfr::imu::mag_z > 0){
        sfr::fault::imu_fault = sfr::fault::imu_fault | constants::fault::mag_z;
        sfr::fault::is_fault = true;
    }
    if(sfr::imu::gyro_x < 0 || sfr::imu::gyro_x > 0){
        sfr::fault::imu_fault = sfr::fault::imu_fault | constants::fault::gyro_x;
        sfr::fault::is_fault = true;
    }
    if(sfr::imu::gyro_y < 0 || sfr::imu::gyro_y > 0){
        sfr::fault::imu_fault = sfr::fault::imu_fault | constants::fault::gyro_y;
        sfr::fault::is_fault = true;
    }
    if(sfr::imu::gyro_z < 0 || sfr::imu::gyro_z > 0){
        sfr::fault::imu_fault = sfr::fault::imu_fault | constants::fault::gyro_z;
        sfr::fault::is_fault = true;
    }
    if(sfr::imu::acc_x < 0 || sfr::imu::acc_x > 0){
        sfr::fault::imu_fault = sfr::fault::imu_fault | constants::fault::acc_x;
        sfr::fault::is_fault = true;
    }
    if(sfr::imu::acc_y < 0 || sfr::imu::acc_y > 0){
        sfr::fault::imu_fault = sfr::fault::imu_fault | constants::fault::acc_y;
        sfr::fault::is_fault = true;
    }
    
}
