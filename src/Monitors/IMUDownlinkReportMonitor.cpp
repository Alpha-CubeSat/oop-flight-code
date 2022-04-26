#include "IMUDownlinkReportMonitor.hpp"
IMUDownlinkReportMonitor::IMUDownlinkReportMonitor(unsigned int offset) : TimedControlTask<void>(offset) {}

void IMUDownlinkReportMonitor::execute()
{
    IMUDownlink imu_dlink = IMUDownlink(constants::timecontrol::imu_downlink_offset);
    imu_dlink.execute();
    // Get a requested fragment
    // if (sfr::imu::report_downlinked == true && sfr::imu::fragment_requested == true && sfr::rockblock::imu_first_start != false) {}
    if (sfr::rockblock::imu_first_start == false && sfr::imu::data_downlinked == true) {
        create_imu_downlink_report(sfr::imu::fragment_number);
        sfr::imu::fragment_requested = false;
        sfr::imu::fragment_number++;
        // }
        if (sfr::imu::fragment_number == sfr::rockblock::imu_max_fragments) {
            sfr::imu::imu_dlink_report_ready = false;
            sfr::imu::fragment_number = 0;
        }
    }

    // #ifdef VERBOSE_IMUD
    //     Serial.println("imu_dlink_report_ready: " + String(sfr::imu::imu_dlink_report_ready));
    // #endif
}

void IMUDownlinkReportMonitor::create_imu_downlink_report(int fragment_number)
{

    // sfr::rockblock::imu_report[0] = 88;
    sfr::rockblock::imu_report.push_back(88);
#ifdef VERBOSE_IMUD
    Serial.print(String(88) + " ");
#endif

    // get each byte of fragment number
    // std::vector<unsigned char> fragment(constants::imu::bytes_allocated_fragment);
    // // add fragment number to imu downlink report
    // fragment[0] = (fragment_number >> (0 * 8));
    // int a = constants::imu::bytes_allocated_serial_opcode - 1;
    // size_t i = 0;
    // while (i < constants::imu::bytes_allocated_fragment) {
    // sfr::rockblock::imu_report[a] = fragment[i];
    // a = a + 1;
    // i = i + 1;
    // #ifdef VERBOSE_IMUD
    //         Serial.print(String(fragment[i]) + " ");

    // #endif
    // }

    sfr::rockblock::imu_report.push_back(fragment_number);
#ifdef VERBOSE_IMUD
    Serial.print(String(fragment_number));

#endif

    // check for the three buffer sizes
    assert(sfr::imu::imu_dlink_gyro_x_buffer.size() == sfr::imu::imu_dlink_gyro_y_buffer.size());
    assert(sfr::imu::imu_dlink_gyro_x_buffer.size() == sfr::imu::imu_dlink_gyro_z_buffer.size());

    // keep track of the byte got from the buffer
    int size_limit = 64;
    // add actual image content to imu downlink report
    int buffer_size = sfr::imu::imu_dlink_gyro_x_buffer.size();

    for (int idx = 0; idx < buffer_size; idx++) {
        // full gyro values in one report fragment
        if (size_limit - 3 < 0) {
            break;
        }

        else {

            // get the gyro values from buffer and write to the imu downlink report
            float gyro_x_value = sfr::imu::imu_dlink_gyro_x_buffer.back();
            uint8_t gyro_x = map(gyro_x_value, sfr::imu::gyro_min, sfr::imu::gyro_max, 0, 255);
            sfr::imu::imu_dlink_gyro_x_buffer.pop_back();
            // sfr::rockblock::imu_report[a++] = gyro_x;
            sfr::rockblock::imu_report.push_back(gyro_x);
            size_limit--;
#ifdef VERBOSE_IMUF
            Serial.println("gyro_x_value = " + String(gyro_x_value));

#endif

            float gyro_y_value = sfr::imu::imu_dlink_gyro_y_buffer.back();
            uint8_t gyro_y = map(gyro_y_value, sfr::imu::gyro_min, sfr::imu::gyro_max, 0, 255);
            sfr::imu::imu_dlink_gyro_y_buffer.pop_back();
            sfr::rockblock::imu_report.push_back(gyro_y);
            size_limit--;
#ifdef VERBOSE_IMUF
            Serial.println("gyro_y_value = " + String(gyro_y_value));

#endif

            float gyro_z_value = sfr::imu::imu_dlink_gyro_z_buffer.back();
            uint8_t gyro_z = map(gyro_z_value, sfr::imu::gyro_min, sfr::imu::gyro_max, 0, 255);
            sfr::imu::imu_dlink_gyro_z_buffer.pop_back();
            sfr::rockblock::imu_report.push_back(gyro_z);
            size_limit--;
#ifdef VERBOSE_IMUF
            Serial.println("gyro_z_value = " + String(gyro_z_value));

#endif
#ifdef VERBOSE_IMUD
            Serial.print(" " + String(gyro_x) + " " + String(gyro_y) + " " + String(gyro_z));

#endif
        }
        // end marker
        if (idx == buffer_size - 1) {
            sfr::rockblock::imu_report.push_back(0xFF);
#ifdef VERBOSE_IMUD
            Serial.print(" " + String(0xFF));

#endif
        }
#ifdef VERBOSE_IMUD
        Serial.println(" ");

#endif
    }
    sfr::imu::imu_dlink_report_ready = true;
    sfr::imu::report_downlinked = false;
}