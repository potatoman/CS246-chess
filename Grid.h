#ifndef __GRID_H__
#define __GRID_H__
#include <iostream>
#include <vector>
#include "Cell.h"
#include "TextDisplay.h"

enum class PieceType { Pawn, Bishop, Knight, Rook, Queen, King, None };
enum class Colour { White, Black, None };
enum class BotLevel { Level1, Level2, Level3 };

class Grid {
  std::vector<std::vector<Cell>> board;
  std::vector<PieceType> player1pieces;
  std::vector<PieceType> player2pieces;
  Cell *WKpos;
  Cell *BKpos;
  bool WKmoved, BKmoved, LWRmoved, RWRmoved, LBRmoved, RBRmoved = false;


  TextDisplay *td;



  bool legalMoveCheck(PieceType piece, int rowA, int colA, int rowB, int colB);
  bool checkCheck(Colour colour);
  bool movementCheck(PieceType piece, Colour colour, int rowA, int colA, int rowB, int colB);
  bool checkCheckMate(Colour colour);
  bool stalemateCheck(Colour colour);
  bool blockCheck(PieceType piece, Colour colour, int rowA, int colA, int rowB, int colB);

 public:
  Grid();
  ~Grid();

  
  

  void init();
  void add(Colour colour, PieceType piece, int row, int col);
  void remove(int row, int col);

  int move(int rowA, int colA, int rowB, int colB);
  void botMove(int botLevel, Colour colour);

  
  //friend std::ostream &operator<<(std::ostream &out, const Grid &g);
};

#endif