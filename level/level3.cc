#include "level3.h"
#include "../board/board.h"
#include "../block/block.h"
using namespace std;

Level_3::Level_3()
  : Level{3}
{}

unique_ptr<Block> Level_3::generateBlock(Board& b) {
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
