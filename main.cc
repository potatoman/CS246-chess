#include <iostream>
#include <string>
#include "Grid.h"
using namespace std;

PieceType findPiece (char piece) {
    toupper(piece);
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
    }
}

int findCol(char col) {
    toupper(col);
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
    }
}

int main() {
    string cmd;
    Grid g;
    Colour colour;

    while(cin >> cmd) {
        if (cmd == "game") {
            
        } else if (cmd == "move") {
            //TODO - add check to make sure they are moving their own pieces
            int rowA, colA, rowB, colB;
            string initialPos;
            string endPos;
            cin >> initialPos;
            cin >> endPos;
            colA = findCol(initialPos[0]);
            colB = findCol(endPos[0]);
            rowA = initialPos[1] - '0';
            rowB = endPos[1] - '0';
            if (rowA >= 8 || rowB >= 8 || colA >= 8 || colB >= 8) {
                cout << "invalid coordinates" << endl;
                continue;
            }
            if (g.move(rowA, colA, rowB, colB) == 1) {
                //end game
            }
        } else if (cmd == "setup") {
            string setupCommand;
            while (true) {
                cin >> setupCommand;
                if (setupCommand == "+") {
                    char tempPiece;
                    PieceType piece = PieceType::Pawn;
                    string cell;
                    int row, col;
                    cin >> tempPiece;
                    cin >> cell;
                    toupper(tempPiece);
                    piece = findPiece(tempPiece);
                    col = findCol(cell[0]);
                    row = cell[1] - '0';
                    g.add(colour, piece, row, col);
                } else if (setupCommand == "-") {
                    string cell;
                    int row, col;
                    cin >> cell;
                    col = findCol(cell[0]);
                    row = cell[1] - '0';
                    g.remove(row, col);
                } else if (setupCommand == "=") {
                    if (colour == Colour::White) {
                        colour = Colour::Black;
                    } else {
                        colour = Colour::White;
                    }
                } else {
                    break;
                }
            }
        } else if (cmd == "resign") {

        }
    }

}