#ifndef _LEVEL_0_H_
#define _LEVEL_0_H_
#include <string>
#include <vector>
#include <memory>
#include "level.h"

class Board;
class Block;

class Level_0 : public Level {
  size_t currPos;
public:
  Level_0();
  std::unique_ptr<Block> generateBlock(Board&) override;
};

#endif
