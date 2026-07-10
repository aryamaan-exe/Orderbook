#include "CommandHandler.hpp"

void BackgroundHandler(MapPtr cmdmap) {
    while (true) {
      for (auto& [book, cmdlist] : *cmdmap) {
        std::thread t{
          [&]{
            for (auto& command : cmdlist) {
              command.Process();
            }
          }
        };
        t.detach();
      }
    }
  }

CommandHandler::CommandHandler() {
  mapptr_ = std::make_shared<CommandMap>(commands_);
  std::thread t{BackgroundHandler, mapptr_};
  t.join();
}

void CommandHandler::AddCommand(Command command) {
  commands_[command.GetOrderbook()].push_back(command);
}