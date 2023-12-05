#ifndef __CELL_H__
#define __CELL_H__
#include <iostream>
#include <vector>

enum class PieceType { Pawn, Bishop, Knight, Rook, Queen, King, NONE};
enum class Colour { White, Black, None };

class Cell {
  PieceType piece;
  Colour cellColour;
  Colour pieceColour;
  int r, c;
  

 public:
  Cell();
  Cell(Colour cellColour, int r, int c);
  Cell(PieceType piece, Colour cellColour, Colour pieceColour, int r, int c);
  
  //sets the pieceType to NONE
  void remove();
  //sets the pieceType and pieceColour
  void add(PieceType pieceType, Colour pieceColour);
  //returns pieceType
  PieceType getPieceType();
  //returns pieceColour
  Colour getPieceColour();
  //returns cellColour
  Colour getCellColour();
  //returns r
  int getRow();
  //returns c
  int getCol();

};
#endif
