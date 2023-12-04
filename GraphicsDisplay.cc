#include "GraphicsDisplay.h"

GraphicsDisplay::GraphicsDisplay(Xwindow& w): window{w} {}

void GraphicsDisplay::init() {
    this->gridSize = 64;
    this->cellSize = 63;
}

void GraphicsDisplay::notify(Cell &c) {
    draw(c); //encapsulates draw so that we can't get random windows
}

void GraphicsDisplay::draw(Cell &c) {
    int color = 1; //sets rectangle to black by default so you can see something
    if (c.getCellColour() == Colour::Black) {
        color = 0;
    }
    if (c.getRow() == 0 && c.getCol() == 0) {
        window.drawString(3 * cellSize, 3 * cellSize, "1");
        window.drawString((cellSize - 3) * cellSize, (cellSize - 3) * cellSize, "a");
    } else if (c.getRow() == 0 && c.getCol() == 1) {
        window.drawString((cellSize - 3) * cellSize * c.getCol(), (cellSize - 3) * cellSize, "b");
    } else if (c.getRow() == 0 && c.getCol() == 2) {
        window.drawString((cellSize - 3) * cellSize * c.getCol(), (cellSize - 3) * cellSize, "c");
    } else if (c.getRow() == 0 && c.getCol() == 3) {
        window.drawString((cellSize - 3) * cellSize * c.getCol(), (cellSize - 3) * cellSize, "d");
    } else if (c.getRow() == 0 && c.getCol() == 4) {
        window.drawString((cellSize - 3) * cellSize * c.getCol(), (cellSize - 3) * cellSize, "e");
    } else if (c.getRow() == 0 && c.getCol() == 5) {
        window.drawString((cellSize - 3) * cellSize * c.getCol(), (cellSize - 3) * cellSize, "f");
    } else if (c.getRow() == 0 && c.getCol() == 6) {
        window.drawString((cellSize - 3) * cellSize * c.getCol(), (cellSize - 3) * cellSize, "g");
    } else if (c.getRow() == 0 && c.getCol() == 7) {
        window.drawString((cellSize - 3) * cellSize * c.getCol(), (cellSize - 3) * cellSize, "h");
    }
    if (c.getRow() == 1 && c.getCol() == 0) {
        window.drawString(cellSize - 3, cellSize - 3, "2");
    } else if (c.getRow() == 2 && c.getCol() == 0) {
        window.drawString(cellSize - 3, cellSize - 3, "3");
    } else if (c.getRow() == 3 && c.getCol() == 0) {
        window.drawString(cellSize - 3, cellSize - 3, "4");
    } else if (c.getRow() == 4 && c.getCol() == 0) {
        window.drawString(cellSize - 3, cellSize - 3, "5");
    } else if (c.getRow() == 5 && c.getCol() == 0) {
        window.drawString(cellSize - 3, cellSize - 3, "6");
    } else if (c.getRow() == 6 && c.getCol() == 0) {
        window.drawString(cellSize - 3, cellSize - 3, "7");
    } else if (c.getRow() == 7 && c.getCol() == 0) {
        window.drawString(cellSize - 3, cellSize - 3, "8");
    }

    if(c.getPieceType() == PieceType::Bishop) {
        window.fillRectangle(c.getCol() * cellSize, c.getRow() * cellSize, cellSize - 2, cellSize - 2, 1);
        window.drawString(c.getCol() * cellSize, c.getRow() * cellSize, "Bishop");
    } else if (c.getPieceType() == PieceType::Pawn) {
        window.fillRectangle(c.getCol() * cellSize, c.getRow() * cellSize, cellSize - 2, cellSize - 2, 1);
        window.drawString(c.getCol() * cellSize, c.getRow() * cellSize, "Pawn");
    } else if (c.getPieceType() == PieceType::King) {
        window.fillRectangle(c.getCol() * cellSize, c.getRow() * cellSize, cellSize - 2, cellSize - 2, 1);
        window.drawString(c.getCol() * cellSize, c.getRow() * cellSize, "King");
    } else if (c.getPieceType() == PieceType::Knight) {
        window.fillRectangle(c.getCol() * cellSize, c.getRow() * cellSize, cellSize - 2, cellSize - 2, 1);
        window.drawString(c.getCol() * cellSize, c.getRow() * cellSize, "Knight");
    } else if (c.getPieceType() == PieceType::Queen) {
        window.fillRectangle(c.getCol() * cellSize, c.getRow() * cellSize, cellSize - 2, cellSize - 2, 1);
        window.drawString(c.getCol() * cellSize, c.getRow() * cellSize, "Queen");
    } else if (c.getPieceType() == PieceType::Rook) {
        window.fillRectangle(c.getCol() * cellSize, c.getRow() * cellSize, cellSize - 2, cellSize - 2, 1);
        window.drawString(c.getCol() * cellSize, c.getRow() * cellSize, "Rook");
    }
    std::cout << cellSize << std::endl;
    window.fillRectangle(c.getCol() * cellSize, c.getRow() * cellSize, cellSize, cellSize, color);
}
