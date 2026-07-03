#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include "Order.hpp"
#include <functional>
#include <optional>
#include <map>
#include <list>

using OrderList = std::list<Order>;
using OrderIterator = OrderList::iterator;

struct OrderLocation {
  Side side;
  Price price;
  OrderIterator iterator;
};

class Orderbook {
public:
  void AddOrder(Order order);
  bool CancelOrder(OrderID id);
  std::optional<Order> GetBestBid();
  std::optional<Order> GetBestAsk();
private:
  std::map<Price, OrderList, std::greater<Price>> bids_;
  std::map<Price, OrderList> asks_;
};

#endif