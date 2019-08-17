#ifndef _SEQUENCE_H_
#define _SEQUENCE_H_
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

struct Sequence {
  std::vector<std::string> theSequence;

  Sequence(std::string fileName) {
  // Try to read in the file
    std::ifstream inFile(fileName);
    if (!inFile) {
      std::cout << "Unable to open file " << fileName << std::endl;;
      return;
    }

  // Put all content of the file into a string text
    std::string copy;
    getline(inFile, copy);

    size_t till = copy.find_first_of(' ');
    while (till != std::string::npos) {
      std::string part = copy.substr(0, till);
      theSequence.emplace_back(part);
      copy.erase(0, till+1);
      till = copy.find_first_of(' ');
    }
    theSequence.emplace_back(copy);
  }

// Test method
  std::string prettyprint() const {
    std::string result;
    result += "Sequence: ";
    for (auto part : theSequence) {
      result += part;
      result += " ";
    }
    return result;    
  }
};

#endif
