#include "Gameboard.h"
#include "Arduino.h"

Gameboard::Gameboard(int w, int h) {
  cursorPosition = 0;
  width = w;
  height = h;
  numberOfCells = w * h;
  board[numberOfCells] = {0};
}

void Gameboard::moveLeft() {
  if (cursorPosition > 0) {
    cursorPosition--;
  }
}

void Gameboard::moveRight() {
  if (cursorPosition < width) {
    cursorPosition++;
  }
}

void Gameboard::dropPiece() {
  
}

bool Gameboard::checkWinner() {
  
}

// Prints the board to the serial monitor
void Gameboard::boardToString() {
  
}

// Outputs board to LED array
void Gameboard::boardToLEDs(CRGB *leds) {
  
}

int Gameboard::getWidth() {
  return width;
}

int Gameboard::getHeight() {
  return height;
}

