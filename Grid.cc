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

            if (row == 0 || row == 7) { 
                //adds back lines
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
            } else if (row == 1 || row == 6) { 
                //adds pawn lines
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
            } else { 
                // fills in board with empty cells
                board[row][col] = Cell(cellColour, row, col);
                gd->notify(board[row][col]);
            }
            
        }
    }
    updateAllThreats();
}

Cell* Grid::findCell(int r, int c) {
    return &board[r][c];
}
std::vector<Piece> * Grid::getPiecesArray(Colour colour) {
    if (colour == Colour::White) {
        return &WhitePieces;
    } else {
        return &BlackPieces;
    }
}

void Grid::add(Colour pieceColour, int rowA, int colA, int rowB, int colB) { 
    if (board[rowB][colB].getPieceType() != PieceType::NONE) { this->removePieceFromVector(rowB, colB); }
    //cout << "check 1.1" << endl;
    //cout << "calling getPiece on row, col " << rowA << " " << colA << endl;
    PieceType pieceType = getPiece(pieceColour, rowA, colA)->getPieceType();
    //cout << "check 1.2" << endl;
    getPiece(pieceColour, rowA, colA)->updateCoords(rowB, colB);
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
}

int Grid::move(int rowA, int colA, int rowB, int colB) {
    PieceType piece = board[rowA][colA].getPieceType();
    Colour colour = board[rowA][colA].getPieceColour();
    Colour opColour = Colour::White;
    if (colour == Colour::White) {
        opColour = Colour::Black;
    }
    //makes sure that there is no attempt to move a piece on itself
    if (rowA == rowB && colA == colB) { 
        cout << "you are trying to move to the square you are already in" << endl;
        return 3; 
    }
    //checks if the requested move is legal for that piece
    if(!legalMoveCheck(piece, rowA, colA, rowB, colB)) { 
        cout << "not a legal move" << endl;
        return 3; 
    }
    
    //TODO - what does the Wking || Bking check do?
    //updates if rooks moved or not to allow for a castling check shortcut
    if (piece == PieceType::Rook && (!LWRmoved || !RWRmoved || !LBRmoved || !RBRmoved)) {
        if (rowA == 0 && colA == 0) { LWRmoved = true; }
        if (rowA == 0 && colA == 7) { RWRmoved = true; }
        if (rowA == 7 && colA == 0) { LBRmoved = true; }
        if (rowA == 7 && colA == 7) { RBRmoved = true; }
    }

    if (piece == PieceType::King && (!WKmoved || !BKmoved)) {
        if (colour == Colour::White) {
            WKmoved = true;
        }
        if (colour == Colour::Black) {
            BKmoved = true;
        }
    }

    //TODO - promotioncheck()
    
    this->add(colour, rowA, colA, rowB, colB);

    if (piece == PieceType::King && (abs(colB - colA) == 2)) {
        if (colB == 2) {
            cout << "is it even callling this" << endl;
            this->add(colour, rowA, 0, rowB, 3);
            this->updateAllThreats(rowB, 3);
            td->updateTD(board[rowA][0]);
            gd->notify(board[rowA][0]);
            td->updateTD(board[rowB][3]);
            gd->notify(board[rowB][3]);
        } else if (colB == 6) {
            cout << "is it even callling this??" << endl;
            this->add(colour, rowA, 7, rowB, 5);
            this->updateAllThreats(rowB, 5);
            td->updateTD(board[rowA][7]);
            gd->notify(board[rowA][7]);
            td->updateTD(board[rowB][5]);
            gd->notify(board[rowB][5]);
        }
    }

    this->updateAllThreats(rowB, colB);
    td->updateTD(board[rowA][colA]);
    gd->notify(board[rowA][colA]);
    td->updateTD(board[rowB][colB]);
    gd->notify(board[rowB][colB]);

    if (checkCheck(opColour)) {
        cout << "opponent is in check" << endl;
        if (checkCheckMate(opColour)) {
            cout << "you've checkmated your opponent" << endl;
            return 1;
        }
        //TODO - discuss if we want to return smth special for checking/output smth
    } 
    if (stalemateCheck(opColour)) {
        cout << "you've reached a stalemate" << endl;
        return 2;
    }
    randomize(colour);
    return 0;
}


