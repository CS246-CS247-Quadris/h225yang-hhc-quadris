#ifndef ASSIGNMENT_TEXTDISPLAY_H
#define ASSIGNMENT_TEXTDISPLAY_H
#include <iostream>
#include <iomanip>
#include <vector>
#include "observer.h"
#include "../board/board.h"

struct Cell;
class Board;
class Block;

class TextDisplay: public Observer {
    std::vector<std::vector<char>> theDisplay;
    int height, width, currLv, score, hiscore;
    Block *nextBlock;
    Block *currBlock;
    std::vector<Cell> ghost;

    // private method
    void cleanDisplay();
    void updateDisplay();

public:
    TextDisplay(int, int);
    void notify(Subject *whoNotified) override;
    friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

#endif //ASSIGNMENT_TEXTDISPLAY_H
