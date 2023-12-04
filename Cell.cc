#include "Cell.h"

Cell::Cell(): piece{PieceType::NONE}, cellColour{Colour::None} {}
Cell::Cell(Colour cellColour, int r, int c): piece{PieceType::NONE}, cellColour{cellColour}, pieceColour{Colour::None}, r{r}, c{c} {}
Cell::Cell(PieceType piece, Colour cellColour, Colour pieceColour, int r, int c): piece{piece}, cellColour{cellColour}, pieceColour{pieceColour}, r{r}, c{c} {}

void Cell::add(PieceType pieceType, Colour pieceColour) {
    this->piece = pieceType;
    this->pieceColour = pieceColour;
}

void Cell::remove() {
    piece = PieceType::NONE;
    pieceColour = Colour::None;
}

PieceType Cell::getPieceType() {
    return piece;
}
Colour Cell::getCellColour() {
    return cellColour;
}
Colour Cell::getPieceColour() {
    return pieceColour;
}
int Cell::getRow() {
    return r;
}
int Cell::getCol() {
    return c;
}