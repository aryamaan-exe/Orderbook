#ifndef CMD_HANDLER_HPP
#define CMD_HANDLER_HPP

#include <unordered_map>
#include <list>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include "Command.hpp"

struct Worker {
  std::jthread thread;
  std::queue<Command> queue;
  std::mutex mutex;
  std::condition_variable cv;
};

using WorkerMap = std::unordered_map<BookPtr, Worker>;

class CommandHandler {
public:
  void AddCommand(Command command);
  void Stop();
  void WorkerLoop(std::stop_token token, Worker& worker);
private:
  WorkerMap workers_;
};

#endif