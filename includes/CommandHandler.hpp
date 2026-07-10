#ifndef CMD_HANDLER_HPP
#define CMD_HANDLER_HPP

#include <unordered_map>
#include <list>
#include <thread>
#include <memory>
#include "Command.hpp"

using CommandMap = std::unordered_map<Orderbook*, std::list<Command>>;
using MapPtr = std::shared_ptr<CommandMap>;

class CommandHandler {
public:
  CommandHandler();
  void AddCommand(Command command);
private:
  CommandMap commands_;
  MapPtr mapptr_;
};

#endif