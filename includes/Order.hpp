#ifndef ORDER_HPP
#define ORDER_HPP

#include <optional>
#include <stdexcept>

enum class Side { Buy, Sell };
enum class OrderType { Market, Limit };

class Order {
public:
  class Builder {
  public:
    Builder(std::string symbol, OrderType type): symbol_(symbol), type_(type) {}

    Builder& SetSide(Side side) {
      side_ = side;
      return *this;
    }

    Builder& SetPrice(unsigned price) {
      price_ = price;
      return *this;
    }

    Builder& SetQuantity(unsigned quantity) {
      quantity_ = quantity;
      return *this;
    }

    Order Build() const {
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
  private:
    std::string symbol_;
    OrderType type_;
    std::optional<Side> side_;
    std::optional<unsigned> price_;
    std::optional<unsigned> quantity_;
  };

  Order(OrderType type, Side side, unsigned quantity, std::optional<unsigned> price = std::nullopt):
    type_(type), side_(side), price_(price), quantity_(quantity) {}

  
  OrderType GetType() { return type_; }
  Side GetSide() { return side_; }
  unsigned GetQuantity() { return quantity_; }
    
  unsigned GetPrice() {
    if (!price_.has_value()) {
      throw std::logic_error("Price does not exist for this market type or has not been set.");
    }
    
    return price_.value();
  }
  
private:
  std::string symbol_;
  OrderType type_;
  Side side_;
  std::optional<unsigned> price_;
  unsigned quantity_;
};

#endif