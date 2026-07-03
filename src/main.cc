#include "Order.hpp"
#include <iostream>

int main() {
  Order o = Order::Builder("", OrderType::Limit)
            .SetSide(Side::Sell)
            .SetPrice(100)
            .SetQuantity(50)
            .Build();
  Order o2 = Order::Builder("", OrderType::Limit)
            .SetSide(Side::Sell)
            .SetPrice(100)
            .SetQuantity(50)
            .Build();
  
  std::cout << o.GetOrderID() << std::endl;
  std::cout << o2.GetOrderID() << std::endl;
}