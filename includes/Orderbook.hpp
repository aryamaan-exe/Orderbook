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
using Symbol = std::string;

struct OrderLocation {
  Side side;
  Price price;
  OrderIterator iterator;
};

using OrderMap = std::unordered_map<OrderID, OrderLocation>;

class Orderbook {
public:
  Orderbook();
  Orderbook(std::string symbol);
  void AddOrder(Order order);
  bool CancelOrder(OrderID id);
  std::optional<Order> GetBestBid();
  std::optional<Order> GetBestAsk();
private:
  template <typename Book>
  void AddToBook(Book& book, const Order& order, Price price);

  void MatchOrder(Order& order);

  template <typename Book, typename CrossPredicate>
  void Match(Book& book, Order& order, CrossPredicate crosses, bool match_fully = false);

  Symbol symbol_;
  BidBook bids_;
  AskBook asks_;
  OrderMap order_locations_;
};

#endif