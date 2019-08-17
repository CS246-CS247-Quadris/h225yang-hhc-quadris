#include "level1.h"
#include "../board/board.h"
#include "../block/block.h"
using namespace std;

Level_1::Level_1()
  : Level{1} 
{}

unique_ptr<Block> Level_1::generateBlock(Board& b) {
  char letter = blocks[rand() % 12];
  auto bp = make_unique<Block>(Cell{initX, initY, letter}, letter, false);
  return move(bp);
}
