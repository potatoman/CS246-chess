#ifndef __GRAPHICSDISPLAY_H__
#define __GRAPHICSDISPLAY_H__
#include "Cell.h"
#include "window.h"

class GraphicsDisplay {
    Xwindow &window;
    int height, width = 500;
    int gridSize = 64;
    int cellSize = 63;
    void draw(Cell &c);
  public:
    GraphicsDisplay(Xwindow& w);
    void notify(Cell &c);
    void init();
    void drawCoords(Cell &c);

};

#endif
