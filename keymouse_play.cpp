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
#include "keynames.h"
#include "keymouse_play.h"

int keymouse_play::start(const char *keyseq)
{
  if (!keyseq_done) return 0;
  strncpy(keysequence, keyseq, sizeof(keysequence)-1);
  keysequence[sizeof(keysequence)-1] = '\0';
  char *tok = strtok(keysequence, " \t");
  return keyseq_done = keyseq_handle(tok);
}

int keymouse_play::loop(void)
{
  if (keyseq_done) return 1;
  if (keyseq_delay && (millis() < keyseq_delay)) {
    return 0;
  }
  keyseq_delay = 0;

  char *tok = strtok(NULL, " \t");
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

int keymouse_play::print_key_find(const char *keyname)
{
  KEY_NAME_NUM_t *key_elem;
  key_elem = key_find(keyname);
  Serial.print(keyname);
  Serial.print(' ');
  if (key_elem) {
    Serial.printf("= key_name %s key_num 0x%x\n",
        key_elem->key_name, key_elem->key_num);
    Keyboard.press(key_elem->key_num);
    Keyboard.release(key_elem->key_num);
    return 1;
  }
  else {
    Serial.println("not found");
    return 0;
  }
}

int keymouse_play::keyseq_handle(char *token)
{
  if (token == NULL || *token == '\0') return 1;

  if (*token == '\'') {
    Keyboard.write(token + 1, strlen(token)-2);
    Serial.write(token + 1, strlen(token)-2);
    Serial.println();
  }
  else if (*token == '~') {
    keyseq_delay = millis() + 10*strtoul(token+1, NULL, 10);
  }
  else if (print_key_find(token)) {
  }
  else if (memcmp("ALT-", token, 4) == 0) {
    Keyboard.press(MODIFIERKEY_ALT);
    Serial.println("ALT pressed");
    print_key_find(token+4);
    Keyboard.release(MODIFIERKEY_ALT);
    Serial.println("ALT released");
  }
  else if (memcmp("CTRL-", token, 5) == 0) {
    Keyboard.press(MODIFIERKEY_CTRL);
    Serial.println("CTRL pressed");
    print_key_find(token+5);
    Keyboard.release(MODIFIERKEY_CTRL);
    Serial.println("CTRL released");
  }
  else if (memcmp("SHIFT-", token, 6) == 0) {
    Keyboard.press(MODIFIERKEY_SHIFT);
    Serial.println("SHIFT pressed");
    print_key_find(token+6);
    Keyboard.release(MODIFIERKEY_SHIFT);
    Serial.println("SHIFT released");
  }
  else if (memcmp("GUI-", token, 4) == 0) {
    Keyboard.press(MODIFIERKEY_GUI);
    Serial.println("GUI pressed");
    print_key_find(token+4);
    Keyboard.release(MODIFIERKEY_GUI);
    Serial.println("GUI released");
  }
  else {
    Serial.printf("TBD: %s\n", token);
  }
  return 0;
}
