#include "Grid.h"
using namespace std;



Grid::Grid(): board{}, WhitePieces{}, BlackPieces{}, WKpos{nullptr}, BKpos{nullptr}, td{nullptr} {}


Grid::~Grid() {}


void Grid::init() {
    board = std::vector<std::vector<Cell>>(8, std::vector<Cell>(8));
    td = new TextDisplay();
    gd = new GraphicsDisplay{x};
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
                board[row][col] = Cell(piece, cellColour, pieceColour, row, col);
                if (pieceColour == Colour::White) {
                    WhitePieces.emplace_back(Piece{piece, pieceColour, row, col});
                } else {
                    BlackPieces.emplace_back(Piece{piece, pieceColour, row, col});
                }
                td->updateTD(board[row][col]);
                gd->notify(board[row][col]);
            } else if (row == 1 || row == 6) { //adds pawn line
                piece = PieceType::Pawn;
                if (row == 1) {
                    pieceColour = Colour::White;
                } else {
                    pieceColour = Colour::Black;
                }
                board[row][col] = Cell(piece, cellColour, pieceColour, row, col);
                if (pieceColour == Colour::White) {
                    WhitePieces.emplace_back(Piece{piece, pieceColour, row, col});
                } else {
                    BlackPieces.emplace_back(Piece{piece, pieceColour, row, col});
                }
                td->updateTD(board[row][col]);
                gd->notify(board[row][col]);
            } else { // fills in board
                board[row][col] = Cell(cellColour, row, col);
                gd->notify(board[row][col]);
            }
            
        }
    }
}

Cell* Grid::findCell(int r, int c) {
    return &board[r][c];
}

void Grid::add(Colour pieceColour, int rowA, int colA, int rowB, int colB) {
    if (board[rowB][colB].getPieceType() != PieceType::NONE) { this->removePieceFromVector(rowB, colB); }
    PieceType pieceType = getPiece(pieceColour, rowA, colA)->getPieceType();
    getPiece(pieceColour, rowA, colA)->updateCoords(rowB, colB);
    //cout << "we got to here 4" << endl;
    //getPiece(pieceColour, rowA, colB)->setRow(rowB);
    //cout << "we got to here 5" << endl;
    board[rowB][colB].add(pieceType, pieceColour);
    board[rowA][colA].remove();
    
}

void Grid::add(Colour pieceColour, PieceType piece, int row, int col) {
    board[row][col].add(piece, pieceColour);
    if (board[row][col].getPieceType() != PieceType::NONE) { this->removePieceFromVector(row, col); }
    this->addPieceToVector(pieceColour, piece, row, col);
    td->updateTD(board[row][col]);
    gd->notify(board[row][col]);
    cout << "piece added" << endl;
    // add to pieces vector
}

void Grid::removePieceFromVector(int row, int col) {
    int pos = 0;
    for (auto i : WhitePieces) {
        if (i.getRow() == row && i.getCol() == col) {
            WhitePieces.erase(WhitePieces.begin() + pos);
            return;
        }
        pos++;
    }
    pos = 0;
    for (auto i : BlackPieces) {
        if (i.getRow() == row && i.getCol() == col) {
            BlackPieces.erase(BlackPieces.begin() + pos);
            return;
        }
        pos++;
    }
}

void Grid::addPieceToVector(Colour pieceColour, PieceType piece, int row, int col) {
    if (pieceColour == Colour::White) {
        WhitePieces.emplace_back(Piece{piece, pieceColour, 7 - row, col});
    } else {
        BlackPieces.emplace_back(Piece{piece, pieceColour, 7 - row, col});
    }

}

void Grid::addPieceToVector(Piece piece) {
    if (piece.getPieceColour() == Colour::White) {
        WhitePieces.emplace_back(piece);
    } else {
        BlackPieces.emplace_back(piece);
    }
}



void Grid::remove(int row, int col) {
    board[row][col].remove();
    this->removePieceFromVector(row, col);
    td->updateTD(board[row][col]);
    gd->notify(board[row][col]);
    // remove from pieces vector
}

