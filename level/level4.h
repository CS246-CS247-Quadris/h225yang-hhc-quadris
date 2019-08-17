#ifndef _LEVEL_4_H_
#define _LEVEL_4_H_
#include <string>
#include <vector>
#include <memory>
#include "level.h"

class Board;
class Block;

class Level_4 : public Level {
	int blockDropCtr = 1;
	std::string blocks = "IJLOSSZZT";
  size_t currPos = 0;
public:	
	Level_4();
	std::unique_ptr<Block> generateBlock(Board&) override;	
};
#endif
