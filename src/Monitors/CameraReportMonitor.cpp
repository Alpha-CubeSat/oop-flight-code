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
        
        imgFile.read(tempbuffer, constants::camera::content_length);
        create_camera_report(tempbuffer, sfr::camera::fragment_number_requested, sfr::camera::serial_requested);
        sfr::camera::fragment_requested = false;
    }

    //Prepare data from an image taken for downlink
    else if (sfr::camera::report_downlinked == true && sfr::camera::images_written != 0) {
        #ifdef VERBOSE
        Serial.println("Report monitor started");
        Serial.println("Current serial: " + String(sfr::camera::current_serial));
        Serial.println("Current fragment: " + String(sfr::camera::fragment_number));
        #endif

        if (sfr::camera::full_image_written == true || sfr::camera::fragment_number == 0){
            sfr::camera::full_image_written = false;
        }

        String filename = "";
        if (sfr::camera::current_serial < 10){
            filename += "0";
        }
        filename += String(sfr::camera::current_serial);

        if (sfr::camera::fragment_number < 10) {
            filename += "0";
        }
        filename += String(sfr::camera::fragment_number) + ".jpg";

        imgFile = SD.open(filename.c_str(), FILE_READ);

        uint8_t tempbuffer[constants::camera::content_length*2];
        uint8_t parsedbuffer[constants::camera::content_length];


        for(int i = 0; i < sizeof(tempbuffer); i++){
            tempbuffer[i] = imgFile.read();
        }
        
        imgFile.read(tempbuffer, constants::camera::content_length);


        int x = 0;
        for(int i = 0; i < sizeof(tempbuffer); i++){
            int byte_0;
            int byte_1;
            if(tempbuffer[i] <= 90 && tempbuffer[i] >= 65){
                byte_0 = tempbuffer[i]- 55;
            } else{
                byte_0 = tempbuffer[i]- 48;
            }

            if(tempbuffer[i+1] <= 90 && tempbuffer[i+1] >= 65){
                byte_1 = tempbuffer[i+1]- 55;
            } else{
                byte_1 = tempbuffer[i+1]- 48;
            }

            parsedbuffer[x] = byte_1 + (byte_0 * 16);
            x++;
            i++;
        }

        imgFile.close();

        //TODO
        /*if (sfr::camera::fragment_number == sfr::camera::max_fragments && sfr::camera::max_fragments != 0){
            add_possible_command();
            sfr::camera::fragment_number = 0;
            sfr::camera::current_serial += 1;
        }*/


        create_camera_report(parsedbuffer, sfr::camera::fragment_number, sfr::camera::current_serial);
        sfr::camera::fragment_number++;  
    }

    //TODO
    /*if (sfr::camera::fragment_number == sfr::camera::max_fragments+1  && sfr::camera::current_serial == sfr::camera::images_written &&sfr::camera::max_fragments != 0){
        sfr::camera::report_ready = false;
    }*/
    
}

void CameraReportMonitor::create_camera_report(uint8_t tempbuffer[constants::camera::content_length], int fragment_number, short serial_number){
    sfr::rockblock::camera_report[0] = (serial_number & 255);
    sfr::rockblock::camera_report[1] = ((serial_number >> 8) & 255);

    std::vector<unsigned char> fragment(constants::camera::bytes_allocated_fragment);
    for (size_t i = 0; i < constants::camera::bytes_allocated_fragment; i++){
        fragment[3 - i] = (fragment_number >> (i * 8));
    }

    size_t i = 0;
    int a = constants::camera::bytes_allocated_serial;
    while(i < constants::camera::bytes_allocated_fragment){
        sfr::rockblock::camera_report[a] = fragment[i];
        a = a + 1;
        i = i + 1;
    }

    int x = 0;
    int y = constants::camera::bytes_allocated_serial + constants::camera::bytes_allocated_fragment;
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