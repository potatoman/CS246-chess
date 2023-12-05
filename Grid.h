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
  bool WKmoved, BKmoved, LWRmoved, RWRmoved, LBRmoved, RBRmoved = false;

  GraphicsDisplay *gd{};
  Xwindow x;
  TextDisplay *td;

  //adds pieces to the board and vector when initially setting up the board
  void add(Colour pieceColour, int rowA, int colA, int rowB, int colB);

  //removes the piece in row col its vector of pieces
  void removePieceFromVector(int row, int col);

  //adds the piece in row col to its respective vector of pieces
  void addPieceToVector(Colour pieceColour, PieceType piece, int row, int col);
  
  //adds the piece in row col to its respective vector of pieces
  void addPieceToVector(Piece piece);

  //determines if a move from rowA colA to rowB colB is possible for the piece and the given board state
  bool legalMoveCheck(PieceType piece, int rowA, int colA, int rowB, int colB);

  //will call updateCellsThreatening on every rook, bishop since their cells threatened changes 
  //whenever a piece gets out of the way and calls updateThreatStatus on every piece that is under attack
  //to check if it is now being blocked by another piece or if the move made it no longer be under attack
  //does this for all white and black pieces
  void updateAllThreats(int row, int col);

  void updateAllThreats();

  //will return true if king is in check by checking is the king's underattack boolean is true
  bool checkCheck(Colour colour);

  //checks if the move from rowA colA to rowB colB is allowed for the given piece, ignoring any pieces that might be blocking
  bool movementCheck(PieceType piece, Colour colour, int rowA, int colA, int rowB, int colB);

  //returns true if the colour is in checkmate. Does this by checking if the colour has any legal moves,
  //if it does then its not in checkmate
  bool checkCheckMate(Colour colour);

  //same as checkCheckMate because you're just checking if the colour has any legal moves
  bool stalemateCheck(Colour colour);

  //randomizes the order of a vector array containing pieces
  void randomize(Colour colour);
 public:
  Grid();
  ~Grid();

  
  
  //initializes the base chess board, along with text and graphic display
  void init();

  //finds a given cell
  Cell* findCell(int r, int c);

  //effectively how we actually move a piece from point A to B
  void add(Colour pieceColour, PieceType piece, int row, int col);
  
  //removes the piece in row col from its vector of pieces and from the cell
  void remove(int row, int col);

  // update pieces locations and potentially remove from pieces vector if theres a capture
  int move(int rowA, int colA, int rowB, int colB);

  void botMove(BotLevel botLevel, Colour colour);

  void botLvl1(Colour colour);
  void botLvl2(Colour colour);
  void botLvl3(Colour colour);
  void botLvl4(Colour colour);
  int convertValues(PieceType piece);

  //returns false if theres nothing blocking the move (no piece in between the coordinates)
  bool blockCheck2(int rowA, int colA, int rowB, int colB);

  //returns the number of pieces in an vector
  int getNumOfPiece(Colour pieceColour);

  //returns a pointer to a piece based off of iteration
  Piece* getPiece(Colour pieceColour, int index);

  //returns a pointer to a piece based on position
  Piece* getPiece(Colour pieceColour, int r, int c);

  //returns a piece object based on position
  Piece getPieceObject(Colour pieceColour, int r, int c);

  std::vector<Piece> * getPiecesArray(Colour colour);

  //prints out the grid
  friend std::ostream &operator<<(std::ostream &out, const Grid &g);
};

#endif