#ifndef Button_h
#define Button_h

#include <string>
#include <ArduinoJson.h>

class Button {
  public:
    String Name;
    int Index;
    Button(String nameParam, int indexParam) {
      Name = nameParam;
      Index = indexParam;
    }
};

#endif