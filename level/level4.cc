#include "level4.h"
#include "../board/board.h"
#include "../block/block.h"
using namespace std;

Level_4::Level_4()
  : Level{4}
{}

unique_ptr<Block> Level_4::generateBlock(Board& b){
  if(b.getStepCtr() % 5 == 0 && b.getStepCtr() != 0) 
    return make_unique<Block>(Cell{5, 14, '*'}, '*', true);

  char letter;
  if(b.isRandom()) letter = blocks[rand() % 9];
  else {
    letter = b.get_3_4_seq()[currPos];
    ++currPos;
    if(currPos == b.get_3_4_seq().size()) currPos = 0;
  }
 
  auto bp = make_unique<Block>(Cell{initX, initY, letter}, letter, true);
  return move(bp);
}
