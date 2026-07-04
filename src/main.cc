#include "Order.hpp"
#include "Orderbook.hpp"
#include <iostream>

int main() {
  Order o = Order::Builder("", OrderType::Limit)
            .SetSide(Side::Sell)
            .SetPrice(100)
            .SetQuantity(500)
            .Build();
  Order o2 = Order::Builder("", OrderType::Limit)
            .SetSide(Side::Buy)
            .SetPrice(105)
            .SetQuantity(510)
            .Build();
  
  Orderbook book;
  book.AddOrder(o2);
  book.AddOrder(o);

  std::cout << book.GetBestBid().value().GetQuantity() << std::endl;
}