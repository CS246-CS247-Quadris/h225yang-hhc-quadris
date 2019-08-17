#include "block.h"
#include <iostream>
using namespace std;

map<char, vector<Cell>> Block::ORIGIN_BLOCKS
{
	{'I', {Cell{0, 0, 'I'}, Cell{1, 0, 'I'}, Cell{2, 0, 'I'}, Cell{3, 0, 'I'}}},
	{'J', {Cell{0, 0, 'J'}, Cell{1, 0, 'J'}, Cell{2, 0, 'J'}, Cell{0, 1, 'J'}}},
	{'L', {Cell{0, 0, 'L'}, Cell{1, 0, 'L'}, Cell{2, 0, 'L'}, Cell{2, 1, 'L'}}},
	{'O', {Cell{0, 0, 'O'}, Cell{1, 0, 'O'}, Cell{0, 1, 'O'}, Cell{1, 1, 'O'}}},
	{'S', {Cell{0, 0, 'S'}, Cell{1, 0, 'S'}, Cell{1, 1, 'S'}, Cell{2, 1, 'S'}}},
	{'Z', {Cell{1, 0, 'Z'}, Cell{2, 0, 'Z'}, Cell{0, 1, 'Z'}, Cell{1, 1, 'Z'}}},
	{'T', {Cell{0, 1, 'T'}, Cell{1, 1, 'T'}, Cell{2, 1, 'T'}, Cell{1, 0, 'T'}}},
	{'*', {Cell{0, 0, '*'}}}
};

//Two-param constructor which will generate 8 original blocks.
Block::Block(Cell loc, char type, bool isHeavy):Block{loc, type, ORIGIN_BLOCKS.at(type), isHeavy}{}

Block::Block(Cell loc, char type, vector<Cell> shape, bool isHeavy):loc{loc}, type{type}, shape{shape}, isHeavy{isHeavy}{
	width = 0;
	height = 0;
	//find the height and width of the block
	for(auto& cell : this->shape){
		if(cell.x > static_cast<int>(width))
			width = cell.x;
		if(cell.y > static_cast<int>(height))
			height = cell.y;
		//set the coordinates of the cells relative to location
		cell.x += loc.x;
		cell.y += loc.y;
	}
	width++;
	height++;
	determineSurface();
}

vector<Cell> Block::fakeDown(int step){
	Block temp{*this};
	temp.down(step);
	return temp.getShape();
}

vector<Cell> Block::fakeLeft(){
	Block temp{*this};
	temp.left();
	return temp.getShape();
}

vector<Cell> Block::fakeRight(){
	Block temp{*this};
	temp.right();
	return temp.getShape();
}


void Block::down(int step){
	int size = shape.size();
	for(int i = 0; i < size; ++i){
		shape[i].y -= step;
	}
	loc.y -= step;
	determineSurface();
}

void Block::left(){
	int size = shape.size();
	for(int i = 0; i < size; ++i){
		--shape[i].x;
	}
	--loc.x;
	determineSurface();
}

void Block::right(){
	int size = shape.size();
	for(int i = 0; i < size; ++i){
		++shape[i].x;
	}
	++loc.x;
	determineSurface();
}

void Block::rotate(bool isClockwise){
	int size = shape.size();
	int temp;
	temp = height;
	height = width;
	width = temp;
	for(int i = 0; i < size; ++i){
		temp = shape[i].x - loc.x;
		shape[i].x = shape[i].y - loc.y;
		shape[i].y = temp;
		if(isClockwise)
			shape[i].y = height - shape[i].y - 1; 
		else
			shape[i].x = width - shape[i].x - 1;	
		shape[i].x += loc.x;
		shape[i].y += loc.y;
	}
	determineSurface();
}

vector<Cell> Block::fakeRotate(bool isClockwise){
	Block temp{*this};
	temp.rotate(isClockwise);
	return temp.getShape();
}


size_t Block::getWidth() const {return width;}
size_t Block::getHeight() const {return height;}
bool Block::getIsHeavy() const {return isHeavy;}
void Block::setWidth(int width){this->width = width;}
void Block::setHeight(int height){this->height = height;}
void Block::setIsHeavy(bool isHeavy){this->isHeavy = isHeavy;}
std::vector<Cell> Block::getTopSurface() const {return topSurface;}
std::vector<Cell> Block::getBotSurface() const {return botSurface;}
std::vector<Cell> Block::getShape() const {return shape;}
Cell Block::getLoc() const {return loc;}

/*-------------------------------------*/
void Block::determineSurface(){
	vector<Cell> top;
	vector<Cell> bot;
	top.resize(width);
	bot.resize(width);
	int relative_x;
	for(Cell cell: shape){
		relative_x = width - cell.x + loc.x - 1;
		if(top[relative_x].text == '\0' || top[relative_x].y < cell.y)
			top[relative_x] = cell;
		if(bot[relative_x].text == '\0' || bot[relative_x].y > cell.y)
			bot[relative_x] = cell;
	}
	topSurface.clear();
	botSurface.clear();
	for(size_t i = 0; i < width; ++i){
		if(top[i].text != '\0')
			topSurface.push_back(top[i]);
		if(bot[i].text != '\0')
			botSurface.push_back(bot[i]);
	}
}

std::ostream &operator<<(std::ostream &out, const Block &block) {
	vector<Cell> shape = block.getShape();
	Cell loc = block.getLoc();
	vector<vector<Cell>> vec;
	vector<Cell> row;
	for(size_t j = 0; j < block.getHeight(); ++j){
		for(size_t i = 0; i < block.getWidth(); ++i){
			row.emplace_back(Cell{0 , 0, ' '});
		}
		vec.push_back(row);
		row.clear();
	}

	int size = shape.size();
	for(int i = 0; i < size; ++i){
		vec[shape[i].y - loc.y][shape[i].x - loc.x].text = shape[i].text;
	}

	for(int i = block.getHeight() - 1; i >= 0; --i){
		for(size_t j = 0; j < block.getWidth(); ++j){
			out<<vec[i][j].text;
		}
		out<<endl;
	}
	return out;
}











