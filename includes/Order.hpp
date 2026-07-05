#ifndef ORDER_HPP
#define ORDER_HPP

#include <optional>
#include <stdexcept>
#include <string>

using Price = unsigned;
using Quantity = unsigned;
using OrderID = unsigned long long;

enum class Side { Buy, Sell };
enum class OrderType { Market, Limit };

class Order {
public:
  class Builder {
  public:
    Builder(std::string symbol, OrderType type): symbol_(symbol), type_(type) {}

    Builder& SetSide(Side side); 
    Builder& SetPrice(Price price); 
    Builder& SetQuantity(Quantity quantity); 

    Order Build() const;
  private:
    std::string symbol_;
    OrderType type_;
    std::optional<Side> side_;
    std::optional<Price> price_;
    std::optional<Quantity> quantity_;
  };

  Order(OrderType type, Side side, Quantity quantity, std::optional<Price> price = std::nullopt):
    type_(type), side_(side), price_(price), quantity_(quantity), id_(next_order_id_++) {}

  
  OrderType GetType() const { return type_; }
  Side GetSide() const { return side_; }
  Quantity GetQuantity() const { return quantity_; }
  OrderID GetOrderID() const { return id_; }
    
  void ReduceQuantity(Quantity amount);
    
  Price GetPrice() {
    if (!price_.has_value()) {
      throw std::logic_error("Price does not exist for this order type or has not been set.");
    }
    
    return price_.value();
  }
  
private:
  std::string symbol_;
  OrderType type_;
  Side side_;
  std::optional<Price> price_;
  Quantity quantity_;
  const OrderID id_;
  inline static OrderID next_order_id_ = 1;
};

#endif