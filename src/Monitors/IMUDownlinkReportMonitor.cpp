#include "IMUDownlinkReportMonitor.hpp"
IMUDownlinkReportMonitor::IMUDownlinkReportMonitor(unsigned int offset) : TimedControlTask<void>(offset) {}

void IMUDownlinkReportMonitor::execute()
{
    if (sfr::imu::fragment_number_requested < sfr::rockblock::imu_max_fragments) {
        sfr::imu::fragment_requested = true;
        sfr::imu::fragment_number_requested++;
    }

    IMUDownlink imu_dlink = IMUDownlink(constants::timecontrol::imu_downlink_offset);
    imu_dlink.execute();
    // Get a requested fragment
    // if (sfr::imu::report_downlinked == true && sfr::imu::fragment_requested == true && sfr::rockblock::imu_first_start != false) {}
    // #ifdef VERBOSE_IMUMM
    //     Serial.println("sfr::imu::report_downlinked: " + String(sfr::imu::report_downlinked));
    //     Serial.println("sfr::imu::fragment_requested: " + String(sfr::imu::fragment_requested));
    //     Serial.println("sfr::imu::data_downlinked: " + String(sfr::imu::data_downlinked));
    //     Serial.println("sfr::rockblock::imu_first_start: " + String(sfr::rockblock::imu_first_start));

    // #endif
    if (sfr::rockblock::imu_first_start == false && sfr::imu::data_downlinked == true && sfr::imu::report_downlinked == true && sfr::imu::fragment_requested == true) {
#ifdef VERBOSE_IMUMM
        Serial.println("start creating imu downlink report");

#endif
        create_imu_downlink_report(sfr::imu::fragment_number);
        sfr::imu::fragment_requested = false;
        sfr::imu::fragment_number++;
        if (sfr::imu::fragment_number == sfr::rockblock::imu_max_fragments) {
            sfr::imu::imu_dlink_report_ready = false;
            sfr::imu::fragment_number = 0;
        }
    }
}

void IMUDownlinkReportMonitor::create_imu_downlink_report(int fragment_number)
{
    sfr::rockblock::imu_report.push_back(88);
#ifdef VERBOSE_IMUF
    // Serial.print(String(88) + " ");
    Serial.print(88, HEX);
    Serial.print(" ");
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

    // get each byte of fragment number
    std::vector<unsigned char> fragment(constants::imu::bytes_allocated_fragment);
    for (size_t i = 0; i < constants::imu::bytes_allocated_fragment; i++) {
        fragment[2 - i] = (fragment_number >> (i * 8));
        sfr::rockblock::imu_report.push_back(fragment[i]);
    }
    // sfr::rockblock::imu_report.push_back(fragment_number);
    // #ifdef VERBOSE_IMUF
    //     // Serial.print(String(fragment_number));
    //     Serial.print(fragment_number, HEX);
    //     Serial.print(" ");

    // #endif

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
            sfr::rockblock::imu_report.push_back(gyro_x);
            size_limit--;

            float gyro_y_value = sfr::imu::imu_dlink_gyro_y_buffer.back();
            uint8_t gyro_y = map(gyro_y_value, sfr::imu::gyro_min, sfr::imu::gyro_max, 0, 255);
            sfr::imu::imu_dlink_gyro_y_buffer.pop_back();
            sfr::rockblock::imu_report.push_back(gyro_y);
            size_limit--;

            float gyro_z_value = sfr::imu::imu_dlink_gyro_z_buffer.back();
            uint8_t gyro_z = map(gyro_z_value, sfr::imu::gyro_min, sfr::imu::gyro_max, 0, 255);
            sfr::imu::imu_dlink_gyro_z_buffer.pop_back();
            sfr::rockblock::imu_report.push_back(gyro_z);
            size_limit--;

#ifdef VERBOSE_IMUF
            // Serial.print(" " + String(gyro_x) + " " + String(gyro_y) + " " + String(gyro_z));
            Serial.print(gyro_x, HEX);
            Serial.print(" ");
            Serial.print(gyro_x, HEX);
            Serial.print(" ");
            Serial.print(gyro_x, HEX);
            Serial.print(" ");

#endif
        }
        // end marker
        if (idx == buffer_size - 1) {
            sfr::rockblock::imu_report.push_back(0xFF);
#ifdef VERBOSE_IMUF
            // Serial.print(" " + String(0xFF));
            Serial.print(0xFF, HEX);
            Serial.print(" ");
#endif
        }
#ifdef VERBOSE_IMUF
        // Serial.println(" ");

#endif
    }
#ifdef VERBOSE_IMUMM
    Serial.println("sfr::imu::imu_dlink_report_ready");

#endif
    sfr::imu::imu_dlink_report_ready = true;
    sfr::imu::report_downlinked = false;
}