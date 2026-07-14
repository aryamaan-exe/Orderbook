#include "CommandHandler.hpp"
#include "Command.hpp"

void CommandHandler::WorkerLoop(std::stop_token token, Worker& worker) {
  std::stop_callback cb(token, [&worker] {
    worker.cv.notify_all();
  });
  
  while (true) {
    std::unique_lock lock{worker.mutex};
    worker.cv.wait(lock, [&] {
      return !worker.queue.empty() || token.stop_requested();
    });

    if (worker.queue.empty() && token.stop_requested()) return;

    Command cmd = worker.queue.front();
    worker.queue.pop();
    lock.unlock();
    cmd.Process();
  }
}

void CommandHandler::AddCommand(Command command) {
  BookPtr ptr{command.GetOrderbook()};
  Worker& worker{workers_[ptr]};
  
  
  std::lock_guard l{worker.mutex}; 
  worker.queue.push(command);
  if (!worker.thread.joinable()) {
    worker.thread = std::jthread{[this, &worker](std::stop_token token) {
      WorkerLoop(token, worker);
    }};
  }
  worker.cv.notify_one();
}

void CommandHandler::Stop() {
  for (auto& [_, worker] : workers_) {
    worker.thread.request_stop();
  } 

  for (auto& [_, worker] : workers_) {
    if (worker.thread.joinable()) worker.thread.join();
  }
}