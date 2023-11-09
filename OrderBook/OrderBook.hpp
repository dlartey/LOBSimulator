//
//  OrderBook.hpp
//  TradingEngine
//
//  Created by Shreyas Honnalli on 06/11/2023.
//

#ifndef OrderBook_hpp
#define OrderBook_hpp

#include <stdio.h>
#include <map>
#include <ostream>


class OrderBook
{
    std::map<int, int> bids, asks;
    void add(int price, int amount, bool bid);


public:
 
    bool is_empty() const;
    void add_bid(int price, int ammount);
    void add_ask(int price, int amount);



    friend std::ostream& operator<<(std::ostream& os, const OrderBook& book);

    
};

#endif /* OrderBook_hpp */
