/* Example code for TM1637 4 digit 7 segment display with Arduino. More info: www.www.makerguides.com */

// Include the library:
#include "TM1637Display.h"
#include <SPI.h>
// Define the connections pins:
#define CLK 6
#define DIO 7
#define MAX6675_SO 3
#define MAX6675_CS 4
#define MAX6675_SCK 5

// Create variable:
int temperature_celsius;
int temperature_fahrenheit;
TM1637Display display = TM1637Display(CLK, DIO);
// Create degree Celsius symbol:
const uint8_t celsius[] = {
  SEG_A | SEG_B | SEG_F | SEG_G,  // Circle
  SEG_A | SEG_D | SEG_E | SEG_F   // C
};

// Create degree Fahrenheit symbol:
const uint8_t fahrenheit[] = {
  SEG_A | SEG_B | SEG_F | SEG_G,  // Circle
  SEG_A | SEG_E | SEG_F | SEG_G   // F
};

void setup() {
  // Set the display brightness (0-7):
  display.setBrightness(0);
  // Clear the display:
  display.clear();
  // Begin serial communication at a baud rate of 9600:
  Serial.begin(9600);
  // Wait for console opening:
}

void loop() {
  // Set the brightness:
  float temperatura = leer_termopar();
  Serial.println(temperatura);
  display.setBrightness(7);
  display.showNumberDec(temperatura, false, 2, 0);
  display.setSegments(celsius, 2, 2);

  delay(150);
}

double leer_termopar()
{

    uint16_t v;
    pinMode(MAX6675_CS, OUTPUT);
    pinMode(MAX6675_SO, INPUT);
    pinMode(MAX6675_SCK, OUTPUT);

    digitalWrite(MAX6675_CS, LOW);
    delay(1);

    // Read in 16 bits,
    //  15    = 0 always
    //  14..2 = 0.25 degree counts MSB First
    //  2     = 1 if thermocouple is open circuit
    //  1..0  = uninteresting status

    v = shiftIn(MAX6675_SO, MAX6675_SCK, MSBFIRST);
    v <<= 8;
    v |= shiftIn(MAX6675_SO, MAX6675_SCK, MSBFIRST);

    digitalWrite(MAX6675_CS, HIGH);
    if (v & 0x4)
    {
        // Bit 2 indicates if the thermocouple is disconnected
        return NAN;
    }

    // The lower three bits (0,1,2) are discarded status bits
    v >>= 3;

    // The remaining bits are the number of 0.25 degree (C) counts
    return v * 0.25;
} 
