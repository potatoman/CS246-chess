#include <iostream>
#include "textdisplay.h"
#include "cell.h"
using namespace std;

TextDisplay::TextDisplay(): theDisplay{8, std::vector<char>(8, '_')} { }

void TextDisplay::updateTD(Cell &c) {
  if (c.getColour() == Colour::White) {
    if (c.getPieceType() == PieceType::Pawn) {
        theDisplay[c.getRow()][c.getCol()] = 'P';
    } else if (c.getPieceType() == PieceType::Bishop) {
        theDisplay[c.getRow()][c.getCol()] = 'B';
    } else if (c.getPieceType() == PieceType::Knight) {
        theDisplay[c.getRow()][c.getCol()] = 'N';
    } else if (c.getPieceType() == PieceType::Rook) {
        theDisplay[c.getRow()][c.getCol()] = 'R';
    } else if (c.getPieceType() == PieceType::Queen) {
        theDisplay[c.getRow()][c.getCol()] = 'Q';
    } else {
        theDisplay[c.getRow()][c.getCol()] = 'K';
    }
  } else if (c.getColour() == Colour::Black) {
    if (c.getPieceType() == PieceType::Pawn) {
        theDisplay[c.getRow()][c.getCol()] = 'p';
    } else if (c.getPieceType() == PieceType::Bishop) {
        theDisplay[c.getRow()][c.getCol()] = 'b';
    } else if (c.getPieceType() == PieceType::Knight) {
        theDisplay[c.getRow()][c.getCol()] = 'n';
    } else if (c.getPieceType() == PieceType::Rook) {
        theDisplay[c.getRow()][c.getCol()] = 'r';
    } else if (c.getPieceType() == PieceType::Queen) {
        theDisplay[c.getRow()][c.getCol()] = 'q';
    } else {
        theDisplay[c.getRow()][c.getCol()] = 'k';
    }
  } else {
    theDisplay[c.getRow()][c.getCol()] = '_';
  }
}

TextDisplay::~TextDisplay() {}



ostream &operator<<(ostream &out, const TextDisplay &td) {
  cout << "a b c d e f g";
  for(int i = 0; i < 8; i++) {
    cout << i;
    for(int j = 0; j < 8; j++) {
      out << td.theDisplay[i][j];
    }
    if (i != 7) {
      out << endl;
    }
    
  }
  return out;
}
