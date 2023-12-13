#include <vector>

#ifndef Note_h
#define Note_h

#include <string>
#include <vector>
#include <ArduinoJson.h>

class Note {
  public:
    // the name of this note, including the octave e.g.C3
    String Name;
    // the octave for this notes
    int Octave;
    // the MIDI note number
    int MidiNoteNumber;
    // the MIDI channel this note is playing on, used when stopping notes after changing channels
    int MidiPlayingChannel;
    // constructor
    Note(const String nameParam, int octaveParam, int midiNoteNumberParam) {
      Name = nameParam;
      Octave = octaveParam;
      MidiNoteNumber = midiNoteNumberParam;
    }
};

// returns all possible MIDI notes the app supports
std::vector<Note> getAllNotes() {
  std::vector<Note> notes;
  notes.push_back(Note("G9", 9, 127));
  notes.push_back(Note("Gb9", 9, 126));
  notes.push_back(Note("F#9", 9, 126));
  notes.push_back(Note("F9", 9, 125));
  notes.push_back(Note("E9", 9, 124));
  notes.push_back(Note("D#9", 9, 123));
  notes.push_back(Note("D#9", 9, 123));
  notes.push_back(Note("D9", 9, 122));
  notes.push_back(Note("C#9", 9, 121));
  notes.push_back(Note("C#9", 9, 121));
  notes.push_back(Note("C9", 9, 120));
  notes.push_back(Note("B8", 8, 119));
  notes.push_back(Note("Bb8", 8, 118));
  notes.push_back(Note("A#8", 8, 118));
  notes.push_back(Note("A8", 8, 117));
  notes.push_back(Note("Ab8", 8, 116));
  notes.push_back(Note("G#8", 8, 116));
  notes.push_back(Note("G8", 8, 115));
  notes.push_back(Note("Gb8", 8, 114));
  notes.push_back(Note("F#8", 8, 114));
  notes.push_back(Note("F8", 8, 113));
  notes.push_back(Note("E8", 8, 112));
  notes.push_back(Note("Eb8", 8, 111));
  notes.push_back(Note("D#8", 8, 111));
  notes.push_back(Note("D8", 8, 110));
  notes.push_back(Note("Db8", 8, 109));
  notes.push_back(Note("C#8", 8, 109));
  notes.push_back(Note("C8", 8, 108));
  notes.push_back(Note("B7", 7, 107));
  notes.push_back(Note("Bb7", 7, 106));
  notes.push_back(Note("A#7", 7, 106));
  notes.push_back(Note("A7", 7, 105));
  notes.push_back(Note("Ab7", 7, 104));
  notes.push_back(Note("G#7", 7, 104));
  notes.push_back(Note("G7", 7, 103));
  notes.push_back(Note("Gb7", 7, 102));
  notes.push_back(Note("F#7", 7, 102));
  notes.push_back(Note("F7", 7, 101));
  notes.push_back(Note("E7", 7, 100));
  notes.push_back(Note("Eb7", 7, 99));
  notes.push_back(Note("D#7", 7, 99));
  notes.push_back(Note("D7", 7, 98));
  notes.push_back(Note("Db7", 7, 97));
  notes.push_back(Note("C#7", 7, 97));
  notes.push_back(Note("C7", 7, 96));
  notes.push_back(Note("B6", 6, 95));
  notes.push_back(Note("Bb6", 6, 94));
  notes.push_back(Note("A#6", 6, 94));
  notes.push_back(Note("A6", 6, 93));
  notes.push_back(Note("Ab6", 6, 92));
  notes.push_back(Note("G#6", 6, 92));
  notes.push_back(Note("G6", 6, 91));
  notes.push_back(Note("Gb6", 6, 90));
  notes.push_back(Note("F#6", 6, 90));
  notes.push_back(Note("F6", 6, 89));
  notes.push_back(Note("E6", 6, 88));
  notes.push_back(Note("Eb6", 6, 87));
  notes.push_back(Note("D#6", 6, 87));
  notes.push_back(Note("D6", 6, 86));
  notes.push_back(Note("Db6", 6, 85));
  notes.push_back(Note("C#6", 6, 85));
  notes.push_back(Note("C6", 6, 84));
  notes.push_back(Note("B5", 5, 83));
  notes.push_back(Note("Bb5", 5, 82));
  notes.push_back(Note("A#5", 5, 82));
  notes.push_back(Note("A5", 5, 81));
  notes.push_back(Note("Ab5", 5, 80));
  notes.push_back(Note("G#5", 5, 80));
  notes.push_back(Note("G5", 5, 79));
  notes.push_back(Note("Gb5", 5, 78));
  notes.push_back(Note("F#5", 5, 78));
  notes.push_back(Note("F5", 5, 77));
  notes.push_back(Note("E5", 5, 76));
  notes.push_back(Note("Eb5", 5, 75));
  notes.push_back(Note("D#5", 5, 75));
  notes.push_back(Note("D5", 5, 74));
  notes.push_back(Note("Db5", 5, 73));
  notes.push_back(Note("C#5", 5, 73));
  notes.push_back(Note("C5", 5, 72));
  notes.push_back(Note("B4", 4, 71));
  notes.push_back(Note("Bb4", 4, 70));
  notes.push_back(Note("A#4", 4, 70));
  notes.push_back(Note("A4", 4, 69)); // concert A
  notes.push_back(Note("Ab4", 4, 68));
  notes.push_back(Note("G#4", 4, 68));
  notes.push_back(Note("G4", 4, 67));
  notes.push_back(Note("Gb4", 4, 66));
  notes.push_back(Note("F#4", 4, 66));
  notes.push_back(Note("F4", 4, 65));
  notes.push_back(Note("E4", 4, 64));
  notes.push_back(Note("Eb4", 4, 63));
  notes.push_back(Note("D#4", 4, 63));
  notes.push_back(Note("D4", 4, 62));
  notes.push_back(Note("Db4", 4, 61));
  notes.push_back(Note("C#4", 4, 61));
  notes.push_back(Note("C4", 4, 60)); // middle C
  notes.push_back(Note("B3", 3, 59));
  notes.push_back(Note("Bb3", 3, 58));
  notes.push_back(Note("A#3", 3, 58));
  notes.push_back(Note("A3", 3, 57));
  notes.push_back(Note("Ab3", 3, 56));
  notes.push_back(Note("G#3", 3, 56));
  notes.push_back(Note("G3", 3, 55));
  notes.push_back(Note("Gb3", 3, 54));
  notes.push_back(Note("F#3", 3, 54));
  notes.push_back(Note("F3", 3, 53));
  notes.push_back(Note("E3", 3, 52));
  notes.push_back(Note("Eb3", 3, 51));
  notes.push_back(Note("D#3", 3, 51));
  notes.push_back(Note("D3", 3, 50));
  notes.push_back(Note("Db3", 3, 49));
  notes.push_back(Note("C#3", 3, 49));
  notes.push_back(Note("C3", 3, 48));
  notes.push_back(Note("B2", 2, 47));
  notes.push_back(Note("Bb2", 2, 46));
  notes.push_back(Note("A#2", 2, 46));
  notes.push_back(Note("A2", 2, 45));
  notes.push_back(Note("Ab2", 2, 44));
  notes.push_back(Note("G#2", 2, 44));
  notes.push_back(Note("G2", 2, 43));
  notes.push_back(Note("Gb2", 2, 42));
  notes.push_back(Note("F#2", 2, 42));
  notes.push_back(Note("F2", 2, 41));
  notes.push_back(Note("E2", 2, 40));
  notes.push_back(Note("Eb2", 2, 39));
  notes.push_back(Note("D#2", 2, 39));
  notes.push_back(Note("D2", 2, 38));
  notes.push_back(Note("Db2", 2, 37));
  notes.push_back(Note("C#2", 2, 37));
  notes.push_back(Note("C2", 2, 36));
  notes.push_back(Note("B1", 1, 35));
  notes.push_back(Note("Bb1", 1, 34));
  notes.push_back(Note("A#1", 1, 34));
  notes.push_back(Note("A1", 1, 33));
  notes.push_back(Note("Ab1", 1, 32));
  notes.push_back(Note("G#1", 1, 32));
  notes.push_back(Note("G1", 1, 31));
  notes.push_back(Note("Gb1", 1, 30));
  notes.push_back(Note("F#1", 1, 30));
  notes.push_back(Note("F1", 1, 29));
  notes.push_back(Note("E1", 1, 28));
  notes.push_back(Note("Eb1", 1, 27));
  notes.push_back(Note("D#1", 1, 27));
  notes.push_back(Note("D1", 1, 26));
  notes.push_back(Note("Db1", 1, 25));
  notes.push_back(Note("C#1", 1, 25));
  notes.push_back(Note("C1", 1, 24));
  notes.push_back(Note("B0", 0, 23));
  notes.push_back(Note("Bb0", 0, 22));
  notes.push_back(Note("A#0", 0, 22));
  notes.push_back(Note("A0", 0, 21));
  return notes;
}

#endif