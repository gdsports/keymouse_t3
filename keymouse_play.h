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

#ifndef _KEYMOUSE_PLAY_H_
#define _KEYMOUSE_PLAY_H_

#include <Keyboard.h>
#include <Mouse.h>
#include <keynames.h>

class keymouse_play {
  public:
    keymouse_play() { };
    int start(const char *keyseq);
    int loop(void);
    KEY_NAME_NUM_t *key_find(const char *keyname);
    int print_key_find(const char *keyname);

  private:
    static int compar(const void *pkey, const void *pelem);
    int keyseq_handle(char *token);
    int compound_key(const char *token);
    uint32_t elapsedmSecs(void);

  private:
    char *keysequence = NULL;
    uint32_t keyseq_delay = 0;
    uint32_t keyseq_millis = 0;
    bool keyseq_done = true;
};

#endif /* _KEYMOUSE_PLAY_H_ */
