#include "CameraMonitor.hpp"

CameraMonitor::CameraMonitor(unsigned int offset) : TimedControlTask<void>(offset), adaCam(&Serial5) {}

void CameraMonitor::execute()
{
    switch (sfr::camera::mode)
    {
    case sensor_mode_type::normal:
        Serial.println("Camera is in Normal Mode");
        break;
    case sensor_mode_type::abnormal_init:
        Serial.println("Camera is in Abnormal Initialization Mode");
        break;
    case sensor_mode_type::retry:
        Serial.println("Camera is in Retry Mode");
        bool began = false;
        for (int retry_attempts = 0; retry_attempts < sfr::camera::max_retry_attempts; retry_attempts++)
        {
            if (adaCam.begin())
            {
                transition_to_normal();
                began = true;
                break;
            }
        }
        if (began == false)
            transition_to_abnormal_init();
        break;
    }
}

void CameraMonitor::transition_to_normal()
{
    // updates camera mode to normal
    sfr::camera::mode = sensor_mode_type::normal;
#ifdef VERBOSE
    Serial.println("turned on camera");
#endif
    adaCam.setImageSize(VC0706_160x120);
    sfr::camera::powered = true;
    sfr::camera::turn_on = false;
}

void CameraMonitor::transition_to_abnormal_init()
{
    // updates camera mode to abnormal_init
    // trips fault
    sfr::camera::mode = sensor_mode_type::abnormal_init;
    sfr::fault::fault_3 = sfr::fault::fault_1 | constants::fault::camera_init;
    Pins::setPinState(constants::camera::power_on_pin, LOW);
    sfr::camera::powered = false;
    sfr::camera::turn_on = false;
}

void CameraMonitor::transition_to_retry()
{
    // updates camera mode to retry
    // this mode will call either transition_to_normal or transition_to_abnormal_init
    // called when command to retry processes
    sfr::camera::mode = sensor_mode_type::retry;
    sfr::camera::powered = false;
    sfr::camera::turn_on = true;
}