bool Grid::legalMoveCheck(PieceType piece, int rowA, int colA, int rowB, int colB) {
    if (rowA < 0 || rowA > 7 || colA < 0 || colA > 7 || rowB < 0 || rowB > 7 || colB < 0 || colB > 7) { return false; }
    Piece taken;
    bool capture = false;
    //stores a taken piece if a piece was taken so that we can put it back after we simulate the move
    if (board[rowB][colB].getPieceType() != PieceType::NONE) {
        if (piece == PieceType::Pawn && colA == colB) {
            cout << "pawns can't capture moving forwards" << endl;
            return false;
        }
        taken = Piece{board[rowB][colB].getPieceType(), board[rowB][colB].getPieceColour(), rowB, colB};
        capture = true;
    } else {
        if (piece == PieceType::Pawn && colA != colB) {
            cout << "pawns can only move diagonally if they are capturing" << endl;
            return false;
        }
    }
    Colour colour = board[rowA][colA].getPieceColour();
    bool castled = false;
    //stops any attempt at moving onto your own pieces
    if (board[rowB][colB].getPieceColour() == board[rowA][colA].getPieceColour()) {
        cout << "same piece colour at destination" << endl;
        return false;
    }
    //validates any movement of a piece, making sure that the actual movement is valid
    if (!movementCheck(piece, colour, rowA, colA, rowB, colB)) {
        cout << "the piece you are trying to move cannot move like that" << endl;
        return false;
    }
    //will check if there is a piece blocking the path between rowA colA and rowB colB except for knights
    //since knights jump over pieces and we already checked that the ending cell does not contain an ally piece
    if (piece != PieceType::Knight) {
        if (blockCheck2(rowA, colA, rowB, colB)) {
            cout << "there is a piece blocking the path" << endl;
            return false;
        }
    }

    //handles castling
    if ((piece == PieceType::King) && (colA == 4) && abs(colA-colB) == 2) {
        if (checkCheck(colour)) { 
            cout << "cannot castle because you are in check" << endl;
            return false;
        }
        if ((colB == 2 && blockCheck2(rowA, 0, rowA, 4)) || (colB == 6 && blockCheck2(rowA, 7, rowA, 4))) {
            cout << "cannot castle because there is a piece in between" << endl;
            return false;
        }
        if (colour == Colour::White) {
            if (WKmoved == true) {
                cout << "cannot castle because you've already moved the white king" << endl;
                return false;
            }
            if ((colB == 2 && LWRmoved) || (colB == 6 && RWRmoved)) {
                cout << "cannot castle in that direction because you've already moved that rook" << endl;
                return false;
            }  
        } else {
            if (BKmoved == true) {
                cout << "cannot castle because you've already moved the black king" << endl;
                return false;
            }
            if ((colB == 2 && LBRmoved) || (colB == 6 && RBRmoved)) {
                cout << "cannot castle in that direction because you've already moved that rook" << endl;
                return false;
            } 
        }
        int middleCol = ((colB + colA)/2);
        this->add(colour, rowA, colA, rowB, middleCol);
        this->updateAllThreats(rowB, middleCol);
        if (checkCheck(colour)) {
            cout << "cannot castle because the spot in between would put you in check" << endl;
            this->add(colour, rowB, middleCol, rowA, colA);
            this->updateAllThreats(rowA, colA);
            return false;
        }

        this->add(colour, rowB, middleCol, rowB, colB);
        this->updateAllThreats(rowB, colB);
        if (checkCheck(colour)) {
            cout << "cannot castle because the the ending position would put you in check" << endl;
            this->add(colour, rowB, colB, rowA, colA);
            this->updateAllThreats(rowA, colA);
            return false;
        }
        //this->add(colour, rowB, colB, rowA, colA);
        this->updateAllThreats(rowA, colA);




        /*
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
        WKpos = &board[rowB][colB]; */
    } else {
        //simulates the actual move so that we can do various checks on it
        cout << "check 4" << endl;
        this->add(colour, rowA, colA, rowB, colB);
    }
    //will call updateAllThreats now that the piece has moved, allowing us to check for things like reveal checks
    cout << "check 5" << endl;
    this->updateAllThreats(rowB, colB);
    cout << "check 6" << endl;
    if (checkCheck(colour)) {
        cout << "check 6.1" << endl;
        if (castled) {
            if (colB == 6) {
                this->add(colour, rowA, 5, rowB, 7);
                board[rowA][5].remove();
            } else {
                this->add(colour, rowA, 3, rowB, 0);
                board[rowA][3].remove();
            }
        }
        //we then revert the movement since legalMoveCheck does not actually move it and is used for various other checks
        //where we do not want the move to stick
        this->add(colour, rowB, colB, rowA, colA);
        cout << "check 6.2" << endl;
        if (capture) {
            cout << "check 6.3" << endl;
            board[rowB][colB].add(taken.getPieceType(), taken.getPieceColour());
            this->addPieceToVector(taken);
            updateAllThreats(rowB, colB);
        }
        
        updateAllThreats(rowA, colA);
        cout << "check 6.4" << endl;
        cout << "this move puts you in check so it is not legal" << endl;
        return false;
    }
cout << "check 7" << endl;
    this->add(colour, rowB, colB, rowA, colA);
    cout << "check 8" << endl;
    if (capture) {
        board[rowB][colB].add(taken.getPieceType(), taken.getPieceColour());
        this->addPieceToVector(taken);
        updateAllThreats(rowB, colB);
    }
    updateAllThreats(rowA, colA);
    return true;
}

