#include "CameraControlTask.hpp"

CameraControlTask::CameraControlTask(unsigned int offset): TimedControlTask<void>(offset), adaCam(&Serial5)
{
    //Fault check for SD card
    if (!SD.begin(254))
    {
        sfr::fault::fault_3 = sfr::fault::fault_3 | constants::fault::sd_card;
    }
    pinMode(constants::camera::power_on_pin, OUTPUT);
    digitalWrite(constants::camera::power_on_pin, LOW);  
}

void CameraControlTask::execute()
{
    if (!SD.begin(254)) {
        sfr::fault::fault_3 = sfr::fault::fault_3 | constants::fault::sd_card;
        Serial.println("SD CARD FAILED");
    } else{
        if (sfr::camera::take_photo && sfr::camera::powered)
        {
            if (!adaCam.takePicture()){
                Serial.println("Failed to snap!");
            }
            else {
                Serial.println("Picture taken!");
                sfr::camera::jpglen = adaCam.frameLength();
                Serial.println("Camera frame length: " + String(sfr::camera::jpglen));
                if (sfr::camera::jpglen > 0)
                {
                    sfr::camera::take_photo = false;
                }
                
            }
        }

        if (sfr::camera::turn_on)
        {
            Serial.println("turned on camera");
            digitalWrite(constants::camera::power_on_pin, HIGH);
            if (!adaCam.begin())
            {
                sfr::fault::fault_3 = sfr::fault::fault_3 | constants::fault::camera_on_failed;
            } else{
                adaCam.setImageSize(VC0706_160x120);
                sfr::camera::powered = true;
                sfr::camera::turn_on = false;
            }
        }
        
        if (sfr::camera::turn_off)
        {
            digitalWrite(constants::camera::power_on_pin, LOW);
            sfr::camera::powered = false;
            sfr::camera::turn_off = false;
        }

        if (sfr::camera::jpglen > 0)
        {
            filetocreate = "";
            if (sfr::camera::images_written < 10)
            {
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
                    imgFile.print(0,HEX);
                }
                imgFile.print(buffer[i],HEX);
            }
        
            sfr::camera::jpglen -= bytesToRead;
            imgFile.close();
            sfr::camera::fragments_written++;
            if(sfr::camera::jpglen == 0){
                sfr::camera::images_written++;
                Serial.println("Done writing file");
            }
        }
    }
}