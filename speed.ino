// DIY BiCycle Speedometer
// distance in meter and speed in kilometer/hour
// displays a * when magnet on Hall sensor and displays wheelcount
// arduino nano
// hall sensor
// oled display
// https://www.instructables.com/DIY-Cycle-Speedometer/

// DIY BiCycle Speedometer distance and km/h arduino nano hall sensor oled display
// https://www.instructables.com/DIY-Cycle-Speedometer/

// https://github.com/ldijkman/arduino_bicycle_computer/blob/main/speed.ino


#include <Adafruit_SSD1306.h>
#include <Wire.h>


#define pi 3.14
#define diameter .71                 // wheel diameter in meters .71m = 71cm = 710mm
#define numberofreadings 6


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

const int chipSelect = 9;

int n = 0;
int count = 0;
double instant = 0;
double previnstant = 0;
int wheelcount = 0;
int lastwheelcount = 0;
float velocity = 0;
float distance = 0;
int temp;
unsigned long starttime;



void setup()
{
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), cross, FALLING);

  display.begin(SSD1306_SWITCHCAPVCC , 0x3C);
  display.setTextColor(WHITE);
  display.setTextSize(1);
}



void loop()
{
  if (wheelcount == lastwheelcount) {
    if (millis() - starttime >= 10000) {
      velocity = 0;                       // if for 10 seonds no wheelcount set speed to zero 0 km/h
    }
  } else {
    starttime = millis();
    lastwheelcount = wheelcount;
  }

  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(0, 0);
  display.print(velocity * 18 / 5, 1);
  display.setCursor(85, 0);
  display.print("km");

  display.setTextSize(1);
  display.setCursor(0, 25);
  display.print(distance, 0);
  display.print(" m");

  display.setTextSize(1);

  if (digitalRead(2)) {
    display.setCursor(75, 25);
    display.print(" ");
  } else {
    display.setCursor(75, 25);
    display.print("*");
  }

  display.setCursor(85, 25);
  display.print(wheelcount);

  display.display();
}






void cross()
{
  previnstant = instant;
  instant = micros() / 1000.0;
  wheelcount++;

  if (wheelcount > 2)
  {
    distance = wheelcount * diameter * pi;
    velocity = (diameter * pi / (instant - previnstant))  * 1000;

  }

}
