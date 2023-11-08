#include <gtest/gtest.h>
#include <OrderBook.hpp>

TEST(OrderBookTets, OrderBookEmptyByDefault)
{
    OrderBook book;
    EXPECT_TRUE(book.is_empty());
}