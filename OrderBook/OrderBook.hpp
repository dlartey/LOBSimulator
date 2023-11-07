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

class OrderBook
{
    std::map<double, int> bids, asks;

public:
    inline bool is_empty()
    {
        return bids.empty() && asks.empty();
    }
};

#endif /* OrderBook_hpp */
