#ifndef TRADE_HPP
#define TRADE_HPP

#include <fstream>
#include <format>
#include "Order.hpp"

using TradeID = unsigned long;

class Trade {
public:
  Trade(Order& ask, Order& bid, Price price, Quantity quantity): 
  ask_(ask), bid_(bid), price_(price), quantity_(quantity) {
    id_ = next_trade_id_++;
  }
  
  friend inline std::ostream& operator<<(std::ostream& os, const Trade& trade);
private:
  inline static TradeID next_trade_id_ = 1;
  TradeID id_{};
  Order& ask_;
  Order& bid_; 
  Price price_;
  Quantity quantity_;
};

inline std::ostream& operator<<(std::ostream& os, const Trade& trade) {
  os << std::format("TRADE_ID: {} BID_ID: {} ASK_ID: {} PRICE: {} QUANTITY: {}\n",
                    trade.id_, trade.ask_.GetOrderID(), trade.bid_.GetOrderID(),
                    trade.price_, trade.quantity_);

  return os;
};

#endif