#ifndef TRADE_HPP
#define TRADE_HPP

#include <fstream>
#include <format>
#include "Order.hpp"

using TradeID = unsigned long;

class Trade {
public:
  friend std::ostream& operator<<(std::ostream& os, Trade& trade);
private:
  TradeID id_;
  Order& ask_;
  Order& bid_; 
  Price price;
  Quantity quantity;
};

std::ostream& operator<<(std::ostream& os, Trade& trade) {
  os << std::format("TRADE ID: {} BID ID: {} ASK ID: {} PRICE: {} QUANTITY: {}\n",
                    trade.id_, trade.ask_.GetOrderID(), trade.bid_.GetOrderID(),
                    trade.price, trade.quantity);
  return os;
}

#endif