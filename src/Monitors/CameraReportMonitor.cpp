#include "CameraReportMonitor.hpp"
CameraReportMonitor::CameraReportMonitor(unsigned int offset)
    : TimedControlTask<void>(offset)
{
}

void CameraReportMonitor::execute()
{
    // Get a requested fragment
    if (sfr::rockblock::camera_report.empty() && fragment_requested) {
        create_camera_report(fragment_number_requested, serial_requested);
        fragment_requested = false;
    }
    // Prepare data from an image taken for downlink
    else if (sfr::rockblock::camera_report.empty() && images_written != 0) {
#ifdef VERBOSE
        Serial.println("Report monitor started");
        Serial.println("Current serial: " + String(current_serial));
        Serial.println("Current fragment: " + String(fragment_number));
#endif
        create_camera_report(fragment_number, current_serial);
        if (fragment_number == sfr::rockblock::camera_max_fragments[current_serial]) {
            current_serial += 1;
        } else {
            fragment_number++;
        }
    }

    if (fragment_number == sfr::rockblock::camera_max_fragments[current_serial]) {
        sfr::camera::report_ready = false;
        fragment_number = 0;
    }
}

void CameraReportMonitor::create_camera_report(int fragment_number, uint8_t serial_number)
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
    imgFile = SD.open(filename.c_str(), FILE_READ);

    // parse hex stored as chars into actual hex
    uint8_t tempbuffer[constants::camera::content_length * 2];
    uint8_t parsedbuffer[constants::camera::content_length];
    for (size_t i = 0; i < sizeof(tempbuffer); i++) {
        tempbuffer[i] = imgFile.read();
    }
    imgFile.read(tempbuffer, constants::camera::content_length);
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

    sfr::rockblock::camera_report.push_back(42);
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
