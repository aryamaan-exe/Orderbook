#include "Orderbook.hpp"
#include <fstream>
#include <thread>

Orderbook::Orderbook(std::string symbol): symbol_(symbol) {}

Orderbook::Orderbook(): symbol_("") {}

template <typename Book>
void Orderbook::AddToBook(Book& book, const Order& order, Price price) {
  auto& orders = book[price];
  orders.push_back(order);

  auto it = std::prev(orders.end());
  order_locations_[order.GetOrderID()] = {
    order.GetSide(),
    price,
    it
  };
}

void Orderbook::AddOrder(Order order) {
  MatchOrder(order);
  
  if (order.GetQuantity() == 0) return;
  if (order.GetType() != OrderType::Limit) return;

  Price price = order.GetPrice();


  if (order.GetSide() == Side::Buy) {
    AddToBook(bids_, order, price);
  } else {
    AddToBook(asks_, order, price);
  }

}

template <typename Book, typename CrossPredicate>
void Orderbook::Match(
  Book& book,
  Order& order,
  CrossPredicate crosses,
  bool match_fully
) {
  while (order.GetQuantity() != 0 && !book.empty()) {
    auto level = book.begin();

    if (level == book.end() || !crosses(level->first)) break;

    Order& resting = level->second.front();
    Quantity order_quantity = order.GetQuantity();
    Quantity amount = std::min(order_quantity, resting.GetQuantity());
    Price price{resting.GetPrice()};

    if (amount != order_quantity && match_fully) return;
    
    order.ReduceQuantity(amount);
    resting.ReduceQuantity(amount);
    
    log_.push_back(Trade{
      order, resting, price, amount
    });

    if (resting.GetQuantity() == 0) CancelOrder(resting.GetOrderID());
  }
}

void Orderbook::MatchOrder(Order& order) {
  OrderType type = order.GetType();
  bool match_fully{};

  if (order.GetSide() == Side::Buy) {
    switch (type) {
      case OrderType::FillOrKill:
        match_fully = true;
      case OrderType::Limit:
        Match(asks_, order, [&](Price p) { return p <= order.GetPrice(); }, match_fully);
        break;
      case OrderType::Market:
        Match(asks_, order, [](Price) { return true; });
        break;
      default:
        throw std::logic_error("Order type not implemented yet.");
    }
  } else {
    switch (type) {
      case OrderType::FillOrKill:
        match_fully = true;
      case OrderType::Limit:
        Match(bids_, order, [&](Price p) { return p >= order.GetPrice(); }, match_fully);
        break;
      case OrderType::Market:
        Match(bids_, order, [](Price) { return true; });
        break;
      default:
        throw std::logic_error("Order type not implemented yet.");
    }
  }
}

bool Orderbook::CancelOrder(OrderID id) {
  auto location_it = order_locations_.find(id);
  if (location_it == order_locations_.end()) return false;
 
  OrderLocation& location = location_it->second;

  if (location.side == Side::Buy) {
    auto price_it = bids_.find(location.price);
    if (price_it == bids_.end()) return false;

    OrderList& orders = price_it->second;
    orders.erase(location.iterator);
    if (orders.empty()) bids_.erase(location.price);

  } else {    
    auto price_it = asks_.find(location.price);
    if (price_it == asks_.end()) return false;

    OrderList& orders = price_it->second;
    orders.erase(location.iterator);
    if (orders.empty()) asks_.erase(location.price);
  } 

  order_locations_.erase(location_it);
  return true;
}

std::optional<Order> Orderbook::GetBestBid() { 
  if (bids_.empty()) return std::nullopt;

  return bids_.cbegin()->second.front();
}

std::optional<Order> Orderbook::GetBestAsk() { 
  if (asks_.empty()) return std::nullopt;

  return asks_.cbegin()->second.front();
}

void Orderbook::WriteOrders(std::string filename) {
  std::ofstream ofs{filename};
  
  for (const Trade& trade : log_) {
    ofs << trade;
  }
}