void Grid::updateAllThreats() {
    for (auto &i : WhitePieces) {
        i.updateCellsThreatening(*this);
    }
    for (auto &i : BlackPieces) {
        i.updateCellsThreatening(*this);
    }
    for (auto &i : WhitePieces) {
        i.updateThreatStatus(*this);
    }
    for (auto &i : BlackPieces) {
        i.updateThreatStatus(*this);
    }
}

void Grid::updateAllThreats(int row, int col) {
    for (auto &i : WhitePieces) {
       if ((i.getCol() == col && i.getRow() == row) || i.getPieceType() == PieceType::Bishop ||
        i.getPieceType() == PieceType::Rook || i.getPieceType() == PieceType::Queen) {
            i.updateCellsThreatening(*this);
        }
        if (i.getThreatStatus() == true) {
            i.updateThreatStatus(*this);
        }
    }
    /*for (int i = 0; i < length; i++) {
        if ((WhitePieces[i].getCol() == col && WhitePieces[i].getRow() == row) || WhitePieces[i].getPieceType() == PieceType::Bishop ||
        WhitePieces[i].getPieceType() == PieceType::Rook || WhitePieces[i].getPieceType() == PieceType::Queen) {
            WhitePieces[i].updateCellsThreatening(*this);
        }
        if (WhitePieces[i].getThreatStatus() == true) {
            WhitePieces[i].updateThreatStatus(*this);
        }
    }*/
    for (auto &i : BlackPieces) {
       if ((i.getCol() == col && i.getRow() == row) || i.getPieceType() == PieceType::Bishop ||
        i.getPieceType() == PieceType::Rook || i.getPieceType() == PieceType::Queen) {
            i.updateCellsThreatening(*this);
        }
        if (i.getThreatStatus() == true) {
            i.updateThreatStatus(*this);
        }
    }
    /*for (int i = 0; i < length; i++) {
        if ((BlackPieces[i].getCol() == col && BlackPieces[i].getRow() == row) || BlackPieces[i].getPieceType() == PieceType::Bishop ||
        BlackPieces[i].getPieceType() == PieceType::Rook || BlackPieces[i].getPieceType() == PieceType::Queen) {
            BlackPieces[i].updateCellsThreatening(*this);
        }
        if (BlackPieces[i].getThreatStatus() == true) {
            BlackPieces[i].updateThreatStatus(*this);
        }
    }*/
}

