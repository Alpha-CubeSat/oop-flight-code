#include "CameraControlTask.hpp"

CameraControlTask::CameraControlTask(): 
    adaCam(&Serial2){
        SD.begin(254);
        adaCam.begin();
        adaCam.setImageSize(VC0706_640x480);
        sfr::camera::take_photo = true;
    }

void CameraControlTask::execute(){    
    Serial.println("got here");
    if(sfr::camera::take_photo){
        adaCam.takePicture();
        
        // Open the file for writing
        File imgFile = SD.open("TEST_TODAY.JPG", FILE_WRITE);
        
        // Get the size of the image (frame) taken  
        uint16_t jpglen = adaCam.frameLength();
        
        // Read all the data up to # bytes!
        byte wCount = 0; // For counting # of writes
        while (jpglen > 0) {
            // read 32 bytes at a time;
            uint8_t *buffer;
            uint8_t bytesToRead = min(64, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
            buffer = adaCam.readPicture(bytesToRead);
            imgFile.write(buffer, bytesToRead);
            if(++wCount >= 64) { // Every 2K, give a little feedback so it doesn't appear locked up
                wCount = 0;
            }
            jpglen -= bytesToRead;
        }
        imgFile.close();
        Serial.println("photo taken");
        sfr::camera::photo_taken = true;
        sfr::camera::take_photo = false;
    }
}