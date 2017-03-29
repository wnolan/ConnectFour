#include <FastLED.h>

class Gameboard {
  
  private:
    int cursorPosition;
    int width;
    int height;
    int numberOfCells;
    int board[];

  public:
    Gameboard(int w, int h);
    void moveLeft();
    void moveRight();
    void dropPiece();
    bool checkWinner();
    void boardToString();
    void boardToLEDs(CRGB *leds);
    int getWidth();
    int getHeight();
};