bool Grid::checkCheck(Colour colour) {  
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

bool Grid::checkCheckMate(Colour colour) {  
    //checks the pieces cellsthreatening to determine if it can take a piece or block the check
    if (colour == Colour::White) {
        for (auto i : WhitePieces) {
            for (auto j : *i.getCellsThreatening()) {
                cout << "checking if row " << i.getRow() << ", col " << i.getCol() << ", to row " << j->getRow() << ", col " << j->getCol() << " is legal" << endl;
                if (legalMoveCheck(i.getPieceType(), i.getRow(), i.getCol(), j->getRow(), j->getCol())) {
                    return false;
                }
            }
            if (i.getPieceType() == PieceType::Pawn) {
                if (i.getRow() == 1) {
                    cout << "checking if row " << i.getRow() << ", col " << i.getCol() << ", to row " << i.getRow() + 2 << ", col " << i.getCol() << " is legal" << endl;
                    if (legalMoveCheck(PieceType::Pawn, i.getRow(), i.getCol(), i.getRow() + 2, i.getCol())) {
                        return false;
                    }
                }
                cout << "checking if row " << i.getRow() << ", col " << i.getCol() << ", to row " << i.getRow() + 1 << ", col "<< i.getCol() << " is legal" << endl;
                if (legalMoveCheck(PieceType::Pawn, i.getRow(), i.getCol(), i.getRow() + 1, i.getCol())) {
                    return false;
                }
            }
        } 
    } else {
        for (auto i : BlackPieces) {
            for (auto j : *i.getCellsThreatening()) {

                cout << "checking if row " << i.getRow() << ", col " << i.getCol() << ", to row " << j->getRow() << ", col "<< j->getCol() << " is legal" << endl;
                if (legalMoveCheck(i.getPieceType(), i.getRow(), i.getCol(), j->getRow(), j->getCol())) {
                    return false;
                }
            }
            if (i.getPieceType() == PieceType::Pawn) {
                if (i.getRow() == 7) {
                    cout << "checking if row " << i.getRow() << ", col " << i.getCol() << ", to row " << i.getRow() - 2 << ", col "<< i.getCol() << " is legal" << endl;
                    if (legalMoveCheck(PieceType::Pawn, i.getRow(), i.getCol(), i.getRow() - 2, i.getCol())) {
                        return false;
                    }
                }
                cout << "checking if row " << i.getRow() << ", col " << i.getCol() << ", to row " << i.getRow() - 1 << ", col "<< i.getCol() << " is legal" << endl;
                if (legalMoveCheck(PieceType::Pawn, i.getRow(), i.getCol(), i.getRow() - 1, i.getCol())) {
                    return false;
                }
            }
        }

    }
    return true; 
}

bool Grid::blockCheck2(int rowA, int colA, int rowB, int colB) {
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
            return true;
        }
        rowStart += rowIncrement;
        colStart += colIncrement;
    }
    return false;
}

bool Grid::stalemateCheck(Colour colour) { return checkCheckMate(colour); }

