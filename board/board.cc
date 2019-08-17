#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <memory>
#include "board.h"
#include "../command/stdcmd.h"
#include "../display/textdisplay.h"
#include "../display/graphicsdisplay.h"

using namespace std;


Board::Board() {};

void Board::init(vector<char> seq, size_t height, size_t width, size_t initial_lv,
                 int highscore, int seed, bool graphic_mode) {
    // clean the pre-exist board
    theBoard.clear();
    surface.clear();
    currBlock.reset();
    nextBlock.reset();
    currLevel.reset();
    td.reset();
    gd.reset();
    rowCount.clear();
    bonus_map.clear();
    bonus_vec.clear();
    hintBlock.clear();
    subscript();

    // initial the board
    graphicMode = graphic_mode;

    td = make_unique<TextDisplay>(height, width);
    attach(&(*td));

    if(graphicMode) {
        gd = make_unique<GraphicsDisplay>(height, width, 540, 330);
        attach(&(*gd));
    }

    boardHeight = height;
    boardWidth = width;
    blockSeq = seq;
    this->seed = seed;

    srand(seed);


    // main board
    theBoard.resize(boardHeight);
    for(size_t i = 0; i < boardHeight; ++i){
        for(size_t j = 0; j < boardWidth; ++j){
            theBoard[i].push_back(Cell{static_cast<int>(j), static_cast<int>(i), ' '});
        }
    }


    // surface
    surface.resize(boardWidth);
    for(size_t k = 0; k < boardWidth; ++k) {
        surface[k].push_back(Cell{static_cast<int>(k), -1, '*'}); // the initial surface, ground y = -1
    }

    // bonus
    bonus_map.resize(boardHeight);
    for(size_t i = 0; i < boardHeight; ++i){
        for(size_t j = 0; j < boardWidth; ++j){

            bonus_map[i].emplace_back(-1);
        }
    }


    // rowCount
    rowCount.resize(boardHeight, 0);


	stepCtr = 0;
    currScore = 0;
    highScore = highscore;
    bonus_index = 0;
    isHint = false;
    gg = false;
    random = true;

    currLevel = make_unique<Level_0>();
    for(size_t l = 0; l < initial_lv; ++l) {
        levelUp();
    }

    currBlock = currLevel->generateBlock(*this);
    nextBlock = currLevel->generateBlock(*this);

    notifyAll();
}


void Board::levelUp() {
    unique_ptr<Level> temp = levelFactory.levelup(*currLevel);
    srand(seed);
    if(temp->currLv != currLevel->currLv){
      stepCtr = 0;
      currLevel = move(temp);
	}
}


void Board::levelDown() {
    unique_ptr<Level> temp = levelFactory.leveldown(*currLevel);
    srand(seed);
    if(temp->currLv != currLevel->currLv){
		stepCtr = 0;
		currLevel = move(temp);
	}
}

bool Board::isEnd() {
    return gg;
}


void Board::setRandom(bool isRandom) {
    random = isRandom;
}


void Board::alterBlock(Cell where, char whatBlock) {
    auto temp = make_unique<Block>(where, whatBlock, currBlock->getIsHeavy()); // to check if game over
    currBlock = std::move(temp);
}

bool Board::getisHint() {
    return isHint;
}

vector<Cell> Board::getHintBlock() {
    return hintBlock;
}

void Board::hint() {
    isHint = true;
    Block hint{*currBlock};
    vector<int> fakeRowCount = rowCount;
    for (int i = 0; i < 4 ; ++i) {
        for (int k = 0; k <= hint.getLoc().x; ++k) {
        	for (int j = 0; j < i; ++j) {
				vector <Cell> temp = hint.fakeRotate(true);
				if (canItMove(temp)) {
        	    	hint.rotate(true);
				}
				else{
					break;
				}
        	}
            for (int j = 0; j <= k; ++j) {
                if(hint.getLoc().x > 0) {
                    vector <Cell> temp = hint.fakeLeft();
                    if (canItMove(temp)) {
                        hint.left();
                    }
					else{
						break;
					}
                }
				else{
					break;
				}
            }
            int min = boardHeight;
            determineStep(min, hint.getBotSurface());
            hint.down(min);
            for (auto cell : hint.getShape()) {
                fakeRowCount[cell.y]++;
            }
            for(size_t i = 0; i < boardHeight; ++i) {
                if(fakeRowCount[i] == static_cast<int>(boardWidth)) {
                    hintBlock = hint.getShape();
                    return;
                }
            }
            fakeRowCount = rowCount;
            hint = *currBlock;
        }
		int rightSpace = boardWidth - hint.getLoc().x - hint.getWidth();
        for (int l = 0; l <= rightSpace; ++l) {
        	for (int j = 0; j < i; ++j) {
				vector <Cell> temp = hint.fakeRotate(true);
				if (canItMove(temp)) {
        	    	hint.rotate(true);
				}
				else{
					break;
				}
        	}
            for (int j = 0; j < l; ++j) {
                if(hint.getLoc().x + hint.getWidth() <= boardWidth) {
                    vector<Cell> temp = hint.fakeRight();
                    if(canItMove(temp)) {
                        hint.right();
                    }
					else{
						break;
					}
                }
				else{
					break;
				}
            }
            int min = boardHeight;
            determineStep(min, hint.getBotSurface());
            hint.down(min);

            for (auto cell : hint.getShape()) {
                fakeRowCount[cell.y]++;
            }
            for(size_t i = 0; i < boardHeight; ++i) {
                if(fakeRowCount[i] == static_cast<int>(boardWidth)) {
                    hintBlock = hint.getShape();
                    return;
                }
            }
            fakeRowCount = rowCount;
            hint = *currBlock;
        }
    }
    int min = boardHeight;
    determineStep(min, hint.getBotSurface());
    hint.down(min);
    hintBlock = hint.getShape();
}