int Grid::move(int rowA, int colA, int rowB, int colB) {
    // update pieces locations and potentially remove from pieces vector if theres a capture
    PieceType piece = board[rowA][colA].getPieceType();
    Colour colour = board[rowA][colA].getPieceColour();
    //cout << rowA << " " << colA << " " << rowB << " " << colB << endl;
    Colour opColour = Colour::White;
    if (colour == Colour::White) {
        opColour = Colour::Black;
    }
    
    if (rowA == rowB && colA == colB) { 
        cout << "you are trying to move to the square you are already in" << endl;
        return 3; 
    }
    
    if(!legalMoveCheck(piece, rowA, colA, rowB, colB)) { 
        cout << "not a legal move" << endl;
        return 3; 
    }
    
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
    
    this->add(colour, rowA, colA, rowB, colB);

    this->updateAllThreats(rowB, colB);
    td->updateTD(board[rowA][colA]);
    gd->notify(board[rowA][colA]);
    td->updateTD(board[rowB][colB]);
    gd->notify(board[rowA][colB]);

    if (checkCheck(opColour)) {
        cout << "opponent is in check" << endl;
        if (checkCheckMate(opColour)) {
            return 1;
        }
        //TODO - discuss if we want to return smth special for checking/output smth
    }
    //cout << "opposite side is not in check" << endl;
    return 0;
}


bool Grid::legalMoveCheck(PieceType piece, int rowA, int colA, int rowB, int colB) {
    Piece taken;
    bool capture = false;
    if (board[rowB][colB].getPieceType() != PieceType::NONE) {
        taken = Piece{board[rowB][colB].getPieceType(), board[rowB][colB].getPieceColour(), rowB, colB};
        capture = true;
    }
    Colour colour = board[rowA][colA].getPieceColour();


    bool castled = false;
    if (board[rowB][colB].getPieceColour() == board[rowA][colA].getPieceColour()) {
        cout << "same piece colour at destination" << endl;
        return false;
    }


    if (!movementCheck(piece, colour, rowA, colA, rowB, colB)) {
        cout << "the piece you are trying to move cannot move like that" << endl;
        return false;
    }

    if (piece != PieceType::Knight) {
        if (blockCheck2(rowA, colA, rowB, colB)) {
            cout << "there is a piece blocking the path" << endl;
            return false;
        }
    }

    

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
            this->add(colour, rowA, colA, rowB, colB);
            board[rowA][colA].remove();
            this->add(colour, rowA, 0, rowB, 3);
            board[rowA][0].remove();
            castled = true;
        } else if (colB == 6) {
            if (colour == Colour::White) {
                if (LWRmoved && WKmoved) { return false; }
            } else {
                if (LBRmoved && BKmoved) { return false; }
            }
            this->add(colour, rowA, colA, rowB, colB);
            board[rowA][colA].remove();
            this->add(colour, rowA, 7, rowB, 5);
            board[rowA][7].remove();
            castled = true;
        } else {
            if (colour == Colour::White) {
                WKmoved = true;
            } else {
                BKmoved = true;
            }
            this->add(colour, rowA, colA, rowB, colB);
            board[rowA][colA].remove();
        }
        WKpos = &board[rowB][colB];
    } else {
        
        this->add(colour, rowA, colA, rowB, colB);
    }

    
    this->updateAllThreats(rowB, colB);
    if (checkCheck(colour)) {
        if (castled) {
            if (colB == 6) {
                this->add(colour, rowA, 5, rowB, 7);
                board[rowA][5].remove();
            } else {
                this->add(colour, rowA, 3, rowB, 0);
                board[rowA][3].remove();
            }
        }
        this->add(colour, rowB, colB, rowA, colA);
        if (capture) {
            board[rowB][colB].add(taken.getPieceType(), taken.getPieceColour());
            this->addPieceToVector(taken);
            updateAllThreats(rowB, colB);
        }
        
        updateAllThreats(rowA, colA);
        
        cout << "this move puts you in check so it is not legal" << endl;
        return false;
    }


    this->add(colour, rowB, colB, rowA, colA);
    if (capture) {
        board[rowB][colB].add(taken.getPieceType(), taken.getPieceColour());
        this->addPieceToVector(taken);
        updateAllThreats(rowB, colB);
    }
    updateAllThreats(rowA, colA);
    return true;
}

