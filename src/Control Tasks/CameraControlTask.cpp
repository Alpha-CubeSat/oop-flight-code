#include "CameraControlTask.hpp"

CameraControlTask::CameraControlTask()
    : adaCam(&Serial5)
{
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
#ifdef VERBOSE
        Serial.print("Optical sensor intialization failed at step: ");
        Serial.println(sfr::camera::start_progress);
#endif
    }

    if (sfr::camera::init_mode == (uint16_t)sensor_init_mode_type::in_progress) {
        switch (sfr::camera::start_progress) {
        case 0: // step 0 - initialize SD card
        {
            if (!SD.begin(254)) {
#ifdef VERBOSE
                Serial.println("\n\n\nSD CARD FAILED\n\n\n");
#endif
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
#ifdef VERBOSE
                Serial.println("Powered on optical sensor");
#endif
                sfr::camera::start_progress++;
            } else {
#ifdef VERBOSE
                Serial.println("Not receiving optical sensor serial response");
#endif
            }
            break;
        }
        case 3: // step 3  - set resolution
        {
            if (adaCam.setImageSize(sfr::camera::set_res)) {
                if (adaCam.getImageSize() != sfr::camera::set_res) {
#ifdef VERBOSE
                    Serial.println("Setting resolution failed");
#endif
                    sfr::camera::init_mode = (uint16_t)sensor_init_mode_type::failed;
                } else {
#ifdef VERBOSE
                    Serial.println("Setting resolution succeeded");
#endif
                    sfr::camera::start_progress++;
                }
            }
            break;
        }
        case 4: // step 4 - get resolution
        {
            uint8_t get_res = adaCam.getImageSize();
            if (get_res == sfr::camera::set_res) {
#ifdef VERBOSE
                Serial.print("Resolution fetched successfully: ");
                Serial.println(get_res);
#endif
                sfr::camera::start_progress++;
            } else {
#ifdef VERBOSE
                Serial.println("Resolution fetch error");
#endif
                sfr::camera::init_mode = (uint16_t)sensor_init_mode_type::failed;
            }
            break;
        }
        case 5: // completed initialization
        {
            sfr::camera::init_mode = (uint16_t)sensor_init_mode_type::complete;
            sfr::camera::power_setting = (uint8_t)sensor_power_mode_type::do_nothing;
            sfr::camera::powered = true;
            break;
        }
        }
    }
}

void CameraControlTask::execute()
{
    // handle latent turn on / turn off variables
    if (sfr::camera::power_setting == (uint8_t)sensor_power_mode_type::off && sfr::camera::powered == false) {
        sfr::camera::power_setting = (uint8_t)sensor_power_mode_type::do_nothing;
        sfr::camera::failed_times = 0;
    }
    if (sfr::camera::power_setting == (uint8_t)sensor_power_mode_type::on && sfr::camera::powered == true) {
        sfr::camera::power_setting = (uint8_t)sensor_power_mode_type::do_nothing;
    }

    if (sfr::camera::power_setting == (uint8_t)sensor_power_mode_type::on && sfr::camera::powered == false) {
#ifdef VERBOSE
        Serial.println("Turned on optical sensor");
#endif
        sfr::camera::init_mode = (uint16_t)sensor_init_mode_type::awaiting;
        camera_init();
        if (sfr::camera::init_mode == (uint16_t)sensor_init_mode_type::complete) {
            sfr::camera::power_setting = (uint8_t)sensor_power_mode_type::do_nothing;
            transition_to_normal();
            sfr::camera::powered = true;
        } else {
            if (sfr::camera::failed_times == sfr::camera::failed_limit) {
                transition_to_abnormal_init();
            } else {
                sfr::camera::failed_times = sfr::camera::failed_times + 1;
                sfr::camera::init_mode = (uint16_t)sensor_init_mode_type::awaiting;
            }
        }
    }

    if (sfr::camera::power_setting == (uint8_t)sensor_power_mode_type::off && sfr::camera::powered == true) {
#ifdef VERBOSE
        Serial.println("Turned off optical sensor");
#endif
        camera_shutdown();
        sfr::camera::powered = false;
        sfr::camera::power_setting = (uint8_t)sensor_power_mode_type::do_nothing;

        // reset number of failed camera initialization attempts every time camera is turned off
        sfr::camera::failed_times = 0;
    }

    // handle taking and storing photos
    if (sfr::camera::take_photo == true && sfr::camera::powered == true) {
        if (!adaCam.takePicture()) {
#ifdef VERBOSE
            Serial.println("Failed to snap!");
#endif
        } else {
            jpglen = adaCam.frameLength();
#ifdef VERBOSE
            Serial.println("\n\n\nPicture taken!\n\n\n");
            Serial.println("Camera frame length: " + String(jpglen));
            Serial.println("##### Start writing the picture to SD card #####");
#endif
            if (jpglen > 0) {
                sfr::camera::take_photo = false;
            }
        }
    }

    if (jpglen > 0) {
        String filename = "";
        if (sfr::camera::images_written < 10) {
            filename += "0";
        }
        filename += String(sfr::camera::images_written);

        if (sfr::camera::fragments_written < 10) {
            filename += "0";
        }
        filename += String(sfr::camera::fragments_written) + ".jpg";

        File imgFile = SD.open(filename.c_str(), FILE_WRITE);

        uint8_t *buffer;
        uint8_t bytesToRead = min(constants::camera::content_length, jpglen);
        buffer = adaCam.readPicture(bytesToRead);

        for (int i = 0; i < bytesToRead; i++) {
            if (buffer[i] < 16) {
                imgFile.print(0, HEX);
            }
            imgFile.print(buffer[i], HEX);
        }

        jpglen -= bytesToRead;
        imgFile.close();
        sfr::camera::fragments_written++;
        if (jpglen == 0) {
            sfr::rockblock::camera_max_fragments[sfr::camera::images_written] = sfr::camera::fragments_written;
            sfr::camera::images_written++;
#ifdef VERBOSE
            Serial.println("Done writing file");
#endif
            sfr::camera::power_setting = (uint8_t)sensor_power_mode_type::off;
        }
    }
}

void CameraControlTask::transition_to_normal()
{
    // updates camera mode to normal
    sfr::camera::mode = (uint16_t)sensor_mode_type::normal;
#ifdef VERBOSE
    Serial.println("Optical sensor initialization successful");
#endif
}

void CameraControlTask::transition_to_abnormal_init()
{
    // updates camera mode to abnormal_init
    // trips fault
    // turns camera off
#ifdef VERBOSE
    Serial.println("Optical sensor initialization failed, turning off");
#endif
    sfr::camera::mode = (uint16_t)sensor_mode_type::abnormal_init;
    camera_shutdown();

    sfr::camera::power_setting = (uint8_t)sensor_power_mode_type::do_nothing;
    sfr::camera::powered = false;
    sfr::camera::init_mode = (uint16_t)sensor_init_mode_type::awaiting;
}

void CameraControlTask::camera_shutdown()
{
    Pins::setPinState(constants::camera::power_on_pin, LOW);
    pinMode(constants::camera::rx, OUTPUT);
    pinMode(constants::camera::tx, OUTPUT);
    Pins::setPinState(constants::camera::rx, LOW);
    Pins::setPinState(constants::camera::tx, LOW);
}
