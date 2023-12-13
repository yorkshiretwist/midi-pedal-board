// run this to enable dragging and dropping of the config file to the SD card
// remember to set the USB type to "MTP Disk (Experimental)"

// include the libraries needed to read and write files from the SD card
#include <ArduinoJson.h>
#include <SD.h>
#include <SPI.h>
#include <MTP_Teensy.h>

// set the chipSet for the SD card
#define CS_SD BUILTIN_SDCARD

void setup() {

  // waitn for the Serial to be available (no more than 3 seconds)
  while (!Serial && millis () < 3000);

  // mandatory to begin the MTP session.
  MTP.begin();

  // Add SD Card
  SD.begin(CS_SD);
  MTP.addFilesystem(SD, "SD Card");
}

void loop() {
  MTP.loop();
}