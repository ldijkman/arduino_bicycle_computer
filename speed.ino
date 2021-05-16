// DIY BiCycle Speedometer 
// distance in meter and speed in kilometer/hour 
// arduino nano 
// hall sensor 
// oled display
// https://www.instructables.com/DIY-Cycle-Speedometer/

#include <Adafruit_SSD1306.h>
#include <Wire.h>


#define pi 3.14
#define brakelight 8
#define diameter .71 // wheel diameter in meters
#define numberofreadings 6
#define cutoff 3
#define velocitycutoff 1.2   // in m/s
#define displaychangeafter 5000

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

const int chipSelect = 9;

int n = 0;
int count = 0;
double instant = 0;
double previnstant = 0;
double prevprevinstant = 0;
int wheelcount = 0;
int lastwheelcount = 0;
float velocity = 0;
float accleration = 0;
float distance = 0;
int temp;
bool unit1 = 0;
bool unit2 = 1;
bool unit3 = 0;
int unitmillis1 = 0;
int unitmillis2 = 0;
int unitmillis3 = 0;
unsigned long starttime;



void setup()
{
  pinMode(2, INPUT_PULLUP);
  pinMode(brakelight, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), cross, FALLING);

  display.begin(SSD1306_SWITCHCAPVCC , 0x3C);
  display.setTextColor(WHITE);
  display.setTextSize(1);


}

void loop()
{

  if (0 > accleration)
  {
    digitalWrite(brakelight, HIGH);
  }

  if (0 < accleration)
  {
    digitalWrite(brakelight, LOW);
  }



  {

    // if (unit1 == 1 && unit2 == 1 && unit3 == 1)
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

    //  display.display();

    //} else {

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

    //  display.setCursor(60, 25);
    //  display.print(wheelcount);
    display.display();

    // }



  }

  if (unitmillis1 != millis() / displaychangeafter + 1)
  {
    unitmillis1 = millis() / displaychangeafter + 1;
    unit1 = 1;
  }
  if (unitmillis2 != millis() / displaychangeafter + 2)
  {
    unitmillis2 = millis() / displaychangeafter + 2;
    unit2 = 1;
  }
  if (unitmillis3 != millis() / displaychangeafter)
  {
    unitmillis3 = millis() / displaychangeafter;
    unit3 = 1;
  }

}

void cross()
{
  //delay(50);
  prevprevinstant = previnstant;
  previnstant = instant;
  instant = micros() / 1000.0;
  wheelcount++;


  if (wheelcount > 2)
  {
    distance = wheelcount * diameter * pi;
    velocity = diameter / (instant - previnstant) * pi;
    accleration = 2 * diameter * pi * (1 / (instant - previnstant) - 1 / (previnstant - prevprevinstant)) / (instant - prevprevinstant);
    velocity = velocity * 1000;
    accleration = accleration * 100000000;

    unit3 = 0;
    unit2 = 0;
    unit1 = 0;

  }


}
