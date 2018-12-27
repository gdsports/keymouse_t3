/*
 * Demonstrate keymacros triggered by CP buttons and captouch.
 * Turns a CP into an application launcher. As long as the application
 * can be launched from command line, there is no need to install software
 * on the computer.
 *
 * Pressing the left button opens Chrome to Google.
 * Pressing the right button opens Chrome to Youtube.
 * Touching captouch #3 (A4) opens Chrom to Adafruit.
 */

#include <Adafruit_CircuitPlayground.h>
#include <keymouse_play.h>
keymouse_play keyplay;

void setup() {
  Keyboard.begin();
  delay(2000);
  Serial.begin(115200);
  Serial.println("Circuit Playground Key Macros");

  CircuitPlayground.begin();
}

bool leftButtonDown = false;
bool rightButtonDown = false;
bool cap3Touched = false;
uint16_t cap3vector[4] = {208, 208, 208, 208};
uint8_t cap3idx = 0;

// To open a one line command window on Windows press Windows logo key + R key.
// GUI-R refers to the Windows logo key.
// To open a one line command window on Ubuntu press ALT-F2. This might also
// work on Raspbian.
//
#if 0
// Ubuntu Linux and maybe Raspbian
const char google[] =
"ALT-F2 ~100 'chromium-browser' ~10 ENTER ~100 'https://www.google.com/' ENTER";

const char youtube[] =
"ALT-F2 ~100 'chromium-browser' ~10 ENTER ~100 'https://www.youtube.com/' ENTER";

const char adafruit[] =
"ALT-F2 ~100 'chromium-browser' ~10 ENTER ~100 'https://adafruit.com/' ENTER";
#else
// Windows
const char google[] =
"GUI-R ~100 'chrome' ~10 ENTER ~100 'https://www.google.com/' ENTER";

const char youtube[] =
"GUI-R ~100 'chrome' ~10 ENTER ~100 'https://www.youtube.com/' ENTER";

const char adafruit[] =
"GUI-R ~100 'chrome' ~10 ENTER ~100 'https://adafruit.com/' ENTER";
#endif

void loop() {
  if (CircuitPlayground.leftButton() && !leftButtonDown) {
    Serial.println("Left button pressed!");
    leftButtonDown = true;
    keyplay.start(google);
  }
  else if (!CircuitPlayground.leftButton() && leftButtonDown) {
    Serial.println("Left button released!");
    leftButtonDown = false;
  }
  if (CircuitPlayground.rightButton() && !rightButtonDown) {
    Serial.println("right button pressed!");
    rightButtonDown = true;
    keyplay.start(youtube);
  }
  else if (!CircuitPlayground.rightButton() && rightButtonDown) {
    Serial.println("right button released!");
    rightButtonDown = false;
  }

  keyplay.loop();

  //Serial.print("Capsense #3: ");
  // Average the last 4 readings to determine whether a touch occurred.
  uint16_t capReading = CircuitPlayground.readCap(3);
  //Serial.print(capReading);
  cap3vector[cap3idx++] = capReading;
  cap3idx &= 0x03;  // Limit cap3idx 0..3
  uint16_t cap3average = (cap3vector[0] + cap3vector[1] + cap3vector[2] + cap3vector[3]) >> 2;
  //Serial.print(' ');
  //Serial.println(cap3average);
  if (cap3average > 220 && !cap3Touched) {
    Serial.println("cap3 touched!");
    cap3Touched = true;
    keyplay.start(adafruit);
  }
  else if (cap3average <= 220 && cap3Touched) {
    Serial.println("cap3 released!");
    cap3Touched = false;
  }
#if 0
  Serial.print("Capsense #2: "); Serial.println(CircuitPlayground.readCap(2));
  if (! CircuitPlayground.isExpress()) {  // CPX does not have this captouch pin
    Serial.print("Capsense #0: "); Serial.println(CircuitPlayground.readCap(0));
  }
  Serial.print("Capsense #1: "); Serial.println(CircuitPlayground.readCap(1));
  Serial.print("Capsense #12: "); Serial.println(CircuitPlayground.readCap(12));
  Serial.print("Capsense #6: "); Serial.println(CircuitPlayground.readCap(6));
  Serial.print("Capsense #9: "); Serial.println(CircuitPlayground.readCap(9));
  Serial.print("Capsense #10: "); Serial.println(CircuitPlayground.readCap(10));
#endif
}
