#include "Command.hpp"

#include <stdexcept>
#include <memory>

Command::Command(CommandType type, Orderbook& book, Order& order):
  type_(type), book_(book), order_(order) {}

void Command::Process() {
  switch (type_) {
    case CommandType::Add:
      book_.AddOrder(order_);
      break;
    case CommandType::Cancel:
      book_.CancelOrder(order_.GetOrderID());
      break;
    default:
      throw std::logic_error("Unknown command type");
      break;
  }
}

BookPtr Command::GetOrderbook() { return BookPtr{&book_, [](Orderbook*){}}; }