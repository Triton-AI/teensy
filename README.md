# teensy
Firmware for ECE Robocar Teensy breakout board to allow speed control and RC transceiver interface.

The firmware package shall allow for interception of 3 phases of pulse signals from a sensored brushless motor, as well as 4 channels from an RC transceiver.  The firmware will perform the following functions:

- Calculate speed based on encoder pulses
- Emulating a joystick, communicate speed, throttle, steering, Ch3 (tbd), and Ch4 (tbd) to single board computer (SBC) via usb
- Listen for commanded speed, steering via usb
- Determine control effort required based on commanded speed and current speed, utilizing PI control
- Output throttle to ESC and steering position to servo in form of PWM
