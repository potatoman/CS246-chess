#include "Piece.h"
#include "Grid.h"

Piece::Piece(PieceType piece, Colour colour, int r, int c): piece{piece}, pieceColour{colour}, r{r}, c{c}, underAttack{false} {}

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
        if (pieceColour == Colour::White) {
            cellsThreatening.emplace_back(g.findCell(r+1, c+1));
            cellsThreatening.emplace_back(g.findCell(r+1, c-1));
        } else {
            cellsThreatening.emplace_back(g.findCell(r-1, c+1));
            cellsThreatening.emplace_back(g.findCell(r-1, c-1));
        }
    } else if (piece == PieceType::Rook || piece == PieceType::Queen || piece == PieceType::King) { 
            if (r != 0) {
                cellsThreatening.emplace_back(g.findCell(r-1, c));
            }
            if (r != 7) {
                cellsThreatening.emplace_back(g.findCell(r+1, c));
            }
            if (c != 0) {
                cellsThreatening.emplace_back(g.findCell(r, c-1));
            }
            if (c != 7) {
                cellsThreatening.emplace_back(g.findCell(r, c+1));
            }
            if (piece != PieceType::King) {
                currentRow = r;
                currentCol = c;
                currentCol -=2;
                while (currentCol >= 0 && !g.blockCheck2(r, c, r, currentCol)) {
                    // add cell at [r][currentCol] to the vector
                    cellsThreatening.emplace_back(g.findCell(r, currentCol));
                    currentCol-=1;
                }
                currentCol = c + 2;
                while (currentCol <= 7 && !g.blockCheck2(r, c, r, currentCol)) {
                    // add cell at [r][currentCol] to the vector
                    cellsThreatening.emplace_back(g.findCell(r, currentCol));
                    currentCol+=1;
                }
                currentCol = c;
                currentRow = r + 2;
                while (currentRow <= 7 && !g.blockCheck2(r, c, currentRow, c)) {
                    // add cell at [currentRow][c] to the vector
                    cellsThreatening.emplace_back(g.findCell(currentRow, c));
                    currentRow+=1;
                }
                currentRow = r - 2;
                while (currentRow >= 0 && !g.blockCheck2(r, c, currentRow, c)) {
                    // add cell at [currentRow][c] to the vector
                    cellsThreatening.emplace_back(g.findCell(currentRow, c));
                    currentRow-=1;
                }
        }
       
    } else if (piece == PieceType::Knight) {
        if (r + 1 <= 7 && c + 2 <= 7) { cellsThreatening.emplace_back(g.findCell(r+1, c+2)); }
        if (r + 1 <= 7 && c - 2 >= 0) { cellsThreatening.emplace_back(g.findCell(r+1, c-2)); }
        if (r - 1 >= 0 && c + 2 <= 7) { cellsThreatening.emplace_back(g.findCell(r-1, c+2)); }
        if (r - 1 >= 0 && c - 2 >= 0) { cellsThreatening.emplace_back(g.findCell(r-1, c-2)); }
        if (r - 2 >= 0 && c + 1 <= 7) { cellsThreatening.emplace_back(g.findCell(r-2, c+1)); }
        if (r - 2 >= 0 && c - 1 >= 0) { cellsThreatening.emplace_back(g.findCell(r-2, c-1)); }
        if (r + 2 <= 7 && c + 1 <= 7) { cellsThreatening.emplace_back(g.findCell(r+2, c+1)); }
        if (r + 2 <= 7 && c - 1 >= 0) { cellsThreatening.emplace_back(g.findCell(r+2, c-1)); }
    } 
    if (piece == PieceType::Bishop || piece == PieceType::Queen || piece == PieceType::King) {
        // add first one in each direction
        if (r != 0 && c != 0) {
            cellsThreatening.emplace_back(g.findCell(r+1, c+1));
        }
        if (r != 0 && c != 7) {
            cellsThreatening.emplace_back(g.findCell(r+1, c-1));
        }
        if (r != 7 && c != 0) {
            cellsThreatening.emplace_back(g.findCell(r-1, c+1));
        }
        if (r != 7 && c != 0) {
            cellsThreatening.emplace_back(g.findCell(r-1, c-1));
        }
        if (piece != PieceType::King) {
            currentRow = r + 2;
            currentCol = c + 2;
            while (currentRow <= 7 && currentCol <= 7 && !g.blockCheck2(r, c, currentRow, currentCol)) {
                // add cell to vector
                cellsThreatening.emplace_back(g.findCell(currentRow, currentCol));
                currentRow++;
                currentCol++;
            }

            currentRow = r + 2;
            currentCol = c - 2;
            while (currentRow <= 7 && currentCol >= 0 && !g.blockCheck2(r, c, currentRow, currentCol)) {
                // add cell to vector
                cellsThreatening.emplace_back(g.findCell(currentRow, currentCol));
                currentRow++;
                currentCol--;
            }

            currentRow = r - 2;
            currentCol = c + 2;
            while (currentRow >= 0 && currentCol <= 7 && !g.blockCheck2(r, c, currentRow, currentCol)) {
                // add cell to vector
                cellsThreatening.emplace_back(g.findCell(currentRow, currentCol));
                currentRow--;
                currentCol++;
            }

            currentRow = r - 2;
            currentCol = c - 2;
            while (currentRow >= 0 && currentCol >= 0 && !g.blockCheck2(r, c, currentRow, currentCol)) {
                // add cell to vector
                cellsThreatening.emplace_back(g.findCell(currentRow, currentCol));
                if () {
                    
                }
                currentRow--;
                currentCol--;
            }
        }
        
    }
    
}

void Piece::updateThreatStatus() {
    
}
