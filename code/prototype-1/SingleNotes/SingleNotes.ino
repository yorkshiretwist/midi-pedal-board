// run this to use the unit in cord mode
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

// set the chipSet for the SD card
#define CS_SD BUILTIN_SDCARD

// ============================================================================
// state variables

// the MIDI channel number to send messages
const int midiChannel = 1;

// the index of the current active button
int currentButtonIndex = -1;

// whether hold note mode is on or off
bool holdMode = true;

// flag to indicate if we are changing key
bool changingKey = false;

// flag to indicate if we are changing register
bool changingRegister = false;

// the notes currently playing
std::vector<Note> currentPlayingNotes;

// ============================================================================
// JSON config

DynamicJsonDocument jsonDoc(100000);

DeserializationError jsonError;

// read the JSON file
void readJson() {
  File dataFile = SD.open("config.json");
  jsonError = deserializeJson(jsonDoc, dataFile);
  dataFile.close();
}

// print the error if the JSON file could not be read
void checkJson() {
  if (jsonError != DeserializationError::Ok) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(jsonError.f_str());
  }
}

// ============================================================================
// MIDI functions

// start  playing the given note
void playNote(int noteNumber) {
  usbMIDI.sendNoteOn(noteNumber, 99, midiChannel);
}

// stop playing the given note
void stopNote(int noteNumber) {
  usbMIDI.sendNoteOff(noteNumber, 0, midiChannel);
}

// ============================================================================
// note variables and functions

std::vector<Note> allNotes;

// turn all the currently playing notes off
void turnAllNotesOff() {
  Serial.println("Turning all notes off");
  // turn the notes currently playing off
  for(int x = 0; x < (int)currentPlayingNotes.size(); x++) {
    stopNote(currentPlayingNotes[x].MidiNoteNumber);
  }
  // turn the current button light off
  if (currentButtonIndex > -1) {
    digitalWrite(ledPins[currentButtonIndex], LOW);
  }
  // clear all current notes
  currentPlayingNotes.clear();
}

