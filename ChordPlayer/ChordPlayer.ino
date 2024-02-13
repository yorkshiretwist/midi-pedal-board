// run this to use the unit in chord mode
// remember to set the USB type to "Serial + MIDI"

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

// MIDI notes: https://computermusicresource.com/midikeys.html

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

// the index of the current active button
int currentButtonIndex = -1;

// whether hold note mode is on or off
bool holdMode = true;

// the current chord name, shown on the screen
String currentChordName = "";

// the notes currently playing
std::vector<Note> currentPlayingNotes;

// store all possible MIDI notes in an array
std::vector<Note> allNotes;

// the deserialised JSON file
JsonDocument jsonDoc;

// an error deserialising the JSoN file (if any)
DeserializationError jsonError;

// array of modes
const String modes[] = {"Key", "Chn"};

// the current mode index
int currentModeIndex = 0;

// the previous mode index
int previousModeIndex = -1;

// flag to say if we are changing MIDI channel
bool changingMidiChannel = false;

// the enabled MIDI channels
std::vector<int> midiChannels;

// the MIDI channel number to send messages
int currentMidiChannelIndex = 0;

// the previous MIDI channel
int previousMidiChannelIndex = -1;

// the names to show for each MIDI channel - these correspond to the patches in the DAW
std::vector<String> midiChannelNames;

// the array of keys
std::vector<String> keys;

// flag to indicate if we are changing key
bool changingKey = false;

// current key index
int currentKeyIndex = 0;

// previous key index
int previousKeyIndex = -1;

// the array of registers
std::vector<String> registers;

// flag to indicate if we are changing register
bool changingRegister = false;

// current register index
int currentRegisterIndex = 0;

// previous register index
int previousRegisterIndex = -1;

// set the default spread, which is to play all notes at the same time
String spread = "Normal";

// if the unit is ready for playing
bool initialised = false;

// ============================================================================
// screen functions

int fontSize = 4;

void display() {
  display("");
}

void blankLine(int lineSize) {
  tft.setTextSize(lineSize);
  tft.println(" ");
  tft.setTextSize(fontSize);
}

void display(String errorMessage) {
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(fontSize);

  tft.println("Key: " + keys[currentKeyIndex]);
  blankLine(2);

  if (registers[currentRegisterIndex].compareTo("Low") == 0) {
    tft.println("| |         ");
  } else if (registers[currentRegisterIndex].compareTo("Middle") == 0) {
    tft.println("  |  | | |  ");
  } else if (registers[currentRegisterIndex].compareTo("Wide") == 0) {
    tft.println("  | | | || |");
  } else if (registers[currentRegisterIndex].compareTo("High") == 0) {
    tft.println("       | | |");
  }
  blankLine(2);

  if (currentChordName.compareTo("") != 0) {
    tft.println(currentChordName);
    blankLine(2);
  }

  if (errorMessage.compareTo("") != 0) {
    tft.setTextColor(ILI9341_RED);
    tft.println(errorMessage);
    blankLine(2);
  }

  tft.setTextSize(3);
  tft.println("[Ch " + (String)midiChannels[currentMidiChannelIndex] + "] " + midiChannelNames[currentMidiChannelIndex]);
  blankLine(1);
  
  tft.setTextSize(3);
  tft.println("C/D mode: " + modes[currentModeIndex]);
}

void displayWelcome() {
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(3);
  tft.println("Starting up...");
  delay(1500);

  // display the values from the config

  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);

  tft.println("CONFIG");
  tft.println("MIDI channels:");
  String midiChannelString = "";
  for(int x = 0; x < (int)midiChannels.size(); x++) {
    midiChannelString += (String)midiChannels[x] + ", ";
  }
  tft.println(midiChannelString.substring(0, midiChannelString.length() - 2));

  tft.println("Keys:");
  String keyString = "";
  for(int x = 0; x < (int)keys.size(); x++) {
    keyString += keys[x] + ", ";
  }
  tft.println(keyString.substring(0, keyString.length() - 2));

  tft.println("Registers:");
  String registerString = "";
  for(int x = 0; x < (int)registers.size(); x++) {
    registerString +=  registers[x] +", ";
  }
  tft.println(registerString.substring(0, registerString.length() - 2));

  tft.println("Press any button to start");
}

// ============================================================================
// JSON functions

