#include "CameraControlTask.hpp"

#ifndef GIT
CameraControlTask::CameraControlTask(): 
    adaCam(&Serial2){
        //TODO add fault check for sd card
        SD.begin(254);
        pinMode(constants::camera::sleep_pin,OUTPUT);
        digitalWrite(constants::camera::sleep_pin,LOW);
        sfr::camera::take_photo = true;
    }

void CameraControlTask::execute(){   

    if(sfr::camera::turn_on){
        Serial.println("turned on camera");
        digitalWrite(constants::camera::sleep_pin,HIGH);
        adaCam.begin();
        adaCam.setImageSize(VC0706_160x120); 
        sfr::camera::powered = true;
        sfr::camera::turn_on = false;

    }
    if(sfr::camera::turn_off){
        digitalWrite(constants::camera::sleep_pin,LOW);
        sfr::camera::powered = false;
        sfr::camera::turn_off = false;
    }

    if(sfr::camera::take_photo && sfr::camera::powered){
        Serial.println("took picture");
        adaCam.takePicture();
        sfr::camera::take_photo = false;
        Serial.println(adaCam.frameLength());
        jpglen = adaCam.frameLength();

        // Create an image with the name IMAGExx.JPG
        strcpy(filename, "IMAGE00.JPG");
        for (int i = 0; i < 100; i++) {
            filename[5] = '0' + i/10;
            filename[6] = '0' + i%10;
            // create if does not exist, do not open existing, write, sync after write
            if (!SD.exists(filename)) {
                break;
            }
        }
    }

    if(jpglen > 0){
        // Open the file for writing
        File imgFile = SD.open(filename, FILE_WRITE);        
        
        // Read all the data up to # bytes!
        uint8_t *buffer;
        uint8_t bytesToRead = min(64, jpglen); 
        buffer = adaCam.readPicture(bytesToRead);
        imgFile.write(buffer, bytesToRead);
        jpglen -= bytesToRead;
        imgFile.close();     
    }
}
#else
CameraControlTask::CameraControlTask(){}
void CameraControlTask::execute(){}
#endif
