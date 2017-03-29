#include <FastLED.h>
#include <NESpad.h>

#include "Gameboard.h"

#define WIDTH 10
#define HEIGHT 10
#define NUM_LEDS WIDTH * HEIGHT
#define DATA_PIN 8

CRGB leds[NUM_LEDS];
Gameboard gameboard = Gameboard(7, 6);

NESpad controller1 = NESpad(2,3,4); // strobe/clock/data pin numbers for controller 1
NESpad controller2 = NESpad(5,6,7); // strobe/clock/data pin numbers for controller 2
byte state1 = 0;
byte state2 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

void checkController(NESpad controller) {
  byte state = controller.buttons();

  if (state & NES_A) {
    gameboard.dropPiece();
  } else if (state & NES_LEFT) {
    gameboard.moveLeft();
  } else if (state & NES_RIGHT) {
    gameboard.moveRight();
  }
}

