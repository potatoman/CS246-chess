#include "Grid.h"
using namespace std;



Grid::Grid(): board{}, WhitePieces{}, BlackPieces{}, td{nullptr} {}


Grid::~Grid() {}


void Grid::init() { 
    board = std::vector<std::vector<Cell>>(8, std::vector<Cell>(8));
    td = new TextDisplay();
    gd = new GraphicsDisplay{x};
    WhitePieces.clear();
    BlackPieces.clear();
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
    PieceType pieceType = getPiece(pieceColour, rowA, colA)->getPieceType();
    getPiece(pieceColour, rowA, colA)->updateCoords(rowB, colB);
    board[rowB][colB].add(pieceType, pieceColour);
    board[rowA][colA].remove();
}

int Grid::add(PieceType pieceType, Colour colour, int row, int col) {
    Piece piece = Piece{pieceType, colour, row, col};
    addPieceToVector(piece);
    updateAllThreats();
    board[row][col].add(pieceType, colour);
    Colour opColour;
    if (piece.getPieceColour() == Colour::White) {
        opColour == Colour::Black;
    } else {
        opColour == Colour::White;
    }
    if (checkCheck(opColour)) {
        td->updateTD(board[row][col]);
        gd->notify(board[row][col]);
        if (checkCheckMate(opColour)) {
            return 1;
        } else if (stalemateCheck(opColour)) {
            return 2;
        }
    } else {
        td->updateTD(board[row][col]);
        gd->notify(board[row][col]);
        return 0;
    }
}

void Grid::add(Colour pieceColour, PieceType piece, int row, int col) {
    board[row][col].add(piece, pieceColour);
    if (board[row][col].getPieceType() != PieceType::NONE) { this->removePieceFromVector(row, col); }
    this->addPieceToVector(pieceColour, piece, row, col);
    td->updateTD(board[row][col]);
    gd->notify(board[row][col]);
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
    if (piece == PieceType::Pawn && ((colour == Colour::White && rowB == 7) ||
    (colour == Colour::Black && rowB == 0))) {
        remove(rowA, colA);
        return 4;
    }
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
    this->add(colour, rowA, colA, rowB, colB);

    if (piece == PieceType::King && (abs(colB - colA) == 2)) {
        if (colB == 2) {
            this->add(colour, rowA, 0, rowB, 3);
            this->updateAllThreats();
            td->updateTD(board[rowA][0]);
            gd->notify(board[rowA][0]);
            td->updateTD(board[rowB][3]);
            gd->notify(board[rowB][3]);
        } else if (colB == 6) {
            this->add(colour, rowA, 7, rowB, 5);
            this->updateAllThreats();
            td->updateTD(board[rowA][7]);
            gd->notify(board[rowA][7]);
            td->updateTD(board[rowB][5]);
            gd->notify(board[rowB][5]);
        }
    }

    this->updateAllThreats();
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
        if (blockCheck(rowA, colA, rowB, colB)) {
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
        if ((colB == 2 && blockCheck(rowA, 0, rowA, 4)) || (colB == 6 && blockCheck(rowA, 7, rowA, 4))) {
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
        this->updateAllThreats();
        if (checkCheck(colour)) {
            cout << "cannot castle because the spot in between would put you in check" << endl;
            this->add(colour, rowB, middleCol, rowA, colA);
            this->updateAllThreats();
            return false;
        }

        this->add(colour, rowB, middleCol, rowB, colB);
        this->updateAllThreats();
        if (checkCheck(colour)) {
            cout << "cannot castle because the the ending position would put you in check" << endl;
            this->add(colour, rowB, colB, rowA, colA);
            this->updateAllThreats();
            return false;
        }
        this->updateAllThreats();
    } else {
        //simulates the actual move so that we can do various checks on it
        this->add(colour, rowA, colA, rowB, colB);
    }
    //will call updateAllThreats now that the piece has moved, allowing us to check for things like reveal checks
    this->updateAllThreats();
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
        //we then revert the movement since legalMoveCheck does not actually move it and is used for various other checks
        //where we do not want the move to stick
        this->add(colour, rowB, colB, rowA, colA);
        if (capture) {
            board[rowB][colB].add(taken.getPieceType(), taken.getPieceColour());
            this->addPieceToVector(taken);
            updateAllThreats();
        }
        
        updateAllThreats();
        cout << "this move puts you in check so it is not legal" << endl;
        return false;
    }
    this->add(colour, rowB, colB, rowA, colA);
    if (capture) {
        board[rowB][colB].add(taken.getPieceType(), taken.getPieceColour());
        this->addPieceToVector(taken);
        updateAllThreats();
    }
    updateAllThreats();
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
                if (legalMoveCheck(i.getPieceType(), i.getRow(), i.getCol(), j->getRow(), j->getCol())) {
                    return false;
                }
            }
            if (i.getPieceType() == PieceType::Pawn) {
                if (i.getRow() == 1) {
                    if (legalMoveCheck(PieceType::Pawn, i.getRow(), i.getCol(), i.getRow() + 2, i.getCol())) {
                        return false;
                    }
                }
                if (legalMoveCheck(PieceType::Pawn, i.getRow(), i.getCol(), i.getRow() + 1, i.getCol())) {
                    return false;
                }
            }
        } 
    } else {
        for (auto i : BlackPieces) {
            for (auto j : *i.getCellsThreatening()) {
                if (legalMoveCheck(i.getPieceType(), i.getRow(), i.getCol(), j->getRow(), j->getCol())) {
                    return false;
                }
            }
            if (i.getPieceType() == PieceType::Pawn) {
                if (i.getRow() == 7) {
                    if (legalMoveCheck(PieceType::Pawn, i.getRow(), i.getCol(), i.getRow() - 2, i.getCol())) {
                        return false;
                    }
                }
                if (legalMoveCheck(PieceType::Pawn, i.getRow(), i.getCol(), i.getRow() - 1, i.getCol())) {
                    return false;
                }
            }
        }

    }
    return true; 
}

