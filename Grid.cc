#include <iostream>
#include "Grid.h"
using namespace std;



Grid::Grid(): board{}, player1pieces{}, player2pieces{}, WKpos{nullptr}, BKpos{nullptr}, td{nullptr} {}


Grid::~Grid() {}

void Grid::init() {
    board = std::vector<std::vector<Cell>>(8, std::vector<Cell>(8));
    td = new TextDisplay();
    Colour pieceColour = Colour::White;
    Colour cellColour = Colour::White;
    PieceType piece = PieceType::Rook;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if ((row + col)%2 == 0) {
                cellColour = Colour::Black;
            } else {
                cellColour = Colour::White;
            }

            if (row == 0 || row == 7) { //adds back lines
                if (row == 0) {
                    pieceColour = Colour::White;
                } else {
                    pieceColour = Colour::Black;
                }
                if (col == 1 || col == 6) {
                    piece = PieceType::Bishop;
                } else if (col == 2 || col == 5) {
                    piece = PieceType::Knight;
                } else if (col == 3) {
                    piece = PieceType::Queen;
                } else if (col == 4) {
                    piece = PieceType::King;
                } else if (col == 0 || col == 7) {
                    piece = PieceType::Rook;
                }
                
                board[row][col] = Cell(piece, cellColour, pieceColour, 7 - row, col);
                td->updateTD(board[row][col]);
            } else if (row == 1 || row == 6) { //adds pawn line
                if (row == 1) {
                    pieceColour = Colour::White;
                } else {
                    pieceColour = Colour::Black;
                }
        
                board[row][col] = Cell(PieceType::Pawn, cellColour, pieceColour, 7 - row, col);
                td->updateTD(board[row][col]);
            } else { // fills in board
                
                board[row][col] = Cell(cellColour, 7 - row, col);
            }
            
        }
    }
}

void Grid::add(Colour pieceColour, PieceType piece, int row, int col) {
    board[row][col].add(piece, pieceColour);
    td->updateTD(board[row][col]);
    cout << "piece added" << endl;
}

void Grid::remove(int row, int col) {
    board[row][col].remove();
    td->updateTD(board[row][col]);
}

int Grid::move(int rowA, int colA, int rowB, int colB) {
    PieceType piece = board[rowA][colA].getPieceType();
    Colour colour = board[rowA][colA].getCellColour();
    Colour opColour = Colour::White;
    if (colour == Colour::White) {
        opColour = Colour::Black;
    }
    if (rowA == rowB && colA == colB) { return 3; }
    if(!legalMoveCheck(piece, rowA, colA, rowB, colB)) { return 3; }
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
        if (checkCheckMate(colour)) {
            return 1;
        }
        //TODO - discuss if we want to return smth special for checking/output smth
    }
    return 0;
}

bool Grid::legalMoveCheck(PieceType piece, int rowA, int colA, int rowB, int colB) {
    Colour colour = board[rowA][colA].getCellColour();
    bool castled = false;
    if (movementCheck(piece, colour, rowA, colA, rowB, colB)) {
        return true;
    }
    if (blockCheck(piece, colour, rowA, colA, rowB, colB)) {
        return true;
    }
    //PieceType pieceB = board[rowB][colB].getPieceType();
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
    return false;
}

bool Grid::checkCheck(Colour colour) { return true; } // WRITE

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

    return false;
}
bool Grid::checkCheckMate(Colour colour) { return true; }  // MUST WRITE

bool Grid::blockCheck(PieceType piece, Colour colour, int rowA, int colA, int rowB, int colB) {
    if (piece == PieceType::Knight) {
        //knight can't get blocked unless the spot its going to is occupied by ally
        if (board[rowB][colB].getCellColour() == colour) {
            return true;
        }
    } else if (piece == PieceType::Bishop) {
        for (int i = 0; i < abs(rowA - rowB); i++) {
            rowA++;
            colA++;
            if ((rowA == rowB && board[rowA][colA].getCellColour() == colour) || board[rowA][colA].getPieceType() != PieceType::None) {
                return true;
            }
        }
    } else if (piece == PieceType::Rook) {
        if (colA == colB) { //for vertical movement
            for (int i = 0; i < abs(rowA - rowB); i++) {
                rowA++;
                if ((rowA == rowB && board[rowA][colA].getCellColour() == colour) || board[rowA][colA].getPieceType() != PieceType::None) {
                    return true;
                }
            }
        } else { //for horizontal movement
            for (int i = 0; i < abs(colA - colB); i++) {
                colA++;
                if ((colA == colB && board[rowA][colA].getCellColour() == colour) || board[rowA][colA].getPieceType() != PieceType::None) {
                    return true;
                }
            }
        }
    } else if (piece == PieceType::Queen) {
        if (colA == colB) { //for vertical movement
            for (int i = 0; i < abs(rowA - rowB); i++) {
                rowA++;
                if ((rowA == rowB && board[rowA][colA].getCellColour() == colour) || board[rowA][colA].getPieceType() != PieceType::None) {
                    return true;
                }
            }
        } else if (rowA == rowB) { //for horizontal movement
            for (int i = 0; i < abs(colA - colB); i++) {
                colA++;
                if ((colA == colB && board[rowA][colA].getCellColour() == colour) || board[rowA][colA].getPieceType() != PieceType::None) {
                    return true;
                }
            }
        } else { //for diagonal movement
            for (int i = 0; i < abs(rowA - rowB); i++) {
                rowA++;
                colA++;
                if ((rowA == rowB && board[rowA][colA].getCellColour() == colour) || board[rowA][colA].getPieceType() != PieceType::None) {
                    return true;
                }
            }
        }
    } else if (piece == PieceType::King) {
        if (colA - colB == 2) { //for castling
            if (board[rowA][colA - 1].getPieceType() != PieceType::None) {
                return true;
            } else if (board[rowA][colB].getPieceType() != PieceType::None) {
                return true;
            }
        } else if (colA - colB == -2) {
            if (board[rowA][colA + 1].getPieceType() != PieceType::None) {
                return true;
            } else if (board[rowA][colB].getPieceType() != PieceType::None) {
                return true;
            }
        } else { //for all other movements
            if (board[rowB][colB].getCellColour() == colour) {
                return true;
            }
        }
    } else if (piece == PieceType::Pawn) {
        if (colA != colB) { //for captures
            if (board[rowB][colB].getCellColour() == colour) {
                return true;
            }
        } else { //for regular movement, for loop so that it can also work for double move
            for (int i = 0; i < abs(rowA - rowB); i++) {
                rowA++;
                if (board[rowA][colA].getPieceType() != PieceType::None) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Grid::stalemateCheck(Colour colour) { return true; } // MUST WRITE

void Grid::botMove(int botLevel, Colour colour) { return; }

ostream &operator<<(ostream &out, const Grid &g) {
  return out << *g.td << endl;
}
