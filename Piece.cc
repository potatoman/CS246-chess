#include "Piece.h"


PieceType Piece::getPieceType() {
    return piece;
}
Colour Piece::getColour() {
    return pieceColour;
}
void Piece::updateCoords(int row, int col) {
    r = row;
    c = col;
}
