#ifndef __GRID_H__
#define __GRID_H__
#include <vector>
#include "Cell.h"
#include "TextDisplay.h"
#include "GraphicsDisplay.h"
#include "Piece.h"
#include "window.h"
class Piece;

enum class BotLevel { Level1, Level2, Level3, Level4 };

class Grid {
  std::vector<std::vector<Cell>> board;
  std::vector<Piece> WhitePieces;
  std::vector<Piece> BlackPieces;
  Cell *WKpos;
  Cell *BKpos;
  bool WKmoved, BKmoved, LWRmoved, RWRmoved, LBRmoved, RBRmoved = false;

  GraphicsDisplay *gd{};
  Xwindow x;
  TextDisplay *td;



  bool legalMoveCheck(PieceType piece, int rowA, int colA, int rowB, int colB);
  bool checkCheck(Colour colour);
  bool movementCheck(PieceType piece, Colour colour, int rowA, int colA, int rowB, int colB);
  bool checkCheckMate(Colour colour);
  bool stalemateCheck(Colour colour);
  bool blockCheck(PieceType piece, Colour colour, int rowA, int colA, int rowB, int colB);
  void removePieceFromVector(int row, int col);
  void addPieceToVector(Colour pieceColour, PieceType piece, int row, int col);
  void addPieceToVector(Piece piece);
  void add(Colour pieceColour, int rowA, int colA, int rowB, int colB);
  void updateAllThreats(int row, int col);

 public:
  Grid();
  ~Grid();

  
  

  void init();
  void add(Colour pieceColour, PieceType piece, int row, int col);
  void remove(int row, int col);
  Cell* findCell(int r, int c);
  int getNumOfPiece(Colour pieceColour);
  Piece* getPiece(Colour pieceColour, int index);
  Piece* getPiece(Colour pieceColour, int r, int c);
  int move(int rowA, int colA, int rowB, int colB);
  void botMove(BotLevel botLevel, Colour colour);
  bool blockCheck2(int rowA, int colA, int rowB, int colB);

  
  friend std::ostream &operator<<(std::ostream &out, const Grid &g);
};

#endif