// list files on SD card, then ask whether want to read or delete

#include <SD.h>

const int chipSelect = BUILTIN_SDCARD;

File root;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // wait for Serial Monitor to connect. Needed for native USB port boards only:
  while (!Serial);
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("1. is a card inserted?");
    Serial.println("2. is your wiring correct?");
    Serial.println("3. did you change the chipSelect pin to match your shield or module?");
    Serial.println("Note: press reset or reopen this serial monitor after fixing your issue!");
    while (true);
  }
  Serial.println("initialization done.");
  root = SD.open("/");
  printDirectory(root, 0);
  Serial.println("done!");
}

void loop() {
  Serial.println("What would you like to do next? Type 'read' or 'delete' or 'list':");
  while(Serial.available()==0) {
  }   
 String thisString = Serial.readString();
 if (thisString == "read\n"){
     Serial.println("Type the filename you would like to read:");
       while(Serial.available()==0) {
  }   
     String fileName = Serial.readString();
     Serial.println("Is this file a fragment or full JPG? Type 'fragment' or 'full':");
     while(Serial.available()==0) {
     }   
     String type = Serial.readString();
     if (type == "fragment\n"){
        sendFragment(fileName.trim());
     }
     if (type == "full\n"){
        sendFullFile(fileName.trim());
     }

     
 }
 else if (thisString == "delete\n"){
     Serial.println("Type the filename you would like to delete:");
       while(Serial.available()==0) {
  }   
     String thisString = Serial.readString();
     removeFile(thisString.trim());
 }
 else if (thisString == "list\n"){
    root = SD.open("/");
    printDirectory(root, 0);
    Serial.println("done!");
 }
 else {
    Serial.println("Command not recognized. Please type 'read' or 'delete' or 'list':");
 }
}

void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

void sendFullFile(String fileName){
  File dataFile = SD.open(fileName.c_str());
  // if the file is available, write to it:
  if (dataFile) {
    while (dataFile.available()) {
      char c = dataFile.read();
      if(c < 0x10){
        Serial.print(0);
      }
      Serial.print(c, HEX);
    }
    dataFile.close();
    Serial.println();
    Serial.println("File read complete");
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening file");
  }
}

void sendFragment(String fileName){
  File dataFile = SD.open(fileName.c_str());
  // if the file is available, write to it:
  if (dataFile) {
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    dataFile.close();
    Serial.println();
    Serial.println("File read complete");
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening file");
  }
}

void removeFile(String fileName){
  if (SD.exists(fileName.c_str())) {
    Serial.println(fileName +  " exists.");
  } else {
    Serial.println(fileName +  " doesn't exist.");
  }
  // delete the file:
  Serial.println("Removing " + fileName +  "...");
  SD.remove(fileName.c_str());
  if (!SD.exists(fileName.c_str())){
    Serial.println(fileName + " successfully deleted.");
  }
   root = SD.open("/");
   printDirectory(root, 0);

}
