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
        camera_init();
        if (sfr::camera::init_mode == camera_init_mode_type::complete) {
            transition_to_normal();
        }
        if (sfr::camera::init_mode == camera_init_mode_type::failed) {
            transition_to_abnormal_init();
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

void CameraControlTask::camera_init()
{
    if (sfr::camera::init_mode == camera_init_mode_type::awaiting) {
        // Called camera_init function and initialization process has not yet started
        sfr::camera::init_start_time = millis();
        sfr::camera::step_time = millis();
        sfr::camera::init_mode = camera_init_mode_type::in_progress;
    }
    if (sfr::camera::init_mode == camera_init_mode_type::in_progress && ((millis() - sfr::camera::init_start_time) > sfr::camera::init_timeout)) {
        // Camera initalization process is in progress but has exceeded timeout duration
        sfr::camera::init_mode = camera_init_mode_type::failed;
        Serial.print("Camera intialization failed at step: ");
        Serial.println(sfr::camera::start_progress);
    }

    else if (sfr::camera::init_mode == camera_init_mode_type::in_progress) {
        switch (sfr::camera::start_progress) {
        case 0: // step 0 - setting power
            Pins::setPinState(constants::camera::power_on_pin, HIGH);
            sfr::camera::start_progress++;
            break;
        case 1:                                           // step 1 - call begin method
            if (millis() - sfr::camera::step_time >= sfr::camera::begin_delay) //need to determine this delay
            {
                if (adaCam.begin()) {
                    Serial.println("powered on camera");
                    sfr::camera::step_time = millis();
                    sfr::camera::start_progress++;
                } else {
                    Serial.println("not receiving serial response");
                }
                //
            }
            break;
        case 2: // step 2  - set resolution
            if (millis() - sfr::camera::step_time >= sfr::camera::resolution_set_delay) {
                if (adaCam.setImageSize(sfr::camera::set_res)) {
                    Serial.println("resolution commanded successfully");
                    sfr::camera::step_time = millis();
                    sfr::camera::start_progress++;
                }
            }
            break;
        case 3: // step 3 - get resolution
            if (millis() - sfr::camera::step_time >= sfr::camera::resolution_set_delay) {
                uint8_t get_res = adaCam.getImageSize();
                if (get_res == sfr::camera::set_res) {
                    Serial.print("resolution fetched successfully: ");
                    Serial.println(get_res);
                    sfr::camera::step_time = millis();
                    sfr::camera::start_progress++;
                }
            }
            break;
        case 4: // completed initialization
            sfr::camera::init_mode = camera_init_mode_type::complete;
            sfr::camera::turn_on = false;
            sfr::camera::powered = true;
        }
    }
}

void CameraControlTask::transition_to_normal()
{
    // updates camera mode to normal
    sfr::camera::mode = sensor_mode_type::normal;
#ifdef VERBOSE
    Serial.println("camera initialization successful");
#endif
}

void CameraControlTask::transition_to_abnormal_init()
{
    // updates camera mode to abnormal_init
    // trips fault
    sfr::camera::mode = sensor_mode_type::abnormal_init;
    sfr::fault::fault_3 = sfr::fault::fault_3 | constants::fault::camera_on_failed;
    Pins::setPinState(constants::camera::power_on_pin, LOW);
    pinMode(constants::camera::rx, OUTPUT);
    pinMode(constants::camera::tx, OUTPUT);
    Pins::setPinState(constants::camera::rx, LOW);
    Pins::setPinState(constants::camera::tx, LOW);
    sfr::camera::powered = false;
    sfr::camera::turn_off = false;
    sfr::camera::turn_on = false;
    sfr::camera::init_mode = camera_init_mode_type::awaiting;
    sfr::camera::start_progress = 0;
}
