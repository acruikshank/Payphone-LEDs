# Payphone-LEDs
Code to program a teensy controlling a neopixel grid for use in a discarded payphone.

# Usage

1. Open in platform.io.
2. Upload to teensy (nothing will happen).
3. Change serial to address of teensy in send_to_teensy.py
4. Run one of the following commands:
```bash
python send_to_teensy.py 0 'What ever text you like.' # scroll up to 200 chars of text
python send_to_teensy.py 1 # rainbow
python send_to_teensy.py 2 # scrolling color bars
python send_to_teensy.py 3 # random color bars
python send_to_teensy.py 4 # random dots
python send_to_teensy.py 5 # fade out
python send_to_teensy.py 6 # blur
python send_to_teensy.py 7 # chromatic horizontal blur
```

# Serial protocol

Start serial at 9600 baud. Send ascii `0`-`5` to choose the mode. If you chose mode `0`,
follow with an ascii string (characters must be in range 0-130) ending with a new line ('\n').
