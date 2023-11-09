//
//  OrderBook.cpp
//  TradingEngine
//
//  Created by Shreyas Honnalli on 06/11/2023.
//

#include "OrderBook.hpp"

// Check if the order book is empty
bool OrderBook::is_empty() const {
    return bids.empty() && asks.empty();
}

// Add a bid (buy) order to the order book
void OrderBook::add_bid(int price, int amount) {
    add(price, amount, true);
}

// Add an ask (sell) order to the order book
void OrderBook::add_ask(int price, int amount) {
    add(price, amount, false);
}

// Add an order to the order book
void OrderBook::add(int price, int amount, bool bid) {
    if (bid)
        bids[price] += amount; // If it's a bid, add to the bids side
    else
        asks[price] += amount; // If it's an ask, add to the asks side
}

// Overload the << operator to print the order book
std::ostream& operator<<(std::ostream& os, const OrderBook& book) {
    if (book.is_empty()) {
        os << "ORDER BOOK EMPTY";
        return os;
    }

    // Print the asks side of the order book (from highest price to lowest)
    for (auto it = book.asks.rbegin(); it != book.asks.rend(); ++it)
        os << it->first << "\t" << it->second << std::endl;

    os << std::endl;

    // Print the bids side of the order book (from highest price to lowest)
    for (auto it = book.bids.rbegin(); it != book.bids.rend(); ++it)
        os << it->first << "\t" << it->second << std::endl;

    return os;
}