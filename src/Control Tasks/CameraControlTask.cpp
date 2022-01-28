#include "CameraControlTask.hpp"

CameraControlTask::CameraControlTask(unsigned int offset) : TimedControlTask<void>(offset), adaCam(&Serial5) {}

void CameraControlTask::execute()
{
    if (sfr::camera::take_photo == true && sfr::camera::powered == true)
    {
        if (!adaCam.takePicture())
        {
            Serial.println("Failed to snap!");
        }
        else
        {
            Serial.println("Picture taken!");
            sfr::camera::jpglen = adaCam.frameLength();
            Serial.println("Camera frame length: " + String(sfr::camera::jpglen));
            if (sfr::camera::jpglen > 0)
            {
                sfr::camera::take_photo = false;
                sfr::camera::photo_taken_sd_failed = true;
            }
        }
    }

    if (sfr::camera::photo_taken_sd_failed == true)
    {
        if (!SD.begin(254))
        {
            Serial.println("SD CARD FAILED");
        }
        else
        {
            sfr::camera::photo_taken_sd_failed = false;
        }
    }

    if (sfr::camera::turn_on == true && sfr::camera::powered == false)
    {
        Pins::setPinState(constants::camera::power_on_pin, HIGH);
        if (sfr::camera::mode == retry)
        {
            bool began = false;
            for (int retry_attempts = 0; retry_attempts < sfr::camera::max_retry_attempts; retry_attempts++)
            {
                if (adaCam.begin())
                {
                    CameraMonitor::transition_to_normal();
                    //transition_to_normal();
                    began = true;
                    break;
                }
            }
            if (began == false)
            {
                CameraMonitor::transition_to_abnormal_init();
                // transition_to_abnormal_init();
            }
        }
        else if (adaCam.begin())
        {
            CameraMonitor::transition_to_normal();
            //transition_to_normal();
        }
        else
        {
            CameraMonitor::transition_to_abnormal_init();
            //transition_to_abnormal_init();
        }
    }

    if (sfr::camera::turn_off == true && sfr::camera::powered == true)
    {
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

    if (sfr::camera::jpglen > 0 && sfr::camera::photo_taken_sd_failed == false)
    {
        filetocreate = "";
        if (sfr::camera::images_written < 10)
        {
            filetocreate += "0";
        }
        filetocreate += String(sfr::camera::images_written);
        sfr::camera::image_lengths[sfr::camera::images_written] = sfr::camera::jpglen;

        if (sfr::camera::fragments_written < 10)
        {
            filetocreate += "0";
        }
        filetocreate += String(sfr::camera::fragments_written) + ".jpg";
        strcpy(sfr::camera::filename, filetocreate.c_str());

        imgFile = SD.open(sfr::camera::filename, FILE_WRITE);

        uint8_t *buffer;
        uint8_t bytesToRead = min(constants::camera::content_length, sfr::camera::jpglen);
        buffer = adaCam.readPicture(bytesToRead);

        for (int i = 0; i < bytesToRead; i++)
        {
            if (buffer[i] < 16)
            {
                imgFile.print(0, HEX);
#ifdef VERBOSE
                Serial.print(0, HEX);
#endif
            }
            imgFile.print(buffer[i], HEX);
#ifdef VERBOSE
            Serial.print(buffer[i], HEX);
#endif
        }

        Serial.println("");

        sfr::camera::jpglen -= bytesToRead;
        imgFile.close();
        sfr::camera::fragments_written++;
        if (sfr::camera::jpglen == 0)
        {
            sfr::rockblock::camera_max_fragments[sfr::camera::images_written] = sfr::camera::fragments_written;
            sfr::camera::images_written++;
            Serial.println("Done writing file");
        }
    }
}

// void CameraControlTask::transition_to_normal()
// {
//     // updates camera mode to normal
//     sfr::camera::mode = sensor_mode_type::normal;
// #ifdef VERBOSE
//     Serial.println("turned on camera");
// #endif
//     adaCam.setImageSize(VC0706_160x120);
//     sfr::camera::powered = true;
//     sfr::camera::turn_on = false;
// }

// void CameraControlTask::transition_to_abnormal_init()
// {
//     // updates camera mode to abnormal_init
//     // trips fault
//     sfr::camera::mode = sensor_mode_type::abnormal_init;
//     sfr::fault::fault_3 = sfr::fault::fault_1 | constants::fault::camera_init;
//     Pins::setPinState(constants::camera::power_on_pin, LOW);
//     sfr::camera::powered = false;
//     sfr::camera::turn_on = false;
// }

// void CameraControlTask::transition_to_retry()
// {
//     // updates camera mode to retry
//     // this mode will call either transition_to_normal or transition_to_abnormal_init
//     // called when command to retry processes
//     sfr::camera::mode = sensor_mode_type::retry;
//     sfr::camera::powered = false;
//     sfr::camera::turn_on = true;
// }
