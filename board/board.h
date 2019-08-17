#ifndef ASSIGNMENT_BOARD_H
#define ASSIGNMENT_BOARD_H

#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include "subject.h"
#include "../block/block.h"
#include "../command/command.h"
#include "../level/levelfactory.h"


class Level;
class TextDisplay;
class GraphicsDisplay;

struct Bonus {
    int num;
    int level;
};


class Board: public Subject {
    std::vector<std::vector<Cell>> theBoard;
    std::vector<std::vector<Cell>> surface;
    std::vector<int> rowCount;
    std::unique_ptr<Block> currBlock;
    std::unique_ptr<Block> nextBlock;
    std::vector<Cell> hintBlock;
    bool isHint;
    int currScore;
    int highScore;
    int stepCtr;
    int seed;
    bool gg;
    bool random;
    bool graphicMode;
    bool lineCleared;
    std::unique_ptr<Level> currLevel;
    std::vector<char> blockSeq_for_3_4;
    size_t boardHeight;
    size_t boardWidth;
    std::unique_ptr<TextDisplay> td;
    std::unique_ptr<GraphicsDisplay> gd;
    LevelFactory levelFactory;
    std::vector<Bonus> bonus_vec;
    std::vector<std::vector<int>> bonus_map;
    int bonus_index;

    // private method
    void updateAllState();
    void cleanCheck();
    void isGameOver();
    void updateBoard();
    void updateSurface();
    void switchBlock();
    bool determineDrop();
    void determineStep(int &, std::vector<Cell>);
    void updateBonus();
    void action(std::string, std::string, std::string, bool &);
    bool canItMove(std::vector<Cell> temp);
    void levelUp();
    void levelDown();
    void hint();
    void alterBlock(Cell, char);

public:
    Board();
    ~Board();

    bool isEnd();
    bool isRandom();
    bool isLineCleared();
    std::vector<char> blockSeq;
    void init(std::vector<char>, size_t height, size_t width, size_t initial_lv,
              int highscore, int seed, bool graphic_mode);
    void perform(std::shared_ptr<std::vector<std::shared_ptr<StdCmd>>>);
    void setRandom(bool);
    void restart();

	int getStepCtr();
    int getSeed();
    bool getisHint();
    std::vector<char> get_3_4_seq();
    std::vector<Cell> getHintBlock();
    std::vector<std::vector<Cell>> getBoard();
    int getCurrentLevel();
    int getScore();
    int getHighestScore();
    Block* getCurrentBlock();
    Block* getNextBlock();

    friend std::ostream &operator<<(std::ostream &out, const Board &b);
};

#endif //ASSIGNMENT_BOARD_H