void Grid::botLvl1(Colour colour) {
    int pieceToMove;
    int cellToMove;
    Piece* piece;
    Cell* cell;
    //white check
    if (colour == Colour::White) {
        int length = WhitePieces.size();
        pieceToMove = rand() % length;
        piece = &WhitePieces[pieceToMove];
        length = piece->getCellsThreatening()->size();
        //make sure that pawns are correct since they cant move in their cells threatened
        if (piece->getPieceType() == PieceType::Pawn) {

        } else {
             cellToMove = rand() & length;
            cell = piece->getCellsThreatening()[0][cellToMove];
            move(piece->getRow(), piece->getCol(), cell->getRow(), cell->getCol());
        }
        //black check
    } else if (colour == Colour::Black) {
        int length = BlackPieces.size();
        while(true) {
            pieceToMove = rand() % length;
            cout << pieceToMove << endl;
            piece = &BlackPieces[pieceToMove];
            length = piece->getCellsThreatening()->size();
            if (length == 0) {
                continue;
            }
            cellToMove = rand() & length;
            cell = piece->getCellsThreatening()[0][cellToMove];
            if (move(piece->getRow(), piece->getCol(), cell->getRow(), cell->getCol()) == )
            break;
        }
    }
}

void Grid::botLvl2(Colour colour) {
    Piece* piece;
    Cell* cell;
    int rowA, colA, rowB, colB;
    Piece taken;
    //white check
    if (colour == Colour::White) {
        //might have to check for legal move
        //add priority
        int length = WhitePieces.size();
        int cellsThreateningLength;
        for (int i = 0; i < length; i++) {
            piece = &WhitePieces[i];
            cellsThreateningLength = piece->getCellsThreatening()->size();
            rowA = piece->getRow();
            colA = piece->getCol();
            for (int j = 0; j < cellsThreateningLength; j++) {
                cell = piece->getCellsThreatening()[0][j];
                rowB = cell->getRow();
                colB = cell->getCol();
                if (cell->getPieceType() == PieceType::NONE) {
                    add(Colour::White, rowA, colA, rowB, colB);
                    updateAllThreats(rowB, colB);
                    if (checkCheck(Colour::Black)) {
                        //might have to check for checkmate and stalemate and return an actual value, talk abt it
                        randomize(piece->getPieceColour());
                        return;
                    } else {
                        add(Colour::White, rowB, colB, rowA, colA);
                        updateAllThreats(rowA, colA);
                        continue;
                    }
                } else {
                    taken = getPieceObject(colour, rowB, colB);
                    add(colour, rowA, colA, rowB, colB);
                    updateAllThreats(rowB, colB);
                    if (checkCheck(Colour::Black)) {
                        randomize(colour);
                        return;
                    } else {
                        add(colour, rowB, colB, rowA, colA);
                        addPieceToVector(taken);
                        updateAllThreats(rowA, colA);
                        updateAllThreats(rowB, colB);
                        continue;
                    }

                }
            }
        }
        for (int i = 0; i < length; i++) {
            piece = &WhitePieces[i];
            cellsThreateningLength = piece->getCellsThreatening()->size();
            rowA = piece->getRow();
            colA = piece->getCol();
            for (int j = 0; j < cellsThreateningLength; j++) {
                cell = piece->getCellsThreatening()[0][j];
                rowB = cell->getRow();
                colB = cell->getCol();
                if (cell->getPieceType() != PieceType::NONE || cell->getPieceType() != PieceType::King) {
                    move(piece->getRow(), piece->getCol(), cell->getRow(), cell->getCol());
                    return;
                }
            }
        }
        botLvl1(colour);
        //black check
    } else if (colour == Colour::Black) {
        //might have to check for legal move
        //add priority
        int length = BlackPieces.size();
        int cellsThreateningLength;
        for (int i = 0; i < length; i++) {
            piece = &BlackPieces[i];
            cellsThreateningLength = piece->getCellsThreatening()->size();
            rowA = piece->getRow();
            colA = piece->getCol();
            for (int j = 0; j < cellsThreateningLength; j++) {
                cell = piece->getCellsThreatening()[0][j];
                rowB = cell->getRow();
                colB = cell->getCol();
                if (cell->getPieceType() == PieceType::NONE) {
                    add(colour, rowA, colA, rowB, colB);
                    updateAllThreats(rowB, colB);
                    if (checkCheck(Colour::Black)) {
                        //might have to check for checkmate and stalemate and return an actual value, talk abt it
                        randomize(piece->getPieceColour());
                        return;
                    } else {
                        add(colour, rowB, colB, rowA, colA);
                        updateAllThreats(rowA, colA);
                        continue;
                    }
                } else {
                    taken = getPieceObject(colour, rowB, colB);
                    add(colour, rowA, colA, rowB, colB);
                    updateAllThreats(rowB, colB);
                    if (checkCheck(Colour::Black)) {
                        randomize(colour);
                        return;
                    } else {
                        add(colour, rowB, colB, rowA, colA);
                        addPieceToVector(taken);
                        updateAllThreats(rowA, colA);
                        updateAllThreats(rowB, colB);
                        continue;
                    }

                }
            }
        }
        for (int i = 0; i < length; i++) {
            piece = &BlackPieces[i];
            cellsThreateningLength = piece->getCellsThreatening()->size();
            rowA = piece->getRow();
            colA = piece->getCol();
            for (int j = 0; j < cellsThreateningLength; j++) {
                cell = piece->getCellsThreatening()[0][j];
                rowB = cell->getRow();
                colB = cell->getCol();
                if (cell->getPieceType() != PieceType::NONE || cell->getPieceType() != PieceType::King) {
                    move(piece->getRow(), piece->getCol(), cell->getRow(), cell->getCol());
                    return;
                }
            }
        }
        botLvl1(colour);
    }
}

