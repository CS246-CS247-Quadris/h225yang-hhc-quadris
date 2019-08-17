#include "textdisplay.h"
#include "../block/block.h"
#include "../level/level.h"
using namespace std;

TextDisplay::TextDisplay(int height, int width):
        height{height}, width{width}, nextBlock{nullptr}, currBlock{nullptr} {
    theDisplay.resize(height);
    for(int i = 0; i < height; ++i){
        for(int j = 0; j < width; ++j){
            theDisplay[i].emplace_back(' ');
        }
    }
}

void TextDisplay::cleanDisplay() {
    for(auto each : ghost) {
        theDisplay[each.y][each.x] = ' ';
    }
}

void TextDisplay::updateDisplay() {
    for(auto each : ghost) {
        theDisplay[each.y][each.x] = each.text;
    }
}

void TextDisplay::notify(Subject *whoNotified) {
    Board *temp = static_cast<Board *>(whoNotified);
    cleanDisplay();
    for(int i = 0; i < height; ++i){
        for(int j = 0; j < width; ++j){
            theDisplay[i][j] = temp->getBoard()[i][j].text;
        }
    }
    score = temp->getScore();
    hiscore = temp->getHighestScore();
    currLv = temp->getCurrentLevel();
    nextBlock = temp->getNextBlock();
    currBlock = temp->getCurrentBlock();
    ghost = currBlock->getShape();
    if(temp->getisHint()) {
        for (auto cell : temp->getHintBlock()) {
            ghost.push_back(cell);
        }
    }
    updateDisplay();
}

std::ostream &operator<<(std::ostream &out, const TextDisplay &td) {
    out << "Level:" << setw(7) << td.currLv << endl;
    out << "Score:" << setw(7) << td.score << endl;
    out << "Hi Score:" << setw(4) << td.hiscore << endl;
    for(int k = 0; k < td.width; ++k) {
        out << "-";
    }
    out << endl;

    for(int i = td.height - 1; i >= 0; --i){
        for(int j = 0; j < td.width; ++j){
            out << td.theDisplay[i][j];
        }
        out << endl;
    }

    for(int k = 0; k < td.width; ++k) {
        out << "-";
    }
    out << endl;
    out << "Next:" << endl;
    out << *(td.nextBlock);
    return out;
}



