#include "NormalReportControlTask.hpp"

NormalReportControlTask::NormalReportControlTask(){
    int mag_x = map(sfr::imu::mag_x, -sfr::imu::mag, sfr::imu::mag, 0, 255);
    int mag_y = map(sfr::imu::mag_y, -sfr::imu::mag, sfr::imu::mag, 0, 255);
    int mag_z = map(sfr::imu::mag_z, -sfr::imu::mag, sfr::imu::mag, 0, 255);

    int gyro_x = map(sfr::imu::gyro_x, -sfr::imu::gyr, sfr::imu::gyr, 0, 255);
    int gyro_y = map(sfr::imu::gyro_y, -sfr::imu::gyr, sfr::imu::gyr, 0, 255);
    int gyro_z = map(sfr::imu::gyro_z, -sfr::imu::gyr, sfr::imu::gyr, 0, 255);

    int acc_x = map(sfr::imu::acc_x, -sfr::imu::acc, sfr::imu::acc, 0, 255);
    int acc_y = map(sfr::imu::acc_y, -sfr::imu::acc, sfr::imu::acc, 0, 255);
    int acc_z = map(sfr::imu::acc_z, -sfr::imu::acc, sfr::imu::acc, 0, 255);
    
    int voltage = map(sfr::battery::voltage, 0, 1023, 0, 255);

    uint8_t buffer[] = {mag_x, mag_y, mag_z, gyro_x, gyro_y, gyro_z, acc_x, acc_y, acc_z, voltage};
}

void NormalReportControlTask::execute(){ 
    //send receive sbd binary
}

