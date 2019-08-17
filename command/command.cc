#include <string>
#include <iostream>
#include <memory>
#include <algorithm>
#include "command.h"
#include "stdcmd.h"
#include "sequence.h"
using namespace std;

// PImpl Idiom
struct Command::CommandImpl {
  static std::vector<std::string> basicCmd;  
  static std::map<std::string, StdCmd> dict;
};

vector<string> Command::CommandImpl::basicCmd = 
{
  "left", "right", "down", "clockwise", "counterclockwise",
  "drop", "levelup", "leveldown", "norandom", "random",
  "sequence", "restart", "hint", 
  "I", "J", "L", "O", "S", "Z", "T", "*"
};
map<string, StdCmd> Command::CommandImpl::dict = 
{
  {"left", StdCmd{"left"}}, {"lef", StdCmd{"left"}},
  {"right", StdCmd{"right"}}, {"ri", StdCmd{"right"}}, {"rig", StdCmd{"right"}}, {"righ", StdCmd{"right"}},
  {"down", StdCmd{"down"}}, {"do", StdCmd{"down"}}, {"dow", StdCmd{"down"}},
  {"clockwise", StdCmd{"clockwise"}}, {"cl", StdCmd{"clockwise"}}, {"clo", StdCmd{"clockwise"}}, {"cloc", StdCmd{"clockwise"}}, {"clock", StdCmd{"clockwise"}}, {"clockw", StdCmd{"clockwise"}}, {"clockwi", StdCmd{"clockwise"}}, {"clockwis", StdCmd{"clockwise"}},
  {"counterclockwise", StdCmd{"counterclockwise"}}, {"co", StdCmd{"counterclockwise"}}, {"cou", StdCmd{"counterclockwise"}}, {"coun", StdCmd{"counterclockwise"}}, {"count", StdCmd{"counterclockwise"}}, {"counte", StdCmd{"counterclockwise"}}, {"counter", StdCmd{"counterclockwise"}}, {"counterc", StdCmd{"counterclockwise"}}, {"countercl", StdCmd{"counterclockwise"}}, {"counterclo", StdCmd{"counterclockwise"}}, {"countercloc", StdCmd{"counterclockwise"}}, {"counterclock", StdCmd{"counterclockwise"}}, {"counterclockw", StdCmd{"counterclockwise"}}, {"counterclockwi", StdCmd{"counterclockwise"}}, {"counterclockwis", StdCmd{"counterclockwise"}},
  {"drop", StdCmd{"drop"}}, {"dr",  StdCmd{"drop"}}, {"dro", StdCmd{"drop"}},
  {"levelup", StdCmd{"levelup"}}, {"levelu", StdCmd{"levelup"}},
  {"leveldown", StdCmd{"leveldown"}}, {"leveld", StdCmd{"leveldown"}}, {"leveldo", StdCmd{"leveldown"}}, {"leveldow", StdCmd{"leveldown"}},
  {"norandom", StdCmd{"norandom"}}, {"n", StdCmd{"norandom"}}, {"no", StdCmd{"norandom"}}, {"nor", StdCmd{"norandom"}}, {"nora", StdCmd{"norandom"}}, {"noran", StdCmd{"norandom"}}, {"norand", StdCmd{"norandom"}}, {"norando", StdCmd{"norandom"}},
  {"random", StdCmd{"random"}},   {"ra", StdCmd{"random"}}, {"ran", StdCmd{"random"}}, {"rand", StdCmd{"random"}}, {"rando", StdCmd{"random"}},
  {"sequence", StdCmd{"sequence"}},   {"s", StdCmd{"sequence"}}, {"se", StdCmd{"sequence"}}, {"seq", StdCmd{"sequence"}}, {"sequ", StdCmd{"sequence"}}, {"seque", StdCmd{"sequence"}}, {"sequen", StdCmd{"sequence"}}, {"sequenc", StdCmd{"sequence"}},
  {"restart", StdCmd{"restart"}}, {"re", StdCmd{"restart"}}, {"res", StdCmd{"restart"}}, {"rest", StdCmd{"restart"}}, {"resta", StdCmd{"restart"}}, {"restar", StdCmd{"restart"}},
  {"hint", StdCmd{"hint"}},  {"h", StdCmd{"hint"}}, {"hi", StdCmd{"hint"}}, {"hin", StdCmd{"hint"}},
  {"I", StdCmd{"I"}}, {"J", StdCmd{"J"}}, {"L", StdCmd{"L"}}, {"O", StdCmd{"O"}}, {"S", StdCmd{"S"}}, {"Z", StdCmd{"Z"}}, {"T", StdCmd{"T"}}, {"*", StdCmd{"*"}}, 
};

Command::Command()
: pImpl{make_unique<CommandImpl>()}
{}

Command::~Command() =default;

bool Command::isBasicCmd(string s) const {
  if (find(pImpl->basicCmd.begin(), pImpl->basicCmd.end(), s) != pImpl->basicCmd.end()) return true;
  return false; 
}

