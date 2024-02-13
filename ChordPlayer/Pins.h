const int bounceTime = 30;

// bottom row
int button0pin = 0;
int button1pin = 1;
int button2pin = 2;
int button3pin = 24;
int button4pin = 33;

// middle row
int button5pin = 26;
int button6pin = 28;
int button7pin = 29;
int button8pin = 30;
int button9pin = 32;

// top row
int buttonApin = 22;
int buttonBpin = 20;
int buttonCpin = 16;

Bounce button0 = Bounce(button0pin, bounceTime);
Bounce button1 = Bounce(button1pin, bounceTime);
Bounce button2 = Bounce(button2pin, bounceTime);
Bounce button3 = Bounce(button3pin, bounceTime);
Bounce button4 = Bounce(button4pin, bounceTime);

// middle row
Bounce button5 = Bounce(button5pin, bounceTime);
Bounce button6 = Bounce(button6pin, bounceTime);
Bounce button7 = Bounce(button7pin, bounceTime);
Bounce button8 = Bounce(button8pin, bounceTime);
Bounce button9 = Bounce(button9pin, bounceTime);

// top row
Bounce buttonA = Bounce(buttonApin, bounceTime);
Bounce buttonB = Bounce(buttonBpin, bounceTime);
Bounce buttonC = Bounce(buttonCpin, bounceTime);

// define pins for LEDs - buttons 0 - 5
//int ledPins[5] =  {37,36,35,34,33};

void setupPins() {
  // set button pins to input
  
  // bottom row
  pinMode(button0pin, INPUT_PULLUP);
  pinMode(button1pin, INPUT_PULLUP);
  pinMode(button2pin, INPUT_PULLUP);
  pinMode(button3pin, INPUT_PULLUP);
  pinMode(button4pin, INPUT_PULLUP);

  // middle row
  pinMode(button5pin, INPUT_PULLUP);
  pinMode(button6pin, INPUT_PULLUP);
  pinMode(button7pin, INPUT_PULLUP);
  pinMode(button8pin, INPUT_PULLUP);
  pinMode(button9pin, INPUT_PULLUP);

  // top row
  pinMode(buttonApin, INPUT_PULLUP);
  pinMode(buttonBpin, INPUT_PULLUP);
  pinMode(buttonCpin, INPUT_PULLUP);

  // set LED pins to output
  //for(int x = 0; x < 5; x++) {
  //  pinMode(ledPins[x], OUTPUT);
  //}
}