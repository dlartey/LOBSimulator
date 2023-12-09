//
//  OrderBook.hpp
//  TradingEngine
//
//  Created by Shreyas Honnalli on 06/11/2023.
//

#ifndef OrderBook_hpp
#define OrderBook_hpp
#include <mutex>

extern std::mutex orderBookMutex;


#include <iostream> // Correct header for std::ostream
#include <map>
#include <list>

struct Order {
    int id;
    double price;
    double quantity;

    Order(int id, double price, double quantity) : id(id), price(price), quantity(quantity) {}
};

using OrderList = std::list<Order>;

class OrderBook {
private: // Explicitly stating that bids and asks are private
    std::map<double, OrderList> bids, asks;

public:
    bool is_empty() const;
    void add_order(int id, double price, double quantity, bool is_bid);
    void remove_order(int id, double price, bool is_bid);
    void modify_order(int id, double old_price, double new_price, double new_quantity, bool is_bid);
    void print_order_book() const;

    std::map<double, OrderList> getBids();
    std::map<double, OrderList> getAsks();

    friend std::ostream& operator<<(std::ostream& os, const OrderBook& book);
};

#endif /* OrderBook_hpp */

