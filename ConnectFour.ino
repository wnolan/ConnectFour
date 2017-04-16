/*
 *  Josh Sanders
 *  CSc 4110 Embedded Systems
 *  Connect 4 Project
 *  
 *  This program is designed to run a game of Connect 4 on a matrix of 100 leds. The game utilizes the a 7x6 grid
 *  from the bottom left corner for the game board, the 7th row from the bottom displays the piece moving while 
 *  the user chooses a column, and the top left corner displays the current player.
 *  
 *  The NESpad library is used so that an NES controller can be used to control gameplay.
 *  
 *  The FastLED library is used to easily create a matrix of 
 */
 
#include <FastLED.h>
#include <NESpad.h>

#define WIDTH 10
#define HEIGHT 10
#define NUM_LEDS WIDTH * HEIGHT
#define DATA_PIN 8

int player;
int currentposition;
CRGB playercolor;
CRGB leds[NUM_LEDS];

NESpad controller[2];
controller[0] = NESpad(2,3,4); // strobe/clock/data pin numbers for controller 1
controller[1] = NESpad(5,6,7); // strobe/clock/data pin numbers for controller 2
byte state1 = 0;
byte state2 = 0;
boolean dropped;

void setup() {
  
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  
  for ( int i = 40; i <= 96; i++ ) { // light up game board
    if ( i%10 < 7 ) {
      leds[i] = CRGB::White;
    }
  }
  
  player = -1;
}


void checkController(NESpad controller,currentposition,playercolor) {
  
  byte state = controller.buttons();
  dropped = false;

  
  if (state & NES_A) { // when A is pressed, drop the piece
    
    while ( leds[currentposition+10] == CRGB::White && leds[currentposition+10] < 99 ) { 
      // while the next place down is not taken and is inside the matrix
      leds[currentposition] = CRGB::White; // set current position to white
      currentposition = currentposition+10; // move current position down a row
      leds[currentposition] = playercolor; // set new current position to the player's color
      FastLED.show(); // show the new state of the game board
      delay(500);
    }
    dropped = true; // after drop is complete, set dropped to true to end turn
  
  } else if (state & NES_LEFT) { // when left is pressed
     
      if (currentposition > 40) { // if there is a place to the left
        leds[currentposition] = CRGB::White; // set current position to white 
        currentposition--; // move currentposition to the left
        leds[currentposition] = playercolor; // set new current position to player's color
      } else { // if there is no position to the left
        leds[currentposition] = CRGB::White; // set current position to white
        currentposition = 46; // wrap around to right most position
        leds[currentposition] = playercolor; // set new current position to the player's color
      }
      FastLED.show(); // show the new state of the board
  
  } else if (state & NES_RIGHT) { // when right is pressed

      if (currentposition < 46) { // if there is a place to the right
        leds[currentposition] = CRGB::White; // set current position to black 
        currentposition++; // move currentposition to the right
        leds[currentposition] = playercolor; // set new current position to player's color
      } else { // if there is no position to the left
        leds[currentposition] = CRGB::White; // set current position to black
        cursorPosition = 40; // wrap around to left most position
        leds[currentposition] = playercolor; // set new current position to the player's color
      }
      FastLED.show(); // show the new state of the board
      
  }
}

boolean winner(int location, CRGB color) { 
  return ( horizontal(location, color) || vertical(location,color) || diagonal(location,color) );
} 

boolean horizontal (int location, CRGB color) {

  switch(location%10) {
    
    case 0: return (hleft(int location, CRGB color));
    
    case 1: return ( hleft(int location, CRGB color) 
                     || hmidleft(int location, CRGB color));
             
    case 2: return ( hleft(int location, CRGB color ) 
                     || hmidleft(int location, CRGB color) 
                     || hmidright(int location, CRGB color));
             
    case 3: return ( hleft(int location, CRGB color) 
                     || hmidleft(int location, CRGB color) 
                     || hmidright(int location, CRGB color) 
                     || hright(int location, CRGB color));
             
    case 4: return ( hmidleft(int location, CRGB color) 
                     || hmidright(int location, CRGB color) 
                     || hright(int location, CRGB color));
             
    case 5: return ( hmidright(int location, CRGB color) 
                     || hright(int location, CRGB color));
         
    case 6: return ( hright(int location, CRGB color));

    default: return false;
  }
  
}

