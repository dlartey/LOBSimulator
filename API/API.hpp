//
// Created by Daniel on 29/01/2024.
//
#ifndef API_hpp
#define API_hpp

#include <iostream>
#include <httplib.h>
#include "OrderBook.hpp"

class API {
private:
    static httplib::Server s;
    static httplib::Client cli;

public:
    static void getOrders(OrderBook &o);
    static void deleteOrder(OrderBook &o);
    static void submitOrder(OrderBook &o);
    static void startServer(OrderBook &o);

};

#endif
