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
  Cell();  // Default constructor
  Cell(Colour cellColour, int r, int c);
  Cell(PieceType piece, Colour cellColour, Colour pieceColour, int r, int c);
  
  void remove();
  void add(PieceType pieceType, Colour pieceColour);
  PieceType getPieceType();
  Colour getPieceColour();
  Colour getCellColour();
  int getRow();
  int getCol();

};
#endif
