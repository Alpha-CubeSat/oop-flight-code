#include "FaultMonitor.hpp"

FaultMonitor::FaultMonitor(unsigned int offset): TimedControlTask<void>(offset){}

void FaultMonitor::execute(){

    if(sfr::fault::mode == fault_mode_type::active){
        //FAULT_1
        if((sfr::imu::mag_x > constants::imu::max_mag_x || sfr::imu::mag_x < constants::imu::min_mag_x) && sfr::fault::check_mag_x){
            sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::mag_x;
        }
        if((sfr::imu::mag_y > constants::imu::max_mag_y || sfr::imu::mag_y < constants::imu::min_mag_y) && sfr::fault::check_mag_y){
            sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::mag_y;
        }
        if((sfr::imu::mag_z > constants::imu::max_mag_z || sfr::imu::mag_z < constants::imu::min_mag_z) && sfr::fault::check_mag_z){
            sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::mag_z;
        }
        if((sfr::imu::gyro_x > constants::imu::max_gyro_x || sfr::imu::gyro_x < constants::imu::min_gyro_x) && sfr::fault::check_gyro_x){
            sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::gyro_x;
        }
        if((sfr::imu::gyro_y > constants::imu::max_gyro_y || sfr::imu::gyro_y < constants::imu::min_gyro_y) && sfr::fault::check_gyro_y){
            sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::gyro_y;
        }
        if((sfr::imu::gyro_z > constants::imu::max_gyro_z || sfr::imu::gyro_z < constants::imu::min_gyro_z) && sfr::fault::check_gyro_z){
            sfr::fault::fault_1 = sfr::fault::fault_1 | constants::fault::gyro_z;
        }

        //FAULT_2
        if((sfr::temperature::temp_c > constants::temperature::max_temp_c || sfr::temperature::temp_c < constants::temperature::min_temp_c) && sfr::fault::check_temp_c){
            sfr::fault::fault_2 = sfr::fault::fault_2 | constants::fault::temp_c;
        }
        if((sfr::current::solar_current > constants::current::max_solar_current || sfr::current::solar_current < constants::current::min_solar_current) && sfr::fault::check_solar_current){
            sfr::fault::fault_2 = sfr::fault::fault_2 | constants::fault::solar_current;
        }
        if((sfr::battery::voltage >  constants::battery::max_voltage || sfr::battery::voltage < constants::battery::min_voltage) && sfr::fault::check_voltage){
            sfr::fault::fault_2 = sfr::fault::fault_2 | constants::fault::voltage;
        }

        if(sfr::fault::fault_1 > 0 || sfr::fault::fault_2 > 0 || sfr::fault::fault_3 > 0){
            MissionManager::transition_to_safe();
        }
    }
    
}
