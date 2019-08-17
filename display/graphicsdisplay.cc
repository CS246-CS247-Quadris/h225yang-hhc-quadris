#include <iostream>
#include "graphicsdisplay.h"
#include "../board/subject.h"
#include "../board/board.h"
using namespace std;

GraphicsDisplay::GraphicsDisplay(int height, int width, int winHeight, int winWidth):
 height{height}, width{width}, winHeight{winHeight}, winWidth{winWidth}, xw{winWidth + 150, winHeight + 60} {
	//background
 	xw.fillRectangle(0, 0, winWidth +150, winHeight + 60, Xwindow::Black);
	//board
 	xw.fillRectangle(30, 30, winWidth, winHeight, Xwindow::White);
	//display title & member names
 	xw.fillRectangle(380, 30, 90, 60, Xwindow::White);
	xw.drawString(385, 45, "---Quadris---");
	xw.drawString(385, 55, "  Chuan Liu");
	xw.drawString(385, 65, " Hanzhen Yang");
	xw.drawString(385, 75, " Hongqian Wu");
	//display score
 	xw.fillRectangle(380, 220, 75, 110, Xwindow::White);
	xw.drawString(390, 240, "Level: ");
	xw.drawString(390, 270, "Hi Score: ");
	xw.drawString(390, 300, "Score: ");
	//display next block
 	xw.fillRectangle(380, 395, 75, 80, Xwindow::White);
	xw.drawString(390, 415, "Next: ");
}

void GraphicsDisplay::displayCell(Cell cell, int cellSize, bool remove = false){
	int x = cell.x * cellSize + 30;
	int y = (height - cell.y - 1) * cellSize + 30;
	if(remove){
		cell.text = '\0';
	}
	switch(cell.text){
		case 'I':
			xw.fillRectangle(x , y , cellSize, cellSize, Xwindow::Red);
		break;
		case 'J':
			xw.fillRectangle(x , y , cellSize, cellSize, Xwindow::Blue);
		break;
		case 'L':
			xw.fillRectangle(x , y , cellSize, cellSize, Xwindow::Orange);
		break;
		case 'O':
			xw.fillRectangle(x , y , cellSize, cellSize, Xwindow::Yellow);
		break;
		case 'S':
			xw.fillRectangle(x , y , cellSize, cellSize, Xwindow::Green);
		break;
		case 'Z':
			xw.fillRectangle(x , y , cellSize, cellSize, Xwindow::Gray);
		break;
		case 'T':
			xw.fillRectangle(x , y , cellSize, cellSize, Xwindow::Purple);
		break;
		case '*':
			xw.fillRectangle(x , y , cellSize, cellSize, Xwindow::Brown);
		break;
		case '?':
			xw.fillRectangle(x , y , cellSize, cellSize, Xwindow::Black);
			break;
		default:
			xw.fillRectangle(x , y , cellSize, cellSize, Xwindow::White);
		break;
	}
}

void GraphicsDisplay::notify(Subject* whoNotified) {
	Board* board = static_cast<Board *>(whoNotified);
	int cellSize = winHeight / height;
	vector<Cell> currBlock = board->getCurrentBlock()->getShape();
	vector<Cell> nextBlock = board->getNextBlock()->getShape();
	vector<vector<Cell>> theBoard = board->getBoard();
	int level = board->getCurrentLevel();
	
	if(level != currLv){
 		xw.fillRectangle(390, 245, 65, 15, Xwindow::White);
		xw.drawString(390, 255, to_string(level));
		currLv = level;
	}
	bool lastBlockDropped = false;
	int size = prevNextBlock.size();
	for(int i = 0; i < size; ++i){
		if(nextBlock[i].x != prevNextBlock[i].x || 
			nextBlock[i].y != prevNextBlock[i].y)
			lastBlockDropped = true;
	}

	// remove prev curr block
	for(auto cell : prevBlock){
		displayCell(cell, cellSize, true);	
	}

	// display board
	for(auto row : theBoard){
		for(auto cell : row){
			if(cell.text != ' ' || board->isLineCleared()){
				displayCell(cell, cellSize);	
			}
		}
	}

	// display curr block
	if(board->getisHint()) {
		for (auto cell : board->getHintBlock()) {
			currBlock.push_back(cell);
		}
	}
	for(auto cell : currBlock){
		displayCell(cell, cellSize);	
	}

	prevBlock = currBlock;

	if(lastBlockDropped || size == 0){
 		xw.fillRectangle(390, 275, 65, 15, Xwindow::White);
		xw.drawString(390, 285, to_string(board->getHighestScore()));
 		xw.fillRectangle(390, 305, 65, 15, Xwindow::White);
		xw.drawString(390, 315, to_string(board->getScore()));
		for(auto cell : prevNextBlock){
			cell.x += width * 2 + 2;
			cell.y -= 25;
			displayCell(cell, cellSize/2, true);	
		}
		for(auto cell : nextBlock){
			cell.x += width * 2 + 2;
			cell.y -= 25;
			displayCell(cell, cellSize/2);	
		}
	}
	prevNextBlock = nextBlock;
}
