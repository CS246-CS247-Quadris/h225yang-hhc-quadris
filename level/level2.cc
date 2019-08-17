#include "level2.h"
#include "../board/board.h"
#include "../block/block.h"
using namespace std;

Level_2::Level_2() 
  : Level{2}
{}

unique_ptr<Block> Level_2::generateBlock(Board& b){
  char letter = blocks[rand() % 7];
  auto bp = make_unique<Block>(Cell{initX, initY, letter}, letter, false);
  return move(bp);
}
