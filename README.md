# IRLEDS
Control 3 Pin RGB LED Strips with an Arduino and an IR Remote control.

# Wiring
Wiring is written in code, but it's as follows :
  LED Data -> Pin 7
  IR Sensor -> Pin 8
 
# Requirements 
This sketch requires two librairies, FastLED, to control the LEDS, and IRremote to receive data from the remote.

# Use
To use the code, you might have to change a few IR code values in if statements directly, to make figuring out codes easier, I made a little ir code catcher program, simply upload it to an arduino with an ir sensor on pin 8, open the serial monitor and note which code is sent when pressing each button. 
