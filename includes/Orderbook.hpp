#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include "Order.hpp"
#include "Trade.hpp"
#include <functional>
#include <optional>
#include <map>
#include <list>
#include <vector>

using OrderList = std::list<Order>;
using OrderIterator = OrderList::iterator;
using BidBook = std::map<Price, OrderList, std::greater<Price>>;
using AskBook = std::map<Price, OrderList>;
using Symbol = std::string;
using TradeLog = std::vector<Trade>;

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
  void WriteOrders(std::string filename);
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
  TradeLog log_;
};

#endif