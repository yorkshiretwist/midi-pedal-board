const int bounceTime = 30;

// bottom row buttons
Bounce button0 = Bounce(0, bounceTime);
Bounce button1 = Bounce(6, bounceTime);
Bounce button2 = Bounce(27, bounceTime);
Bounce button3 = Bounce(24, bounceTime);
Bounce button4 = Bounce(30, bounceTime);
Bounce button5 = Bounce(32, bounceTime);

// top row button
Bounce buttonA = Bounce(23, bounceTime);
Bounce buttonB = Bounce(22, bounceTime);
Bounce buttonC = Bounce(41, bounceTime);
Bounce buttonD = Bounce(40, bounceTime);

// define pins for LEDs - buttons 0 - 5
int ledPins[5] =  {37,36,35,34,33};

void setupPins() {
  // set button pins to input
  
  // bottom row
  pinMode(0, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(27, INPUT_PULLUP);
  pinMode(24, INPUT_PULLUP);
  pinMode(30, INPUT_PULLUP);
  pinMode(32, INPUT_PULLUP);

  // top row
  pinMode(23, INPUT_PULLUP);
  pinMode(22, INPUT_PULLUP);
  pinMode(41, INPUT_PULLUP);
  pinMode(40, INPUT_PULLUP);

  // set LED pins to output
  for(int x = 0; x < 5; x++) {
    pinMode(ledPins[x], OUTPUT);
  }
}