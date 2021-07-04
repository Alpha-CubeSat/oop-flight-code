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
    if (!SD.begin(254))
    {
        sfr::fault::fault_3 = sfr::fault::fault_3 | constants::fault::sd_card;
    }

    if (sfr::camera::turn_on)
    {
        Serial.println("turned on camera");
        digitalWrite(constants::camera::power_on_pin, HIGH);
        while (!adaCam.begin())
        {
            sfr::fault::fault_3 = sfr::fault::fault_3 | constants::fault::camera_on_failed;
        }
        adaCam.setImageSize(VC0706_160x120);
        sfr::camera::powered = true;
        sfr::camera::turn_on = false;
    }
    if (sfr::camera::turn_off)
    {
        digitalWrite(constants::camera::power_on_pin, LOW);
        sfr::camera::powered = false;
        sfr::camera::turn_off = false;
    }

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
            String filetocreate = "Image";
            if (sfr::camera::images_written < 10)
            {
                filetocreate += "0";
            }
            filetocreate += String(sfr::camera::images_written) + ".JPG";
            // Create an image with the name IMAGExx.JPG
            strcpy(sfr::camera::filename, filetocreate.c_str());
            // create if does not exist, do not open existing, write, sync after write
            if (!SD.exists(sfr::camera::filename))
            {
            }
        }
    }

    if (sfr::camera::jpglen > 0)
    {
        sfr::camera::image_lengths[sfr::camera::images_written] = sfr::camera::jpglen;
        // Open the file for writing
        Serial.println(sfr::camera::filename);
        File imgFile = SD.open(sfr::camera::filename, FILE_WRITE);
        Serial.println("Writing file");
        Serial.println("Image size: " + String(sfr::camera::jpglen));
        // Read all the data up to # bytes!
        uint8_t *buffer;
        uint8_t bytesToRead = min(64, sfr::camera::jpglen);
        while (sfr::camera::jpglen > 0)
        {
            buffer = adaCam.readPicture(bytesToRead);
            imgFile.write(buffer, bytesToRead);
            sfr::camera::jpglen -= bytesToRead;
            bytesToRead = min(64, sfr::camera::jpglen);
        }
        imgFile.close();
        sfr::camera::report_ready = true;
        sfr::camera::images_written++;
        Serial.println("Done writing file");
    }
}