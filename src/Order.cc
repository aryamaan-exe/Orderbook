#include "Order.hpp"

using Builder = Order::Builder;

Builder& Builder::SetSide(Side side) {
  side_ = side;
  return *this;
}

Builder& Builder::SetPrice(Price price) {
  price_ = price;
  return *this;
}

Builder& Builder::SetQuantity(Quantity quantity) {
  quantity_ = quantity;
  return *this;
}

Order Builder::Build() const {
  if (!quantity_.has_value()) {
    throw std::logic_error("Quantity is required.");
  }

  if (!side_.has_value()) {
    throw std::logic_error("Order side is required.");
  }

  if (type_ == OrderType::Market && price_.has_value()) {
    throw std::logic_error("Market order cannot have a price.");
  } else if (type_ == OrderType::Limit && !price_.has_value()) {
    throw std::logic_error("Limit order needs a price."); 
  }

  return Order(type_, side_.value(), quantity_.value(), price_);
}