void Grid::updateAllThreats(int row, int col) {
    int length = WhitePieces.size();
    for (int i = 0; i < length; i++) {
        if ((WhitePieces[i].getCol() == col && WhitePieces[i].getRow() == row) || WhitePieces[i].getPieceType() == PieceType::Bishop ||
        WhitePieces[i].getPieceType() == PieceType::Rook || WhitePieces[i].getPieceType() == PieceType::Queen) {
            WhitePieces[i].updateCellsThreatening(*this);
        }
        if (WhitePieces[i].getThreatStatus() == true) {
            WhitePieces[i].updateThreatStatus(*this);
        }
    }
    length = BlackPieces.size();
    for (int i = 0; i < length; i++) {
        if ((BlackPieces[i].getCol() == col && BlackPieces[i].getRow() == row) || BlackPieces[i].getPieceType() == PieceType::Bishop ||
        BlackPieces[i].getPieceType() == PieceType::Rook || BlackPieces[i].getPieceType() == PieceType::Queen) {
            BlackPieces[i].updateCellsThreatening(*this);
        }
        if (BlackPieces[i].getThreatStatus() == true) {
            BlackPieces[i].updateThreatStatus(*this);
        }
    }
}

bool Grid::checkCheck(Colour colour) {  // return true if king is in check
    if (colour == Colour::White) {
        for (auto i : WhitePieces) {
            if (i.getPieceType() == PieceType::King) {
                if (i.getThreatStatus() == true) {
                    return true;
                } 
                return false;
            }
        }
    }
    for (auto i : BlackPieces) {
        if (i.getPieceType() == PieceType::King) {
            if (i.getThreatStatus() == true) {
                return true;
            } 
            return false;
        }
    }
    // check if king has underattack boolean set to true
    return false; 
}

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

bool Grid::blockCheck2(int rowA, int colA, int rowB, int colB) { // returns false if theres nothing blocking the move (no piece in between the coordinates)
    cout << "called block check, rowA: " << rowA << " colA: " << colA << " rowB: " << rowB << " colB: " << colB << endl;
    int rowIncrement;
    int colIncrement;
    
    if (rowA == rowB) {
        rowIncrement = 0;
    } else {
        rowIncrement = (rowB - rowA)/(abs(rowB - rowA));
    }
    if (colA == colB) {
        colIncrement = 0;
    } else {
        colIncrement = (colB - colA)/(abs(colB - colA));
    }

    int rowStart = rowA + rowIncrement;
    int colStart = colA + colIncrement;

    while(rowStart != rowB || colStart != colB) {
        if (board[rowStart][colStart].getPieceType() != PieceType::NONE) {
            cout << "piece in the way at row" << rowStart << " and column: " << colStart  << endl;
            return true;
        }
        rowStart += rowIncrement;
        colStart += colIncrement;
    }
    
    cout << "nothing in the way" << endl;
    return false;
}

