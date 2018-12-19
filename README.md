# Key and Mouse Playback for Teensy 3/LC

Play USB keyboard and USB mouse output from an ASCII string.

Example: The following string launches the Chrome browser to Google using
Windows.

"GUI-KEY_R ~100 'chrome' SPACE 'https://www.google.com/' ENTER"

GUI-KEY_R -- The GUI key refers to the Windows logo key. KEY_R is the 'R' key
so GUI-KEY_R is equivalent to pressing and holding the GUI key, pressing and
releasing the 'R' key, then releasing the GUI key. On Windows, this opens a
one line command window.

~100 -- This means delay for 100 * 10 milliseconds = 1 second. This allows time
for the command window to open.

'chrome' -- This enters 'chrome' into the command line window.

SPACE -- This appends a SPACE character to the command.

'https://www.google.com/' -- This appends the Google URL into the command line.

ENTER -- This is equivalent to pressing the keyboard Enter key.

The key names are listed in keynames.h.

For Linux, change GUI-KEY_R to ALT-F2.

WARNING: Strings with embedded spaces are not currently supported. For example,
the string 'chrome https://www.google.com/' does not work because it has an
embedded space between 'chrome' and the URL. See the above example for how to
handle this.

## TBD

* USB mouse output.
