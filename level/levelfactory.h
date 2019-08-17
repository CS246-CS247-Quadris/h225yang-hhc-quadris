#include "level.h"
#include "level0.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"
#include "level4.h"
#include <memory>

class LevelFactory{
	public:
	std::unique_ptr<Level> levelup(Level&);
	std::unique_ptr<Level> leveldown(Level&);
};
