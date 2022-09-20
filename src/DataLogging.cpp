#include "DataLogging.hpp"

const int chipSelect = BUILTIN_SDCARD;
File DataFile;

// int called = 0;

// void call(){
//     called = 1;
// }

// int IsCalled(){
//     return called;
// }

void DataLogSetup(){
    
    if (!SD.begin(chipSelect)) {
        Serial.println("SD Card initialization failed!");
        return;
    }
    Serial.println("SD Card initialization done.");
    SD.remove("Data.txt");
    DataFile = SD.open("Data.txt", FILE_WRITE);
    // DataFile.println("w_x, w_y, w_z, mag_x, mag_y, mag_z, de_I_x, de_I_y, de_I_z, pt_I_x, pt_I_y, pt_I_z");
    DataFile.println("mag_x_raw, mag_y_raw, mag_z_raw, smooth_x, smooth_y, smooth_z, pt_I_z");
    DataFile.close();
}

void DataLog(float Data[], int size)
{
    DataFile = SD.open("Data.txt", FILE_WRITE);
    if (DataFile) {
        for(int i = 0 ; i< size;i++) {
            DataFile.print(Data[i]);
            DataFile.print(", ");
        }
        DataFile.println();

        DataFile.close();
        Serial.println("done.");
    } else {
        // if the file didn't open, print an error:
        Serial.println("error opening Data.txt");
    }
    DataFile.close();
}