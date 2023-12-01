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
                    piece = PieceType::Knight;
                } else if (col == 2 || col == 5) {
                    piece = PieceType::Bishop;
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
    Colour colour = board[rowA][colA].getPieceColour();
    cout << rowA << " " << colA << " " << rowB << " " << colB << endl;
    Colour opColour = Colour::White;
    if (colour == Colour::White) {
        opColour = Colour::Black;
    }
    
    if (rowA == rowB && colA == colB) { return 3; }
    cout << "not same" << endl;
    if(!legalMoveCheck(piece, rowA, colA, rowB, colB)) { return 3; }
    cout << "legal move" << endl;
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
    board[rowA][colA].remove();
    if (checkCheck(opColour)) {
        if (checkCheckMate(colour)) {
            return 1;
        }
        //TODO - discuss if we want to return smth special for checking/output smth
    }
    cout << "opposite side is not in check" << endl;
    td->updateTD(board[rowA][colA]);
    td->updateTD(board[rowB][colB]);
    return 0;
}

bool Grid::legalMoveCheck(PieceType piece, int rowA, int colA, int rowB, int colB) {
    Colour colour = board[rowA][colA].getPieceColour();
    bool castled = false;
    if (!movementCheck(piece, colour, rowA, colA, rowB, colB)) {
        return false;
    }
    cout << "is a valid move" << endl;
    if (blockCheck(piece, colour, rowA, colA, rowB, colB)) {
        return false;
    }
    cout << "is not being blocked" << endl;
    //PieceType pieceB = board[rowB][colB].getPieceType();
    if (piece == PieceType::King) {
        //checking castle
        //TODO - check if theres a check somwhere along the way
        if (colB == 2) {
            //only need to check colour and if the pieces moved or not since columns r the same and rows we keep
            if (colour == Colour::White) {
                if (LWRmoved && WKmoved) { return false; }
            } else {
                if (LBRmoved && BKmoved) { return false; }
            }
            board[rowB][colB].add(PieceType::King, colour);
            board[rowA][colA].remove();
            board[rowA][0].remove();
            board[rowA][3].add(PieceType::Rook, colour);
            castled = true;
        } else if (colB == 6) {
            if (colour == Colour::White) {
                if (LWRmoved && WKmoved) { return false; }
            } else {
                if (LBRmoved && BKmoved) { return false; }
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
    cout << "is not in check" << endl;
    return true;
}

bool Grid::checkCheck(Colour colour) { return false; } // WRITE

bool Grid::movementCheck(PieceType piece, Colour colour, int rowA, int colA, int rowB, int colB) {
    if (piece == PieceType::Rook) { 
        cout << "rook movement" << endl;
        if (rowA == rowB || colA == colB) { return true; } 
    } else if (piece == PieceType::Bishop){
        cout << "bishop movement" << endl;
        if (abs(rowA-rowB) == abs(colA-colB)) {return true;}
    }
	else if (piece == PieceType::Knight) {
        cout << "knight movement" << endl;
        if ((abs(rowA - rowB) == 1 && abs(colA-colB) == 2) || 
        (abs(rowA - rowB) == 2 && abs(colA-colB) == 1)) {return true;}
    }
	else if (piece == PieceType::Queen) {
        cout << "Queen movement" << endl;
        if (abs(rowA-rowB) == abs(colA-colB)) {return true;}
        if (rowA == rowB || colA == colB) { return true; }
    }
	else if (piece == PieceType::King) {
        cout << "king movement" << endl;
        if ((abs(rowA-rowB) == 1 && abs(colA-colB) == 1) || (abs(rowA-rowB) == 1 && abs(colA-colB) == 0) ||
        (abs(rowA-rowB) == 0 && abs(colA-colB) == 1) || (abs(rowA-rowB) == 0 && abs(colA-colB) == 2)) { return true; }
    } else if(piece == PieceType::Pawn) {
        cout << "pawn movement" << endl;
        if ((abs(rowA - rowB) == 1 && abs(colA-colB) == 0) || (abs(rowA-rowB) == 2 && colA-colB == 0 && (rowA == 1 || rowA == 6)) || 
        (abs(rowA-rowB) == 1 && abs(colA-colB) == 1)) {return true;}
    }
    return false;
}
bool Grid::checkCheckMate(Colour colour) { return false; }  // MUST WRITE

bool Grid::blockCheck(PieceType piece, Colour colour, int rowA, int colA, int rowB, int colB) {
    if (piece == PieceType::Knight) {
        //knight can't get blocked unless the spot its going to is occupied by ally
        cout << "knight block check" << endl;
        if (board[rowB][colB].getPieceColour() == colour) {
            return true;
        }
    } else if (piece == PieceType::Bishop) {
        cout << "bishop block check" << endl;
        for (int i = 0; i < abs(rowA - rowB); i++) {
            if (rowA > rowB && colA > colB) {
                rowA--;
                colA--;
            } else if (rowA < rowB && colA < colB) {
                rowA++;
                colA++;
            } else if (rowA < rowB && colA > colB) {
                rowA++;
                colA--;
            } else if (rowA > rowB && colA < colB) {
                rowA--;
                colB++;
            }
            if ((rowA == rowB && board[rowA][colA].getPieceColour() == colour) || board[rowA][colA].getPieceType() != PieceType::None) {
                return true;
            }
        }
    } else if (piece == PieceType::Rook) {
        cout << "rook block check" << endl;
        if (colA == colB) { //for vertical movement
            for (int i = 0; i < abs(rowA - rowB); i++) {
                if (rowA > rowB) {
                    rowA--;
                } else if (rowA < rowB) {
                    rowA++;
                }
                if ((rowA == rowB && board[rowA][colA].getPieceColour() == colour) || board[rowA][colA].getPieceType() != PieceType::None) {
                    return true;
                }
            }
        } else { //for horizontal movement
            for (int i = 0; i < abs(colA - colB); i++) {
                if (colA > colB) {
                    colA--;
                } else if (colA < colB) {
                    colA++;
                }
                if ((colA == colB && board[rowA][colA].getPieceColour() == colour) || board[rowA][colA].getPieceType() != PieceType::None) {
                    return true;
                }
            }
        }
    } else if (piece == PieceType::Queen) {
        cout << "queen block check" << endl;
        if (colA == colB) { //for vertical movement
            for (int i = 0; i < abs(rowA - rowB); i++) {
                if (rowA > rowB) {
                    rowA--;
                } else if (rowA < rowB) {
                    rowA++;
                }
                if ((rowA == rowB && board[rowA][colA].getPieceColour() == colour) || board[rowA][colA].getPieceType() != PieceType::None) {
                    return true;
                }
            }
        } else if (rowA == rowB) { //for horizontal movement
            for (int i = 0; i < abs(colA - colB); i++) {
                if (colA > colB) {
                    colA--;
                } else if (colA < colB) {
                    colA++;
                }
                if ((colA == colB && board[rowA][colA].getPieceColour() == colour) || board[rowA][colA].getPieceType() != PieceType::None) {
                    return true;
                }
            }
        } else { //for diagonal movement
            for (int i = 0; i < abs(rowA - rowB); i++) {
                 if (rowA > rowB && colA > colB) {
                    rowA--;
                    colA--;
                } else if (rowA < rowB && colA < colB) {
                    rowA++;
                    colA++;
                } else if (rowA < rowB && colA > colB) {
                    rowA++;
                    colA--;
                } else if (rowA > rowB && colA < colB) {
                    rowA--;
                    colB++;
                }
               
                if ((rowA == rowB && board[rowA][colA].getPieceColour() == colour) || board[rowA][colA].getPieceType() != PieceType::None) {
                    return true;
                }
            }
        }
    } else if (piece == PieceType::King) {
        cout << "king block check" << endl;
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
            if (board[rowB][colB].getPieceColour() == colour) {
                return true;
            }
        }
    } else if (piece == PieceType::Pawn) {
        cout << "pawn block check" << endl;
        if (colA != colB) { //for captures
            if (board[rowB][colB].getPieceColour() == colour) {
                return true;
            }
        } else { //for regular movement, for loop so that it can also work for double move
            for (int i = 0; i < abs(rowA - rowB); i++) {
                if (rowA > rowB) {
                    rowA--;
                } else if (rowA < rowB) {
                    rowA++;
                }
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