// read the JSON file
void readJson() {
  File dataFile = SD.open("config.json");
  jsonError = deserializeJson(jsonDoc, dataFile);
  dataFile.close();

  // if the config file could be read OK then populate the values
  if (jsonError == DeserializationError::Ok) {

    // get the keys
    JsonArray configKeys = jsonDoc["keys"].as<JsonArray>();
    String currentKeyName = jsonDoc["defaults"]["key"].as<String>();
    int keyIndex = 0;
    for (JsonVariant configKey : configKeys) {
      String keyName = configKey.as<String>();
      keys.push_back(keyName);
      if (currentKeyName.compareTo(keyName) == 0) {
        currentKeyIndex = keyIndex;
      }
      keyIndex++;
    }
    
    // get the registers
    JsonArray configRegisters = jsonDoc["registers"].as<JsonArray>();
    String currentRegisterName = jsonDoc["defaults"]["register"].as<String>();
    int registerIndex = 0;
    for (JsonVariant configRegister : configRegisters) {
      String registerName = configRegister.as<String>();
      registers.push_back(registerName);
      if (currentRegisterName.compareTo(registerName) == 0) {
        currentRegisterIndex = registerIndex;
      }
      registerIndex++;
    }

    // get the MIDI channels
    JsonArray configMidiChannels = jsonDoc["midiChannels"].as<JsonArray>();
    currentMidiChannelIndex = jsonDoc["defaults"]["midiChannel"].as<int>() - 1;
    for (JsonObject configMidiChannel : configMidiChannels) {
      midiChannelNames.push_back(configMidiChannel["name"].as<String>());
      midiChannels.push_back(configMidiChannel["channelNumber"].as<int>());
    }

  }
}

// print the error if the JSON file could not be read
void checkJson() {
  if (jsonError != DeserializationError::Ok) {
    print(F("deserializeJson() failed with code@"));
    print(jsonError.f_str());
  }
}

// ============================================================================
// note functions

// turn all the currently playing notes off
void turnAllNotesOff() {
  print("Turning all notes off");
  // turn the notes currently playing off
  for(int x = 0; x < (int)currentPlayingNotes.size(); x++) {
    stopNote(currentPlayingNotes[x]);
  }
  // turn the current button light off
  if (currentButtonIndex > -1) {
    //digitalWrite(ledPins[currentButtonIndex], LOW);
  }
  // clear all current notes
  currentPlayingNotes.clear();
  // reset the chord name
  currentChordName = "";
}

// get the note with the given name
Note getNote(std::vector<Note> allNotes, String noteName) {
  for(int x = 0; x < (int)allNotes.size(); x++) {
    if (noteName.compareTo(allNotes[x].Name) == 0) {
      print("Found note " + noteName);
      return allNotes[x];
    }
  }
  print("Could not find note " + noteName);
  return Note("NULL", -1, -1);
}

// this function is not called because the buttonConfig values get screwed up
// I wish I understood C++ more
std::vector<Note> getKeyChordNotes(std::vector<Note> allNotes, JsonObject & buttonConfig, String keyName, String registerName) {
  bool debug = true;
  if (debug) {
    Serial.println("Inside the function:");
    serializeJson(buttonConfig, Serial);
    Serial.println("");
    Serial.println("Getting notes for key: " + keyName + ", register: " + registerName);
    Serial.println("Button name: " + buttonConfig["name"].as<String>());
  }
  std::vector<Note> notes;

  // get the keys
  JsonArray keys = buttonConfig["keys"].as<JsonArray>();
  for (JsonObject key : keys) {
    if (debug) {
      Serial.println("Key: " + key["name"].as<String>());
    }

    if (key["name"].as<String>() == keyName) {

      // get the chords in this key
      JsonArray chords = key["chords"].as<JsonArray>();
      for (JsonObject chord : chords) {
        if (debug) {
          Serial.println("Register: " + chord["register"].as<String>());
        }

        if (chord["register"].as<String>() == registerName) {

          // get the notes in this chord
          JsonArray noteNames = chord["notes"].as<JsonArray>();
          for (JsonVariant noteName : noteNames) {
            Note note = getNote(allNotes, noteName.as<String>());
            notes.push_back(note);
          }
        }
      }
    }
  }
  return notes;
}

// ============================================================================
// mode functions

