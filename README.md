# arduino_bicycle_computer
simple arduino i2c oled bicycle computer

code https://github.com/ldijkman/arduino_bicycle_computer/blob/main/speed.ino

more pixels used in text == better display
code https://github.com/ldijkman/arduino_bicycle_computer/blob/main/speed_better_display.ino

10.5 Km = speed

136 = distance in meter 

* = magnet is now on sensor

61 = wheelcount pulses

<img src="https://github.com/ldijkman/arduino_bicycle_computer/blob/main/oled_speed.jpg">

maybe the pullup resistor is not needed since input is pulled up in the code

A3144 is north pole sensitive (needs correct magnet orientation)

Use a neodymium magnet for greater detection range

<img src="https://github.com/ldijkman/arduino_bicycle_computer/blob/main/A3144_hall_effect.jpg">


Add an AD8232 for heart beat speed rate ;-)

Alike heartbeat on fitness  bicycle 

ECG Monitor https://www.google.com/search?q=AD8232

https://www.thingiverse.com/thing:2443305
