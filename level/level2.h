#ifndef _LEVEL_2_H_
#define _LEVEL_2_H_
#include <string>
#include <vector>
#include <memory>
#include "level.h"

class Board;
class Block;

class Level_2 : public Level {
  std::string blocks = "IJLOSZT";
public:
	Level_2();
	std::unique_ptr<Block> generateBlock(Board&) override;
};

#endif
