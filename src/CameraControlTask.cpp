#include "CameraControlTask.hpp"

CameraControlTask::CameraControlTask(): 
    adaCam(&Serial2){
        //TODO add fault check for sd card
        SD.begin(254);
        adaCam.begin();
        adaCam.setImageSize(VC0706_160x120);
    }

void CameraControlTask::execute(){        
    if(sfr::camera::take_photo){
        adaCam.takePicture();
        sfr::camera::take_photo = false;
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