/*
 * start of horizontal check functions
 */
boolean hleft(int location, CRGB color) {
  /*
   * Returns true if the current position is the far left of a horizontal row of four
   */
  return (leds[location] == color && leds[location+1] == color && leds[location+2] == color && leds[location+3] == color);
}

boolean hmidleft(int location, CRGB color) {
  /*
   * Returns true if the current position is the middle left of a horizontal row of four
   */
  return ( leds[location-1] == color && leds[location+1] == color && leds[location+2] == color );
}

boolean hmidright(int location, CRGB color) {
   /* 
    * Returns true if the current position is the middle right of a horizontal row of four 
    */
  return ( leds[location-2] == color && leds[location-1] == color && leds[location+1] == color );
}

boolean hright(int location, CRGB color) {
  /* 
   * Returns true if the current position is the far right of a horizontal row of four 
   */
  return ( leds[location-3] == color && leds[location-2] == color && leds[location-1] == color );
}
/* 
 *  end of horizontal check fuctions
 */

/*
 * Checking for a vertical set of 4 is simple. This is the only funtion required.
 * Because the pieces are dropped in from the top of the board, a set of 4 will only
 * ever occur going down from the current position
 */
boolean vertical(int location, CRGB color) {
  if (location < 70) { // 
    return ( leds[location+10] == color && leds[location+20] == color && leds[location+30] == color );
  }
  else {
    return false
  }
}

