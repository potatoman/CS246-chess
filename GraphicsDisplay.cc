#include "GraphicsDisplay.h"

GraphicsDisplay::GraphicsDisplay(Xwindow& w): window{w} {}

void GraphicsDisplay::init() {
    this->gridSize = 64;
    this->cellSize = 63;
}

void GraphicsDisplay::notify(Cell &c) {
    draw(c);
}

void GraphicsDisplay::draw(Cell &c) {
    int color = 0;
    if (c.getCellColour() == Colour::Black) {
        color = 2;
    }
    //we build up from the bottom up because otherwise the board squares would overlap the pieces
    window.fillRectangle((c.getCol()) * cellSize, (7 - c.getRow()) * cellSize, cellSize, cellSize, color);
    //in our vector and cells, we have the pieces flipped, so we have to flip them here so that they're the right way around
    if(c.getPieceType() == PieceType::Bishop) {
        if (c.getPieceColour() == Colour::Black) {
            window.fillRectangle((c.getCol()) * cellSize + 10, (7 -c.getRow()) * cellSize + 10, cellSize - 20, cellSize - 20, 3);
            window.drawString(((c.getCol()) * cellSize) + 14, ((8 -c.getRow()) * cellSize - 25), "Bishop");
            drawCoords(c);
        } else if (c.getPieceColour() == Colour::White) {
            window.fillRectangle((c.getCol()) * cellSize + 10, (7 -c.getRow()) * cellSize + 10, cellSize - 20, cellSize - 20, 3);
            window.drawString(((c.getCol()) * cellSize) + 14, ((8 -c.getRow()) * cellSize - 25), "BISHOP");
            drawCoords(c);
        }
    } else if (c.getPieceType() == PieceType::Pawn) {
        if (c.getPieceColour() == Colour::Black) {
            window.fillRectangle((c.getCol()) * cellSize + 10, (7 -c.getRow()) * cellSize + 10, cellSize - 20, cellSize - 20, 3);
            window.drawString(((c.getCol()) * cellSize) + 20, ((8 -c.getRow()) * cellSize - 25), "Pawn");
            drawCoords(c);
        } else if (c.getPieceColour() == Colour::White) {
            window.fillRectangle((c.getCol()) * cellSize + 10, (7 -c.getRow()) * cellSize + 10, cellSize - 20, cellSize - 20, 3);
            window.drawString(((c.getCol()) * cellSize) + 20, ((8 -c.getRow()) * cellSize - 25), "PAWN");
            drawCoords(c);
        }
    } else if (c.getPieceType() == PieceType::King) {
       if (c.getPieceColour() == Colour::Black) {
            window.fillRectangle((c.getCol()) * cellSize + 10, (7 -c.getRow()) * cellSize + 10, cellSize - 20, cellSize - 20, 3);
            window.drawString((( c.getCol()) * cellSize) + 20, ((8 -c.getRow()) * cellSize - 25), "King");
            drawCoords(c);
        } else if (c.getPieceColour() == Colour::White) {
            window.fillRectangle((c.getCol()) * cellSize + 10, (7 -c.getRow()) * cellSize + 10, cellSize - 20, cellSize - 20, 3);
            window.drawString(((c.getCol()) * cellSize) + 20, ((8 -c.getRow()) * cellSize - 25), "KING");
            drawCoords(c);
        }
    } else if (c.getPieceType() == PieceType::Knight) {
        if (c.getPieceColour() == Colour::Black) {
            window.fillRectangle((c.getCol()) * cellSize + 10, (7 -c.getRow()) * cellSize + 10, cellSize - 20, cellSize - 20, 3);
            window.drawString(((c.getCol()) * cellSize) + 15, ((8 -c.getRow()) * cellSize - 25), "Knight");
            drawCoords(c);
        } else if (c.getPieceColour() == Colour::White) {
            window.fillRectangle((c.getCol()) * cellSize + 10, (7 -c.getRow()) * cellSize + 10, cellSize - 20, cellSize - 20, 3);
            window.drawString(((c.getCol()) * cellSize) + 15, ((8 -c.getRow()) * cellSize - 25), "KNIGHT");
            drawCoords(c);
        }
    } else if (c.getPieceType() == PieceType::Queen) {
        if (c.getPieceColour() == Colour::Black) {
            window.fillRectangle((c.getCol()) * cellSize + 10, (7 -c.getRow()) * cellSize + 10, cellSize - 20, cellSize - 20, 3);
            window.drawString(((c.getCol()) * cellSize) + 18, ((8 -c.getRow()) * cellSize - 25), "Queen");
            drawCoords(c);
        } else if (c.getPieceColour() == Colour::White) {
            window.fillRectangle((c.getCol()) * cellSize + 10, (7 -c.getRow()) * cellSize + 10, cellSize - 20, cellSize - 20, 3);
            window.drawString(((c.getCol()) * cellSize) + 18, ((8 -c.getRow()) * cellSize - 25), "QUEEN");
            drawCoords(c);
        }
    } else if (c.getPieceType() == PieceType::Rook) {
        if (c.getPieceColour() == Colour::Black) {
            window.fillRectangle((c.getCol()) * cellSize + 10, (7 -c.getRow()) * cellSize + 10, cellSize - 20, cellSize - 20, 3);
            window.drawString(((c.getCol()) * cellSize) + 20, ((8 -c.getRow()) * cellSize - 25), "Rook");
            drawCoords(c);
        } else if (c.getPieceColour() == Colour::White) {
            window.fillRectangle((c.getCol()) * cellSize + 10, (7 -c.getRow()) * cellSize + 10, cellSize - 20, cellSize - 20, 3);
            window.drawString(((c.getCol()) * cellSize) + 20, ((8 -c.getRow()) * cellSize - 25), "ROOK");
            drawCoords(c);
        }
    }
}

