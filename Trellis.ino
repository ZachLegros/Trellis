/*************************************************** 
  This is a test example for the Adafruit Trellis w/HT16K33

  Designed specifically to work with the Adafruit Trellis 
  ----> https://www.adafruit.com/products/1616
  ----> https://www.adafruit.com/products/1611

  These displays use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include "Adafruit_Trellis.h"

/*************************************************** 
  This example shows reading buttons and setting/clearing buttons in a loop
  "momentary" mode has the LED light up only when a button is pressed
  "latching" mode lets you turn the LED on/off when pressed

  Up to 8 matrices can be used but this example will show 4 or 1
 ****************************************************/

#define MOMENTARY 0
#define LATCHING 1
// set the mode here
#define MODE MOMENTARY


Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0);

#define NUMTRELLIS 1

#define numKeys (NUMTRELLIS * 16)
#define INTPIN A2
// Connect I2C SDA pin to your Arduino SDA line
// Connect I2C SCL pin to your Arduino SCL line

byte ledMatrix[16] = {0,};

void sendMatrix() 
{
  for (int i=0; i<15; i++)
  {
    Serial.print(ledMatrix[i]);
    Serial.print(",");
  }
  Serial.print(ledMatrix[15]);
  Serial.println();
}

void setup() {
  Serial.begin(9600);
 
  // INT pin requires a pullup
  pinMode(INTPIN, INPUT);
  digitalWrite(INTPIN, HIGH);
  
  trellis.begin(0x70);

  // light up all the LEDs in order
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.setLED(i);
    trellis.writeDisplay();   
    delay(50);
  }
  // then turn them off
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.clrLED(i);
    trellis.writeDisplay();    
    delay(50);
  }
}

void loop() {
  delay(30); // 30ms delay is required, dont remove me!
  
  if (MODE == MOMENTARY) {
    // If a button was just pressed or released...
    if (trellis.readSwitches()) {
      // go through every button
      for (uint8_t i=0; i<numKeys; i++) {
      	// if it was pressed, turn it on
      	if (trellis.justPressed(i)) {
      	  trellis.setLED(i);
          ledMatrix[i] = 1;
      	} 
      	// if it was released, turn it off
      	if (trellis.justReleased(i)) {
      	  trellis.clrLED(i);
          ledMatrix[i] = 0;
      	}
      }
      // tell the trellis to set the LEDs we requested
      trellis.writeDisplay();
      sendMatrix();
    }
  }

  if (MODE == LATCHING) {
    // If a button was just pressed or released...
    if (trellis.readSwitches()) {
      // go through every button
      for (uint8_t i=0; i<numKeys; i++) {
        // if it was pressed...
      	if (trellis.justPressed(i)) {
    	    // Alternate the LED
      	  if (trellis.isLED(i))
          {
            trellis.clrLED(i);
            ledMatrix[i] = 0;
          }
      	  else
          {
            trellis.setLED(i);
            ledMatrix[i] = 1; 
          }
        } 
      }
      // tell the trellis to set the LEDs we requested
      trellis.writeDisplay();
    }
  }
}
