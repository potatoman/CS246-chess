#ifndef __CELL_H__
#define __CELL_H__
#include <iostream>
#include <vector>
//#include "textdisplay.h"

enum class PieceType { Pawn, Bishop, Knight, Rook, Queen, King };
enum class Colour { White, Black };

class Cell {
  int r, c;
  PieceType piece;
  Colour colour;


 public:
  Cell();  // Default constructor
  
  void remove();
  void add(PieceType piece, Colour colour);
  PieceType getPieceType();
  Colour getColour();
  int getRow();
  int getCol();

};
#endif
