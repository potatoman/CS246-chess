#include <iostream>
#include <string>
#include "Grid.h"
using namespace std;

PieceType findPiece (char piece) {
    piece = toupper(piece);
    if (piece == 'K') {
        return PieceType::King;
    } else if (piece == 'Q') {
        return PieceType::Queen;
    } else if (piece == 'N') {
        return PieceType::Knight;
    } else if (piece == 'B') {
        return PieceType::Bishop;
    } else if (piece == 'R') {
        return PieceType::Rook;
    } else if (piece == 'P') {
        return PieceType::Pawn;
    } else {
        return PieceType::NONE;
    }
}

Colour findColour (char piece) {
    if (piece > 97) {
        return Colour::Black;
    } else {
        return Colour::White;
    }
}

int findCol(char col) {
    col = toupper(col);
    if (col == 'A') {
        return 0;
    } else if (col == 'B') {
        return 1;
    } else if (col == 'C') {
        return 2;
    } else if (col == 'D') {
        return 3;
    } else if (col == 'E') {
        return 4;
    } else if (col == 'F') {
        return 5;
    } else if (col == 'G') {
        return 6;
    } else if (col == 'H') {
        return 7;
    } else {
        return 0;
    }
}

int main() {
    string cmd;
    Grid g;
    Colour colour = Colour::White;
    bool gameStarted = false;
    Colour whoseTurn = Colour::White;
    enum class PlayerType { Human, Bot };
    PlayerType whitePlayer;
    PlayerType blackPlayer;
    BotLevel whiteBot;
    BotLevel blackBot;
    int whiteScore = 0;
    int blackScore = 0;

    while(cin >> cmd) {
        if (cmd == "game") {
            string white;
            string black;
            gameStarted = true;
            cin >> white >> black;
            white[0] = toupper(white[0]);
            black[0] = toupper(black[0]);
            if (white[0] == 'H') {
                whitePlayer = PlayerType::Human;
            } 
            if (black[0] == 'H') {
                blackPlayer = PlayerType::Human;
            } 
            if (white[0] == 'C') {
                int length = white.length();
                whitePlayer = PlayerType::Bot;
                for (int i = 0; i < length; i++) {
                    if (white[i] == '1') {
                        whiteBot = BotLevel::Level1;
                    } else if (white[i] == '2') {
                        whiteBot = BotLevel::Level2;
                    } else if (white[i] == '3') {
                        whiteBot = BotLevel::Level3;
                    } else if (white[i] == '4') {
                        whiteBot = BotLevel::Level4;
                    }
                }
            }
            if (black[0] == 'C') {
                int length = black.length();
                blackPlayer = PlayerType::Bot;
                for (int i = 0; i < length; i++) {
                    if (black[i] == '1') {
                        blackBot = BotLevel::Level1;
                    } else if (black[i] == '2') {
                        blackBot = BotLevel::Level2;
                    } else if (black[i] == '3') {
                        blackBot = BotLevel::Level3;
                    } else if (black[i] == '4') {
                        blackBot = BotLevel::Level4;
                    }
                }
            }   
        } else if (cmd == "move") {
            //TODO - add check to make sure they are moving their own pieces
            if (!gameStarted) {
                int rowA, colA, rowB, colB;
                string initialPos;
                string endPos;
                cin >> initialPos;
                cin >> endPos;
                colA = findCol(initialPos[0]);
                colB = findCol(endPos[0]);
                rowA = initialPos[1] - '0';
                rowA -= 1;
                rowB = endPos[1] - '0';
                rowB -= 1;
                if (rowA >= 8 || rowB >= 8 || colA >= 8 || colB >= 8) {
                    continue;
                }
                int result = g.move(rowA, colA, rowB, colB);
                if (result == 1) {
                    //end game
                }
            } else {
                if ((whitePlayer == PlayerType::Human && whoseTurn == Colour::White) || 
                (blackPlayer == PlayerType::Human && whoseTurn == Colour::Black)) {
                    int rowA, colA, rowB, colB;
                    string initialPos;
                    string endPos;
                    string piece;
                    cin >> initialPos;
                    cin >> endPos;
                    colA = findCol(initialPos[0]);
                    colB = findCol(endPos[0]);
                    rowA = initialPos[1] - '0';
                    rowA -= 1;
                    rowB = endPos[1] - '0';
                    rowB -= 1;
                    if (rowA >= 8 || rowB >= 8 || colA >= 8 || colB >= 8) {
                        continue;
                    }
                    if (g.findCell(rowA,colA)->getPieceType() == PieceType::Pawn && (rowB == 7 || rowB == 0)) {
                        cin >> piece;
                    }
                    if (g.findCell(rowA,colA)->getPieceColour() == whoseTurn) {
                        int result = g.move(rowA, colA, rowB, colB);
                        if (result == 1) {
                            gameStarted = false;
                            if (whoseTurn == Colour::White) {
                                whiteScore += 1;
                            } else {
                                blackScore += 1;
                            }
                        } else if (result == 2) {
                            gameStarted = false;
                            cout << "Stalemate!" << endl;
                        } else if (result == 3) {
                            continue;
                        } else if (result == 4) {
                            piece = toupper(piece[0]);
                            if (piece[0] == 'Q') {
                                result = g.add(PieceType::Queen, whoseTurn, rowB, colB);
                            } else if (piece[0] == 'K') {
                                result = g.add(PieceType::Knight, whoseTurn, rowB, colB);
                            } else if (piece[0] == 'B') {
                                result = g.add(PieceType::Bishop, whoseTurn, rowB, colB);
                            } else if (piece[0] == 'R') {
                                result = g.add(PieceType::Rook, whoseTurn, rowB, colB);
                            }
                            if (result == 1) {
                                if (whoseTurn == Colour::White) {
                                    cout << "Checkmate! White wins!" << endl;
                                    whiteScore += 1;
                                } else {
                                    cout << "Checkmate! Black wins!" << endl;
                                    blackScore += 1;
                                }
                            } else if (result == 2) {
                                cout << "Stalemate!" << endl;
                            }
                        }
                        if (whoseTurn == Colour::White) {
                            whoseTurn = Colour::Black;
                        } else {
                            whoseTurn = Colour::White;
                        }
                    } else {
                    }
                } else {
                    if (whoseTurn == Colour::White) {
                        int result = g.botMove(blackBot, whoseTurn);
                        if (result == 1) {
                            whiteScore += 1;
                            cout << "Checkmate! White wins!" << endl;
                        } else if (result == 2) {
                            cout << "Stalemate!" << endl;
                        }
                        whoseTurn = Colour::Black;
                    } else {
                        int result = g.botMove(blackBot, whoseTurn);
                        if (result == 1) {
                            blackScore += 1;
                            cout << "Checkmate! Black wins!" << endl;
                        } else if (result == 2) {
                            cout << "Stalemate!" << endl;
                        }
                        whoseTurn = Colour::White;
                    }
                }
            }
            cout << g;
        } else if (cmd == "setup") {
            g.init();
            whoseTurn = Colour::White;
            string setupCommand;
            while (true) {
                cout << g;
                cin >> setupCommand;
                if (setupCommand == "+") {
                    char tempPiece;
                    PieceType piece = PieceType::Pawn;
                    string cell;
                    int row, col;
                    cin >> tempPiece;
                    cin >> cell;
                    piece = findPiece(tempPiece);
                    col = findCol(cell[0]);
                    row = cell[1] - '0';
                    colour = findColour(tempPiece);
                    g.add(colour, piece, row - 1, col);
                } else if (setupCommand == "-") {
                    string cell;
                    int row, col;
                    cin >> cell;
                    col = findCol(cell[0]);
                    row = cell[1] - '0';
                    g.remove(row - 1, col);
                } else if (setupCommand == "=") {
                    string colour;
                    cin >> colour;
                    toupper(colour[0]);
                    if (colour[0] == 'W') {
                        whoseTurn = Colour::White;
                    } else if (colour[0]) {
                        whoseTurn = Colour::Black;
                    }
                } else {
                    break;
                }
            }
        } else if (cmd == "resign") {
            if (whoseTurn == Colour::White) {
                blackScore += 1;
                cout << "Black wins!" << endl;
            } else {
                whiteScore += 1;
                cout << "White wins!" << endl;
            }
        }
    }
    cout << "Final Score:" << endl;
    cout << "White: " << whiteScore << endl;
    cout << "Black: " << blackScore << endl;
}
