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

// get the button with the given index
JsonObject getButton(DynamicJsonDocument jsonDoc, int buttonIndex) {
  JsonArray array = jsonDoc["buttons"].as<JsonArray>();
  for(JsonObject button : array) {
      if (button["index"].as<int>() == buttonIndex) {
        return button;
      }
  }
  JsonObject nullObj = JsonObject();
  nullObj["index"] = "-1";
  return nullObj;
}

#endif