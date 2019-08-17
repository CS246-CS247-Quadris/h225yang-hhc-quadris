#ifndef GRAPHICSDISPLAY_H
#define GRAPHICSDISPLAY_H
#include <iostream>
#include <vector>
#include "observer.h"
#include "window.h"

class Cell;
class GraphicsDisplay: public Observer {
  const int height, width, winHeight, winWidth;
  int currLv = -1;
  std::vector<Cell> prevBlock, prevNextBlock;
  Xwindow xw;

 public:
  GraphicsDisplay(int height, int width, int winHeight, int winWidth);

  void notify(Subject *whoNotified) override;
  void displayCell(Cell, int, bool);
};
#endif