// change mode to the next one up (this is triggered by just one button, so no up/down)
void changeMode() {
  previousModeIndex = currentModeIndex;
  currentModeIndex = currentModeIndex + 1;
  if (currentModeIndex == 2) {
    currentModeIndex = 0;
  }
  print("Changing mode from " + modes[previousModeIndex] + " to " + modes[currentModeIndex]);
}

// ============================================================================
// MIDI functions

// go to the next MIDI channel up, or back to the bottom
void midiChannelUp() {
  changingMidiChannel = true;
  previousMidiChannelIndex = currentMidiChannelIndex;
  currentMidiChannelIndex = currentMidiChannelIndex + 1;
  if (currentMidiChannelIndex == (int)midiChannels.size()) {
    currentMidiChannelIndex = 0;
  }
  print("Changing MIDI channel up from " + (String)midiChannels[previousMidiChannelIndex] + " to " + (String)midiChannels[currentMidiChannelIndex]);
}

// go to the next MIDI channel down, or back to the top
void midiChannelDown() {
  changingMidiChannel = true;
  previousMidiChannelIndex = currentMidiChannelIndex;
  currentMidiChannelIndex = currentMidiChannelIndex - 1;
  if (currentMidiChannelIndex == -1) {
   currentMidiChannelIndex = (int)midiChannels.size() - 1;
  }
  print("Changing MIDI channel down from " + (String)midiChannels[previousMidiChannelIndex] + " to " + (String)midiChannels[currentMidiChannelIndex]);
}

// start  playing the given note
void playNote(Note note) {
  print(" Playing " + note.Name + ", MIDI note " + (String)note.MidiNoteNumber + ", channel " + (String)midiChannels[currentMidiChannelIndex]);
  // set the MIDI channel this note is playing on
  note.MidiPlayingChannel = midiChannels[currentMidiChannelIndex];
  // add it to the array of playing notes
  currentPlayingNotes.push_back(note);
  // play the note on the current MIDI channel
  usbMIDI.sendNoteOn(note.MidiNoteNumber, 99, midiChannels[currentMidiChannelIndex]);
  //usbMIDI.sendProgramChange(program, channel)
}

// stop playing the given note
void stopNote(Note note) {
  print(" Stopping " + note.Name + ", MIDI note " + (String)note.MidiNoteNumber + ", channel " + (String)note.MidiPlayingChannel);
  // stop the note on the channel it is playing on
  usbMIDI.sendNoteOff(note.MidiNoteNumber, 0, note.MidiPlayingChannel);
}

// ============================================================================
// key functions

// go to the next key up, or back to the bottom
void keyUp() {
  previousKeyIndex = currentKeyIndex;
  changingKey = true;
  currentKeyIndex = currentKeyIndex + 1;
  if (currentKeyIndex == (int)keys.size()) {
    currentKeyIndex = 0;
  }
  print("Changing key up from " + keys[previousKeyIndex] + " to " + keys[currentKeyIndex]);
}

// go to the next key down, or back to the top
void keyDown() {
  previousKeyIndex = currentKeyIndex;
  changingKey = true;
  currentKeyIndex = currentKeyIndex - 1;
  if (currentKeyIndex == -1) {
   currentKeyIndex = (int)keys.size() - 1;
  }
  print("Changing key down from " + keys[previousKeyIndex] + " to " + keys[currentKeyIndex]);
}

// ============================================================================
// register functions

// go to the next register up, or back to the bottom
void registerUp() {
  previousRegisterIndex = currentRegisterIndex;
  changingRegister = true;
  currentRegisterIndex = currentRegisterIndex + 1;
  if (currentRegisterIndex == 4) {
    currentRegisterIndex = 0;
  }
  print("Changing register up from " + registers[previousRegisterIndex] + " to " + registers[currentRegisterIndex]);
}

// go to the next register down, or back to the top
void registerDown() {
  previousRegisterIndex = currentRegisterIndex;
  changingRegister = true;
  currentRegisterIndex = currentRegisterIndex - 1;
  if (currentRegisterIndex == -1) {
   currentRegisterIndex = 3;
  }
   print("Changing register down from " + registers[previousRegisterIndex] + " to " + registers[currentRegisterIndex]);
}

void changeRegister(int newRegisterIndex) {
  previousRegisterIndex = currentRegisterIndex;
  changingRegister = true;
  currentRegisterIndex = newRegisterIndex;
  print("Changing register down from " + registers[previousRegisterIndex] + " to " + registers[currentRegisterIndex]);
}

// ============================================================================
// button variables and functions

