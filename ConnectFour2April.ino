#include <FastLED.h>
//#include <NESpad.h>

#define WIDTH 10
#define HEIGHT 10
#define NUM_LEDS WIDTH * HEIGHT
#define DATA_PIN 8

int player;
int currentposition;
CRGB playercolor;
CRGB leds[NUM_LEDS];
Gameboard gameboard = Gameboard(7, 6);

NESpad controller[2];
controller[0] = NESpad(2,3,4); // strobe/clock/data pin numbers for controller 1
controller[1] = NESpad(5,6,7); // strobe/clock/data pin numbers for controller 2
byte state1 = 0;
byte state2 = 0;
boolean dropped;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  player = -1;
}


void checkController(NESpad controller,currentposition,playercolor) {
  
  byte state = controller.buttons();
  dropped = false;

  
  if (state & NES_A) { // when A is pressed, drop the piece
    
    while ( leds[currentposition+10] == CRGB::Black && leds[currentposition+10] < 100 ) { 
      // while the next place down is not taken and is inside the matrix
      leds[currentposition] = CRGB::Black; // set current position to black
      currentposition = currentposition+10; // move current position down a row
      leds[currentposition] = playercolor; // set new current position to the player's color
      FastLED.show(); // show the new state of the game board
      delay(500);
    }
    dropped = true; // after drop is complete, set dropped to true to end turn
  
  } else if (state & NES_LEFT) { // when left is pressed
     
      if (currentposition > 41) { // if there is a place to the left
        leds[currentposition] = CRGB::Black; // set current position to black 
        currentposition--; // move currentposition to the left
        leds[currentposition] = playercolor; // set new current position to player's color
      } else { // if there is no position to the left
        leds[currentposition] = CRGB::Black; // set current position to black
        currentposition = 47; // wrap around to right most position
        leds[currentposition] = playercolor; // set new current position to the player's color
      }
      FastLED.show(); // show the new state of the board
  
  } else if (state & NES_RIGHT) { // when right is pressed

      if (currentposition < 47) { // if there is a place to the right
        leds[currentposition] = CRGB::Black; // set current position to black 
        currentposition++; // move currentposition to the right
        leds[currentposition] = playercolor; // set new current position to player's color
      } else { // if there is no position to the left
        leds[currentposition] = CRGB::Black; // set current position to black
        cursorPosition = 41; // wrap around to left most position
        leds[currentposition] = playercolor; // set new current position to the player's color
      }
      FastLED.show(); // show the new state of the board
      
  }
}

void loop() {
  
  currentposition = 41; // (first column, 4th row)
  
  player++; // next player and set color
  
  if ( player%2 == 0 ){
    playercolor = CRGB::Red;
  } else {
    playercolor = CRGB::Yellow;
  }

  leds[currentposition] = playercolor; //place players peice and update
  FastLED.show();
  
  // read controller and take action
  while (!dropped) // dropped initially set to false, becomes true and exits
  {
    // checks for input, changes the board accordingly, and prints new state
    checkController(controller[player%2],currentposition,playercolor);
  }
  
  // check for winner
}


