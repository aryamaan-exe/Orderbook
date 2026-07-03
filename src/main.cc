#include "Order.hpp"
#include "Orderbook.hpp"
#include <iostream>

int main() {
  Order o = Order::Builder("", OrderType::Limit)
            .SetSide(Side::Sell)
            .SetPrice(100)
            .SetQuantity(50)
            .Build();
  Order o2 = Order::Builder("", OrderType::Limit)
            .SetSide(Side::Sell)
            .SetPrice(150)
            .SetQuantity(50)
            .Build();
  
  Orderbook book;
  book.AddOrder(o);
  book.AddOrder(o2);
}