void Board::restart() {
    init(blockSeq, boardHeight, boardWidth, 0, highScore, seed, graphicMode);        // restart to level_0
}

void Board::isGameOver() {
    for(auto each : nextBlock->getShape()) {
        if(theBoard[each.y][each.x].text != ' ') {
            gg = true;
        }
    }
}

void Board::updateBonus() {
    for(auto cells : currBlock->getShape()) {
        bonus_map[cells.y][cells.x] = bonus_index;
    }
    int temp = static_cast<int>(currBlock->getShape().size());
    bonus_vec.push_back({temp, currLevel->currLv});
    bonus_index++;
}

void Board::updateBoard() {
    for(auto cells : currBlock->getShape()) {
        theBoard[cells.y][cells.x].text = cells.text;
        rowCount[cells.y]++;
    }
}

void Board::updateSurface() {
    for(auto each : currBlock->getTopSurface()) {
        surface[each.x].push_back(each);
    }
}

bool Board::determineDrop() {
    for(auto bot : currBlock->getBotSurface()) {
        for(size_t i = 0; i < surface[bot.x].size(); i++) {
            if(surface[bot.x][i].y == bot.y - 1) {
                return true;
            }
        }
    }
    return false;
}

void Board::switchBlock() {
    if(stepCtr % 5 == 0 && currLevel->currLv == 4 && stepCtr != 0){
        currBlock = currLevel->generateBlock(*this);

        auto queue = make_shared<vector<shared_ptr<StdCmd>>>();
        auto cmd = make_shared<StdCmd>("drop", 1);
        queue->emplace_back(cmd);
        perform(queue);
        --stepCtr;
    }
    else{
        currBlock = std::move(nextBlock);
        nextBlock = currLevel->generateBlock(*this);
    }
}

void Board::updateAllState() { // update board, only call when isDrop
    ++stepCtr;
    updateBoard();
    updateSurface();
    updateBonus();
    cleanCheck();
    isGameOver();
    if(!gg) switchBlock();
}

bool Board::isLineCleared() {
    return lineCleared;
}

void Board::cleanCheck() {
    int cleanLine = 0;
    bool isClean = false;
    for(size_t i = 0; i < boardHeight; ++i) {
        if(rowCount[i] == static_cast<int>(boardWidth)) {
            cleanLine++;
            isClean = true;
            lineCleared = true;
			stepCtr = 0;

            // bonus score
            for(size_t m = 0; m < boardWidth; ++m) {
                bonus_vec[bonus_map[i][m]].num--;
                if(bonus_vec[bonus_map[i][m]].num == 0) {
                    currScore += (bonus_vec[bonus_map[i][m]].level + 1) * (bonus_vec[bonus_map[i][m]].level + 1);
                }
            }

            for(size_t j = 0; j < boardWidth; ++j) {
                for(size_t k = i; k < boardHeight-1; ++k) {
                    bonus_map[k][j] = bonus_map[k+1][j];
                }
            }
            for(size_t j = 0; j < boardWidth; ++j) {
                for(size_t k = i; k < boardHeight-1; ++k) {
                    theBoard[k][j].text = theBoard[k+1][j].text;
                }
            }
            for(size_t l = i; l < boardHeight - 1; ++l) {
                rowCount[l] = rowCount[l + 1];
            }
            for(auto &each : surface) {
                for(auto &cells : each) {
                    if(cells.y == static_cast<int>(i)) {
                        cells.y = -1;
                    } else if(cells.y > static_cast<int>(i)) {
                        cells.y--;
                    }
                }
            }
            i--;
        }
    }

    // basic score
    if(isClean) {
        int point = currLevel->currLv + cleanLine;
        currScore += point * point;
        highScore = (currScore > highScore)? currScore: highScore;
    }
}


