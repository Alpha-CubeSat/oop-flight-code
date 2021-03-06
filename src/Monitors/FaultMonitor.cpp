#include "FaultMonitor.hpp"

FaultMonitor::FaultMonitor(){}

void FaultMonitor::execute(){

    if(sfr::fault::mode == fault_mode_type::active){

        sfr::fault::is_fault = false;
        sfr::fault::fault_1 = 0;
        sfr::fault::fault_2 = 0;
        sfr::fault::fault_3 = 0;

        //FAULT_1
        if((sfr::imu::mag_x < constants::imu::max_mag_x || sfr::imu::mag_x > constants::imu::min_mag_x) && sfr::fault::check_mag_x){
            sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::mag_x;
        }
        if((sfr::imu::mag_y < constants::imu::max_mag_y || sfr::imu::mag_y > constants::imu::min_mag_y) && sfr::fault::check_mag_y){
            sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::mag_y;
        }
        if((sfr::imu::mag_z < constants::imu::max_mag_z || sfr::imu::mag_z > constants::imu::min_mag_z) && sfr::fault::check_mag_z){
            sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::mag_z;
        }
        if((sfr::imu::gyro_x < constants::imu::max_gyro_x || sfr::imu::gyro_x > constants::imu::min_gyro_x) && sfr::fault::check_gyro_x){
            sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::gyro_x;
        }
        if((sfr::imu::gyro_y < constants::imu::max_gyro_y || sfr::imu::gyro_y > constants::imu::min_gyro_y) && sfr::fault::check_gyro_y){
            sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::gyro_y;
        }
        if((sfr::imu::gyro_z < constants::imu::max_gyro_z || sfr::imu::gyro_z > constants::imu::min_gyro_z) && sfr::fault::check_gyro_z){
            sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::gyro_z;
        }
        if((sfr::imu::acc_x < constants::imu::max_acc_x || sfr::imu::acc_x > constants::imu::min_acc_x) && sfr::fault::check_acc_x){
            sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::acc_x;
        }
        if((sfr::imu::acc_y < constants::imu::max_acc_y || sfr::imu::acc_y > constants::imu::min_acc_y) && sfr::fault::check_acc_y){
            sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::acc_y;
        }

        //FAULT_2
        if((sfr::imu::acc_z < constants::imu::max_acc_z || sfr::imu::acc_z > constants::imu::min_acc_z) && sfr::fault::check_acc_z){
            sfr::fault::fault_2 = sfr::fault::fault_2 | constants::fault::acc_z;
        }
        if((sfr::temperature::temp_c < 0 || sfr::temperature::temp_c > 0) && sfr::fault::check_temp_c){
            sfr::fault::fault_2 = sfr::fault::fault_2 | constants::fault::temp_c;
        }
        if((sfr::current::solar_current < 0 || sfr::current::solar_current > 0) && sfr::fault::check_solar_current){
            sfr::fault::fault_2 = sfr::fault::fault_2 | constants::fault::solar_current;
        }
        if((sfr::battery::voltage < 0 || sfr::battery::voltage > 0) && sfr::fault::check_voltage){
            sfr::fault::fault_2 = sfr::fault::fault_2 | constants::fault::voltage;
        }

        if(sfr::fault::fault_1 > 0 || sfr::fault::fault_2 > 0 || sfr::fault::fault_3 > 0){
            sfr::fault::is_fault = true;
        }
    }
    
}
