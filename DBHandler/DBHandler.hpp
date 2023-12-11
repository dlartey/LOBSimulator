//
//  DBHandler.hpp
//  OrderBook
//
//  Created by Shreyas Honnalli on 11/12/2023.
//

#ifndef DBHandler_hpp
#define DBHandler_hpp

#include <stdio.h>
#include <sqlite3.h>
#include <string>
#include "OrderBook.hpp"

class DBHandler
{
private:
    sqlite3 *DB;
    std::string filePathDB;

    void connectToDB();

public:
    DBHandler(std::string parentFilePath);
    ~DBHandler();

    void updateOrderBookFromDB(OrderBook &orderBook);

    sqlite3 *getDB();
    std::string getFilePathDB();
};

#endif /* DBHandler_hpp */
