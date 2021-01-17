#include "CameraControlTask.hpp"

CameraControlTask::CameraControlTask(): 
    adaCam(&Serial2){
        SD.begin(254);
        adaCam.begin();
        adaCam.setImageSize(VC0706_640x480);
        
    }

void CameraControlTask::execute(){    
    delay(3000);
    sfr::camera::take_photo = true;
    if(sfr::camera::take_photo){
        adaCam.takePicture();
        
        // Create an image with the name IMAGExx.JPG
        char filename[13];
        strcpy(filename, "IMAGE00.JPG");
        for (int i = 0; i < 100; i++) {
            filename[5] = '0' + i/10;
            filename[6] = '0' + i%10;
            // create if does not exist, do not open existing, write, sync after write
            if (!SD.exists(filename)) {
                break;
            }
        }
        
        // Open the file for writing
        File imgFile = SD.open(filename, FILE_WRITE);
        
        // Get the size of the image (frame) taken  
        uint16_t jpglen = adaCam.frameLength();
        
        // Read all the data up to # bytes!
        while (jpglen > 0) {
            Serial.println(jpglen);
            // read 32 bytes at a time;
            uint8_t *buffer;
            //uint8_t bytesToRead = min(64, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
            uint8_t bytesToRead = adaCam.frameLength();
            buffer = adaCam.readPicture(bytesToRead);

            Serial.println(sizeof(buffer));
            Serial.println("__________________________________________________________");
            for (int i = 0; i < sizeof(buffer); i++) {
                Serial.println(buffer[i]);
            }

            Serial.println("__________________________________________________________");

            imgFile.write(buffer, bytesToRead);
            jpglen -= bytesToRead;
        }
        imgFile.close();
        Serial.println("GOT HERE");
        Serial.println(filename);
        sfr::camera::photo_taken = true;
    }
}