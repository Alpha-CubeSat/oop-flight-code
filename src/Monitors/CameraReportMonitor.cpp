#include "CameraReportMonitor.hpp"

CameraReportMonitor::CameraReportMonitor(unsigned int offset): TimedControlTask<void>(offset){}

void CameraReportMonitor::execute()
{
  //Set SD card fault
  if (sfr::camera::sd_card_failed == true)
  {
    sfr::fault::fault_3 = sfr::fault::fault_3 | constants::fault::sd_card;
  }
  //Set camera power on fault
  if (sfr::camera::camera_failed == true)
  {
    sfr::fault::fault_3 = sfr::fault::fault_3 | constants::fault::camera_on_failed;
  }
  //Get a requested fragment
  if (sfr::camera::fragment_requested == true)
  {
    Serial.println("Fragment requested");
    uint16_t max_fragments = (sfr::camera::image_lengths[sfr::camera::serial_requested] / 66) + (sfr::camera::image_lengths[sfr::camera::serial_requested] % 66 != 0 ? 1 : 0);
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
      uint8_t tempbuffer[66];
      while (i < sfr::camera::fragment_requested)
      {
        imgFile.read(tempbuffer, sfr::camera::data_length);
        i++;
      }
      sfr::camera::data_length = 66;
      imgFile.read(tempbuffer, sfr::camera::data_length);
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
        sfr::camera::max_fragments = (sfr::camera::image_lengths[sfr::camera::current_serial] / 66) + (sfr::camera::image_lengths[sfr::camera::current_serial] % 66 != 0 ? 1 : 0);
        sfr::camera::full_image_written = false;
      }
      Serial.println("Max fragments: " + String(sfr::camera::max_fragments));
      if (sfr::camera::fragment_number == sfr::camera::max_fragments - 1)
      {
        sfr::camera::data_length = sfr::camera::image_lengths[sfr::camera::current_serial] % 66;
        sfr::camera::full_image_written == true;
      }
      else
      {
        sfr::camera::data_length = 66;
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
      uint8_t tempbuffer[66];
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
    }
    if (sfr::camera::report_ready == true && sfr::camera::current_serial == sfr::camera::images_written)
    {
      sfr::camera::report_ready = false;
      Serial.println("Report finished");
    }
  }
}
