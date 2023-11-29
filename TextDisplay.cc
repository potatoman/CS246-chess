#include <iostream>
#include "textdisplay.h"
#include "cell.h"
using namespace std;

TextDisplay::TextDisplay(): theDisplay{8, std::vector<char>(8, '_')} { }

void TextDisplay::notify(Cell &c) {
  if (c.getColour() == White) {
    if (c.getPieceType() == Pawn) {
        theDisplay[c.getRow()][c.getCol()] = 'P';
    } else if (c.getPieceType() == Bishop) {
        theDisplay[c.getRow()][c.getCol()] = 'B';
    } else if (c.getPieceType() == Knight) {
        theDisplay[c.getRow()][c.getCol()] = 'N';
    } else if (c.getPieceType() == Rook) {
        theDisplay[c.getRow()][c.getCol()] = 'R';
    } else if (c.getPieceType() == Queen) {
        theDisplay[c.getRow()][c.getCol()] = 'Q';
    } else {
        theDisplay[c.getRow()][c.getCol()] = 'K';
    }
  } else {
    if (c.getPieceType() == Pawn) {
        theDisplay[c.getRow()][c.getCol()] = 'p';
    } else if (c.getPieceType() == Bishop) {
        theDisplay[c.getRow()][c.getCol()] = 'b';
    } else if (c.getPieceType() == Knight) {
        theDisplay[c.getRow()][c.getCol()] = 'n';
    } else if (c.getPieceType() == Rook) {
        theDisplay[c.getRow()][c.getCol()] = 'r';
    } else if (c.getPieceType() == Queen) {
        theDisplay[c.getRow()][c.getCol()] = 'q';
    } else {
        theDisplay[c.getRow()][c.getCol()] = 'k';
    }
  }
}

TextDisplay::~TextDisplay() {}



ostream &operator<<(ostream &out, const TextDisplay &td) {

  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
      out << td.theDisplay[i][j];
    }
    if (i != 7) {
      out << endl;
    }
    
  }
  return out;
}
