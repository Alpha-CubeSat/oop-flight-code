#include "CameraControlTask.hpp"

CameraControlTask::CameraControlTask(unsigned int offset)
    : TimedControlTask<void>(offset), adaCam(&Serial5)
{
}

void CameraControlTask::execute()
{
    // handle latent turn on / turn off variables
    if (sfr::camera::turn_off == true && sfr::camera::powered == false) {
        sfr::camera::turn_off = false;
    }
    if (sfr::camera::turn_on == true && sfr::camera::powered == true) {
        sfr::camera::turn_on = false;
    }

    if (sfr::camera::take_photo == true && sfr::camera::powered == true) {
        if (!adaCam.takePicture()) {
            Serial.println("Failed to snap!");
        } else {
            Serial.println("\n\n\nPicture taken!\n\n\n");
            jpglen = adaCam.frameLength();
            Serial.println("Camera frame length: " + String(jpglen));
            Serial.println("##### Start writing the picture to SD card #####");
            if (jpglen > 0) {
                sfr::camera::take_photo = false;
            }
        }
    }

    if (sfr::camera::turn_on == true && sfr::camera::powered == false) {
        camera_init();
        if (sfr::camera::init_mode == (uint16_t)sensor_init_mode_type::complete) {
            transition_to_normal();
        } else if (sfr::camera::init_mode == (uint16_t)sensor_init_mode_type::failed) {
            if (sfr::camera::failed_times == sfr::camera::failed_limit) {
                sfr::camera::failed_times = 0; // reset
                transition_to_abnormal_init();
            } else {
                sfr::camera::failed_times = sfr::camera::failed_times + 1;
                Serial.print("Camera initialization failed times: ");
                Serial.println(sfr::camera::failed_times);
                sfr::camera::init_mode = (uint16_t)sensor_init_mode_type::awaiting;
            }
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
        sfr::camera::init_mode = (uint16_t)sensor_init_mode_type::in_progress;
    }

    if (jpglen > 0) {
        filetocreate = "";
        if (sfr::camera::images_written < 10) {
            filetocreate += "0";
        }
        filetocreate += String(sfr::camera::images_written);
        image_lengths[sfr::camera::images_written] = jpglen;

        if (sfr::camera::fragments_written < 10) {
            filetocreate += "0";
        }
        filetocreate += String(sfr::camera::fragments_written) + ".jpg";
        strcpy(filename, filetocreate.c_str());

        imgFile = SD.open(filename, FILE_WRITE);

        uint8_t *buffer;
        uint8_t bytesToRead = min(constants::camera::content_length, jpglen);
        buffer = adaCam.readPicture(bytesToRead);

        for (int i = 0; i < bytesToRead; i++) {
            if (buffer[i] < 16) {
                imgFile.print(0, HEX);
#ifdef VERBOSE_CAM
                Serial.print(0, HEX);
#endif
            }
            imgFile.print(buffer[i], HEX);
#ifdef VERBOSE_CAM
            Serial.print(buffer[i], HEX);
#endif
        }

#ifdef VERBOSE_CAM
        Serial.println("");
#endif

        jpglen -= bytesToRead;
        imgFile.close();
        sfr::camera::fragments_written++;
        if (jpglen == 0) {
            sfr::rockblock::camera_max_fragments[sfr::camera::images_written] = sfr::camera::fragments_written;
            sfr::camera::images_written++;
            Serial.println("Done writing file");
            sfr::camera::turn_off = true;
        }
    }
}

void CameraControlTask::camera_init()
{
    if (sfr::camera::init_mode == (uint16_t)sensor_init_mode_type::awaiting) {
        // Called camera_init function and initialization process has not yet started
        sfr::camera::init_start_time = millis();
        sfr::camera::init_mode = (uint16_t)sensor_init_mode_type::in_progress;
    }
    if (sfr::camera::init_mode == (uint16_t)sensor_init_mode_type::in_progress && ((millis() - sfr::camera::init_start_time) > sfr::camera::init_timeout)) {
        // Camera initalization process is in progress but has exceeded timeout duration
        sfr::camera::init_mode = (uint16_t)sensor_init_mode_type::failed;
        Serial.print("Camera intialization failed at step: ");
        Serial.println(sfr::camera::start_progress);
    }

    if (sfr::camera::init_mode == (uint16_t)sensor_init_mode_type::in_progress) {
        switch (sfr::camera::start_progress) {
        case 0: // step 0 - initialize SD card
        {
            if (!SD.begin(254)) {
                Serial.println("\n\n\nSD CARD FAILED\n\n\n");
                sfr::camera::init_mode = (uint16_t)sensor_init_mode_type::failed;
            } else {
                sfr::camera::start_progress++;
            }
            break;
        }
        case 1: // step 1 - setting power
        {
            Pins::setPinState(constants::camera::power_on_pin, HIGH);
            sfr::camera::start_progress++;
            break;
        }
        case 2: // step 2 - call begin method
        {
            if (adaCam.begin()) {
                Serial.println("powered on camera");
                sfr::camera::start_progress++;
            } else {
                Serial.println("not receiving serial response");
            }
            break;
        }
        case 3: // step 3  - set resolution
        {
            Serial.println("###### Go into setting resolution #####");
            if (adaCam.setImageSize(sfr::camera::set_res)) {
                if (adaCam.getImageSize() != sfr::camera::set_res) {
                    Serial.println("Setting resolution failed");
                    sfr::camera::init_mode = (uint16_t)sensor_init_mode_type::failed;
                } else {
                    Serial.println("Setting resolution succeeded");
                    sfr::camera::start_progress++;
                }
            }
            break;
        }
        case 4: // step 4 - get resolution
        {
            uint8_t get_res = adaCam.getImageSize();
            if (get_res == sfr::camera::set_res) {
                Serial.print("resolution fetched successfully: ");
                Serial.println(get_res);
                sfr::camera::start_progress++;
            } else {
                Serial.println("Resolution fetch error");
                sfr::camera::init_mode = (uint16_t)sensor_init_mode_type::failed;
            }
            break;
        }
        case 5: // completed initialization
        {
            sfr::camera::init_mode = (uint16_t)sensor_init_mode_type::complete;
            sfr::camera::turn_on = false;
            sfr::camera::powered = true;
            break;
        }
        }
    }
}

void CameraControlTask::transition_to_normal()
{
    // updates camera mode to normal
    sfr::camera::mode = (uint16_t)sensor_mode_type::normal;
#ifdef VERBOSE
#endif
    Serial.println("camera initialization successful");
}

void CameraControlTask::transition_to_abnormal_init()
{
    // updates camera mode to abnormal_init
    // trips fault
    // turns camera off
    Serial.println("camera turned off");
    sfr::camera::mode = (uint16_t)sensor_mode_type::abnormal_init;
    Pins::setPinState(constants::camera::power_on_pin, LOW);
    pinMode(constants::camera::rx, OUTPUT);
    pinMode(constants::camera::tx, OUTPUT);
    Pins::setPinState(constants::camera::rx, LOW);
    Pins::setPinState(constants::camera::tx, LOW);
    sfr::camera::powered = false;
    sfr::camera::turn_off = false;
    sfr::camera::turn_on = false;
    sfr::camera::init_mode = (uint16_t)sensor_init_mode_type::awaiting;
    sfr::camera::start_progress = 0;
}
