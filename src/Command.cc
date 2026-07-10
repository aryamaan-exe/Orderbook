#include "Command.hpp"

#include <stdexcept>

Command::Command(CommandType type, Orderbook& book, Order& order):
  type_(type), book_(book), order_(order) {}

void Command::Process() {
  switch (type_) {
    case CommandType::Add:
      book_.AddOrder(order_);
    case CommandType::Cancel:
      book_.CancelOrder(order_.GetOrderID());
  }
}

Orderbook* Command::GetOrderbook() { return &book_; }