#include "catch_amalgamated.hpp"
#include "Order.hpp"
#include "Orderbook.hpp"

TEST_CASE("Order builder", "[builder]") {
  Order o = Order::Builder("", OrderType::Limit)
            .SetSide(Side::Sell)
            .SetPrice(100)
            .SetQuantity(50)
            .Build();
  
  REQUIRE(o.GetType() == OrderType::Limit);
  REQUIRE(o.GetSide() == Side::Sell);
  REQUIRE(o.GetPrice() == 100);
  REQUIRE(o.GetQuantity() == 50);

  REQUIRE_THROWS(Order::Builder("", OrderType::Limit)
                                .SetSide(Side::Buy)
                                .SetQuantity(50)
                                .Build());

  REQUIRE_THROWS(Order::Builder("", OrderType::Market)
                                .SetSide(Side::Buy)
                                .SetQuantity(50)
                                .SetPrice(100)
                                .Build());
}

TEST_CASE("Matching engine - limit", "[matchl]") {
  Order o = Order::Builder("", OrderType::Limit)
            .SetSide(Side::Sell)
            .SetPrice(100)
            .SetQuantity(510)
            .Build();
  Order o2 = Order::Builder("", OrderType::Limit)
            .SetSide(Side::Buy)
            .SetPrice(105)
            .SetQuantity(500)
            .Build();
  
  Orderbook book;
  book.AddOrder(o);
  book.AddOrder(o2);

  REQUIRE(book.GetBestAsk().value().GetQuantity() == 10);

  Order o3 = Order::Builder("", OrderType::Limit)
            .SetSide(Side::Buy)
            .SetPrice(100)
            .SetQuantity(510)
            .Build();
  
  Order o4 = Order::Builder("", OrderType::Limit)
            .SetSide(Side::Sell)
            .SetPrice(90)
            .SetQuantity(500)
            .Build();

  book.AddOrder(o3);
  REQUIRE(book.GetBestAsk() == std::nullopt);
  REQUIRE(book.GetBestBid().value().GetQuantity() == 500);

  book.AddOrder(o4);
  REQUIRE(book.GetBestBid() == std::nullopt);
  REQUIRE(book.GetBestAsk() == std::nullopt);
}

TEST_CASE("Matching engine - market", "[matchm]") {
  Order o = Order::Builder("", OrderType::Market)
            .SetSide(Side::Sell)
            .SetQuantity(100)
            .Build();
  
  Order o2 = Order::Builder("", OrderType::Market)
            .SetSide(Side::Buy)
            .SetQuantity(100)
            .Build();

  Orderbook book;
  book.AddOrder(o);
  book.AddOrder(o2);

  REQUIRE(book.GetBestAsk() == std::nullopt);
  REQUIRE(book.GetBestBid() == std::nullopt);

  Order o3 = Order::Builder("", OrderType::Limit)
            .SetSide(Side::Sell)
            .SetPrice(200)
            .SetQuantity(105)
            .Build(); 
  
  Order o4 = Order::Builder("", OrderType::Market)
            .SetSide(Side::Buy)
            .SetQuantity(100)
            .Build();
  
  book.AddOrder(o3);
  book.AddOrder(o4);

  REQUIRE(book.GetBestAsk().value().GetQuantity() == 5);
  REQUIRE(book.GetBestBid() == std::nullopt);
}