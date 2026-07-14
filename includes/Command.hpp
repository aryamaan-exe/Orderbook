#ifndef CMD_HPP
#define CMD_HPP

#include "Order.hpp"
#include "Orderbook.hpp"

using BookPtr = std::shared_ptr<Orderbook>;

enum class CommandType { Add, Cancel };

class Command {
public:
  Command(CommandType type, Orderbook& book, Order& order);
  void Process();
  BookPtr GetOrderbook();
private:
  CommandType type_;
  Orderbook& book_;
  Order& order_;

};

#endif