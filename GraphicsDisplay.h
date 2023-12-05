#ifndef __GRAPHICSDISPLAY_H__
#define __GRAPHICSDISPLAY_H__
#include "Cell.h"
#include "window.h"

class GraphicsDisplay {
    Xwindow &window;
    int height, width = 500;
    int gridSize = 64;
    int cellSize = 63;
    //draws the pieces and the chess board based off a given cell
    void draw(Cell &c);
    //draws out the coordinates in the respective squares (a-h, 1-8)
    void drawCoords(Cell &c);
  public:
    GraphicsDisplay(Xwindow& w);
    //encapsulates draw and allows us to call drawCoords after everything has been drawn
    void notify(Cell &c);
    //initializes cell and grid size
    void init();

};

#endif
