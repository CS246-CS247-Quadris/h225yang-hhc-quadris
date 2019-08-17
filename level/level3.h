#ifndef _LEVEL_3_H_
#define _LEVEL_3_H_
#include <string>
#include <vector>
#include <memory>
#include "level.h"

class Board;
class Block;

class Level_3 : public Level {
	std::string blocks = "IJLOSSZZT";
  size_t currPos = 0;
public:	
	Level_3();
	std::unique_ptr<Block> generateBlock(Board&) override;	
};

#endif
