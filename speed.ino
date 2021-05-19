// DIY BiCycle Speedometer https://github.com/ldijkman/arduino_bicycle_computer/blob/main/README.md
// distance in meter and speed in kilometer/hour
// displays a * when magnet on A3144 Hall sensor and displays wheelcount
// arduino nano
// hall sensor A3144 (does not debounce like a switch) https://github.com/ldijkman/arduino_bicycle_computer/raw/main/A3144_hall_effect.jpg
// i2c oled display 128x64
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

// display turns off after 60 seconds no wheelcount to save energy
// display turns on again after new wheelcount

// bike ride test 
// daylight sunlight visbility of oled is disapointing bad
// speed km diplay works very good
// arduino says 29550 meter 13255 weelcounts (=diameter 0.71meter)
// mytracks phone gps says 29.0km
// recalculate wheel settings
// makes 29000 / 13255 = 2,187854 meter wheel circumference
// makes 2,187854 / 3.14 = 0.6967687 meter wheel diameter
// see next time if arduino and gps compare

// 28inch tire inflate at 75psi 75 pounds per square inch = 5.17106797 bar
// think i did 4 Bar

// bought a cheap 3euro 1800mah dumb usb powerbank https://www.action.com/nl-nl/p/grundig-powerbank-sleutelhanger/
// this grundig usb powerbank does not switch off on low current draw
// arduino nano Power Consumption 19 mA ???
// oled maybe Power Consumption	20ma ???
// power enough for 40 hours ???

#include <Adafruit_SSD1306.h>  // https://github.com/adafruit/Adafruit_SSD1306
#include <Wire.h>

// for perfect measurement change wheeldiameter or circumference to match your wheel
#define pi 3.14
#define diameter 0.6967687                    // .71   // wheel diameter in meters .71m = 71cm = 710mm
float circumference = diameter * pi;          // calculate circumference from diameter = distance travel in 1 wheel rotation in meters
// float circumference = 2.187854;            //2.2294;  // wheelcircumference = distance travel in 1 wheel rotation in meters

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
    if (millis() - starttime >= 60000) {                     
      sleepDisplay(&display);             // if for 60 seonds no wheelcount blank screen sleep display
    }
  } else {
    starttime = millis();
    lastwheelcount = wheelcount;
    wakeDisplay(&display);
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

void sleepDisplay(Adafruit_SSD1306* display) {
  display->ssd1306_command(SSD1306_DISPLAYOFF);
}

void wakeDisplay(Adafruit_SSD1306* display) {
  display->ssd1306_command(SSD1306_DISPLAYON);
}
