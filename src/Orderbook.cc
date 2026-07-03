#include "Orderbook.hpp"

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
  Price price = order.GetPrice();

  if (order.GetSide() == Side::Buy) {
    AddToBook(bids_, order, price);
  } else {
    AddToBook(asks_, order, price);
  }
}

bool Orderbook::CancelOrder(OrderID id) {
  if (!order_locations_.contains(id)) { return false; }

  auto location_it = order_locations_.find(id);
  if (location_it == order_locations_.end()) return false;

  OrderLocation& location = location_it->second;

  if (location.side == Side::Buy) {
    auto price_it = bids_.find(location.price);
    if (price_it == bids_.end()) return false;

    OrderList orders = price_it->second;
    orders.erase(location.iterator);
    if (orders.empty()) bids_.erase(location.price);

  } else {    
    auto price_it = asks_.find(location.price);
    if (price_it == asks_.end()) return false;
    
    OrderList orders = price_it->second;
    orders.erase(location.iterator);
    if (orders.empty()) asks_.erase(location.price);
  } 

  order_locations_.erase(location_it);
  return true;
}

std::optional<Order> Orderbook::GetBestBid() { 
  if (bids_.empty()) return std::nullopt;

  return bids_.begin()->second.front();
}

std::optional<Order> Orderbook::GetBestAsk() { 
  if (asks_.empty()) return std::nullopt;

  return asks_.begin()->second.front();
}