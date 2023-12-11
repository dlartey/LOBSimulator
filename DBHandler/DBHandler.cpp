//
//  DBHandler.cpp
//  OrderBook
//
//  Created by Shreyas Honnalli on 11/12/2023.
//

#include "DBHandler.hpp"

DBHandler::DBHandler(std::string parentFilePath)
{
    filePathDB = parentFilePath + "/prime_orderbook.db";
    try
    {
        connectToDB();
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Failed to connect to database.");
    }
}

DBHandler::~DBHandler()
{
    sqlite3_close(DB);
    DB = nullptr;
}

void DBHandler::connectToDB()
{
    int exit = sqlite3_open(filePathDB.c_str(), &DB);
    if (exit != SQLITE_OK)
    {
        sqlite3_close(DB);
        throw std::runtime_error("Successfully connected but failed to open database.");
    }
}

int updateOrderBook(void *orderBookPtr, int argc, char **argv, char **azColName)
{
    OrderBook *orderBook = static_cast<OrderBook *>(orderBookPtr);
    if (argc == 4)
    {
        int index = std::stoi(argv[0]);
        double price_level = std::stod(argv[1]);
        double new_quantity = std::stod(argv[2]);
        std::string id = argv[3];
        bool is_bid = id.find("_ask") == std::string::npos;
        orderBook->add_order(index, price_level, new_quantity, is_bid);
    }
    return 0;
}

void DBHandler::updateOrderBookFromDB(OrderBook &orderBook)
{
    if (DB == nullptr)
        std::cout << "DB is NULL!!";
    std::string SQLStatement = "SELECT * FROM book";
    sqlite3_exec(DB, SQLStatement.c_str(), updateOrderBook, &orderBook, nullptr);
}

sqlite3 *DBHandler::getDB()
{
    return DB;
}

std::string DBHandler::getFilePathDB()
{
    return filePathDB;
}
