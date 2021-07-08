#include "CameraReportMonitor.hpp"
CameraReportMonitor::CameraReportMonitor(unsigned int offset): TimedControlTask<void>(offset){}

void CameraReportMonitor::execute()
{
  //Get a requested fragment
  if (sfr::camera::fragment_requested == true)
  {
    Serial.println("Fragment requested");
    uint16_t max_fragments = (sfr::camera::image_lengths[sfr::camera::serial_requested] / constants::camera::content_length) + (sfr::camera::image_lengths[sfr::camera::serial_requested] % constants::camera::content_length != 0 ? 1 : 0);
    if (sfr::camera::fragment_number_requested >= max_fragments)
    {
      Serial.println("This fragment number doesn't exist!");
    }
    else if (sfr::camera::serial_requested + 1 > sfr::camera::images_written)
    {
      Serial.println("This serial number doesn't exist!");
    }
    else
    {
      String filename = "Image";
      if (sfr::camera::serial_requested < 10)
      {
        filename += "0";
      }
      filename += String(sfr::camera::serial_requested) + ".JPG";
      imgFile = SD.open(filename.c_str(), FILE_READ);
      int i = 0;
      uint8_t tempbuffer[constants::camera::content_length];
      while (i < sfr::camera::fragment_requested)
      {
        imgFile.read(tempbuffer, sfr::camera::data_length);
        i++;
      }
      sfr::camera::data_length = constants::camera::content_length;
      imgFile.read(tempbuffer, sfr::camera::data_length);
      create_camera_report(tempbuffer, sfr::camera::fragment_requested, sfr::camera::serial_requested);
      Serial.println("Fragment request complete");
    }
    sfr::camera::fragment_requested = false;
  }


  //Prepare data from an image taken for downlink
  else if (sfr::camera::report_ready == true && sfr::camera::report_downlinked == true)
  {
    if (sfr::camera::current_serial != sfr::camera::images_written)
    {
      Serial.println("Report monitor started");
      Serial.println("Current serial: " + String(sfr::camera::current_serial));
      Serial.println("Current fragment: " + String(sfr::camera::fragment_number));
      if (sfr::camera::full_image_written == true || sfr::camera::fragment_number == 0)
      {
        sfr::camera::max_fragments = (sfr::camera::image_lengths[sfr::camera::current_serial] / constants::camera::content_length) + (sfr::camera::image_lengths[sfr::camera::current_serial] % constants::camera::content_length != 0 ? 1 : 0);
        sfr::camera::full_image_written = false;
      }
      Serial.println("Max fragments: " + String(sfr::camera::max_fragments));
      if (sfr::camera::fragment_number == sfr::camera::max_fragments - 1)
      {
        sfr::camera::data_length = sfr::camera::image_lengths[sfr::camera::current_serial] % constants::camera::content_length;
        sfr::camera::full_image_written = true;
      }
      else
      {
        sfr::camera::data_length = constants::camera::content_length;
      }
      if (sfr::camera::fragment_number == 0)
      {
        String filename = "Image";
        if (sfr::camera::current_serial < 10)
        {
          filename += "0";
        }
        filename += String(sfr::camera::current_serial) + ".JPG";
        imgFile = SD.open(filename.c_str(), FILE_READ);
      }
      int i = 0;
      sfr::camera::buffer[i++] = sfr::camera::current_serial;
      sfr::camera::buffer[i++] = sfr::camera::fragment_number;
      sfr::camera::buffer[i++] = sfr::camera::max_fragments;
      sfr::camera::buffer[i++] = sfr::camera::data_length;
      uint8_t tempbuffer[constants::camera::content_length];
      imgFile.read(tempbuffer, sfr::camera::data_length);
      for (int j = 0; j < sfr::camera::data_length; j++)
      {
        sfr::camera::buffer[i++] = tempbuffer[j];
      }
      sfr::camera::fragment_number++;
      if (sfr::camera::fragment_number == sfr::camera::max_fragments)
      {
        imgFile.close();
        sfr::camera::fragment_number = 0;
        sfr::camera::current_serial += 1;
        sfr::camera::fragment_requested = true;
      }
      create_camera_report(tempbuffer, sfr::camera::fragment_number, sfr::camera::current_serial);
    }
    if (sfr::camera::report_ready == true && sfr::camera::current_serial == sfr::camera::images_written)
    {
      sfr::camera::report_ready = false;
      Serial.println("Report finished");
    }
  }
}

void CameraReportMonitor::create_camera_report(uint8_t tempbuffer[constants::camera::content_length], int fragment_number, int serial_number){
  using namespace std;

  vector<unsigned char> serial(constants::rockblock::arg1_len);
  for (int i = 0; i < constants::rockblock::arg1_len; i++){
    serial[3 - i] = (serial_number >> (i * 8));
  }

  for(int i = 0; i < constants::rockblock::arg1_len; i++ ) {
    sfr::rockblock::camera_report[i] = serial[i];
  }

  vector<unsigned char> fragment(constants::rockblock::arg2_len);
  for (int i = 0; i < constants::rockblock::arg2_len; i++){
    fragment[3 - i] = (fragment_number >> (i * 8));
  }

  int i = 0;
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
