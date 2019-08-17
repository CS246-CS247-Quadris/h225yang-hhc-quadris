#ifndef _COMMAND_H_
#define _COMMAND_H_
#include <string>
#include <vector>
#include <map>
#include <memory>

struct StdCmd;
struct Sequence;

class Command {
  struct CommandImpl;
  std::unique_ptr<CommandImpl> pImpl;

  bool isBasicCmd(std::string) const;
  int pickRepeat(const std::string) const;
  std::string pickCmd(const std::string) const;
  void addOrder(const std::shared_ptr<StdCmd>, const Sequence) const;
  std::shared_ptr<StdCmd> lookup(const std::string) const;
public:
  Command();
  ~Command();
  std::shared_ptr<StdCmd> smartLookup(const std::string) const;
  void addAlter(const std::string, const std::string);
  void rename(const std::string, const std::string);
  std::shared_ptr<std::vector<std::shared_ptr<StdCmd>>> translate(const std::string) const;
  std::shared_ptr<std::vector<std::shared_ptr<StdCmd>>> translate(const std::string, const Sequence) const;
};

#endif
