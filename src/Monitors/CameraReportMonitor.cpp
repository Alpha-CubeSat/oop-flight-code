#include "CameraReportMonitor.hpp"
CameraReportMonitor::CameraReportMonitor(unsigned int offset): TimedControlTask<void>(offset){}

void CameraReportMonitor::execute(){
    //Get a requested fragment
    if (sfr::camera::report_downlinked == true && sfr::camera::fragment_requested == true){
        uint16_t max_fragments = (sfr::camera::image_lengths[sfr::camera::serial_requested] / constants::camera::content_length) + (sfr::camera::image_lengths[sfr::camera::serial_requested] % constants::camera::content_length != 0 ? 1 : 0);
        String filename = "Image";
        if (sfr::camera::serial_requested < 10){
            filename += "0";
        }
        filename += String(sfr::camera::serial_requested) + ".JPG";
        imgFile = SD.open(filename.c_str(), FILE_READ);
        int i = 0;
        uint8_t tempbuffer[constants::camera::content_length];
        while (i < sfr::camera::fragment_number_requested){
            imgFile.read(tempbuffer, sfr::camera::data_length);
            i = i + 1;
        }
        sfr::camera::data_length = constants::camera::content_length;
        imgFile.read(tempbuffer, sfr::camera::data_length);
        create_camera_report(tempbuffer, sfr::camera::fragment_number_requested, sfr::camera::serial_requested);
        sfr::camera::fragment_requested = false;
    }

    //Prepare data from an image taken for downlink
    else if (sfr::camera::report_downlinked == true) {
        #ifdef VERBOSE
        Serial.println("Report monitor started");
        Serial.println("Current serial: " + String(sfr::camera::current_serial));
        Serial.println("Current fragment: " + String(sfr::camera::fragment_number));
        #endif


        if (sfr::camera::full_image_written == true || sfr::camera::fragment_number == 0 || sfr::camera::max_fragments == 0){
            sfr::camera::max_fragments = (sfr::camera::image_lengths[sfr::camera::current_serial] / constants::camera::content_length) + (sfr::camera::image_lengths[sfr::camera::current_serial] % constants::camera::content_length != 0 ? 1 : 0);
            sfr::camera::full_image_written = false;
        }

        #ifdef VERBOSE
        Serial.println("Max fragments: " + String(sfr::camera::max_fragments));
        #endif

        if (sfr::camera::fragment_number == sfr::camera::max_fragments - 1 && sfr::camera::max_fragments != 0){
            sfr::camera::data_length = sfr::camera::image_lengths[sfr::camera::current_serial] % constants::camera::content_length;
            sfr::camera::full_image_written = true;
        }
        else {
            sfr::camera::data_length = constants::camera::content_length;
        }

        if (sfr::camera::fragment_number == 0) {
            String filename = "Image";
            if (sfr::camera::current_serial < 10){
                filename += "0";
            }
            filename += String(sfr::camera::current_serial) + ".JPG";
            imgFile = SD.open(filename.c_str(), FILE_READ);
        }
            
        size_t i = 0;
        sfr::camera::buffer[i++] = sfr::camera::current_serial;
        sfr::camera::buffer[i++] = sfr::camera::fragment_number;
        sfr::camera::buffer[i++] = sfr::camera::max_fragments;
        sfr::camera::buffer[i++] = sfr::camera::data_length;
        uint8_t tempbuffer[constants::camera::content_length];
        imgFile.read(tempbuffer, sfr::camera::data_length);
        for (int j = 0; j < sfr::camera::data_length; j++){
            sfr::camera::buffer[i++] = tempbuffer[j];
        }
        if (sfr::camera::fragment_number == sfr::camera::max_fragments && sfr::camera::max_fragments != 0){
            imgFile.close();
            add_possible_command();
            sfr::camera::fragment_number = 0;
            Serial.println("INCEASING CURRENT SERIAL");
            sfr::camera::current_serial += 1;
            sfr::camera::fragment_requested = true;
        }
        create_camera_report(tempbuffer, sfr::camera::fragment_number, sfr::camera::current_serial);
        sfr::camera::fragment_number++;
        
    }
    
}

void CameraReportMonitor::create_camera_report(uint8_t tempbuffer[constants::camera::content_length], int fragment_number, int serial_number){
    std::vector<unsigned char> serial(constants::rockblock::arg1_len);
    for (size_t i = 0; i < constants::rockblock::arg1_len; i++){
        serial[3 - i] = (serial_number >> (i * 8));
    }

    for(size_t i = 0; i < constants::rockblock::arg1_len; i++ ) {
        sfr::rockblock::camera_report[i] = serial[i];
    }

    std::vector<unsigned char> fragment(constants::rockblock::arg2_len);
    for (size_t i = 0; i < constants::rockblock::arg2_len; i++){
        fragment[3 - i] = (fragment_number >> (i * 8));
    }

    size_t i = 0;
    int a = constants::rockblock::arg1_len;
    while(i < constants::rockblock::arg2_len){
        sfr::rockblock::camera_report[a] = fragment[i];
        a = a + 1;
        i = i + 1;
    }

    int x = 0;
    int y = constants::rockblock::arg1_len + constants::rockblock::arg2_len;
    while(x < constants::camera::content_length){
        sfr::rockblock::camera_report[y] = tempbuffer[x];
        y = y + 1;
        x = x + 1;
    }
    sfr::camera::report_ready = true;
    sfr::camera::report_downlinked = false;
}

void CameraReportMonitor::add_possible_command(){

    uint16_t max_fragments = (sfr::camera::image_lengths[sfr::camera::current_serial] / constants::camera::content_length) + (sfr::camera::image_lengths[sfr::camera::current_serial] % constants::camera::content_length != 0 ? 1 : 0);

    sfr::rockblock::camera_max_fragments[sfr::camera::current_serial] = max_fragments;

    uint32_t converted_serial =  __builtin_bswap32(sfr::camera::current_serial);

    for(int i = 0; i < constants::rockblock::opcode_len; i++){
        sfr::rockblock::camera_commands[sfr::camera::current_serial][i] = constants::rockblock::request_image_fragment[i];
    }

    for(int i = constants::rockblock::opcode_len; i < (constants::rockblock::arg1_len+constants::rockblock::opcode_len); i++){
        sfr::rockblock::camera_commands[sfr::camera::current_serial][i] = (converted_serial >> (8*i)) & 0xff;
    }
}