#ifndef __PIECE_H__
#define __PIECE_H__
#include <iostream>
#include <vector>
#include "Cell.h"
//#include "textdisplay.h"


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
    void updateCoords(int row, int col);

};
#endif