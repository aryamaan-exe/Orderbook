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
  MatchOrder(order);

  if (order.GetQuantity() == 0) return;

  if (order.GetSide() == Side::Buy) {
    AddToBook(bids_, order, price);
  } else {
    AddToBook(asks_, order, price);
  }

}

void Orderbook::MatchBuyLimit(Order& order) {
  while (order.GetQuantity() != 0 && !asks_.empty()) {
    auto best_ask_level = asks_.begin();

    if (best_ask_level->first > order.GetPrice()) break;

    Order& best_ask = best_ask_level->second.front();

    Quantity amount = std::min(best_ask.GetQuantity(), order.GetQuantity());
    order.ReduceQuantity(amount);
    best_ask.ReduceQuantity(amount);

    if (best_ask.GetQuantity() == 0) {
      CancelOrder(best_ask.GetOrderID());
    }
  }
}

void Orderbook::MatchSellLimit(Order& order) {
  while (order.GetQuantity() != 0 && !bids_.empty()) {
    auto best_bid_level = bids_.begin();

    if (best_bid_level->first < order.GetPrice()) break;

    Order& best_bid = best_bid_level->second.front();

    Quantity amount = std::min(best_bid.GetQuantity(), order.GetQuantity());
    order.ReduceQuantity(amount);
    best_bid.ReduceQuantity(amount);

    if (best_bid.GetQuantity() == 0) {
      CancelOrder(best_bid.GetOrderID());
    }
  }
}

void Orderbook::MatchOrder(Order& order) {
  OrderType type = order.GetType();

  if (order.GetSide() == Side::Buy) {
    if (type == OrderType::Limit) {
      MatchBuyLimit(order);
    }
  } else {
    if (type == OrderType::Limit) {
      MatchSellLimit(order);
    }
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