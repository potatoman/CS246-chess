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
    bool underAttack = false;



 public:
    Piece();  // Default constructor
    Piece(PieceType piece, Colour colour, int r, int c);
  
    //returns piece
    PieceType getPieceType();

    //returns pieceColour
    Colour getPieceColour();

    //returns underAttack
    bool getThreatStatus();

    //sets underAttack to stat
    void setThreatStatus(bool stat);

    //returns r
    int getRow();

    //returns c
    int getCol();

    //returns a pointer to CellsThreatening
    std::vector<Cell*> * getCellsThreatening();
    
    //updates all of the cells this piece is threatening by looking at the board
    void updateCellsThreatening(Grid &g);

    //updates underAttack based off of every opposing pieces cellsThreatening 
    void updateThreatStatus(Grid &g);

    //updates an opposing pieces threatStatus based off of its row and column
    void updateOpposingPieceThreatStatus(Grid &g, Colour attackingPieceColour, int r, int c);

    //sets r to row and c to col
    void updateCoords(int row, int col);

    //returns true if a cell is threatened by this piece
    bool checkThreat(int row, int col);
};
#endif
