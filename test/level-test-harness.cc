#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <memory>
#include "../board/board.h"
#include "../block/block.h"
#include "../command/command.h"
#include "../command/sequence.h"
#include "../command/stdcmd.h"
using namespace std;

// Test function for printing blocks
void print(Block& block){
  vector<Cell> shape = block.getShape();
  vector<vector<Cell>> vec;
  vector<Cell> row;
  cout<<"==================="<<endl;
  for(int j = 0; j < 18 ; ++j){
    for(int i = 0; i < 11; ++i){
      row.emplace_back(Cell{i + j * 11 , 0, '-'});
    }
    vec.push_back(row);
    row.clear();
  }

  int size = shape.size();
  for(int i = 0; i < size; ++i){
    vec[shape[i].y][shape[i].x].text = shape[i].text;
  }
  cout<<"=========top surface=========="<<endl;
  for(auto cell : block.getTopSurface()){
    cout<<"x: "<<cell.x<<"   y: "<<cell.y<<endl;
    cout<<"text: "<<cell.text<<endl;
  }
  cout<<"=========top surface=========="<<endl;
  cout<<"=========bot surface=========="<<endl;
  for(auto cell : block.getBotSurface()){
    cout<<"x: "<<cell.x<<"   y: "<<cell.y<<endl;
  }
  cout<<"=========bot surface=========="<<endl;
  for(int i = 17; i >= 0; --i){
    for(int j = 0; j < 11; ++j){
      cout<<vec[i][j].text;
    }
    cout<<endl;
  }
  cout<<"==================="<<endl;
}

int main(int argc, char* argv[]) {
    bool graphic_mode = true;
    int seed = 0;
    string scriptfile = "sequence.txt";
    int start_level = 0;
    
    for(int i = 1; i < argc ; ++i) {
        istringstream arg1(argv[i]);
        string flag;
        arg1 >> flag;
        if(flag == "-text") {
            graphic_mode = false;
        } else if(flag == "-scriptfile") {
            scriptfile = argv[i + 1];
            i++;
        } else if(flag == "-seed") {
            seed = stoi(argv[i + 1]);
            i++;
        } else if(flag == "-startlevel") {
            start_level = stoi(argv[i + 1]);
            i++;
        } else {
            cerr << "invalid flags" << endl;
            return 1;
        }
    }

    Sequence seq{scriptfile};
    vector<char> seq_char;
    for(auto c : seq.theSequence) {
        seq_char.push_back(c[0]);
    }

    Board b;

    b.init(seq_char, 18, 11, start_level, 0, seed, false);


    //TODO graphic, seed flags

    cout << b;

    Command cmdbook;
    auto queue = make_shared<vector<shared_ptr<StdCmd>>>();

    string s;
    while(cin >> s) {
        if (s=="add") {
            string alter, basicCmd;
            cin >> alter >> basicCmd;
            cmdbook.addAlter(alter, basicCmd);
        }
        else if (s=="rename") {
            string oldName, newName;
            cin >> oldName >> newName;
            cmdbook.rename(oldName, newName);
        }
        else {
            auto sc = cmdbook.smartLookup(s);
            if (sc) {
                if (sc->basicCmd == "norandom" || sc->basicCmd == "sequence") {
                    string fileName;
                    cin >> fileName;
                    Sequence seq{fileName};
                    queue = cmdbook.translate(sc->basicCmd, seq);
                }
                else {
                    queue = cmdbook.translate(s);
                }
            }
            //printQueue(queue);
        }

        cout << "here" << endl;

        b.perform(queue);

        cout << b;
        if(b.isEnd()) {
            cout << "Game Over" << endl;
            break;
        }
    }
}




// Level Module Test Harness
/*int main() {
  Board b;
  vector<char> sequence = {'L', 'L', 'L'};
  b.init(sequence, 18, 11, 0, 0, 1);
  // test for level 0
  
  auto lp = make_shared<Level_0>(); 
  for (size_t i=0; i<sequence.size(); ++i) {
    auto bp = lp->generateBlock(b);
    print(*bp);
  }

  // test for level 1
  // enter any thing to trigger generateBlock(), see different shapes
  //   on each generation to show "random"
  auto lp = make_shared<Level>(0); 
  Block *bp = nullptr;
  string s;
  Board board;
  while (cin >> s) {
    bp = lp->generateBlock(b);
    print(*bp);
  }
  
  return 0;
}*/

/*
int main(int argc, char* argv[]) {
    bool graphic_mode = true;
    int seed = 0;
    string scriptfile = "sequence.txt";
    int start_level = 0;
    for(int i = 1; i < argc ; ++i) {
        istringstream arg1(argv[i]);
        string flag;
        arg1 >> flag;
        if(flag == "-text") {
            graphic_mode = false;
        } else if(flag == "-scriptfile") {
            scriptfile = argv[i + 1];
            i++;
        } else if(flag == "-seed") {
            seed = stoi(argv[i + 1]);
            i++;
        } else if(flag == "-startlevel") {
            start_level = stoi(argv[i + 1]);
            i++;
        } else {
            cerr << "invalid flags" << endl;
            return 1;
        }
    }

    Sequence seq{scriptfile};
    vector<char> seq_char;
    for(auto c : seq.theSequence) {
        seq_char.push_back(c[0]);
    }

    Board b;

    b.init(seq_char, 18, 11, start_level, 0, seed);


    //TODO graphic, seed flags

    cout << b;

    Command cmdbook;
    auto queue = make_shared<vector<shared_ptr<StdCmd>>>();

    string s;
    while(cin >> s) {
        if (s=="add") {
            string alter, basicCmd;
            cin >> alter >> basicCmd;
            cmdbook.addAlter(alter, basicCmd);
        }
        else if (s=="rename") {
            string oldName, newName;
            cin >> oldName >> newName;
            cmdbook.rename(oldName, newName);
        }
        else {
            auto sc = cmdbook.smartLookup(s);
            if (sc) {
                if (sc->basicCmd == "norandom" || sc->basicCmd == "sequence") {
                    string fileName;
                    cin >> fileName;
                    Sequence seq{fileName};
                    queue = cmdbook.translate(sc->basicCmd, seq);
                }
                else {
                    queue = cmdbook.translate(s);
                }
            }
            //printQueue(queue);
        }

        cout << "here" << endl;

        b.perform(queue);

        cout << b;
        if(b.isEnd()) {
            cout << "Game Over" << endl;
            break;
        }
    }
}*/
