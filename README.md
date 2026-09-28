# Digistump ATtiny85 Mouse Jiggler

A tiny USB mouse jiggler for the Digispark ATtiny85 board. It nudges the cursor
by a small random amount at random intervals, keeping your computer awake.

> A mouse jiggler is a device or software that simulates mouse movement to
> prevent a computer from entering sleep mode or activating a screensaver.

> Digispark is a tiny ATtiny85-based USB dev board that uses the [Micronucleus bootloader](https://github.com/micronucleus/micronucleus).

## Features

- Pseudorandom movement using a Linear Congruential Generator (LCG)
- Seeded by environmental noise read from a floating analog pin
- Small random movements at random intervals
- Smooth movement spread over several steps instead of an instant jump
- All parameters configurable

## Installation

1. Install the Arduino IDE.
2. Add the Digispark board package via *File → Preferences → Additional Boards
   Manager URLs:
   `https://raw.githubusercontent.com/ArminJo/DigistumpArduino/master/package_digistump_index.json`
3. Install **Digistump AVR Boards** in *Tools → Board → Boards Manager*.
4. Select *Tools → Board → Digispark (Default - 16.5mhz)*.
5. Open the sketch and click **Upload**. When the IDE prompts, plug in the
   Digispark.

## Disclaimer

Some organizations prohibit mouse jigglers on managed devices. Check your
local policy before use.
