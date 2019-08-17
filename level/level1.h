#ifndef _LEVEL_1_H_
#define _LEVEL_1_H_
#include <string>
#include <vector>
#include <memory>
#include "level.h"

class Board;
class Block;

class Level_1 : public Level {
  std::string blocks = "SZIIJJLLOOTT";
public:
  Level_1();
  std::unique_ptr<Block> generateBlock(Board&) override;
};

#endif
