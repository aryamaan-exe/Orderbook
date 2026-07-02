#include "catch_amalgamated.hpp"
#include "Order.hpp"

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

