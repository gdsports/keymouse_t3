/*
   MIT License

   Copyright (c) 2018 gdsports625@gmail.com

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

#include <Arduino.h>
#include "keymouse_play.h"

#define DEBUG_ON	0

#if DEBUG_ON
#define dbprint(...)	Serial.print(__VA_ARGS__)
#define dbprintln(...)	Serial.println(__VA_ARGS__)
#else
#define dbprint(...)
#define dbprintln(...)
#endif

#define WHITESPACE " \t\n\r\f"

int keymouse_play::start(const char *keyseq)
{
  if (!keyseq_done) return 0;
  keysequence = strdup(keyseq);
  if (keysequence == NULL) return 0;
  char *tok = strtok(keysequence, WHITESPACE);
  return keyseq_done = keyseq_handle(tok);
}

int keymouse_play::loop(void)
{
  if (keyseq_done) return 1;
  if (keyseq_delay && (millis() < keyseq_delay)) {
    return 0;
  }
  keyseq_delay = 0;

  char *tok = strtok(NULL, WHITESPACE);
  return keyseq_done = keyseq_handle(tok);
}

int keymouse_play::compar(const void *pkey, const void *pelem)
{
  const KEY_NAME_NUM_t *pElem = (const KEY_NAME_NUM_t *)pelem;
  const char *pKey = (const char *)pkey;

  return strcmp(pKey, pElem->key_name);
}

KEY_NAME_NUM_t *keymouse_play::key_find(const char *keyname)
{
  size_t keylen = strlen(keyname);
  char keyname_upper[keylen+1];

  for (size_t i = 0; i < keylen; i++) {
    keyname_upper[i] = toupper(keyname[i]);
  }
  keyname_upper[keylen] = '\0';
  return (KEY_NAME_NUM_t *)bsearch(keyname_upper, KEY_NAME_NUM, KEY_NAME_NUM_SIZE,
      sizeof(KEY_NAME_NUM[0]), compar);
}

// Handle compound keys such as CTRL-ALT-DELETE
int keymouse_play::compound_key(const char *token)
{
  int cycles = 0;
  const char *separator = token;

  while ((separator = strchr(token, '-'))) {
    if (memcmp("ALT-", token, 4) == 0) {
      Keyboard.press(KEY_LEFT_ALT);
      dbprintln("ALT pressed");
    }
    else if (memcmp("CTRL-", token, 5) == 0) {
      Keyboard.press(KEY_LEFT_CTRL);
      dbprintln("CTRL pressed");
    }
    else if (memcmp("SHIFT-", token, 6) == 0) {
      Keyboard.press(KEY_LEFT_SHIFT);
      dbprintln("SHIFT pressed");
    }
    else if (memcmp("GUI-", token, 4) == 0) {
      Keyboard.press(KEY_LEFT_GUI);
      dbprintln("GUI pressed");
    }
    token = separator + 1;
    cycles++;
  }
  print_key_find(token);
  Keyboard.releaseAll();
  Mouse.release(MOUSE_ALL);
  return cycles;
}

int keymouse_play::print_key_find(const char *keyname)
{
  KEY_NAME_NUM_t *key_elem;
  key_elem = key_find(keyname);
  dbprint(keyname);
  dbprint(' ');
  if (key_elem) {
    dbprint("= key_name ");
    dbprint(key_elem->key_name);
    dbprint(" key_num 0x");
    dbprintln(key_elem->key_num, HEX);
    if (memcmp("MOUSE_", keyname, 6) == 0) {
      Mouse.press(key_elem->key_num);
      Mouse.release(key_elem->key_num);
    }
    else {
      Keyboard.press(key_elem->key_num);
      Keyboard.release(key_elem->key_num);
    }
    return 1;
  }
  else {
    dbprintln("not found");
    return 0;
  }
}

int keymouse_play::keyseq_handle(char *token)
{
  if (token == NULL || *token == '\0') {
    if (keysequence) free(keysequence);
    Keyboard.releaseAll();
    Mouse.release(MOUSE_ALL);
    return 1;
  }

  if (*token == '\'') {
    Keyboard.write((const uint8_t*)token + 1, strlen(token)-2);
    Serial.write(token + 1, strlen(token)-2);
    dbprintln();
  }
  else if (*token == '~') {
    keyseq_delay = millis() + 10*strtoul(token+1, NULL, 10);
  }
  else if ((*token == '+') || (*token == '-')) {
    KEY_NAME_NUM_t *key_elem;
    key_elem = key_find(token+1);
    if (key_elem) {
      if (memcmp("MOUSE_", token+1, 6) == 0) {
        if (*token == '+') {
          dbprint("mouse press ");
          dbprintln(token+1);
          Mouse.press(key_elem->key_num);
        }
        else {
          dbprint("mouse release ");
          dbprintln(token+1);
          Mouse.release(key_elem->key_num);
        }
      }
      else {
        if (*token == '+') {
          dbprint("key press ");
          dbprintln(token+1);
          Keyboard.press(key_elem->key_num);
        }
        else {
          dbprint("key release ");
          dbprintln(token+1);
          Keyboard.release(key_elem->key_num);
        }
      }
    }
  }
  else if (print_key_find(token)) {
  }
  else if (compound_key(token)) {
  }
  else {
    dbprint("TBD: ");
    dbprintln(token);
  }
  return 0;
}
