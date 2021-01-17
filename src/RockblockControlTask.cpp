#include "RockblockControlTask.hpp"

RockblockControlTask::RockblockControlTask():
    rockblock(Serial4){
    Serial4.begin(19200);
    rockblock.setPowerProfile(IridiumSBD::DEFAULT_POWER_PROFILE);
    delay(1000);
    int err = rockblock.begin();
    if(err != ISBD_SUCCESS){
        //sfr::rockblock::fault_report[constants::fault::rockblock_begin] = 1;
    }
    rockblock.adjustSendReceiveTimeout(30);
}

void RockblockControlTask::execute(){
    rockblock.getSignalQuality(quality);
    waitingMessages = rockblock.getWaitingMessageCount();

    if(((waitingMessages == -1 || waitingMessages > 0) && quality > 2) || (checkReady() && quality > 2)){
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

        //triggered is byte



        uint8_t report[70] = {mag_x, mag_y, mag_z, gyro_x, gyro_y, gyro_z, acc_x, acc_y, acc_z, voltage};
        size_t send_size = sizeof(report);

        //rockblock.sendReceiveSBDBinary(report, send_size, receive_buffer, receive_size);

        char str[receive_size+1];
        str[receive_size] = '\0';
        for (int i = 0; i < receive_size; i++){
            str[i] = receive_buffer[i];
        }

        String commandStr(str);

        int commaIdx = commandStr.indexOf(",");
        int exclIdx = commandStr.indexOf("!");

        if (commaIdx != 2 || exclIdx != 5){
            //sfr::rockblock::fault_report[constants::fault::incorrect_command] = 1;
        } else{
            int opcode = commandStr.substring(0, commaIdx).toInt();
            int argument = commandStr.substring(commaIdx + 1, exclIdx).toInt();
            handleCommand(opcode, argument);
        } 
        sfr::rockblock::last_downlink = millis();
    }
}

void RockblockControlTask::handleCommand(int opcode, int argument){
    Serial.print("the opcode is: ");
    Serial.println(opcode);
    Serial.print("the argument is: ");
    Serial.println(argument);
}

bool RockblockControlTask::checkReady(){
    if(millis() - sfr::rockblock::last_downlink >= sfr::rockblock::downlink_period){
        return true;
    } else{
        return false;
    }
}