void GraphicsDisplay::drawCoords(Cell &c) {
    //separated this out to prevent the coordinates from being deleted when a piece moved
    if ((c.getRow() == 7 || c.getRow()) == 0 && c.getCol() == 0) {
        window.drawString(3 ,(cellSize * 8 - 53), "1");
        window.drawString((cellSize - 5), (cellSize * 8 - 5), "a");
    } else if ((c.getRow() == 7 || c.getRow() == 0) && c.getCol() == 1) {
        window.drawString((cellSize * (c.getCol() + 1) - 5), (cellSize * 8 - 5), "b");
    } else if ((c.getRow() == 7 || c.getRow() == 0) && c.getCol() == 2) {
        window.drawString((cellSize * (c.getCol() + 1) - 5), (cellSize * 8 - 5), "c");
    } else if ((c.getRow() == 7 || c.getRow() == 0) && c.getCol() == 3) {
        window.drawString((cellSize * (c.getCol() + 1) - 5), (cellSize * 8 - 5), "d");
    } else if ((c.getRow() == 7 || c.getRow() == 0) && c.getCol() == 4) {
        window.drawString((cellSize * (c.getCol() + 1) - 5), (cellSize * 8 - 5), "e");
    } else if ((c.getRow() == 7 || c.getRow() == 0) && c.getCol() == 5) {
        window.drawString((cellSize * (c.getCol() + 1) - 5), (cellSize * 8 - 5), "f");
    } else if ((c.getRow() == 7 || c.getRow() == 0) && c.getCol() == 6) {
        window.drawString((cellSize * (c.getCol() + 1) - 5), (cellSize * 8 - 5), "g");
    } else if ((c.getRow() == 7 || c.getRow() == 0) && c.getCol() == 7) {
        window.drawString((cellSize * (c.getCol() + 1) - 10), (cellSize * 8 - 5), "h");
    }
    if (c.getRow() == 1 && c.getCol() == 0) {
        window.drawString(3 ,(cellSize * (7 - c.getRow() + 1) - 53), "2");
    } else if (c.getRow() == 2 && c.getCol() == 0) {
        window.drawString(3 ,(cellSize * (7 - c.getRow() + 1) - 53), "3");
    } else if (c.getRow() == 3 && c.getCol() == 0) {
        window.drawString(3 ,(cellSize * (7 - c.getRow() + 1) - 53), "4");
    } else if (c.getRow() == 4 && c.getCol() == 0) {
        window.drawString(3 ,(cellSize * (7 - c.getRow() + 1) - 53), "5");
    } else if (c.getRow() == 5 && c.getCol() == 0) {
        window.drawString(3 ,(cellSize * (7 - c.getRow() + 1) - 53), "6");
    } else if (c.getRow() == 6 && c.getCol() == 0) {
        window.drawString(3 ,(cellSize * (7 - c.getRow() + 1) - 53), "7");
    } else if (c.getRow() == 7 && c.getCol() == 0) {
        window.drawString(3 ,(cellSize * (7 - c.getRow() + 1) - 53), "8");
    }
}