void Grid::botLvl3(Colour colour) {
    Piece* piece;
    Cell* cell;
    int rowA, colA, rowB, colB;
    //white check
    if (colour == Colour::White) {
        Piece taken;
        int length = WhitePieces.size();
        int cellsThreateningLength;
        for (int i = 0; i < length; i++) {
            piece = &WhitePieces[i];
            if (piece->getThreatStatus()) {
                cellsThreateningLength = piece->getCellsThreatening()->size();
                rowA = piece->getRow();
                colA = piece->getCol();
                for (int j = 0; j < cellsThreateningLength; j++) {
                    cell = WhitePieces[i].getCellsThreatening()[0][j];
                    rowB = cell->getRow();
                    colB = cell->getCol();
                    if (cell->getPieceType() == PieceType::NONE) {
                        add(colour, rowA, colA, rowB, colB);
                        updateAllThreats(rowB, colB);
                        if (checkCheck(colour) || piece->getThreatStatus()) {
                            add(colour, rowB, colB, rowA, colA);
                            updateAllThreats(rowA, colA);
                            continue;
                        } else {
                            randomize(colour);
                            //may have to check for mate
                            return;
                        }
                    } else {
                        taken = getPieceObject(Colour::Black, rowB, colB);
                        add(colour, rowA, colA, rowB, colB);
                        updateAllThreats(rowB, colB);
                        if (checkCheck(colour) || piece->getThreatStatus()) {
                            add(colour, rowB, colB, rowA, colA);
                            addPieceToVector(taken);
                            updateAllThreats(rowA, colA);
                            updateAllThreats(rowB, colB);
                            continue;
                        } else {
                            randomize(colour);
                            //may have to check for mate
                            return;
                        }
                    }
                }
            }                    
        }
        botLvl2(colour);
    }
    //black check
    if (colour == Colour::Black) {
        Piece taken;
        int length = BlackPieces.size();
        int cellsThreateningLength;
        for (int i = 0; i < length; i++) {
            piece = &BlackPieces[i];
            if (piece->getThreatStatus()) {
                cellsThreateningLength = piece->getCellsThreatening()->size();
                rowA = piece->getRow();
                colA = piece->getCol();
                for (int j = 0; j < cellsThreateningLength; j++) {
                    cell = BlackPieces[i].getCellsThreatening()[0][j];
                    rowB = cell->getRow();
                    colB = cell->getCol();
                    if (cell->getPieceType() == PieceType::NONE) {
                        add(colour, rowA, colA, rowB, colB);
                        updateAllThreats(rowB, colB);
                        if (checkCheck(colour) || piece->getThreatStatus()) {
                            add(colour, rowB, colB, rowA, colA);
                            updateAllThreats(rowA, colA);
                            continue;
                        } else {
                            randomize(colour);
                            return;
                        }
                    } else {
                        taken = getPieceObject(Colour::White, rowB, colB);
                        add(colour, rowA, colA, rowB, colB);
                        updateAllThreats(rowB, colB);
                        if (checkCheck(colour) || piece->getThreatStatus()) {
                            add(colour, rowB, colB, rowA, colA);
                            addPieceToVector(taken);
                            updateAllThreats(rowA, colA);
                            updateAllThreats(rowB, colB);
                            continue;
                        } else {
                            randomize(colour);
                            return;
                        }
                    }
                }
            }                    
        }
        botLvl2(colour);
    }

}