// when a button is triggered
void triggerButton(int buttonIndex) {

  // if we've not initialised the box yet then do it
  if (initialised == false) {
    print("Initialising... ");
    initialised = true;
    display();
    return;
  }

  // ensure we have some JSON config
  checkJson();

  print("Button triggered: " + (String)buttonIndex);

  // if the button index is 5 or above then handle special functions
  if (buttonIndex >= 5) {

    // middle row

    // button 6 - low register
    if (buttonIndex == 5) {
      changeRegister(0);
    }

    // button 7 - middle register
    if (buttonIndex == 6) {
      changeRegister(1);
    }

    // button 8 - wide register
    if (buttonIndex == 7) {
      changeRegister(2);
    }

    // button 9 - high register
    if (buttonIndex == 8) {
      changeRegister(3);
    }

    // top row

    // button A - change mode
    if (buttonIndex == 10) {
      changeMode();
    }

    // button B - change key down/MIDI channel down
    if (buttonIndex == 11) {
      if (modes[currentModeIndex].compareTo("Key") == 0) {
        keyDown();
      } else if (modes[currentModeIndex].compareTo("Chn") == 0) {
        midiChannelDown();
      }
    }

    // button C - change key up/MIDI channel up
    if (buttonIndex == 12) {
      if (modes[currentModeIndex].compareTo("Key") == 0) {
        keyUp();
      } else if (modes[currentModeIndex].compareTo("Chn") == 0) {
        midiChannelUp();
      }
    }

    display();
    return;
  }
  
  // print the current button for debugging
  print("Current button: " + (String)currentButtonIndex);
  
  // stop playing all notes
  turnAllNotesOff();

  // if we've pressed the same button and we're not changing key, register, or MIDI channel then
  // reset the current button index and return, as we don't want to play new notes
  if (currentButtonIndex == buttonIndex && changingKey == false && changingRegister == false && changingMidiChannel == false) {
    print("Resetting current button");
    currentButtonIndex = -1;
    print("");
    display();
    return;
  }

  // reset some state variables
  changingKey = false;
  changingRegister = false;
  changingMidiChannel = false;

  // set the current button
  print("Setting current button to " + (String)buttonIndex);
  currentButtonIndex = buttonIndex;
  
  // get the button
  JsonObject buttonConfig; //getButton(jsonDoc, buttonIndex);
  JsonArray array = jsonDoc["buttons"].as<JsonArray>();
  for(JsonObject button : array) {
      if (button["index"].as<int>() == buttonIndex) {
        buttonConfig = button;
      }
  }

  print(serializeJson(buttonConfig, Serial));
  if (buttonConfig == NULL) {
    print("Could not find button");
    display("Could not find button");
    return;
  } else {
    print("Found button " + buttonConfig["index"].as<String>());
  }

  // get the current key and register
  String keyName = keys[currentKeyIndex];
  String registerName = registers[currentRegisterIndex];

  //std::vector<Note> notes = getKeyChordNotes(allNotes, button, keys[currentKeyIndex], registers[currentRegisterIndex]);

  // this section should be in functions, but because I don't understand C++ the variable values get corrupted

  bool debugNotes = false;
  if (debugNotes) {
    print("Button config:");
    if (printDebug) {
      serializeJson(buttonConfig, Serial);
    }
    print("");
    print("Getting notes for key: " + keyName + ", register: " + registerName);
    print("Button name: " + buttonConfig["name"].as<String>());
  }
  std::vector<Note> notes;

  // get the keys
  JsonArray keys = buttonConfig["keys"].as<JsonArray>();
  for (JsonObject key : keys) {

    if (keyName.compareTo(key["name"].as<String>()) == 0) {

      if (debugNotes) {
        print("Found key: " + key["name"].as<String>());
      }

      // get the chords in this key
      JsonArray chords = key["chords"].as<JsonArray>();
      for (JsonObject chord : chords) {

        if (registerName.compareTo(chord["registerName"].as<String>()) == 0) {

          if (debugNotes) {
            print("Found register: " + chord["registerName"].as<String>() + ", chord " + chord["chordName"].as<String>());
          }

          currentChordName = chord["chordName"].as<String>();

          // set the default spread
          spread = "Normal";

          // get the spread type, if it is set
          if (chord.containsKey("spread")) {
            spread = chord["spread"].as<String>();
          }

          // get the notes in this chord
          JsonArray noteNames = chord["notes"].as<JsonArray>();
          for (JsonVariant noteName : noteNames) {

            //Note note = getNote(allNotes, noteName.as<String>());
            for(int x = 0; x < (int)allNotes.size(); x++) {
              String noteNameStr = noteName.as<String>();
              if (noteNameStr.compareTo(allNotes[x].Name) == 0) {
                print("Found note " + noteNameStr);
                notes.push_back(allNotes[x]);
              }
            }
          }
        }
      }
    }
  }

  // if no notes found return
  if ((int)notes.size() == 0) {
    print("No notes found");
    print("");
    display("No notes found");
    return;
  }

  // play all notes at the same time
  if (spread == "Normal") {
    for(int x = 0; x < (int)notes.size(); x++) {
      playNote(notes[x]);
    }

  // arpeggiate notes up
  } else if (spread == "Up") {
    for(int x = 0; x < (int)notes.size(); x++) {
      playNote(notes[x]);
      delay(65);
    }

  // arpeggiate notes down
  } else if (spread == "Down") {
    for(int x = (int)notes.size() - 1; x >= 0 ; x--) {
      playNote(notes[x]);
      delay(65);
    }
  }

  // turn the LED for the triggered button on
  //digitalWrite(ledPins[currentButtonIndex], HIGH);

  print("");
  display();
}


