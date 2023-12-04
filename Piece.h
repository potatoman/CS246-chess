#ifndef __PIECE_H__
#define __PIECE_H__
#include <iostream>
#include <vector>
#include "Grid.h"
//#include "textdisplay.h"
class Grid;

class Piece {
    std::vector<Cell*> cellsThreatening;
    PieceType piece;
    Colour pieceColour;
    int r, c;
    bool underAttack;



 public:
    Piece();  // Default constructor
    Piece(PieceType piece, Colour colour, int r, int c);
  

    PieceType getPieceType();
    Colour getPieceColour();
    bool getThreatStatus();
    void setThreatStatus(bool stat);
    int getRow();
    int getCol();
    std::vector<Cell*> * getCellsThreatening();
    void setRow(int r);
    void setCol(int c);
    void updateCellsThreatening(Grid &g);
    void updateThreatStatus(Grid &g);
    void updateOpposingPieceThreatStatus(Grid &g, Colour attackingPieceColour, int r, int c);
    void updateCoords(int row, int col);
    bool checkThreat(int row, int col);
    

};
#endif