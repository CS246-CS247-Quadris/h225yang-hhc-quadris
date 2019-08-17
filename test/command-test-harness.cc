#include <iostream>
#include <fstream>
#include <memory>
#include "command.h"
#include "stdcmd.h"
#include "sequence.h"
using namespace std;

// Test function
void printQueue(shared_ptr<vector<shared_ptr<StdCmd>>> queue) {
  if (!queue || queue->size()==0) cout << "printQueue: Empty Queue" << endl;
  else {
    int i = 1;
    for(auto element : *queue) {
      cout << "printQueue: " << i << "/" << queue->size() << ": " << element->prettyprint() << endl;
      ++i;
    }
  }
}

int main() {
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
      printQueue(queue);
      if (queue) queue->clear();
    }
  } // END of while
  return 0;
}