// ============================================================================
// setup function

void setup() {

  print("setup() called");

  // waitn for the Serial to be available (no more than 3 seconds)
  while (!Serial && millis () < 3000);

  // initialise the screen
  tft.begin();

  // try to initialise the SD card
  if (!SD.begin(CS_SD)) {
    print("SD card failed, or not present");
    // don't do anything more:
    return;
  }
  print("SD card initialized");

  // setup the pins, including handling debouncing
  setupPins();

  // get all the possible notes
  allNotes = getAllNotes();

  // read the JSON configuration file
  readJson();

  // cycle through all LEDs
  /*
  for(int x = 0; x < 3; x++) { //(int)sizeof(ledPins)
    print("Testing LED on pin " + (String)ledPins[x]);
    digitalWrite(ledPins[x], HIGH);
    delay(100);
    digitalWrite(ledPins[x], LOW);
  }
  */

  displayWelcome();
}

// ============================================================================
// main loop

void loop() {

  // Update all the buttons.  There should not be any long
  // delays in loop(), so this runs repetitively at a rate
  // faster than the buttons could be pressed and released.
  button0.update();
  button1.update();
  button2.update();
  button3.update();
  button4.update();
  
  button5.update();
  button6.update();
  button7.update();
  button8.update();
  button9.update();

  buttonA.update();
  buttonB.update();
  buttonC.update();

  // 1
  if (button0.fallingEdge()) {
    triggerButton(0);
  }

  // 2
  if (button1.fallingEdge()) {
    triggerButton(1);
  }

  // 3
  if (button2.fallingEdge()) {
    triggerButton(2);
  }

  // 4
  if (button3.fallingEdge()) {
    triggerButton(3);
  }

  // 5
  if (button4.fallingEdge()) {
    triggerButton(4);
  }

  // 6
  if (button5.fallingEdge()) {
    triggerButton(5);
  }

  // 7
  if (button6.fallingEdge()) {
    triggerButton(6);
  }

  // 8
  if (button7.fallingEdge()) {
    triggerButton(7);
  }

  // 9
  if (button8.fallingEdge()) {
    triggerButton(8);
  }

  // 10
  if (button9.fallingEdge()) {
    triggerButton(9);
  }

  // A
  if (buttonA.fallingEdge()) {
    triggerButton(10);
  }

  // B
  if (buttonB.fallingEdge()) {
    triggerButton(11);
  }

  // C
  if (buttonC.fallingEdge()) {
    triggerButton(12);
  }

  // if note hold mode is off, turn off the playing notes when buttons 0 - 4 are released
  if (!holdMode)
  {
    if (button0.risingEdge()) {
      turnAllNotesOff();
    }
    if (button1.risingEdge()) {
      turnAllNotesOff();
    }
    if (button2.risingEdge()) {
      turnAllNotesOff();
    }
    if (button3.risingEdge()) {
      turnAllNotesOff();
    }
    if (button4.risingEdge()) {
      turnAllNotesOff();
    }
  }
  
  while (usbMIDI.read()) {
    // ignore incoming messages
  }
}