void Board::determineStep(int &min, vector<Cell> botSurface) {
    for(auto bot : botSurface) {
        for(auto &each : surface[bot.x]) {
            if((bot.y - each.y - 1) < min) {
                min = bot.y - each.y - 1;
            }
        }
    }
}

bool Board::canItMove(vector<Cell> temp) {
    for(auto each : temp) {
        if(each.x >= static_cast<int>(boardWidth) || each.y >= static_cast<int>(boardHeight)
           || theBoard[each.y][each.x].text != ' ') {
            return false;
        }
    }
    return true;
}

void Board::action(string recieverType, string cmd, string order, bool &isDrop) {
    if(recieverType == "Block") {
        if(cmd == "left") {
            if(currBlock->getLoc().x > 0) {
                vector<Cell> temp = currBlock->fakeLeft();
                if(canItMove(temp)) {
                    currBlock->left();
                    isDrop = determineDrop();
                }
            }
        } else if(cmd == "right") {
            if(currBlock->getLoc().x + currBlock->getWidth() < boardWidth) {
                vector<Cell> temp = currBlock->fakeRight();
                if(canItMove(temp)) {
                    currBlock->right();
                    isDrop = determineDrop();
                }
            }
        } else if(cmd == "down") {
            if(!(currBlock->getLoc().y == 14 && currBlock->getLoc().x == 0 && determineDrop())) {
                vector<Cell> temp = currBlock->fakeDown(1);
                if(canItMove(temp)) {
                    currBlock->down(1);
                    isDrop = determineDrop();
                }
            }
            isDrop = determineDrop();
        } else if(cmd == "drop") {
            isDrop = true;
            int min = boardHeight;
            determineStep(min, currBlock->getBotSurface());
            currBlock->down(min);
        } else if(cmd == "clockwise" || "counterclockwise") {
            bool isclockwise = (cmd == "clockwise");
            vector<Cell> temp = currBlock->fakeRotate(isclockwise);
            if(canItMove(temp)) {
                currBlock->rotate(isclockwise);
                isDrop = determineDrop();
            }
        }
        if(isDrop) {
            updateAllState();
        }
        cleanCheck();
    } else if(recieverType == "Level") {
        if(cmd == "levelup") {
            levelUp();
        } else if(cmd == "leveldown") {
            levelDown();
        }
    } else if(recieverType == "Board") {
        if(cmd == "norandom") {
            if(currLevel->currLv == 3 || currLevel->currLv == 4) {
                istringstream ss{order};
                char temp;
                vector<char> seq;
                while(ss >> temp) {
                    seq.push_back(temp);
                }
                blockSeq_for_3_4 = seq;
                random = false;
            }
        } else if(cmd == "random") {
            if(currLevel->currLv == 3 || currLevel->currLv == 4) {
                random = true;
            }
        } else if(cmd == "I" || cmd == "J" || cmd == "L" || cmd == "O"
                  || cmd == "S" || cmd == "Z" || cmd == "T" || cmd == "*") {
            alterBlock(currBlock->getLoc(),cmd[0]); // string to char
        } else if(cmd == "restart") {
            restart();
        } else if(cmd == "hint") {
            hint();
            for (auto &cell : hintBlock) {
                cell.text = '?';
            }
        }
    }
}

void Board::perform(shared_ptr<vector<shared_ptr<StdCmd>>> theCmd_ptr) {
    auto theCmd = *theCmd_ptr;
    string recieverType;
    string cmd;
    bool isDrop;
    for(size_t i = 0; i < theCmd.size(); ++i) {
        isDrop = false;
        recieverType = theCmd[i]->receiverType;
        cmd = theCmd[i]->basicCmd;
        for(int j = 0; j < theCmd[i]->times; ++j) {
            action(recieverType, cmd, theCmd[i]->order, isDrop);
            if(gg) break;
        }
        if(recieverType == "Block" && !isDrop && currBlock->getIsHeavy()) {
            action("Block", "down", "", isDrop);
        }
        notifyAll();
        isHint = false;
        lineCleared = false;
    }
}

Board::~Board() {}

int Board::getStepCtr(){ return stepCtr;}

int Board::getSeed() {
    return seed;
}

bool Board::isRandom() {
    return random;
}
std::vector<char> Board::get_3_4_seq() {
    return blockSeq_for_3_4;
}

std::vector<std::vector<Cell>> Board::getBoard(){
    return theBoard;
}

int Board::getCurrentLevel(){
    return currLevel->currLv;
}

int Board::getScore(){
    return currScore;
}
int Board::getHighestScore(){
    return highScore;
}

Block* Board::getCurrentBlock(){
    return &(*currBlock);
}

Block* Board::getNextBlock(){
    return &(*nextBlock);
}

std::ostream &operator<<(std::ostream &out, const Board &b) {
    out << *(b.td);
    return out;
}




