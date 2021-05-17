// DIY BiCycle Speedometer https://github.com/ldijkman/arduino_bicycle_computer/blob/main/README.md
// distance in meter and speed in kilometer/hour
// displays a * when magnet on Hall sensor and displays wheelcount
// arduino nano
// hall sensor A3144 (does not debounce like a switch) https://github.com/ldijkman/arduino_bicycle_computer/raw/main/A3144_hall_effect.jpg
// i2c oled display
// use a neodymium magnet for greater detection range
// https://www.instructables.com/DIY-Cycle-Speedometer/

// DIY BiCycle Speedometer distance and km/h arduino nano hall sensor oled display
// https://www.instructables.com/DIY-Cycle-Speedometer/

// https://github.com/ldijkman/arduino_bicycle_computer
// https://github.com/ldijkman/arduino_bicycle_computer/blob/main/speed.ino

// https://github.com/ldijkman/arduino_bicycle_computer/discussions

// i2c pins on arduino nano to 128x64 i2c oled display
// A4 = SDA
// A5 = SCL

// Digital input 2 connected to Hall Effect sensor A3144 https://github.com/ldijkman/arduino_bicycle_computer/raw/main/A3144_hall_effect.jpg


#include <Adafruit_SSD1306.h>  // https://github.com/adafruit/Adafruit_SSD1306
#include <Wire.h>

// for perfect measurement change wheeldiameter or circumference to match your wheel
#define pi 3.14
#define diameter .71                     // wheel diameter in meters .71m = 71cm = 710mm
float circumference = diameter * pi;     // calculate circumference from diameter = distance travel in 1 wheel rotation in meters
// float circumference = 2.2294;         // wheelcircumference = distance travel in 1 wheel rotation in meters

#define OLED_RESET 4                    
Adafruit_SSD1306 display(OLED_RESET);
// const int chipSelect = 9;               // i think this is of no use

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
  attachInterrupt(digitalPinToInterrupt(2), input2_interrupt_function_call, FALLING);

  display.begin(SSD1306_SWITCHCAPVCC , 0x3C);    // 0x3C i2c adress of oled display (use i2c scanner example?)
  display.setTextColor(WHITE);                   
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
  // display.print(velocity);                // m/s ?????????
  display.print(velocity * 3.6, 1);          // 1 meter per second = 3.6 kilometers per hour
                                             
  // display.print(velocity * 2.23694, 1);     // m/s to mph                                       
  display.setCursor(85, 0);
  display.print("km");
  // display.setCursor(80, 0);
  // display.print("mph");

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
    display.print("*");        // print a star=* on display if magnet is on hall sensor
  }

  display.setCursor(85, 25);
  display.print(wheelcount);

  display.display();
}






void input2_interrupt_function_call()
{
  previnstant = instant;                        // load previous millis() countervalue in previnstant              
  instant = millis();                           // load current millis() countervalue in instant                        
//instant = micros() / 1000.0; 
  wheelcount++;                                 // add 1 to wheelcount

    distance = wheelcount * circumference;                              // wheelcount x wheel circumference = distance travel in meters
    velocity = (circumference / (instant - previnstant)) * 1000;        // (wheel circumference / elapsed time in millisec) x 1000 = meterpersecond???
 
}
