#include "Order.hpp"
#include "Orderbook.hpp"
#include "Command.hpp"
#include "CommandHandler.hpp"
#include <iostream>

int main() {
  Order o = Order::Builder(OrderType::Limit)
            .SetSide(Side::Sell)
            .SetPrice(100)
            .SetQuantity(500)
            .Build();

  std::cout << o.GetOrderID() << std::endl;

  Orderbook book;
  book.AddOrder(o);
  
  Command cmd{CommandType::Add, book, o};
  CommandHandler cmdh;
  cmdh.AddCommand(cmd);

  // Order o2 = Order::Builder(OrderType::Market)
  //           .SetSide(Side::Buy)
  //           .SetQuantity(500)
  //           .Build();
  
  // Orderbook book;
  // book.AddOrder(o);
  // book.AddOrder(o2);

  // std::cout << book.GetBestAsk().value().GetQuantity() << std::endl;
}