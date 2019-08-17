#include "levelfactory.h"
using namespace std;

unique_ptr<Level> LevelFactory::levelup(Level& currLv){
	if(currLv.currLv == 4) return move(make_unique<Level_4>());
	int nextLv = currLv.currLv + 1;
	if(nextLv == 1){ 
		return move(make_unique<Level_1>());
	}
	if(nextLv == 2) return move(make_unique<Level_2>());
	if(nextLv == 3) return move(make_unique<Level_3>());
	return move(make_unique<Level_4>());
}

unique_ptr<Level> LevelFactory::leveldown(Level& currLv){
  if(currLv.currLv == 0) return move(make_unique<Level_0>());
  int nextLv = currLv.currLv - 1;
  if(nextLv == 0) return move(make_unique<Level_0>());
  if(nextLv == 1) return move(make_unique<Level_1>());
  if(nextLv == 2) return move(make_unique<Level_2>());
  return move(make_unique<Level_3>());
}