bool Grid::blockCheck(int rowA, int colA, int rowB, int colB) {
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

int Grid::botLvl1(Colour colour) {
    int pieceToMove;
    int cellToMove;
    Piece* piece;
    Cell* cell;
    //white check
    if (colour == Colour::White) {
         int lengthOfPieces = WhitePieces.size();
         int length;
        while(true) {
            //finds the piece it is going to try to move
            pieceToMove = rand() % lengthOfPieces;
            piece = &WhitePieces[pieceToMove];
            length = piece->getCellsThreatening()->size();
            //specifies move for pawn since pawn doesnt move where it is threatening
            if (piece->getPieceType() == PieceType::Pawn) {
                if (piece->getRow() == 1) {
                    int flipCoin = rand() % 2;
                    if (flipCoin == 1) {
                        int result = move(piece->getRow(), piece->getCol(), piece->getRow() + 2, piece->getCol());
                        if (result != 3) {
                            return result;
                            break;
                        }
                    } else {
                        int result = move(piece->getRow(), piece->getCol(), piece->getRow() + 1, piece->getCol());
                        if (result != 3) {
                            return result;
                            break;
                        }
                    }
                } else {
                    int result = move(piece->getRow(), piece->getCol(), piece->getRow() + 2, piece->getCol());
                    if (result != 3) {
                        return result;
                        break;
                    }
                }
                //will skip if the length of cells threatening is empty
            } else if (length == 0) {
                continue;
            } else {
                cellToMove = rand() & length;
                cell = piece->getCellsThreatening()[0][cellToMove];
                int result = move(piece->getRow(), piece->getCol(), cell->getRow(), cell->getCol());
                if (result != 3) {
                    return result;
                    break;
                }
            }
        }
        //black check
    } else if (colour == Colour::Black) {
        int lengthOfPieces = BlackPieces.size();
        int length;
        int result;
        while(true) {
            pieceToMove = rand() % lengthOfPieces;
            piece = &BlackPieces[pieceToMove];
            length = piece->getCellsThreatening()->size();
            if (piece->getPieceType() == PieceType::Pawn) {
                if (piece->getRow() == 6) {
                    int flipCoin = rand() % 2;
                    if (flipCoin == 1) {
                        result = move(piece->getRow(), piece->getCol(), piece->getRow() - 2, piece->getCol());
                        if (result != 3) {
                            return result;
                            break;
                        }
                    } else {
                        result = move(piece->getRow(), piece->getCol(), piece->getRow() - 1, piece->getCol());
                        if(result != 3) {
                            return result;
                            break;
                        }
                    }
                } else {
                    result = move(piece->getRow(), piece->getCol(), piece->getRow() - 1, piece->getCol());
                    if (result != 3) {
                        return result;
                        break;
                    }
                }
            } else if (length == 0) {
                continue;
            } else {
                cellToMove = rand() & length;
                cell = piece->getCellsThreatening()[0][cellToMove];
                result = move(piece->getRow(), piece->getCol(), cell->getRow(), cell->getCol()); 
                if (result != 3) {
                    return result;
                    break;
                }
            }
        }
    }
}

int Grid::botLvl2(Colour colour) {
    Piece* piece;
    Cell* cell;
    int rowA, colA, rowB, colB;
    Piece taken;
    //white check
    if (colour == Colour::White) {
        int length = WhitePieces.size();
        int cellsThreateningLength;
        for (int i = 0; i < length; i++) {
            piece = &WhitePieces[i];
            cellsThreateningLength = piece->getCellsThreatening()->size();
            rowA = piece->getRow();
            colA = piece->getCol();
            //prioritizes checks over taking pieces
            for (int j = 0; j < cellsThreateningLength; j++) {
                cellsThreateningLength = piece->getCellsThreatening()->size();
                cell = piece->getCellsThreatening()[0][j];
                rowB = cell->getRow();
                colB = cell->getCol();
                if (cell->getPieceType() == PieceType::NONE) {
                    add(colour, rowA, colA, rowB, colB);
                    updateAllThreats();
                    if (!checkCheck(colour)) {
                        if (checkCheck(Colour::Black)) {
                            td->updateTD(board[rowB][colB]);
                            gd->notify(board[rowB][colB]);
                            td->updateTD(board[rowA][colA]);
                            gd->notify(board[rowA][colA]);
                        randomize(piece->getPieceColour());
                        if (checkCheckMate(Colour::Black)) {
                            return 1;
                        } else if (stalemateCheck(Colour::Black)) {
                            return 2;
                        }
                        return 0;
                        } else {
                            add(colour, rowB, colB, rowA, colA);
                            updateAllThreats();
                            continue;
                        }
                    }
                } else {
                    taken = getPieceObject(Colour::Black, rowB, colB);
                    add(colour, rowA, colA, rowB, colB);
                    updateAllThreats();
                    if (!checkCheck(colour)) {
                        if (checkCheck(Colour::Black)) {
                            td->updateTD(board[rowB][colB]);
                            gd->notify(board[rowB][colB]);
                            td->updateTD(board[rowA][colA]);
                            gd->notify(board[rowA][colA]);
                            randomize(colour);
                            if (checkCheckMate(Colour::Black)) {
                                return 1;
                            } else if (stalemateCheck(Colour::Black)) {
                                return 2;
                            }
                            return 0;
                        } else {
                            add(colour, rowB, colB, rowA, colA);
                            botAdd(taken, rowA, colA);
                            continue;
                        }
                    }
                }
            }
        }
        //checks if there are any pieces to take
        for (int i = 0; i < length; i++) {
            piece = &WhitePieces[i];
            cellsThreateningLength = piece->getCellsThreatening()->size();
            rowA = piece->getRow();
            colA = piece->getCol();
            for (int j = 0; j < cellsThreateningLength; j++) {
                cell = piece->getCellsThreatening()[0][j];
                rowB = cell->getRow();
                colB = cell->getCol();
                if (cell->getPieceType() != PieceType::NONE && cell->getPieceType() != PieceType::King) {
                    return move(piece->getRow(), piece->getCol(), cell->getRow(), cell->getCol());
                }
            }
        }
        return botLvl1(colour);
        //black check
    } else if (colour == Colour::Black) {
        int length = BlackPieces.size();
        int cellsThreateningLength;
        for (int i = 0; i < length; i++) {
            piece = &BlackPieces[i];
            cellsThreateningLength = piece->getCellsThreatening()->size();
            rowA = piece->getRow();
            colA = piece->getCol();
            for (int j = 0; j < cellsThreateningLength; j++) {
                cellsThreateningLength = piece->getCellsThreatening()->size();
                cell = piece->getCellsThreatening()[0][j];
                rowB = cell->getRow();
                colB = cell->getCol();
                if (cell->getPieceType() == PieceType::NONE) {
                    add(colour, rowA, colA, rowB, colB);
                    updateAllThreats();
                    if (!checkCheck(colour)) {
                        if (checkCheck(Colour::White)) {
                            td->updateTD(board[rowB][colB]);
                            gd->notify(board[rowB][colB]);
                            td->updateTD(board[rowA][colA]);
                            gd->notify(board[rowA][colA]);
                            if (checkCheckMate(Colour::White)) {
                                return 1;
                            } else if (stalemateCheck(Colour::White)) {
                                return 2;
                            } 
                            //might have to check for checkmate and stalemate and return an actual value, talk abt it
                            randomize(piece->getPieceColour());
                            return 0;
                        } else {
                            add(colour, rowB, colB, rowA, colA);
                            updateAllThreats();
                            continue;
                        }
                    }
                } else {
                    taken = getPieceObject(Colour::White, rowB, colB);
                    add(colour, rowA, colA, rowB, colB);
                    updateAllThreats();
                    if (!checkCheck(colour)) {
                        if (checkCheck(Colour::White)) {
                            td->updateTD(board[rowB][colB]);
                            gd->notify(board[rowB][colB]);
                            td->updateTD(board[rowA][colA]);
                            gd->notify(board[rowA][colA]);
                            if (checkCheckMate(Colour::White)) {
                                return 1;
                            } else if (stalemateCheck(Colour::White)) {
                                return 2;
                            }
                            randomize(colour);
                            return 0;
                        } else {
                            add(colour, rowB, colB, rowA, colA);
                            botAdd(taken, rowA, colA);
                            continue;
                        }
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
                if (cell->getPieceType() != PieceType::NONE && cell->getPieceType() != PieceType::King) {
                    return move(piece->getRow(), piece->getCol(), cell->getRow(), cell->getCol());
                }
            }
        }
        return botLvl1(colour);
    }
}

void Grid::botAdd(Piece piece, int row, int col) {
    board[row][col].add(piece.getPieceType(), piece.getPieceColour());
    this->addPieceToVector(piece);
    updateAllThreats();
}

int Grid::botLvl3(Colour colour) {
    Piece* piece;
    Cell* cell;
    int rowA, colA, rowB, colB;
    //white check
    if (colour == Colour::White) {
        Piece taken;
        int length = WhitePieces.size();
        int cellsThreateningLength;
        //prioritizes keeping pieces safe then checking then taking pieces
        for (int i = 0; i < length; i++) {
            piece = &WhitePieces[i];
            if (piece->getThreatStatus()) {
                cellsThreateningLength = piece->getCellsThreatening()->size();
                rowA = piece->getRow();
                colA = piece->getCol();
                for (int j = 0; j < cellsThreateningLength; j++) {
                    cellsThreateningLength = piece->getCellsThreatening()->size();
                    cell = WhitePieces[i].getCellsThreatening()[0][j];
                    rowB = cell->getRow();
                    colB = cell->getCol();
                    if (cell->getPieceType() == PieceType::NONE) {
                        add(colour, rowA, colA, rowB, colB);
                        updateAllThreats();
                        //checks if the move puts it in check or if it is still under attack
                        //if the piece is still under attack after exhausting everything it will just continue
                        //and see if it can save another piece before trying to check or take another piece
                        if (checkCheck(colour) || piece->getThreatStatus()) {
                            add(colour, rowB, colB, rowA, colA);
                            updateAllThreats();
                            continue;
                        } else {
                            if (checkCheck(Colour::Black)) {
                                if (checkCheckMate(Colour::Black)) {
                                    return 1;
                                } else if (stalemateCheck(Colour::Black)) {
                                    return 2;
                                }
                            }
                            randomize(colour);
                            return 3;
                        }
                    } else {
                        taken = getPieceObject(Colour::Black, rowB, colB);
                        add(colour, rowA, colA, rowB, colB);
                        updateAllThreats();
                        if (checkCheck(colour) || piece->getThreatStatus()) {
                            add(colour, rowB, colB, rowA, colA);
                            botAdd(taken, rowA, colA);
                            continue;
                        } else {
                            if (checkCheck(Colour::Black)) {
                                if (checkCheckMate(Colour::Black)) {
                                    return 1;
                                } else if (stalemateCheck(Colour::Black)) {
                                    return 2;
                                }
                            }
                            randomize(colour);
                            return 3;
                        }
                    }
                }
            }                    
        }
        return botLvl2(colour);
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
                    cellsThreateningLength = piece->getCellsThreatening()->size();
                    cell = BlackPieces[i].getCellsThreatening()[0][j];
                    rowB = cell->getRow();
                    colB = cell->getCol();
                    if (cell->getPieceType() == PieceType::NONE) {
                        add(colour, rowA, colA, rowB, colB);
                        updateAllThreats();
                        if (checkCheck(colour) || piece->getThreatStatus()) {
                            add(colour, rowB, colB, rowA, colA);
                            updateAllThreats();
                            continue;
                        } else {
                            if (checkCheck(Colour::White)) {
                                if (checkCheckMate(Colour::White)) {
                                    return 1;
                                } else if (stalemateCheck(Colour::White)) {
                                    return 2;
                                }
                            }
                            randomize(colour);
                            return 3;
                        }
                    } else {
                        taken = getPieceObject(Colour::White, rowB, colB);
                        add(colour, rowA, colA, rowB, colB);
                        updateAllThreats();
                        if (checkCheck(colour) || piece->getThreatStatus()) {
                            add(colour, rowB, colB, rowA, colA);
                            botAdd(taken, rowA, colA);
                            continue;
                        } else {
                            if (checkCheck(Colour::White)) {
                                if (checkCheckMate(Colour::White)) {
                                    return 1;
                                } else if (stalemateCheck(Colour::White)) {
                                    return 2;
                                }
                            }
                            randomize(colour);
                            return 3;
                        }
                    }
                }
            }                    
        }
        return botLvl2(colour);
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

int Grid::botMove(BotLevel botLevel, Colour colour) {
    if (botLevel == BotLevel::Level1) {
        return botLvl1(colour);
    } else if (botLevel == BotLevel::Level2) {
        return botLvl2(colour);
    } else if (botLevel == BotLevel::Level3) {
        return botLvl3(colour);
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
    if (pieceColour == Colour::White) {
        for (auto &i : WhitePieces) {
            if (i.getRow() == r && i.getCol() == c) {
                return &i;
            }
        }
    } else {
        length = BlackPieces.size();
        for (auto &i : BlackPieces) {
            if (i.getRow() == r && i.getCol() == c) {
                return &i;
            }
        }
    }
}

Piece Grid::getPieceObject(Colour pieceColour, int r, int c) {
    int length = WhitePieces.size();
    if (pieceColour == Colour::White) {
         for (auto &i : WhitePieces) {
            if (i.getRow() == r && i.getCol() == c) {
                return i;
            }
        }
    } else {
        length = BlackPieces.size();
        for (auto &i : BlackPieces) {
            if (i.getRow() == r && i.getCol() == c) {
                return i;
            }
        }
    }
}

ostream &operator<<(ostream &out, const Grid &g) {
    return out << *g.td << endl;
}
