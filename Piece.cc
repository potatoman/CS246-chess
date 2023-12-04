#include "Piece.h"
#include "Grid.h"

Piece::Piece(PieceType piece, Colour colour, int r, int c): piece{piece}, pieceColour{colour}, r{r}, c{c}, underAttack{false} {}
Piece::Piece() {}

PieceType Piece::getPieceType() {
    return piece;
}
Colour Piece::getPieceColour() {
    return pieceColour;
}
void Piece::setThreatStatus(bool stat) {
    underAttack = stat;
    return;
}
bool Piece::getThreatStatus() {
    return underAttack;
}
void Piece::updateCoords(int row, int col) {
    r = row;
    c = col;
}

int Piece::getRow() {
    return r;
}

int Piece::getCol() {
    return c;
}

void Piece::setCol(int c) {
    this->c = c;
}

void Piece::setRow(int r) {
    this->r = r;
}

void Piece::updateCellsThreatening(Grid &g) {//run on every move on the piece that just moved and every rook, bishop, queen both colours
    int currentCol;
    int currentRow;
    cellsThreatening.clear();
    if (piece == PieceType::Pawn) {
        //threatening top left and top right (maybe bottom if its a black pawn)
        if (pieceColour == Colour::White) {
            if (c != 0) {
                cellsThreatening.emplace_back(g.findCell(r+1, c-1));  
                if (g.findCell(r+1, c-1)->getPieceType() != PieceType::None) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, r+1, c-1); 
                }  
            }
            if (c != 7) {
                cellsThreatening.emplace_back(g.findCell(r+1, c+1));
                if (g.findCell(r+1, c+1)->getPieceType() != PieceType::None) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, r+1, c+1); 
                }  
            }
        } else {
            if (c != 0) {
                cellsThreatening.emplace_back(g.findCell(r-1, c-1));
                if (g.findCell(r-1, c-1)->getPieceType() != PieceType::None) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, r-1, c-1); 
                }  
            }
            if (c != 7) {
                cellsThreatening.emplace_back(g.findCell(r-1, c+1));
                if (g.findCell(r-1, c+1)->getPieceType() != PieceType::None) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, r-1, c+1); 
                }  
            }
        }
    } else if (piece == PieceType::Rook || piece == PieceType::Queen || piece == PieceType::King) { 
            if (r != 0) {
                cellsThreatening.emplace_back(g.findCell(r-1, c));
                if (g.findCell(r-1, c)->getPieceType() != PieceType::None) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, r-1, c); 
                }
            }
            if (r != 7) {
                cellsThreatening.emplace_back(g.findCell(r+1, c));
                if (g.findCell(r+1, c)->getPieceType() != PieceType::None) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, r+1, c); 
                }
            }
            if (c != 0) {
                cellsThreatening.emplace_back(g.findCell(r, c-1));
                if (g.findCell(r, c-1)->getPieceType() != PieceType::None) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, r, c-1); 
                }
            }
            if (c != 7) {
                cellsThreatening.emplace_back(g.findCell(r, c+1));
                if (g.findCell(r, c+1)->getPieceType() != PieceType::None) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, r, c+1); 
                }
            }
            if (piece != PieceType::King) {
                currentRow = r;
                currentCol = c;
                currentCol -=2;
                while (currentCol >= 0 && !g.blockCheck2(r, c, r, currentCol)) {
                    // add cell at [r][currentCol] to the vector
                    cellsThreatening.emplace_back(g.findCell(r, currentCol));
                    if (g.findCell(r, currentCol)->getPieceType() != PieceType::None) { 
                        updateOpposingPieceThreatStatus(g, pieceColour, r, currentCol); 
                    }
                    currentCol-=1;
                }
                currentCol = c + 2;
                while (currentCol <= 7 && !g.blockCheck2(r, c, r, currentCol)) {
                    // add cell at [r][currentCol] to the vector
                    cellsThreatening.emplace_back(g.findCell(r, currentCol));
                    if (g.findCell(r, currentCol)->getPieceType() != PieceType::None) { 
                        updateOpposingPieceThreatStatus(g, pieceColour, r, currentCol); 
                    }
                    currentCol+=1;
                }
                currentCol = c;
                currentRow = r + 2;
                while (currentRow <= 7 && !g.blockCheck2(r, c, currentRow, c)) {
                    // add cell at [currentRow][c] to the vector
                    cellsThreatening.emplace_back(g.findCell(currentRow, c));
                    if (g.findCell(currentRow, c)->getPieceType() != PieceType::None) { 
                        updateOpposingPieceThreatStatus(g, pieceColour, currentRow, c); 
                    }
                    currentRow+=1;
                }
                currentRow = r - 2;
                while (currentRow >= 0 && !g.blockCheck2(r, c, currentRow, c)) {
                    // add cell at [currentRow][c] to the vector
                    cellsThreatening.emplace_back(g.findCell(currentRow, c));
                    if (g.findCell(currentRow, c)->getPieceType() != PieceType::None) { 
                        updateOpposingPieceThreatStatus(g, pieceColour, currentRow, c); 
                    }
                    currentRow-=1;
                }
        }
       
    } else if (piece == PieceType::Knight) {
        if (r + 1 <= 7 && c + 2 <= 7) { 
            cellsThreatening.emplace_back(g.findCell(r+1, c+2)); 
            if (g.findCell(r+1, c+2)->getPieceType() != PieceType::None) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r+1, c+2); 
            }
        }
        if (r + 1 <= 7 && c - 2 >= 0) { 
            cellsThreatening.emplace_back(g.findCell(r+1, c-2)); 
            if (g.findCell(r+1, c-2)->getPieceType() != PieceType::None) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r+1, c-2); 
            }
        }
        if (r - 1 >= 0 && c + 2 <= 7) { 
            cellsThreatening.emplace_back(g.findCell(r-1, c+2)); 
            if (g.findCell(r-1, c+2)->getPieceType() != PieceType::None) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r-1, c+2); 
            }
        }
        if (r - 1 >= 0 && c - 2 >= 0) { 
            cellsThreatening.emplace_back(g.findCell(r-1, c-2)); 
            if (g.findCell(r-1, c-2)->getPieceType() != PieceType::None) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r-1, c-2); 
            }
        }
        if (r - 2 >= 0 && c + 1 <= 7) { 
            cellsThreatening.emplace_back(g.findCell(r-2, c+1)); 
            if (g.findCell(r-2, c+1)->getPieceType() != PieceType::None) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r-2, c+1); 
            }
        }
        if (r - 2 >= 0 && c - 1 >= 0) { 
            cellsThreatening.emplace_back(g.findCell(r-2, c-1)); 
            if (g.findCell(r-2, c-1)->getPieceType() != PieceType::None) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r-2, c-1); 
            }
        }
        if (r + 2 <= 7 && c + 1 <= 7) { 
            cellsThreatening.emplace_back(g.findCell(r+2, c+1)); 
            if (g.findCell(r+2, c+1)->getPieceType() != PieceType::None) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r+2, c+1); 
            }
        }
        if (r + 2 <= 7 && c - 1 >= 0) { 
            cellsThreatening.emplace_back(g.findCell(r+2, c-1)); 
            if (g.findCell(r+2, c-1)->getPieceType() != PieceType::None) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r+2, c-1); 
            }
        }
    } 
    if (piece == PieceType::Bishop || piece == PieceType::Queen || piece == PieceType::King) {
        // add first one in each direction
        if (r != 0 && c != 0) {
            cellsThreatening.emplace_back(g.findCell(r-1, c-1));
            if (g.findCell(r-1, c-1)->getPieceType() != PieceType::None) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r-1, c-1); 
            }
        }
        if (r != 0 && c != 7) {
            cellsThreatening.emplace_back(g.findCell(r-1, c+1));
            if (g.findCell(r-1, c+1)->getPieceType() != PieceType::None) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r-1, c+1); 
            }
        }
        if (r != 7 && c != 0) {
            cellsThreatening.emplace_back(g.findCell(r+1, c-1));
            if (g.findCell(r+1, c-1)->getPieceType() != PieceType::None) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r+1, c-1); 
            }
        }
        if (r != 7 && c != 7) {
            cellsThreatening.emplace_back(g.findCell(r+1, c+1));
            if (g.findCell(r+1, c+1)->getPieceType() != PieceType::None) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r+1, c+1); 
            }
        }
        if (piece != PieceType::King) {
            currentRow = r + 2;
            currentCol = c + 2;
            while (currentRow <= 7 && currentCol <= 7 && !g.blockCheck2(r, c, currentRow, currentCol)) {
                // add cell to vector
                cellsThreatening.emplace_back(g.findCell(currentRow, currentCol));
                if (g.findCell(currentRow, currentCol)->getPieceType() != PieceType::None) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, currentRow, currentCol); 
                }
                currentRow++;
                currentCol++;
            }

            currentRow = r + 2;
            currentCol = c - 2;
            while (currentRow <= 7 && currentCol >= 0 && !g.blockCheck2(r, c, currentRow, currentCol)) {
                // add cell to vector
                cellsThreatening.emplace_back(g.findCell(currentRow, currentCol));
                if (g.findCell(currentRow, currentCol)->getPieceType() != PieceType::None) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, currentRow, currentCol); 
                }
                currentRow++;
                currentCol--;
            }

            currentRow = r - 2;
            currentCol = c + 2;
            while (currentRow >= 0 && currentCol <= 7 && !g.blockCheck2(r, c, currentRow, currentCol)) {
                // add cell to vector
                cellsThreatening.emplace_back(g.findCell(currentRow, currentCol));
                if (g.findCell(currentRow, currentCol)->getPieceType() != PieceType::None) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, currentRow, currentCol); 
                }
                currentRow--;
                currentCol++;
            }

            currentRow = r - 2;
            currentCol = c - 2;
            while (currentRow >= 0 && currentCol >= 0 && !g.blockCheck2(r, c, currentRow, currentCol)) {
                // add cell to vector
                cellsThreatening.emplace_back(g.findCell(currentRow, currentCol));
                if (g.findCell(currentRow, currentCol)->getPieceType() != PieceType::None) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, currentRow, currentCol); 
                }
                currentRow--;
                currentCol--;
            }
        }
        
    }
    
}

