#include "CameraControlTask.hpp"

CameraControlTask::CameraControlTask(unsigned int offset) : TimedControlTask<void>(offset), adaCam(&Serial5) {}

void CameraControlTask::execute()
{
    if (sfr::camera::take_photo == true && sfr::camera::powered == true) {
        if (!adaCam.takePicture()) {
            Serial.println("Failed to snap!");
        } else {
            Serial.println("Picture taken!");
            sfr::camera::jpglen = adaCam.frameLength();
            Serial.println("Camera frame length: " + String(sfr::camera::jpglen));
            if (sfr::camera::jpglen > 0) {
                sfr::camera::take_photo = false;
                sfr::camera::photo_taken_sd_failed = true;
            }
        }
    }

    if (sfr::camera::photo_taken_sd_failed == true) {
        if (!SD.begin(254)) {
            Serial.println("SD CARD FAILED");
        } else {
            sfr::camera::photo_taken_sd_failed = false;
        }
    }
    if (sfr::camera::turn_on == true && sfr::camera::powered == false) {
        sfr::camera::wait_count++;
        if (sfr::camera::wait_count > 50) {
            sfr::camera::turn_on = false;
            sfr::fault::fault_3 = sfr::fault::fault_3 | constants::fault::camera_on_failed;
            Serial.print("Camera intialization failed at step ");
            Serial.println(sfr::camera::start_progress);
        }
        switch (sfr::camera::start_progress) {
        case 0: //step 0 - setting power
            sfr::camera::step_time = millis();
            Pins::setPinState(constants::camera::power_on_pin, HIGH);
            sfr::camera::start_progress++;
            break;
        case 1:                                             //step 1 - call begin method
            if (millis() - sfr::camera::step_time >= 100) { //need to determine this delay
                if (adaCam.begin()) {
                    Serial.println("turned on camera");
                    sfr::camera::step_time = millis();
                    sfr::camera::start_progress++;
                }
            }
            break;
        case 2: //step 2  - set resolution
            if (millis() - sfr::camera::step_time >= 500) {
                if (adaCam.setImageSize(sfr::camera::set_res)) {
                    Serial.println("resolution commanded successfully");
                    sfr::camera::step_time = millis();
                    sfr::camera::start_progress++;
                }
            } else {
            }
            break;
        case 3: //step 3 - get resolution
            if (millis() - sfr::camera::step_time >= 200) {
                uint8_t get_res = adaCam.getImageSize();
                if (get_res == sfr::camera::set_res) {
                    Serial.print("resolution fetched successfully: ");
                    Serial.println(get_res);
                    sfr::camera::step_time = 0;
                    sfr::camera::powered = true;
                    sfr::camera::turn_on = false;
                }
            }
            break;
        }
    }

    if (sfr::camera::turn_off == true && sfr::camera::powered == true) {
        Serial.println("turned off camera");
#ifdef VERBOSE
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

        for (int i = 0; i < bytesToRead; i++) {
            if (buffer[i] < 16) {
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
        if (sfr::camera::jpglen == 0) {
            sfr::rockblock::camera_max_fragments[sfr::camera::images_written] = sfr::camera::fragments_written;
            sfr::camera::images_written++;
            Serial.println("Done writing file");
        }
    }
}
