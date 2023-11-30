#include "Grid.h"

void Grid::init() {
    Colour colour = Colour::White;
    PieceType piece = PieceType::Rook;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (row == 0 || 7) { //adds back lines
                if (row == 0) {
                    colour = Colour::Black;
                } else {
                    colour = Colour::White;
                }
                if (col == 1 || col == 6) {
                        piece = PieceType::Bishop;
                    } else if (col == 2 || col == 5) {
                        piece = PieceType::Knight;
                    } else if (col == 3) {
                        piece = PieceType::Queen;
                    } else if (col == 4) {
                        piece = PieceType::King;
                    }
                board[row][col] = Cell(piece, colour, 8 - row, col);
            } else if (row == 1 || 6) { //adds pawn line
                if (row == 1) {
                    colour = Colour::Black;
                } else {
                    colour = Colour::White;
                }
                board[row][col] = Cell(PieceType::Pawn, colour, 8 - row, col);
            } else { // fills in board
                board[row][col] = Cell();
            }
            
        }
    }
}

void Grid::add(Colour colour, PieceType piece, int row, int col) {
    board[row][col].add(piece, colour);
}

void Grid::remove(int row, int col) {
    board[row][col].remove();
}

int Grid::move(int rowA, int colA, int rowB, int colB) {
    PieceType piece = board[rowA][colA].getPieceType();
    Colour colour = board[rowA][colA].getColour();
    Colour opColour = Colour::White;
    if (colour == Colour::White) {
        opColour = Colour::Black;
    }
    if (rowA == rowB && colA == colB) { return; }
    if(!legalMoveCheck(piece, rowA, colA, rowB, colB)) { return; }
    //TODO - what does the Wking || Bking check do?
    if (piece == PieceType::Rook && (!LWRmoved || !RWRmoved || !LBRmoved || !RBRmoved)) {
        if (rowA == 0) {
            if (colA == 0) {
                RWRmoved = true;
            } else {
                LWRmoved = true;
            }
        } else {
            if (colA == 0) {
                RBRmoved = true;
            } else {
                LBRmoved = true;
            }
        }
    }
    //TODO - promotioncheck()
    board[rowB][colB].add(piece, colour);
    board[rowA][rowA].remove();
    if (checkCheck(opColour)) {
        if (checkcheckMate(colour)) {
            return 1;
        }
        //TODO - discuss if we want to return smth special for checking/output smth
    }
    return 0;
}

bool Grid::legalMoveCheck(PieceType piece, int rowA, int colA, int rowB, int colB) {
    Colour colour = board[rowA][colA].getColour();
    bool castled = false;
    if (movementCheck(piece, colour, rowA, colA, rowB, colB)) {
        return true;
    }
    if (blockCheck()) {
        return true;
    }
    PieceType pieceB = board[rowB][colB].getPieceType();
    if (piece == PieceType::King) {
        //checking castle
        //TODO - check if theres a check somwhere along the way
        if (colB == 2) {
            //only need to check colour and if the pieces moved or not since columns r the same and rows we keep
            if (colour == Colour::White) {
                if (LWRmoved && WKmoved) { return true; }
            } else {
                if (LBRmoved && BKmoved) { return true; }
            }
            board[rowB][colB].add(PieceType::King, colour);
            board[rowA][colA].remove();
            board[rowA][0].remove();
            board[rowA][3].add(PieceType::Rook, colour);
            castled = true;
        } else if (colB == 6) {
            if (colour == Colour::White) {
                if (LWRmoved && WKmoved) { return true; }
            } else {
                if (LBRmoved && BKmoved) { return true; }
            }
            board[rowB][colB].add(PieceType::King, colour);
            board[rowA][colA].remove();
            board[rowA][7].remove();
            board[rowA][5].add(PieceType::Rook, colour);
            castled = true;
        } else {
            if (colour == Colour::White) {
                WKmoved = true;
            } else {
                BKmoved = true;
            }
            board[rowA][colA].remove();
            board[rowB][colB].add(piece, colour);
        }
        WKpos = &board[rowB][colB];
    } else {
        board[rowB][colB].add(piece, colour);
        board[rowA][colA].remove();
    }
    if (checkCheck(colour)) {
        if (castled) {
            if (colB == 6) {
                board[rowA][5].remove();
                board[rowA][7].add(PieceType::Rook, colour);
            } else {
                board[rowA][3].remove();
                board[rowA][0].add(PieceType::Rook, colour);
            }
        }
        board[rowB][colB].remove();
        board[rowA][colB].add(piece, colour);
    }
}
bool Grid::checkCheck(Colour colour) {

}
bool Grid::movementCheck(PieceType piece, Colour colour, int rowA, int colA, int rowB, int colB) {
    if (piece == PieceType::Rook) { 
        if (rowA == rowB || colA == colB) { return true; } 
    } else if (piece == PieceType::Bishop){
        if (abs(rowA-rowB) == abs(colA-colB)) {return true;}
    }
	else if (piece == PieceType::Knight) {
        if ((abs(rowA - rowB) == 1 && abs(colA-colB) == 2) || 
        (abs(rowA - rowB) == 2 && abs(colA-colB) == 2)) {return true;}
    }
	else if (piece == PieceType::Queen) {
        if (abs(rowA-rowB) == abs(colA-colB)) {return true;}
        if (rowA == rowB || colA == colB) { return true; }
    }
	else if (piece == PieceType::King) {
        if ((abs(rowA-rowB) == 1 && abs(colA-colB) == 1) || (abs(rowA-rowB) == 1 && abs(colA-colB) == 0) ||
        (abs(rowA-rowB) == 0 && abs(colA-colB) == 1) || (abs(rowA-rowB) == 0 && abs(colA-colB) == 2)) { return true; }
    }
    else if(piece == PieceType::Pawn) {
        if ((abs(rowA - rowB) == 0 && abs(colA-colB) == 1) || (abs(rowA-rowB) == 1 && colA-colB == 0) || 
        (abs(rowA-rowB) == 1 && abs(colA-colB) == 1)) {return true;}
    }


}
bool Grid::checkCheckMate(Colour colour) {

}

bool Grid::blockCheck() {

}

void Grid::botMove(int botLevel, Colour colour) {}