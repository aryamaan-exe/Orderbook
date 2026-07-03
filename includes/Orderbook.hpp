#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include "Order.hpp"
#include <functional>
#include <optional>
#include <map>
#include <list>

using OrderList = std::list<Order>;
using OrderIterator = OrderList::iterator;
using BidBook = std::map<Price, OrderList, std::greater<Price>>;
using AskBook = std::map<Price, OrderList>;

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
  template <typename Book>
  void AddToBook(Book& book, const Order& order, Price price);

  BidBook bids_;
  AskBook asks_;
  std::unordered_map<OrderID, OrderLocation> order_locations_;
};

#endif