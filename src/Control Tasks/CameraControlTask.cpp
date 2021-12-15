#include "CameraControlTask.hpp"

CameraControlTask::CameraControlTask(unsigned int offset): TimedControlTask<void>(offset), adaCam(&Serial5){}

void CameraControlTask::execute()
{
    if (sfr::camera::take_photo == true && sfr::camera::powered == true) {
        if (!adaCam.takePicture()){
            Serial.println("Failed to snap!");
        } else {
            Serial.println("Picture taken!");
            sfr::camera::jpglen = adaCam.frameLength();
            Serial.println("Camera frame length: " + String(sfr::camera::jpglen));
            if (sfr::camera::jpglen > 0){
                sfr::camera::take_photo = false;
                sfr::camera::photo_taken_sd_failed = true;
            }    
        }
    }

    if(sfr::camera::photo_taken_sd_failed == true){
        if (!SD.begin(254)) {
            Serial.println("SD CARD FAILED");
        } else{
            sfr::camera::photo_taken_sd_failed = false;
        }
    }
        
    if (sfr::camera::turn_on == true && sfr::camera::powered == false) {
        Pins::setPinState(constants::camera::power_on_pin, HIGH);
        if (adaCam.begin()) {
            #ifdef VERBOSE
            Serial.println("turned on camera");
            #endif
            adaCam.setImageSize(VC0706_160x120);
            sfr::camera::powered = true;
            sfr::camera::turn_on = false;
        }
    }
        
    if (sfr::camera::turn_off == true && sfr::camera::powered == true) {
        #ifdef VERBOSE
        Serial.println("turned off camera");
        #endif
        Pins::setPinState(constants::camera::power_on_pin, LOW);
        pinMode(constants::camera::rx, OUTPUT);
        pinMode(constants::camera::tx, OUTPUT);
        Pins::setPinState(constants::camera::rx, LOW);
        Pins::setPinState(constants::camera::tx, LOW);
        sfr::camera::powered = false;
        sfr::camera::turn_off = false;
    }

    if (sfr::camera::jpglen > 0 && sfr::camera::photo_taken_sd_failed == false) {
        filetocreate = "";
        if (sfr::camera::images_written < 10) {
            filetocreate += "0";
        }
        filetocreate += String(sfr::camera::images_written);
        sfr::camera::image_lengths[sfr::camera::images_written] = sfr::camera::jpglen;

        if (sfr::camera::fragments_written < 10) {
            filetocreate += "0";
        }
        filetocreate += String(sfr::camera::fragments_written) + ".jpg";
        strcpy(sfr::camera::filename, filetocreate.c_str());

        imgFile = SD.open(sfr::camera::filename, FILE_WRITE);

        uint8_t *buffer;
        uint8_t bytesToRead = min(constants::camera::content_length, sfr::camera::jpglen);
        buffer = adaCam.readPicture(bytesToRead);

        for(int i = 0; i < bytesToRead; i++){
            if(buffer[i] < 16){
                imgFile.print(0, HEX);
                #ifdef VERBOSE
                    Serial.print(0, HEX);
                #endif
            }
            imgFile.print(buffer[i], HEX);
            #ifdef VERBOSE
                Serial.print(buffer[i],HEX);
            #endif
        }

        Serial.println("");
        
        sfr::camera::jpglen -= bytesToRead;
        imgFile.close();
        sfr::camera::fragments_written++;
        if(sfr::camera::jpglen == 0){
            sfr::rockblock::camera_max_fragments[sfr::camera::images_written] = sfr::camera::fragments_written;
            sfr::camera::images_written++;
            Serial.println("Done writing file");
        }
    }
}
