#include <iostream>
#include <sstream>
#include <string>
#include "command/command.h"
#include "command/sequence.h"
#include "command/stdcmd.h"
#include "block/block.h"
#include "board/board.h"
using namespace std;

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
	b.init(seq_char, 18, 11, start_level, 0, seed, graphic_mode);

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
			if(queue)
				b.perform(queue);
			cout << b;
			if(b.isEnd()) {
				cout << "Game Over" << endl;
				cout << "Starting new game" << endl;
				b.restart();
				cout << b;
			}
			if(queue) queue->clear();
		}
	}
}

