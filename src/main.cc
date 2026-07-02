#include "Order.hpp"
#include <iostream>

int main() {
  Order o = Order::Builder("", OrderType::Limit)
            .SetSide(Side::Sell)
            .SetPrice(100)
            .SetQuantity(50)
            .Build();
  
  std::cout << o.GetPrice() << std::endl;
}