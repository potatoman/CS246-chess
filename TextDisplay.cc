#include <iostream>
#include "TextDisplay.h"
#include "Cell.h"
using namespace std;

TextDisplay::TextDisplay(): theDisplay{8, std::vector<char>(8, '_')} {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if ((j + i)%2 == 0) {
        theDisplay[i][j] = ' ';
      }
    }
  }
 }

void TextDisplay::updateTD(Cell &c) {
  /*if (c.getPieceType() == PieceType::None) {
    cout << "should be none" << endl;
  } else if (c.getPieceType() == PieceType::Rook) {
    cout << "rook" << endl;
  }*/

  if (c.getPieceColour() == Colour::White) {
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
  } else if (c.getPieceColour() == Colour::Black) {
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
      if (c.getCellColour() == Colour::Black) {
        theDisplay[c.getRow()][c.getCol()] = '_';
      } else {
        theDisplay[c.getRow()][c.getCol()] = ' ';
      }
     // update to make shit blank or underlined
  }
}

TextDisplay::~TextDisplay() {}



ostream &operator<<(ostream &out, const TextDisplay &td) {
  
  for(int i = 0; i < 8; i++) {
    cout << 8-i << " ";
    for(int j = 0; j < 8; j++) {
      out << td.theDisplay[i][j];
    }
    out << endl;
    
  }
  out << endl;
  out << "  abcdefgh";
  return out;
}
