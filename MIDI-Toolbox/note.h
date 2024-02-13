#ifndef Note_h
#define Note_h

#include <string>
#include <vector>

// Represents a single MIDI note
class Note {
  public:
    // The name of this note, including the octave e.g.C3
    String Name;

    // the octave for this notes
    int8_t Octave;

    // The MIDI note number
    int8_t MidiNoteNumber;

    // The MIDI channel this note is playing on, used when stopping notes after changing channels
    int8_t MidiPlayingChannel;

    // Construct a new instance of a Note
    Note(const String nameParam, int8_t octaveParam, int8_t midiNoteNumberParam) {
      Name = nameParam;
      Octave = octaveParam;
      MidiNoteNumber = midiNoteNumberParam;
    }
};

#endif