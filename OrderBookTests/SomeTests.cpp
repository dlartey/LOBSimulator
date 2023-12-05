#include <gtest/gtest.h>
#include <OrderBook.hpp>
#include <iostream>
#include <sqlite3.h>
#include <filesystem>
#include <thread>
#include <chrono>
#include <future>
#include <filesystem>
#include <csignal>
#include <csignal>
#include <mutex>
#include <string>
using namespace std;
static std::mutex lastIdMutex;
static int lastProcessedId = 1;
OrderBook globalOrderBook;

TEST(OrderBookTets, OrderBookEmptyByDefault)
{
    OrderBook book;
    EXPECT_TRUE(book.is_empty());
}

string getProjectSourceDirectory(){
    std::string currFilePath = __FILE__;
    std::filesystem::path fullPath(currFilePath);
    cout << fullPath.parent_path().parent_path().string();
    return fullPath.parent_path().parent_path().string();
}

sqlite3* connectToDB(string dbFilePath){
    sqlite3* DB;
    int exit = sqlite3_open(dbFilePath.c_str(), &DB);
    if (exit != SQLITE_OK){
        sqlite3_close(DB);
        throw runtime_error("Failed to open database.");
    }
    return DB;
}

int dcallback(void* notUsed, int argc, char** argv, char** azColName) {
    if (argc == 4) {
        int index = std::stoi(argv[0]);
        // Check if this row has already been processed
        if (index <= lastProcessedId) return 0;

        double price_level = std::stod(argv[1]);
        double new_quantity = std::stod(argv[2]);
        std::string id = argv[3];

        // Add the order to the OrderBook
        bool is_bid = id.find("_ask") == std::string::npos; // Assuming "_ask" is part of the ask IDs
        globalOrderBook.add_order(index, price_level, new_quantity, is_bid);

        // Update the last processed index
        lastProcessedId = index;
    }
    return 0;
}

TEST(OrderBookTets, CheckDBConnectionOK)
{
    string dbFilePath = getProjectSourceDirectory() + "/prime_orderbook.db";
    EXPECT_TRUE(connectToDB(dbFilePath));
}

TEST(OrderBookTets, ReadFromDB)
{
    sqlite3* db = nullptr;
    string dbFilePath = getProjectSourceDirectory() + "/prime_orderbook.db";
    cout << dbFilePath+"\n";
    int rc = sqlite3_open(dbFilePath.c_str(), &db);

    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

    EXPECT_TRUE(db);
    char *zErrMsg = nullptr;
    std::string baseSQLStatement = "SELECT * FROM book";
    std::string modifiedSQLStatement = baseSQLStatement + " WHERE \"index\" > " + std::to_string(lastProcessedId) + " ORDER BY \"index\" ASC";
    //cout << modifiedSQLStatement;
    sqlite3_exec(db, modifiedSQLStatement.c_str(), dcallback, nullptr, &zErrMsg);
    //cout << zErrMsg;
    EXPECT_FALSE(globalOrderBook.is_empty());
}

TEST(OrderBookTets, BidAmount)
{
    sqlite3* db = nullptr;
    string dbFilePath = getProjectSourceDirectory() + "/prime_orderbook.db";
    sqlite3_open(dbFilePath.c_str(), &db);
    EXPECT_TRUE(db);
    std::string baseSQLStatement = "SELECT * FROM book";
    std::string modifiedSQLStatement = baseSQLStatement + " WHERE \"index\" > " + std::to_string(lastProcessedId) + " ORDER BY \"index\" ASC";
    sqlite3_exec(db, modifiedSQLStatement.c_str(), dcallback, nullptr, nullptr);
    ASSERT_EQ(globalOrderBook.getBids().size(), 236);
}

TEST(OrderBookTets, AsksAmount)
{
    sqlite3* db = nullptr;
    string dbFilePath = getProjectSourceDirectory() + "/prime_orderbook.db";
    sqlite3_open(dbFilePath.c_str(), &db);
    EXPECT_TRUE(db);
    std::string baseSQLStatement = "SELECT * FROM book";
    std::string modifiedSQLStatement = baseSQLStatement + " WHERE \"index\" > " + std::to_string(lastProcessedId) + " ORDER BY \"index\" ASC";
    sqlite3_exec(db, modifiedSQLStatement.c_str(), dcallback, nullptr, nullptr);

    ASSERT_EQ(globalOrderBook.getAsks().size(), 204);
}

TEST(OrderBookTets, PriceLevel)
{
    sqlite3* db = nullptr;
    string dbFilePath = getProjectSourceDirectory() + "/prime_orderbook.db";
    sqlite3_open(dbFilePath.c_str(), &db);
    EXPECT_TRUE(db);
    std::string baseSQLStatement = "SELECT * FROM book";
    std::string modifiedSQLStatement = baseSQLStatement + " WHERE \"index\" > " + std::to_string(lastProcessedId) + " ORDER BY \"index\" ASC";
    sqlite3_exec(db, modifiedSQLStatement.c_str(), dcallback, nullptr, nullptr);
    //cout << *max_element(globalOrderBook.getAsks().rbegin(), globalOrderBook.getAsks().rend());
    ASSERT_EQ(3, 204);
}