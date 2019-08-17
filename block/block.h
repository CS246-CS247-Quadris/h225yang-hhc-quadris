#ifndef BLOCK_H
#define BLOCK_H
#include "cell.h"
#include <vector>
#include <cstddef>
#include <string>
#include <map>

//struct StdCmd;

class Block{
	static std::map<char, std::vector<Cell>> ORIGIN_BLOCKS;
	Cell loc;
	char type;
	std::vector<Cell> shape;
	bool isHeavy;
	std::vector<Cell> topSurface;
	std::vector<Cell> botSurface;
	size_t height;
	size_t width;
	void determineSurface();
	public:
	Block(Cell, char, bool);
	Block(Cell, char, std::vector<Cell>, bool);
	void down(int);
	void left();
	void right();
	void rotate(bool);		
	std::vector<Cell> fakeRotate(bool);
	std::vector<Cell> fakeDown(int step);
	std::vector<Cell> fakeLeft();
	std::vector<Cell> fakeRight();
	
	size_t getWidth() const;
	size_t getHeight() const;
	bool getIsHeavy() const;
	void setWidth(int);
	void setHeight(int);
	void setIsHeavy(bool);
	std::vector<Cell> getTopSurface() const;
	std::vector<Cell> getBotSurface() const;
	std::vector<Cell> getShape() const;
	Cell getLoc() const;
};

std::ostream &operator<<(std::ostream &out, const Block &b);

#endif
