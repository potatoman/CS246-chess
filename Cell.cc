#include "Cell.h"

Cell::Cell(): piece{PieceType::None}, colour{Colour::None} {}
Cell::Cell(PieceType piece, Colour colour, int r, int c): piece{piece}, colour{colour}, r{r}, c{c} {}

void Cell::add(PieceType piece, Colour colour) {
    this->piece = piece;
    this->colour = colour;
}

void Cell::remove() {
    piece = PieceType::None;
    colour = Colour::None;
}

PieceType Cell::getPieceType() {
    return piece;
}
Colour Cell::getColour() {
    return colour;
}
int Cell::getRow() {
    return r;
}
int Cell::getCol() {
    return c;
}