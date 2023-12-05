#ifndef __TEXTDISPLAY_H__
#define __TEXTDISPLAY_H__
#include <iostream>
#include <vector>
class Cell;

class TextDisplay {
  std::vector<std::vector<char>> theDisplay;

 public:
  TextDisplay();

  //updates the text display
  void updateTD(Cell &c);

  ~TextDisplay();

  //prints out the board
  friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

#endif