// get the note with the given name
Note getNote(std::vector<Note> allNotes, String noteName) {
  //Serial.print("All notes: ");
  //Serial.println((int)allNotes.size());
  for(int x = 0; x < (int)allNotes.size(); x++) {
    Serial.println(noteName + " => " + allNotes[x].Name + " = ");
    Serial.println(noteName.compareTo(allNotes[x].Name));
    if (noteName.compareTo(allNotes[x].Name) == 0) {
      Serial.println("Found note " + noteName);
      return allNotes[x];
    }
  }
  Serial.println("Could not find note " + noteName);
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
// key variables and functions

const String keys[12] = {"Gb", "Db", "Ab", "Eb", "Bb", "F", "C", "G", "D", "A", "E", "B"};

int currentKeyIndex = 6;

int previousKeyIndex = -1;

// go to the next key up, or back to the bottom
void keyUp() {
  previousKeyIndex = currentKeyIndex;
  changingKey = true;
  Serial.print("Changing key up from ");
  Serial.print(keys[currentKeyIndex]);
  currentKeyIndex = currentKeyIndex + 1;
  if (currentKeyIndex == 12) {
    currentKeyIndex = 0;
  }
  Serial.print(" to ");
  Serial.println(keys[currentKeyIndex]);
}

// go to the next key down, or back to the top
void keyDown() {
  previousKeyIndex = currentKeyIndex;
  changingKey = true;
  Serial.print("Changing key down from ");
  Serial.print(keys[currentKeyIndex]);
  currentKeyIndex = currentKeyIndex - 1;
  if (currentKeyIndex == -1) {
   currentKeyIndex = 11;
  }
  Serial.print(" to ");
  Serial.println(keys[currentKeyIndex]);
}

// ============================================================================
// register variables and functions

const String registers[4] = { "Low", "Middle", "Wide", "High" };

int currentRegisterIndex = 1;

int previousRegisterIndex = -1;

// go to the next register up, or back to the bottom
void registerUp() {
  previousRegisterIndex = currentRegisterIndex;
  changingRegister = true;
  Serial.print("Changing register up from ");
  Serial.print(registers[currentRegisterIndex]);
  currentRegisterIndex = currentRegisterIndex + 1;
  if (currentRegisterIndex == 4) {
    currentRegisterIndex = 0;
  }
  Serial.print(" to ");
  Serial.println(registers[currentRegisterIndex]);
}

// go to the next register down, or back to the top
void registerDown() {
  previousRegisterIndex = currentRegisterIndex;
  changingRegister = true;
  Serial.print("Changing register down from ");
  Serial.print(registers[currentRegisterIndex]);
  currentRegisterIndex = currentRegisterIndex - 1;
  if (currentRegisterIndex == -1) {
   currentRegisterIndex = 3;
  }
  Serial.print(" to ");
  Serial.println(registers[currentRegisterIndex]);
}

// ============================================================================
// button variables and functions

// when a button is triggered
void triggerButton(int buttonIndex) {

  // ensure we have some JSON config
  checkJson();

  Serial.print("Button triggered: ");
  Serial.println(buttonIndex);

  // if the button index is 5 or above then handle special functions
  if (buttonIndex >= 5) {
    // button A - change register down
    if (buttonIndex == 6) {
      registerDown();
    }
    // button B - change register up
    if (buttonIndex == 7) {
      registerUp();
    }
    // button C - change key down
    if (buttonIndex == 8) {
      keyDown();
    }
    // button D - change key up
    if (buttonIndex == 9) {
      keyUp();
    }
    return;
  }
  
  // print the current button for debugging
  Serial.print("Current button: ");
  Serial.println(currentButtonIndex);
  
  // stop playing all notes
  turnAllNotesOff();

  // if we've pressed the same button and we're not changing key or register then
  // reset the current button index and return, as we don't want to play new notes
  if (currentButtonIndex == buttonIndex && changingKey == false && changingRegister == false) {
    Serial.println("Resetting current button");
    currentButtonIndex = -1;
    Serial.println("");
    return;
  }

  // reset some state variables
  changingKey = false;
  changingRegister = false;

  // set the current button
  Serial.print("Setting current button to ");
  Serial.println(buttonIndex);
  currentButtonIndex = buttonIndex;
  
  // get the button
  JsonObject buttonConfig = getButton(jsonDoc, buttonIndex);
  if (buttonConfig["index"].as<int>() < 0) {
    Serial.println("Could not find button");
    return;
  } else {
    Serial.print("Found button : ");
    Serial.println(buttonConfig["index"].as<int>());
    Serial.println("");
  }

  // get the current key and register
  String keyName = keys[currentKeyIndex];
  String registerName = registers[currentRegisterIndex];

  // set the default spread, which is to play all notes at the same time
  String spread = "Normal";

  //std::vector<Note> notes = getKeyChordNotes(allNotes, button, keys[currentKeyIndex], registers[currentRegisterIndex]);

  // this section should be in functions, but because I don't understand C++ the variable values get corrupted

  bool debugNotes = false;
  if (debugNotes) {
    Serial.println("Button config:");
    serializeJson(buttonConfig, Serial);
    Serial.println("");
    Serial.println("Getting notes for key: " + keyName + ", register: " + registerName);
    Serial.println("Button name: " + buttonConfig["name"].as<String>());
  }
  std::vector<Note> notes;

  // get the keys
  JsonArray keys = buttonConfig["keys"].as<JsonArray>();
  if (debugNotes) {
    Serial.print("Number of keys: ");
    Serial.println(keys.size());
  }
  for (JsonObject key : keys) {

    if (keyName.compareTo(key["name"].as<String>()) == 0) {

      if (debugNotes) {
        Serial.println("Found key: " + key["name"].as<String>());
      }

      // get the chords in this key
      JsonArray chords = key["chords"].as<JsonArray>();
      if (debugNotes) {
        Serial.print("Number of chords: ");
        Serial.println(chords.size());
      }
      for (JsonObject chord : chords) {

        if (registerName.compareTo(chord["registerName"].as<String>()) == 0) {

          if (debugNotes) {
            Serial.println("Found register: " + chord["registerName"].as<String>() + ", chord " + chord["chordName"].as<String>());
          }

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
                if (debugNotes) {
                  Serial.println("Found note " + noteNameStr);
                }
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
    Serial.println("No notes found");
    Serial.println("");
    return;
  }

  // play all notes at the same time
  if (spread == "Normal") {
    for(int x = 0; x < (int)notes.size(); x++) {
      Serial.print(" (MIDI ");
      Serial.print(notes[x].MidiNoteNumber);
      Serial.println(")");
      currentPlayingNotes.push_back(notes[x]);
      playNote(notes[x].MidiNoteNumber);
    }

  // arpeggiate notes up
  } else if (spread == "Up") {
    for(int x = 0; x < (int)notes.size(); x++) {
      Serial.print(" (MIDI ");
      Serial.print(notes[x].MidiNoteNumber);
      Serial.println(")");
      currentPlayingNotes.push_back(notes[x]);
      playNote(notes[x].MidiNoteNumber);
      delay(80);
    }

  // arpeggiate notes down
  } else if (spread == "Down") {
    for(int x = (int)notes.size() - 1; x >= 0 ; x--) {
      Serial.print(" (MIDI ");
      Serial.print(notes[x].MidiNoteNumber);
      Serial.println(")");
      currentPlayingNotes.push_back(notes[x]);
      playNote(notes[x].MidiNoteNumber);
      delay(80);
    }
  }

  // turn the LED for the triggered button on
  digitalWrite(ledPins[currentButtonIndex], HIGH);

  Serial.println("");
}


// ============================================================================
// setup function

void setup() {

  // waitn for the Serial to be available (no more than 3 seconds)
  while (!Serial && millis () < 3000);

  // try to initialise the SD card
  if (!SD.begin(CS_SD)) {
    Serial.println("SD card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("SD card initialized");

  // setup the pins, including handling debouncing
  setupPins();

  // get all the possible notes
  allNotes = getAllNotes();

  // read the JSON configuration file
  readJson();
}

// ============================================================================
// main loop

bool initialised = false;

void loop() {

  // if this is the first time round the loop, we've not run the initialisation code yet
  if (!initialised) {

    // cycle through all LEDs
    for(int x = 0; x < 3; x++) { //(int)sizeof(ledPins)
      Serial.print("Testing LED on pin ");
      Serial.println(ledPins[x]);
      digitalWrite(ledPins[x], HIGH);
      delay(100);
      digitalWrite(ledPins[x], LOW);
    }

    // now we're initialised
    initialised = true;
  }

  // Update all the buttons.  There should not be any long
  // delays in loop(), so this runs repetitively at a rate
  // faster than the buttons could be pressed and released.
  button0.update();
  button1.update();
  button2.update();
  button3.update();
  button4.update();
  button5.update();
  buttonA.update();
  buttonB.update();
  buttonC.update();
  buttonD.update();

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

  // A
  if (buttonA.fallingEdge()) {
    triggerButton(6);
  }

  // B
  if (buttonB.fallingEdge()) {
    triggerButton(7);
  }

  // C
  if (buttonC.fallingEdge()) {
    triggerButton(8);
  }

  // D
  if (buttonD.fallingEdge()) {
    triggerButton(9);
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