#include "CameraReportMonitor.hpp"
CameraReportMonitor::CameraReportMonitor()
{
}

void CameraReportMonitor::execute()
{
    // Get a requested fragment
    if (sfr::rockblock::camera_report.empty() && sfr::camera::fragment_requested) {
#ifdef VERBOSE
        Serial.println("Requested Camera fragment " + String(sfr::camera::fragment_number_requested));
#endif
        create_camera_report(sfr::camera::fragment_number_requested, sfr::camera::serial_requested);
        sfr::camera::fragment_requested = false;
    }
    // Prepare data from an image taken for downlink
    else if (sfr::rockblock::camera_report.empty() && sfr::camera::images_written > current_serial) {
#ifdef VERBOSE
        Serial.println("Writing Camera serial " + String(current_serial) + " fragment " + String(fragment_number));
#endif
        create_camera_report(fragment_number, current_serial);
        if (fragment_number == sfr::rockblock::camera_max_fragments[current_serial]) {
            current_serial++;
            fragment_number = 0;
            sfr::camera::report_ready = false;
        } else {
            fragment_number++;
        }
    } else if (sfr::rockblock::camera_report.empty() && sfr::camera::images_written == current_serial) {
        sfr::camera::report_ready = false;
    }
}

void CameraReportMonitor::create_camera_report(uint32_t fragment_number, uint8_t serial_number)
{
    // open image file and read it for specified image/fragment
    String filename = "";
    if (serial_number < 10) {
        filename += "0";
    }
    filename += String(serial_number);
    if (fragment_number < 10) {
        filename += "0";
    }
    filename += String(fragment_number) + ".jpg";
#ifdef SIMULATOR
    SD.begin(254);
#endif
    File imgFile = SD.open(filename.c_str(), FILE_READ);
    // parse hex stored as chars into actual hex
    uint8_t tempbuffer[constants::camera::content_length * 2];
    uint8_t parsedbuffer[constants::camera::content_length];
    for (size_t i = 0; i < sizeof(tempbuffer); i++) {
        tempbuffer[i] = imgFile.read();
    }

    int x = 0;
    for (size_t i = 0; i < sizeof(tempbuffer); i++) {
        int byte_0;
        int byte_1;
        if (tempbuffer[i] <= 90 && tempbuffer[i] >= 65) {
            byte_0 = tempbuffer[i] - 55;
        } else {
            byte_0 = tempbuffer[i] - 48;
        }
        if (tempbuffer[i + 1] <= 90 && tempbuffer[i + 1] >= 65) {
            byte_1 = tempbuffer[i + 1] - 55;
        } else {
            byte_1 = tempbuffer[i + 1] - 48;
        }
        parsedbuffer[x] = byte_1 + (byte_0 * 16);
        x++;
        i++;
    }
    imgFile.close();

    sfr::rockblock::camera_report.push_back(constants::rockblock::camera_report_flag);

    // get each byte of serial number and add to camera report
    sfr::rockblock::camera_report.push_back(serial_number);

    // get each byte of fragment number
    std::vector<unsigned char> fragment(constants::camera::bytes_allocated_fragment);
    for (size_t i = 0; i < constants::camera::bytes_allocated_fragment; i++) {
        fragment[3 - i] = (fragment_number >> (i * 8));
        sfr::rockblock::camera_report.push_back(fragment[i]);
    }

    // add actual image content to camera report
    for (int i = 0; i < constants::camera::content_length; i++) {
        sfr::rockblock::camera_report.push_back(parsedbuffer[i]);
    }

    sfr::camera::report_ready = true;
}
