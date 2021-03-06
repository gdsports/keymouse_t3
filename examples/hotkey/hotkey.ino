#include <keymouse_play.h>

keymouse_play keyplay;

#ifdef TEENSYDUINO
char keysequence[] = \
  "MEDIA_PLAY ~100 MEDIA_MUTE ~100 MEDIA_MUTE ~100 " \
  "MEDIA_VOLUME_INC MEDIA_VOLUME_INC ~100 " \
  "MEDIA_VOLUME_DEC MEDIA_VOLUME_DEC ~100 MEDIA_STOP ~100 " \
  "CTRL-MOUSE_LEFT " \
  "ALT-F2 ~100 'chromium-browser' ~10 ENTER ~100 'https://www.google.com/' ENTER" \
  " ~100 CTRL-ALT-DELETE ~100 ESC";
#else
char keysequence[] = "CTRL-MOUSE_LEFT " \
  "ALT-F2 ~100 'chromium-browser' ~10 ENTER ~100 'https://www.google.com/' ENTER" \
  " ~100 CTRL-ALT-DELETE ~100 ESC";
#endif

void setup()
{
  Serial.begin(115200);
  while (!Serial && millis() < 2000) delay(1);

  Keyboard.begin();
  delay(2000);

  keyplay.print_key_find("KEYPAD_0");
  keyplay.print_key_find("KEYPAD_1");

  keyplay.print_key_find("RIGHT_SHIFT");
  keyplay.print_key_find("SHIFT");

  keyplay.print_key_find("KEY_I");
  keyplay.print_key_find("KEY_INSERT");
  keyplay.print_key_find("INSERT");
  keyplay.print_key_find("BACKSPACE");
  keyplay.print_key_find("KEY_ENTER");
  keyplay.print_key_find("EnTeR");

  // key not found
  keyplay.print_key_find("KEY_QWERTY");

  keyplay.start(keysequence);
}

void loop()
{
  keyplay.loop();
}
