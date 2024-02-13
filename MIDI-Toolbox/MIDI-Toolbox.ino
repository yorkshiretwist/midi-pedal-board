const char version[] = "0.1";

//----------------------------------------------------------------------------//
//                                                                            //
//    MIDI Toolbox                                                            //
//                                                                            //
//    A highly-configurable MIDI controller operated by footswitches,         //
//    designed for use by a musician already using their hands (e.g. a        //
//    guitarist).                                                             //
//                                                                            //
//    Copyright (C) 2023 Chris Taylor                                         //
//    chris@stillbreathing.co.uk                                              //
//    https://stillbreathing.co.uk                                            //
//                                                                            //
//----------------------------------------------------------------------------//
//                                                                            //
//    This program is free software: you can redistribute it and/or modify    //
//    it under the terms of the GNU General Public License as published by    //
//    the Free Software Foundation, either version 3 of the License, or       //
//    (at your option) any later version.                                     //
//                                                                            //
//    This program is distributed in the hope that it will be useful,         //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of          //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           //
//    GNU General Public License for more details.                            //
//                                                                            //
//    You should have received a copy of the GNU General Public License       //
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.  //
//                                                                            //
//----------------------------------------------------------------------------//

// include the debounce library
#include <Bounce.h>

// include standard libraries needed for types
#include <vector>
#include <string>

// include the Note class definition and vector of all notes
#include "Notes.h"

// include the pins setup
#include "Pins.h"

// include the Button class definition and functions
#include "Buttons.h"

// include the libraries needed to read and write files from the SD card
#include <ArduinoJson.h>
#include <SD.h>
#include <SPI.h>

// libraries for the screen
#include "ILI9341_t3.h"
#include "font_Arial.h"

// set the chipSet for the SD card
#define CS_SD BUILTIN_SDCARD

// TFT screen constants
#define TFT_DC  9
#define TFT_CS 10

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);

// ============================================================================
// Debugging

const bool printDebug = true;

void print(String msg) {
  if (!printDebug) {
    return;
  }
  Serial.println(msg);
}

// ============================================================================
// state variables
























