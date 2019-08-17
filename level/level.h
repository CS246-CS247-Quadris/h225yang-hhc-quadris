#ifndef _LEVEL_H_
#define _LEVEL_H_
#include <string>
#include <vector>
#include <memory>

class Board;
class Block;

const int initX = 0;
const int initY = 14;

class Level {
public:
  const int currLv;
  Level(int);
  virtual std::unique_ptr<Block> generateBlock(Board&)=0; // Cannot use const function since Level_0 needs to modify currPos
  virtual ~Level()=0;
};

#endif
