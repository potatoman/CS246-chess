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
std::vector<Cell*> * Piece::getCellsThreatening() {
    return &cellsThreatening;
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

bool Piece::getAttacking() {
    return attacking;
}

void Piece::updateCellsThreatening(Grid &g) {
    //gets run on every move on the piece that just moved and every rook, bishop, queen both colours
    int currentCol;
    int currentRow;
    bool startedAttacking = false;
    cellsThreatening.clear();
    if (piece == PieceType::Pawn) {
        //threatening top left and top right (bottom if its a black pawn)
        if (pieceColour == Colour::White) {
            if (c != 0) {
                cellsThreatening.emplace_back(g.findCell(r+1, c-1));  
                if (g.findCell(r+1, c-1)->getPieceType() != PieceType::NONE) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, r+1, c-1); 
                    startedAttacking = true;
                }  
            }
            if (c != 7) {
                cellsThreatening.emplace_back(g.findCell(r+1, c+1));
                if (g.findCell(r+1, c+1)->getPieceType() != PieceType::NONE) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, r+1, c+1); 
                    startedAttacking = true;
                }  
            }
        } else {
            if (c != 0) {
                cellsThreatening.emplace_back(g.findCell(r-1, c-1));
                if (g.findCell(r-1, c-1)->getPieceType() != PieceType::NONE) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, r-1, c-1);
                    startedAttacking = true;
                }  
            }
            if (c != 7) {
                cellsThreatening.emplace_back(g.findCell(r-1, c+1));
                if (g.findCell(r-1, c+1)->getPieceType() != PieceType::NONE) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, r-1, c+1);
                    startedAttacking = true;
                }  
            }
        }
    //put rook queen and king movement together for the squares immediately around it
    //then filter out the king
    } else if (piece == PieceType::Rook || piece == PieceType::Queen || piece == PieceType::King) { 
        //add pieces immediately around it (that are valid) since it will always be threatening at least those
            if (r != 0) {
                cellsThreatening.emplace_back(g.findCell(r-1, c));
                if (g.findCell(r-1, c)->getPieceType() != PieceType::NONE) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, r-1, c);
                    startedAttacking = true;
                }
            }
            if (r != 7) {
                cellsThreatening.emplace_back(g.findCell(r+1, c));
                if (g.findCell(r+1, c)->getPieceType() != PieceType::NONE) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, r+1, c);
                    startedAttacking = true;
                }
            }
            if (c != 0) {
                cellsThreatening.emplace_back(g.findCell(r, c-1));
                if (g.findCell(r, c-1)->getPieceType() != PieceType::NONE) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, r, c-1);
                    startedAttacking = true;
                }
            }
            if (c != 7) {
                cellsThreatening.emplace_back(g.findCell(r, c+1));
                if (g.findCell(r, c+1)->getPieceType() != PieceType::NONE) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, r, c+1);
                    startedAttacking = true;
                }
            }
            if (piece != PieceType::King) {
                currentRow = r;
                currentCol = c;
                currentCol -=2;
                //checks pieces to the left
                while (currentCol >= 0 && !g.blockCheck2(r, c, r, currentCol)) {
                    // add cell at [r][currentCol] to the vector
                    cellsThreatening.emplace_back(g.findCell(r, currentCol));
                    if (g.findCell(r, currentCol)->getPieceType() != PieceType::NONE) { 
                        updateOpposingPieceThreatStatus(g, pieceColour, r, currentCol);
                        startedAttacking = true;
                    }
                    currentCol-=1;
                }
                currentCol = c + 2;
                //checks pieces to the right
                while (currentCol <= 7 && !g.blockCheck2(r, c, r, currentCol)) {
                    // add cell at [r][currentCol] to the vector
                    cellsThreatening.emplace_back(g.findCell(r, currentCol));
                    if (g.findCell(r, currentCol)->getPieceType() != PieceType::NONE) { 
                        updateOpposingPieceThreatStatus(g, pieceColour, r, currentCol);
                        startedAttacking = true;
                    }
                    currentCol+=1;
                }
                currentCol = c;
                currentRow = r + 2;
                //checks pieces above
                while (currentRow <= 7 && !g.blockCheck2(r, c, currentRow, c)) {
                    // add cell at [currentRow][c] to the vector
                    cellsThreatening.emplace_back(g.findCell(currentRow, c));
                    if (g.findCell(currentRow, c)->getPieceType() != PieceType::NONE) { 
                        updateOpposingPieceThreatStatus(g, pieceColour, currentRow, c);
                        startedAttacking = true;
                    }
                    currentRow+=1;
                }
                currentRow = r - 2;
                //checks pieces below
                while (currentRow >= 0 && !g.blockCheck2(r, c, currentRow, c)) {
                    // add cell at [currentRow][c] to the vector
                    cellsThreatening.emplace_back(g.findCell(currentRow, c));
                    if (g.findCell(currentRow, c)->getPieceType() != PieceType::NONE) { 
                        updateOpposingPieceThreatStatus(g, pieceColour, currentRow, c);
                        startedAttacking = true;
                    }
                    currentRow-=1;
                }
        }
       
    } else if (piece == PieceType::Knight) {
        //knight is always threatening its squares unless they are not on the board
        if (r + 1 <= 7 && c + 2 <= 7) { 
            cellsThreatening.emplace_back(g.findCell(r+1, c+2)); 
            if (g.findCell(r+1, c+2)->getPieceType() != PieceType::NONE) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r+1, c+2);
                startedAttacking = true;
            }
        }
        if (r + 1 <= 7 && c - 2 >= 0) { 
            cellsThreatening.emplace_back(g.findCell(r+1, c-2)); 
            if (g.findCell(r+1, c-2)->getPieceType() != PieceType::NONE) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r+1, c-2);
                startedAttacking = true;
            }
        }
        if (r - 1 >= 0 && c + 2 <= 7) { 
            cellsThreatening.emplace_back(g.findCell(r-1, c+2)); 
            if (g.findCell(r-1, c+2)->getPieceType() != PieceType::NONE) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r-1, c+2);
                startedAttacking = true;
            }
        }
        if (r - 1 >= 0 && c - 2 >= 0) { 
            cellsThreatening.emplace_back(g.findCell(r-1, c-2)); 
            if (g.findCell(r-1, c-2)->getPieceType() != PieceType::NONE) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r-1, c-2);
                startedAttacking = true;
            }
        }
        if (r - 2 >= 0 && c + 1 <= 7) { 
            cellsThreatening.emplace_back(g.findCell(r-2, c+1)); 
            if (g.findCell(r-2, c+1)->getPieceType() != PieceType::NONE) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r-2, c+1);
                startedAttacking = true;
            }
        }
        if (r - 2 >= 0 && c - 1 >= 0) { 
            cellsThreatening.emplace_back(g.findCell(r-2, c-1)); 
            if (g.findCell(r-2, c-1)->getPieceType() != PieceType::NONE) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r-2, c-1);
                startedAttacking = true;
            }
        }
        if (r + 2 <= 7 && c + 1 <= 7) { 
            cellsThreatening.emplace_back(g.findCell(r+2, c+1)); 
            if (g.findCell(r+2, c+1)->getPieceType() != PieceType::NONE) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r+2, c+1);
                startedAttacking = true;
            }
        }
        if (r + 2 <= 7 && c - 1 >= 0) { 
            cellsThreatening.emplace_back(g.findCell(r+2, c-1)); 
            if (g.findCell(r+2, c-1)->getPieceType() != PieceType::NONE) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r+2, c-1);
                startedAttacking = true;
            }
        }
    } 
    if (piece == PieceType::Bishop || piece == PieceType::Queen || piece == PieceType::King) {
        // add first one in each direction
        if (r != 0 && c != 0) {
            cellsThreatening.emplace_back(g.findCell(r-1, c-1));
            if (g.findCell(r-1, c-1)->getPieceType() != PieceType::NONE) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r-1, c-1);
                startedAttacking = true;
            }
        }
        if (r != 0 && c != 7) {
            cellsThreatening.emplace_back(g.findCell(r-1, c+1));
            if (g.findCell(r-1, c+1)->getPieceType() != PieceType::NONE) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r-1, c+1);
                startedAttacking = true;
            }
        }
        if (r != 7 && c != 0) {
            cellsThreatening.emplace_back(g.findCell(r+1, c-1));
            if (g.findCell(r+1, c-1)->getPieceType() != PieceType::NONE) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r+1, c-1);
                startedAttacking = true;
            }
        }
        if (r != 7 && c != 7) {
            cellsThreatening.emplace_back(g.findCell(r+1, c+1));
            if (g.findCell(r+1, c+1)->getPieceType() != PieceType::NONE) { 
                updateOpposingPieceThreatStatus(g, pieceColour, r+1, c+1);
                startedAttacking = true;
            }
        }
        if (piece != PieceType::King) {
            currentRow = r + 2;
            currentCol = c + 2;
            while (currentRow <= 7 && currentCol <= 7 && !g.blockCheck2(r, c, currentRow, currentCol)) {
                //add top right
                cellsThreatening.emplace_back(g.findCell(currentRow, currentCol));
                if (g.findCell(currentRow, currentCol)->getPieceType() != PieceType::NONE) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, currentRow, currentCol);
                    startedAttacking = true;
                }
                currentRow++;
                currentCol++;
            }

            currentRow = r + 2;
            currentCol = c - 2;
            while (currentRow <= 7 && currentCol >= 0 && !g.blockCheck2(r, c, currentRow, currentCol)) {
                // add top left
                cellsThreatening.emplace_back(g.findCell(currentRow, currentCol));
                if (g.findCell(currentRow, currentCol)->getPieceType() != PieceType::NONE) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, currentRow, currentCol);
                    startedAttacking = true;
                }
                currentRow++;
                currentCol--;
            }

            currentRow = r - 2;
            currentCol = c + 2;
            while (currentRow >= 0 && currentCol <= 7 && !g.blockCheck2(r, c, currentRow, currentCol)) {
                // add bottom right
                cellsThreatening.emplace_back(g.findCell(currentRow, currentCol));
                if (g.findCell(currentRow, currentCol)->getPieceType() != PieceType::NONE) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, currentRow, currentCol);
                    startedAttacking = true;
                }
                currentRow--;
                currentCol++;
            }

            currentRow = r - 2;
            currentCol = c - 2;
            while (currentRow >= 0 && currentCol >= 0 && !g.blockCheck2(r, c, currentRow, currentCol)) {
                // add left
                cellsThreatening.emplace_back(g.findCell(currentRow, currentCol));
                if (g.findCell(currentRow, currentCol)->getPieceType() != PieceType::NONE) { 
                    updateOpposingPieceThreatStatus(g, pieceColour, currentRow, currentCol);
                    startedAttacking = true;
                }
                currentRow--;
                currentCol--;
            }
        }
        
    }
    if (!startedAttacking) {
        attacking = false;
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
            attacking = true;
        }
    }
}

bool Piece::checkThreat(int row, int col) {
std::cout << "new call" << std::endl;
    for (auto &i : cellsThreatening) {
        std::cout << i << std::endl;
        if (i->getRow() == row && i->getCol() == col) {
            return true;
        }
    }
    return false;
}
void Piece::updateThreatStatus(Grid &g) {
    if (pieceColour == Colour::White) {
        for (int i = 0; i < g.getNumOfPiece(pieceColour); i++) {
            std::cout << g.getNumOfPiece(pieceColour);
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
