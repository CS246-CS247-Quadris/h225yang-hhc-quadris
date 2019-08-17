#ifndef _STDCMD_H_
#define _STDCMD_H_
#include <string>
#include <fstream>
#include <iostream>

struct StdCmd {
	std::string basicCmd;
	std::string receiverType;
  bool repeatable;
  std::string order;
  int times;

  StdCmd(const std::string s, const int n=0)
  : basicCmd{s}, times{n}
  {
    if ( s=="left" || s=="right" || s=="down" || s=="clockwise" || s=="counterclockwise" || s=="drop") {
      receiverType = "Block";
      repeatable = true;
    }
    else if (s=="levelup" || s=="leveldown") {
      receiverType = "Level";
      repeatable = true;
    }
    else if (s=="norandom" || s=="random" || s=="sequence" || s=="restart" || s=="hint" || s=="I" || s=="J" || s=="L" || s=="O" || s=="S" || s=="Z" || s=="T" || s=="*") {
      receiverType = "Board";
      repeatable = false;
    }
  }
	// Test method
  std::string prettyprint() const {
    std::string result;
    result = "command: " + basicCmd + ", recevier: " + receiverType + ", order: " + order + ", times: " + std::to_string(times);
    return result;
  }
};

#endif