bool Grid::blockCheck(PieceType piece, Colour colour, int rowA, int colA, int rowB, int colB) {
    int numOfSquaresRows = abs(rowA - rowB);
    int numOfSquaresCols = abs(colA - colB);
    if (piece == PieceType::Knight) {
        //knight can't get blocked unless the spot its going to is occupied by ally
        cout << "knight block check" << endl;
        if (board[rowB][colB].getPieceColour() == colour) {
            return true;
        }
    } else if (piece == PieceType::Bishop) {
        cout << "bishop block check" << endl;
        for (int i = 0; i < numOfSquaresRows; i++) {
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
            if ((rowA == rowB && board[rowA][colA].getPieceColour() == colour) || board[rowA][colA].getPieceType() != PieceType::NONE) {
                return true;
            }
        }
    } else if (piece == PieceType::Rook) {
        cout << "rook block check" << endl;
        if (colA == colB) { //for vertical movement
            for (int i = 0; i < numOfSquaresRows; i++) {
                if (rowA > rowB) {
                    rowA--;
                } else if (rowA < rowB) {
                    rowA++;
                }
                if ((rowA == rowB && board[rowA][colA].getPieceColour() == colour) || board[rowA][colA].getPieceType() != PieceType::NONE) {
                    return true;
                }
            }
        } else { //for horizontal movement
            for (int i = 0; i < numOfSquaresCols; i++) {
                if (colA > colB) {
                    colA--;
                } else if (colA < colB) {
                    colA++;
                }
                if ((colA == colB && board[rowA][colA].getPieceColour() == colour) || board[rowA][colA].getPieceType() != PieceType::NONE) {
                    return true;
                }
            }
        }
    } else if (piece == PieceType::Queen) {
        cout << "queen block check" << endl;
        if (colA == colB) { //for vertical movement
            for (int i = 0; i < numOfSquaresRows; i++) {
                if (rowA > rowB) {
                    rowA--;
                } else if (rowA < rowB) {
                    rowA++;
                }
                if ((rowA == rowB && board[rowA][colA].getPieceColour() == colour) || board[rowA][colA].getPieceType() != PieceType::NONE) {
                    return true;
                }
            }
        } else if (rowA == rowB) { //for horizontal movement
            for (int i = 0; i < numOfSquaresCols; i++) {
                if (colA > colB) {
                    colA--;
                } else if (colA < colB) {
                    colA++;
                }
                if ((colA == colB && board[rowA][colA].getPieceColour() == colour) || board[rowA][colA].getPieceType() != PieceType::NONE) {
                    return true;
                }
            }
        } else { //for diagonal movement
            for (int i = 0; i < numOfSquaresRows; i++) {
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
               
                if ((rowA == rowB && board[rowA][colA].getPieceColour() == colour) || board[rowA][colA].getPieceType() != PieceType::NONE) {
                    return true;
                }
            }
        }
    } else if (piece == PieceType::King) {
        cout << "king block check" << endl;
        if (colA - colB == 2) { //for castling
            if (board[rowA][colA - 1].getPieceType() != PieceType::NONE) {
                return true;
            } else if (board[rowA][colB].getPieceType() != PieceType::NONE) {
                return true;
            }
        } else if (colA - colB == -2) {
            if (board[rowA][colA + 1].getPieceType() != PieceType::NONE) {
                return true;
            } else if (board[rowA][colB].getPieceType() != PieceType::NONE) {
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
            for (int i = 0; i < numOfSquaresRows; i++) {
                if (rowA > rowB) {
                    rowA--;
                } else if (rowA < rowB) {
                    rowA++;
                }
                if (board[rowA][colA].getPieceType() != PieceType::NONE) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Grid::stalemateCheck(Colour colour) { return true; } // MUST WRITE

void Grid::botMove(int botLevel, Colour colour) { return; }

int Grid::getNumOfPiece(Colour pieceColour) {
    if (pieceColour == Colour::White) {
        return WhitePieces.size();
    }
    return BlackPieces.size();
}

Piece* Grid::getPiece(Colour pieceColour, int index) {
    if (pieceColour == Colour::White) {
        return &WhitePieces[index];
    }
    return &BlackPieces[index];
}

Piece* Grid::getPiece(Colour pieceColour, int r, int c) {
    int length = WhitePieces.size();
    cout << length << endl;
    cout << "got here 1" << endl;
    if (pieceColour == Colour::White) {
        for (int i = 0; i < length - 1; i++) {
            if (WhitePieces[i].getCol() == c && WhitePieces[i].getRow() == r) {
                cout << "got here 2" << endl;
                return &WhitePieces[i];
            }
        }
    } else {
        cout << "got here why??" << endl;
        length = BlackPieces.size();
        cout << length << endl;
        for (int i = 0; i < length - 1; i++) {
            if (BlackPieces[i].getCol() == c && BlackPieces[i].getRow() == r) {
                return &BlackPieces[i];
            }
        }
    }
}

ostream &operator<<(ostream &out, const Grid &g) {
  return out << *g.td << endl;
}
