#include "CameraControlTask.hpp"

CameraControlTask::CameraControlTask(): 
    adaCam(&Serial2){
        SD.begin(254);
        adaCam.begin();
        adaCam.setImageSize(VC0706_640x480);
        sfr::camera::take_photo = true;
    }

void CameraControlTask::execute(){        
    if(sfr::camera::take_photo){
        adaCam.takePicture();
        sfr::camera::take_photo = false;
        sfr::camera::jpglen = adaCam.frameLength();
        Serial.println("picture has been taken");
    }

    if(sfr::camera::jpglen > 0){
        Serial.println("writing to sd");
        Serial.println(sfr::camera::jpglen);
        // Open the file for writing
        File imgFile = SD.open("TESTING.JPG", FILE_WRITE);        
        
        // Read all the data up to # bytes!
        uint8_t *buffer;
        uint8_t bytesToRead = min(64, sfr::camera::jpglen); 
        buffer = adaCam.readPicture(bytesToRead);
        imgFile.write(buffer, bytesToRead);
        sfr::camera::jpglen -= bytesToRead;
        imgFile.close();     
    }
}