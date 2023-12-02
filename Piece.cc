#include "Piece.h"
#include "Grid.h"


PieceType Piece::getPieceType() {
    return piece;
}
Colour Piece::getPieceColour() {
    return pieceColour;
}
void Piece::updateCoords(int row, int col) {
    r = row;
    c = col;
}

void Piece::updateCellsThreatening(Grid &g) {
    int currentCol;
    int currentRow;
    if (piece == PieceType::Pawn) {
        //threatening top left and top right (maybe bottom if its a black pawn)
    } else if (piece == PieceType::Rook || piece == PieceType::Queen) { 
        // first add first cell in each direction
        currentRow = r;
        currentCol = c;
        currentCol -=2;
        while (currentCol >= 0 && !blockCheck2(r, c, r, currentCol)) {
            // add cell at [r][currentCol] to the vector
            currentCol-=1;
        }
        currentCol = c + 2;
        while (currentCol <= 7 && !blockCheck2(r, c, r, currentCol)) {
            // add cell at [r][currentCol] to the vector
            currentCol+=1;
        }
        currentCol = c;
        currentRow = r + 2;
        while (currentRow <= 7 && !blockCheck2(r, c, currentRow, c)) {
            // add cell at [r][currentCol] to the vector
            currentRow+=1;
        }
        currentRow = r - 2;
        while (currentRow >= 0 && !blockCheck2(r, c, currentRow, c)) {
            // add cell at [r][currentCol] to the vector
            currentRow-=1;
        }
    } else if (piece == PieceType::Knight) {
        if (r + 1 <= 7 && c + 2 <= 7) { /* add to vector*/ }
        if (r + 1 <= 7 && c - 2 >= 0) {}
        if (r - 1 >= 0 && c + 2 <= 7) {}
        if (r - 1 >= 0 && c - 2 >= 0) {}
        if (r - 2 >= 0 && c + 2 <= 7) {}
        if (r - 2 >= 0 && c - 2 >= 0) {}
        if (r + 2 <= 7 && c + 1 <= 7) {}
        if (r + 2 <= 7 && c - 1 >= 0) {}
    } else if (piece == PieceType::King) {
        // add piece in each direction (while still in board bounds)
    }
    if (piece == PieceType::Bishop || piece == PieceType::Queen) {
        // add first one in each direction
        currentRow = r + 2;
        currentCol = c + 2;
        while (currentRow <= 7 && currentCol <= 7 && !blockCheck2(r, c, currentRow, currentCol)) {
            // add cell to vector
            currentRow++;
            currentCol++;
        }

        currentRow = r + 2;
        currentCol = c - 2;
        while (currentRow <= 7 && currentCol >= 0 && !blockCheck2(r, c, currentRow, currentCol)) {
            // add cell to vector
            currentRow++;
            currentCol--;
        }

        currentRow = r - 2;
        currentCol = c + 2;
        while (currentRow >= 0 && currentCol <= 7 && !blockCheck2(r, c, currentRow, currentCol)) {
            // add cell to vector
            currentRow--;
            currentCol++;
        }

        currentRow = r - 2;
        currentCol = c - 2;
        while (currentRow >= 0 && currentCol >= 0 && !blockCheck2(r, c, currentRow, currentCol)) {
            // add cell to vector
            currentRow--;
            currentCol--;
        }
    }
    
}

void Piece::updateThreatStatus() {
    
}
