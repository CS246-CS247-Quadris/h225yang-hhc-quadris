#include "level0.h"
#include "../board/board.h"
#include "../block/block.h"
using namespace std;

Level_0::Level_0()
  : Level{0}, currPos{0}
{}

unique_ptr<Block> Level_0::generateBlock(Board& b) {
  char letter = b.blockSeq.at(currPos);
  ++currPos;
  if (currPos == b.blockSeq.size()) currPos = 0;
  
  auto bp = make_unique<Block>(Cell{initX, initY, letter}, letter, false);
  return move(bp);
}