void Grid::botLvl4(Colour colour) {

}

int Grid::convertValues(PieceType piece) {
    if (piece == PieceType::Pawn) {
        return 1;
    } else if  (piece == PieceType::Bishop || piece == PieceType::Knight) {
        return 3;
    } else if (piece == PieceType::Rook) {
        return 5;
    } else if (piece == PieceType::Queen) {
        return 10;
    }
}

void Grid::botMove(BotLevel botLevel, Colour colour) {
    if (botLevel == BotLevel::Level1) {
        botLvl1(colour);
    } else if (botLevel == BotLevel::Level2) {
        botLvl2(colour);
    } else if (botLevel == BotLevel::Level3) {
        botLvl3(colour);
    } else if (botLevel == BotLevel::Level4) {

    }
 }

 void Grid::randomize(Colour colour) {
    int pos1;
    int pos2;
    int length;
    if (colour == Colour::White) {
        length = WhitePieces.size();
        for (int i = 0; i < 5; i++) {
            pos1 = rand() % length;
            pos2 = rand() % length;
            swap(WhitePieces[pos1], WhitePieces[pos2]);
        }
    } else {
        length = BlackPieces.size();
        for (int i = 0; i < 5; i++) {
            pos1 = rand() % length;
            pos2 = rand() % length;
            swap(BlackPieces[pos1], BlackPieces[pos2]);
        }
    }
 }

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
        cout << "got here 2" << endl;
        /*for (int i = 0; i < length - 1; i++) {
            cout << "in for loop " << i << endl;
            if (WhitePieces[i].getCol() == c && WhitePieces[i].getRow() == r) {
                cout << "got here 3" << endl;
                return &WhitePieces[i];
            }
        }*/
        for (auto &i : WhitePieces) {
            cout << i.getRow() << endl;
            if (i.getRow() == r && i.getCol() == c) {
                return &i;
            }
        }
    } else {
        cout << "got here 4" << endl;
        length = BlackPieces.size();
        cout << length << endl;
        for (auto &i : BlackPieces) {
            cout << i.getRow() << endl;
            if (i.getRow() == r && i.getCol() == c) {
                return &i;
            }
        }
        /*for (int i = 0; i < length - 1; i++) {
            if (BlackPieces[i].getCol() == c && BlackPieces[i].getRow() == r) {
                return &BlackPieces[i];
            }
        }*/
    }
}

Piece Grid::getPieceObject(Colour pieceColour, int r, int c) {
    int length = WhitePieces.size();
    cout << length << endl;
    cout << "womp womp" << endl;
    if (pieceColour == Colour::White) {
        cout << "womp womp bitch" << endl;
         for (auto &i : WhitePieces) {
            cout << i.getRow() << endl;
            if (i.getRow() == r && i.getCol() == c) {
                return i;
            }
        }
    } else {
        cout << "womp womp fucker" << endl;
        length = BlackPieces.size();
        cout << length << endl;
        for (auto &i : BlackPieces) {
            cout << i.getRow() << endl;
            if (i.getRow() == r && i.getCol() == c) {
                return i;
            }
        }
    }
}

ostream &operator<<(ostream &out, const Grid &g) {
    return out << *g.td << endl;
}