boolean diagonal (int location, CRGB color) {

  switch (location/10) {
    case 9: // bottom row
      switch (location%10) {
        case 0: case 1: case 2: // left side can go forward
          return ( left_forward(location, color) );
          
        case 3: // center can go either direction
          return ( left_forward(location, color 
                   || right_back(location, color) );
          
        case 4: case 5: case 6: // right side can go back
          return ( right_back(location, color) );

        default:
          return false;
      }
  
    case 8: // 2nd row from bottom
      switch (location%10) {
        case 0: // first column
          return ( left_forward(location, color) );
        
        case 1: // second column
          return ( left_forward(location, color) 
                   || midleft_forward(location, color));
      
        case 2: // third column
          return ( left_forward(location, color)
                   || midleft_forward(location, color)
                   || midright_back(location, color));

        case 3: // middle column
          return ( left_forward(location, color)
                   || midleft_forward(location, color)
                   || midright_back(location, color)
                   || right_back(location, color));

        case 4: // fifth column
          return ( midleft_forward(location, color)
                   || midright_back(location, color)
                   || right_back(location, color));

        case 5: // sixth column
          return ( midright_back(location, color)
                   || right_back(location, color));

        case 6: // seventh column
          return ( right_back(location, color));
      }
      
    case 7: // 3rd row from bottom
      switch(location%10) {
        case 0: // 3rd row, 1st column
          return ( left_forward(location, color));

        case 1: // 3rd row, 2nd column
          return ( left_forward(location, color)
                   || midleft_forward(location, color)
                   || midright_back(location, color));

        case 2: // 3rd row, 3rd column
          return ( left_forward(location, color)
                   || midleft_forward(location, color)
                   || midright_forward(location, color)
                   || midleft_back(location, color)
                   || midright_back(location, color));

        case 3: // 3rd row, middle column
          return ( left_forward(location, color)
                   || midleft_forward(location, color)
                   || midright_forward(location, color)
                   || midleft_back(location, color)
                   || midright_back(location, color)
                   || right_back(location, color));

        case 4: // 3rd row, 5th column
          return ( midleft_forward(location, color)
                   || midright_forward(location, color)
                   || midleft_back(location, color)
                   || midright_back(location, color)
                   || right_back(location, color));

        case 5: // 3rd row, 6th column
          return ( midright_forward(location, color)
                   || midright_back(location, color)
                   || right_back(location, color));

        case 6: // 3rd row, last column
          return ( right_back(location, color));

        default: return false;
      }

    case 6: // 3rd row from top
      switch (location%10) {
        case 0: // 4th row, 1st column
          return ( left_back(location, color));

        case 1: // 4th row, 2nd column
          return ( left_back(location, color)
                   || midleft_back(location, color)
                   || midleft_forward(location, color));

        case 2: // 4th row, 3rd column
          return ( left_back(location, color)
                   || midleft_back(location, color)
                   || midright_back(location, color)
                   || midleft_forward(location, color)
                   || midright_forward(location, color));

        case 3: // 4th row, middle column
          return ( left_back(location, color)
                   || midleft_back(location, color)
                   || midright_back(location, color)
                   || midleft_forward(location, color)
                   || midright_forward(location, color)
                   || right_forward(location, color));

        case 4: // 4th row, 5th column
          return ( midleft_back(location, color)
                   || midright_back(location, color)
                   || midleft_forward(location, color)
                   || midright_forward(location, color)
                   || right_forward(location, color));

        case 5: // 4th row, 6th column
          return ( midright_back(location, color)
                   || midright_forward(location, color)
                   || right_forward(location, color));

        case 6: // 4th row, last column
          return ( right_forward(location, color));

        default: return false;
      }

    case 5: // 2nd row from top
      switch (location%10) {
        case 0: // 5th row, first column
          return ( left_back(location, color));

        case 1: // 5th row, 2nd column
          return ( left_back(location, color)
                   || midleft_back(location, color));

        case 2: // 5th row, 3rd column
          return ( left_back(location, color)
                   || midleft_back(location, color)
                   || midright_forward(location, color));

        case 3: // 5th row, middle column
          return ( left_back(location, color)
                   || midleft_back(location, color)
                   || midright_forward(location, color)
                   || right_forward(location, color));

        case 4: // 5th row, 4th column
          return ( midleft_back(location, color)
                   || midright_forward(location, color)
                   || right_forward(location, color));

        case 5: // 5th row, 6th column
          return ( midright_forward(location, color)
                   || right_forward(location, color));

        case 6: // 5th row, last column
          return ( right_forward(location, color));

        default: return false;
      }

    case 4: // top row
      switch (location%10) {
        case 0: case 1: case 2:
          return ( left_back(location, color));

        case 3:
          return ( left_back(location, color)
                   || right_forward(location, color));

        case 4: case 5: case 6:
          return ( right_forward(location, color));

        default: return false;
      }
      
    default: return false;     
  }  
}

/*
 *  Start of "forward slash" checking functions
 */
boolean left_forward (int location, CRGB color) {
  return ( leds[location-9] == color && leds[location-18] == color && leds[location-27] == color );
}

boolean midleft_forward (int location, CRGB color) {
  return ( leds[location+9] == color && leds[location-9] == color && leds[location-18] == color );
}

boolean midright_forward (int location, CRGB color) {
  return ( leds[location+18] == color && leds[location+9] == color && leds[location-9] == color );
}

boolean right_forward (int location, CRGB color) {
  return ( leds[location+27] == color && leds[location+18] == color && leds[location+9] == color );
}
/*
 *  End of "forward slash" checking functions
 */

/*
 *  Start of "backslash" checking functions
 */
boolean right_back (int location, CRGB color) {
  return ( leds[location-11] == color && leds[location-22] == color && leds[location-33] == color );
}

boolean midright_back (int location, CRGB color) {
  return ( leds[location-11] == color && leds[location-22] == color && leds[location+11] == color );
}

boolean midleft_back (int location, CRGB color) {
  return ( leds[location-22] == color && leds[location+11] == color && leds[location+22] == color );
}

boolean left_back (int location, CRGB color) {
  return ( leds[location+11] == color && leds[location+22] == color && leds[location+33] == color );
}
/*
 * End of "backslash" checking functions
 */

void loop() {
  
  currentposition = 30; // (first column, 4th row)
  
  player++; // next player and set color
  
  if ( player%2 == 0 ){
    playercolor = CRGB::Red;
  } else {
    playercolor = CRGB::Yellow;
  }

  leds[currentposition] = playercolor; //place player's peice and update
  
  FastLED.show();
  
  // read controller and take action
  while (!dropped) // dropped initially set to false, becomes true and exits
  {
    // checks for input, changes the board accordingly, and prints new state
    checkController(controller[player%2],currentposition,playercolor);
  }

  if (winner(currentposition, playercolor))
  {
    exit;
  }
}