int Command::pickRepeat(const string s) const {
  string copy = s;
  int times=0;
  if (copy.find_first_of("0123456789")==0) {
    while(copy.find_first_of("0123456789")==0) {
      times *= 10;
      times += (copy[0] - '0');
      copy.erase(0, 1);
    }
  }
  else times=1;
  return times;
}

string Command::pickCmd(const string s) const {
  string left;
  if (s.find_first_not_of("0123456789") == std::string::npos) return left;
  left = s.substr(s.find_first_not_of("0123456789"));
  return left;
}

// lookup(s) returns a (shared) pointer to the corresponding StdCmd of s
//   if exist, otherwise returns a nullptr.
shared_ptr<StdCmd> Command::lookup(const string s) const {
  shared_ptr<StdCmd> scp;
  if (pImpl->dict.find(s) == pImpl->dict.end()) return scp;

  string copy = s;
  while (!isBasicCmd(copy)) copy = pImpl->dict.at(copy).basicCmd;
  scp = make_shared<StdCmd>(copy);
  return scp;
}

shared_ptr<StdCmd> Command::smartLookup(const string s) const {
  shared_ptr<StdCmd> scp;
  if (!lookup(pickCmd(s))) return scp;
  scp = make_shared<StdCmd>(lookup(pickCmd(s))->basicCmd, pickRepeat(s));
  return scp;
}

void Command::addAlter(const string alter, const string cmd) {
  // If alter already exist, return
  if (pImpl->dict.find(alter) != pImpl->dict.end()) return;
  // Else add the new pair with alter and cmd
  shared_ptr<StdCmd> scp = lookup(cmd);
  if (scp) pImpl->dict.insert(pair<string, StdCmd>(alter, *scp));
}

void Command::rename(const string oldName, const string newName) {
  shared_ptr<StdCmd> scp = lookup(oldName);
  if (!scp) return; // If no such oldName, return
  // Else erase old pair and insert new pair
  pImpl->dict.erase(oldName);
  pImpl->dict.insert(pair<string, StdCmd>(newName, *scp));
}

// translate(s) returns a pointer to a vector of pointers of StdCmd (all shared pointers)
//   based on s. 
//   Size of the returned vector:
//   1) will be n, if s has a number n!=0 in its front AND the cmd part of s is valid
//   2) will be 1, if s has no number in its front AND the cmd part of s is valid
//   3) will be 0, otherwise
shared_ptr<vector<shared_ptr<StdCmd>>> Command::translate(const string s) const {
  shared_ptr<vector<shared_ptr<StdCmd>>> voc; // null shared_ptr

  int times = pickRepeat(s);
  if (times==0) return voc;
  shared_ptr<StdCmd> scp;
  scp = smartLookup(s);

  if (scp) {
    voc = make_shared<vector<shared_ptr<StdCmd>>>();

    if (!scp->repeatable && times!=0) scp->times = 1;
    voc->emplace_back(scp);
  }
  return voc;
}

void Command::addOrder(shared_ptr<StdCmd> scp, const Sequence seq) const {
  for (auto c : seq.theSequence) scp->order += c;
}

// translate function for commands that requires a file, i.e, "norandom" and "sequence"
shared_ptr<vector<shared_ptr<StdCmd>>> Command::translate(const std::string s, const Sequence seq) const {
  shared_ptr<vector<shared_ptr<StdCmd>>> voc;
  
  int times = pickRepeat(s);
  if (times==0) return voc;
  string cmd = pickCmd(s);
  shared_ptr<StdCmd> scp = lookup(cmd);

  if (scp && scp->basicCmd == "norandom") {
    addOrder(scp, seq);
    scp->times = 1;
    voc = make_shared<vector<shared_ptr<StdCmd>>>();
    voc->emplace_back(scp);
  }
  else if (scp && scp->basicCmd == "sequence") {
    voc = make_shared<vector<shared_ptr<StdCmd>>>();
    for (size_t i=0; i<seq.theSequence.size(); ++i) {
      shared_ptr<vector<shared_ptr<StdCmd>>> currVoc;
      string currInput = seq.theSequence.at(i);
      int currTimes = pickRepeat(currInput); 
      if (currTimes==0) continue;
      shared_ptr<StdCmd> currScp = smartLookup(currInput);

      if (currScp) {
        
        if (currScp->basicCmd == "norandom") {
          ++i;
          Sequence currSeq = Sequence{seq.theSequence.at(i)};
          addOrder(currScp, currSeq);
          currScp->times = 1;
          currVoc = make_shared<vector<shared_ptr<StdCmd>>>();
          currVoc->emplace_back(currScp);
        }
        else if (currScp && currScp->basicCmd == "sequence") {
          ++i;
          Sequence currSeq = Sequence{seq.theSequence.at(i)};
          // If there is another "sequence" in current "sequence"
          currVoc = translate(currInput, currSeq);
        }
        else {
          currVoc = translate(currInput);
        }
        // Insert currVoc to the end of voc
        if (currVoc) voc->insert(voc->end(), currVoc->begin(), currVoc->end());
      } // End of if (currScp) 
    } // End of for
  }

  return voc;
}
