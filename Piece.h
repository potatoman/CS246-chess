#ifndef __PIECE_H__
#define __PIECE_H__
#include <iostream>
#include <vector>
//#include "textdisplay.h"

enum class PieceType { Pawn, Bishop, Knight, Rook, Queen, King, None };
enum class Colour { White, Black, None };

class Piece {
    std::vector<Cell> cellsThreatening;
    int r, c;
    PieceType piece;
    Colour pieceColour;
    bool underAttack;



 public:
    Piece();  // Default constructor
    Piece(PieceType piece, Colour colour, int r, int c);
  

    PieceType getPieceType();
    Colour getColour();
    int getRow();
    int getCol();
    void updateMoves();
    void updateThreatStatus();

};
#endif