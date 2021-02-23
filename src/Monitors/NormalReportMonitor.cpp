#include "NormalReportMonitor.hpp"

NormalReportMonitor::NormalReportMonitor(){}

void NormalReportMonitor::execute(){
    uint8_t mag_x = map(sfr::imu::mag_x, constants::imu::min_mag_x, constants::imu::max_mag_x, 0, 255);
    uint8_t mag_y = map(sfr::imu::mag_y, constants::imu::min_mag_y, constants::imu::max_mag_y, 0, 255);
    uint8_t mag_z = map(sfr::imu::mag_z, constants::imu::min_mag_z, constants::imu::max_mag_z, 0, 255);

    uint8_t gyro_x = map(sfr::imu::gyro_x, constants::imu::min_gyro_x, constants::imu::max_gyro_x, 0, 255);
    uint8_t gyro_y = map(sfr::imu::gyro_y, constants::imu::min_gyro_y, constants::imu::max_gyro_y, 0, 255);
    uint8_t gyro_z = map(sfr::imu::gyro_z, constants::imu::min_gyro_z, constants::imu::max_gyro_z, 0, 255);

    uint8_t acc_x = map(sfr::imu::acc_x, constants::imu::min_acc_x, constants::imu::max_acc_x, 0, 255);
    uint8_t acc_y = map(sfr::imu::acc_y, constants::imu::min_acc_y, constants::imu::max_acc_y, 0, 255);
    uint8_t acc_z = map(sfr::imu::acc_z, constants::imu::min_acc_z, constants::imu::max_acc_z, 0, 255);

    uint8_t report[70] = {mag_x, 
                        mag_y, 
                        mag_z, 
                        gyro_x, 
                        gyro_y, 
                        gyro_z, 
                        acc_x, 
                        acc_y, 
                        acc_z, 
                        sfr::button::pressed, 
                        sfr::fault::fault_1};
    
}
