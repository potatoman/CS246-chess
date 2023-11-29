#ifndef __GRID_H__
#define __GRID_H__
#include <iostream>
#include <vector>
#include "cell.h"
#include "textdisplay.h"

enum class PieceType { Pawn, Bishop, Knight, Rook, Queen, King };
enum class Colour { White, Black };
enum class BotLevel { Level1, Level2, Level3 };

class Grid {
  std::vector<std::vector<Cell>> board;
  std::vector<Piece> player1pieces;
  std::vector<Piece> player2pieces;
  Cell *WKpos;
  Cell *BKpos;
  bool WKmoved, BKmoved, LWRmoved, RWRmoved, LBRmoved, RBRmoved;


  TextDisplay *td;



  bool legalMoveCheck(PieceType piece, int rowA, int colA, int rowB, int colB);
  bool checkCheck(colour colour);
  bool movementCheck(PieceType piece);

 public:
  Grid();
  ~Grid();

  
  

  void init();
  void add(colour colour, PieceType piece, int row, int col);
  void remove(int row, int col);

  void move(int rowA, int colA, int rowB, int colB);
  void botMove(int botLevel, colour colour);

  
  //friend std::ostream &operator<<(std::ostream &out, const Grid &g);
};

#endif