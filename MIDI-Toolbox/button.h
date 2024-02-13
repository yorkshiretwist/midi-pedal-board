#ifndef Button_h
#define Button_h

#include <string>

// Represents a single button
// Further parameters are required to correctly invoke most Actions that can be applied to a button
class Button {
  public:
    // The name of the button, either a number or a letter
    String Name;
    
    // The action the button performs, further parameters are needed for most Actions
    String Action;
    
    // The text to display on the screen for this button
    String DisplayText;
    
    // The MIDI channel this buttons Action will use, if negative this will be the default MIDI channel
    int8_t MidiChannel;
    
    // Boolean indicating if this button is in latched mode, default is false
    bool Latch;

    // Construct a new instance of a Button
    Button(String nameParam, String actionParam, String displayTextParam = "", int8_t midiChannelParam = -1, bool latchParam = false) {
      Name = nameParam;
      Action = actionParam;
      DisplayText = displayTextParam;
      MidiChannel = midiChannelParam;
      Latch = latchParam;
    }
};

#endif