void Piece::updateOpposingPieceThreatStatus(Grid &g, Colour attackingPieceColour, int r, int c) {
    Colour oppositeColour;
    if (attackingPieceColour == Colour::White) {
        oppositeColour = Colour::Black;
    } else {
        oppositeColour = Colour::White;
    }
    for (int i = 0; i < g.getNumOfPiece(oppositeColour); i++) {
        if (g.getPiece(oppositeColour, i)->getRow() == r && g.getPiece(oppositeColour, i)->getCol() == c) {
            g.getPiece(oppositeColour, i)->setThreatStatus(true);
        }
    }
}

bool Piece::checkThreat(int row, int col) { // checks if the cell at row, col is under threat by this piece
    for (auto i : cellsThreatening) {
        if (i->getRow() == row && i->getCol() == col) {
            return true;
        }
    }
    return false;
}
void Piece::updateThreatStatus(Grid &g) { //run after move on every piece that is under attack
    if (pieceColour == Colour::White) {
        for (int i = 0; i < g.getNumOfPiece(pieceColour); i++) {
            if (g.getPiece(Colour::Black, i)->checkThreat(r, c)) {
                underAttack = true;
                return;
            }
        }
    } else {
        for (int i = 0; i < g.getNumOfPiece(pieceColour); i++) {
            if (g.getPiece(Colour::White, i)->checkThreat(r, c)) {
                underAttack = true;
                return;
            }
        }
    }
    underAttack = false;
    return;
}
