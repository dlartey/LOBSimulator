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
#include <string> // For std::stoi and std::stod
#include "OrderBook.hpp" // Include your OrderBook definition

using namespace std;

// Define mutex and OrderBook instance globally
OrderBook globalOrderBook;
volatile std::sig_atomic_t gSignalStatus;
static int lastID = 0;
static std::mutex lastIdMutex;
static int lastProcessedId = 0;

using namespace std;

// Signal handler function
void signal_handler(int signal) {
    gSignalStatus = signal;
}



int callback(void* notUsed, int argc, char** argv, char** azColName) {
    std::lock_guard<std::mutex> guard(orderBookMutex);

    if (argc == 4) {
        int index = std::stoi(argv[0]);
        // Check if this row has already been processed
        std::lock_guard<std::mutex> lk(lastIdMutex);
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


void asyncFunction(string baseSQLStatement, sqlite3* DB) {
    static bool isRunning = false;
    {
        std::lock_guard<std::mutex> lk(lastIdMutex);
        if (isRunning) return; // Ensures single execution
        isRunning = true;
    }

    // Loop to continuously fetch new entries
    while (gSignalStatus != SIGINT) {
        std::string modifiedSQLStatement;
        {
            std::lock_guard<std::mutex> lk(lastIdMutex);
            modifiedSQLStatement = baseSQLStatement + " WHERE \"index\" > " + std::to_string(lastProcessedId) + " ORDER BY \"index\" ASC";
        }

        sqlite3_exec(DB, modifiedSQLStatement.c_str(), callback, nullptr, nullptr);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    isRunning = false;
}


void asyncSQLTest(string SQLStatement, sqlite3* DB){
    thread(asyncFunction, SQLStatement, DB).detach();
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

string getProjectSourceDirectory()
{
    std::string currFilePath = __FILE__;
    std::filesystem::path fullPath(currFilePath);
    return fullPath.parent_path().string();
}

int main(int argc, const char* argv[]) {
    // Register signal handler for graceful shutdown
    std::signal(SIGINT, signal_handler);

    string dbFilePath = getProjectSourceDirectory() + "/prime_orderbook.db";
    sqlite3* DB = nullptr;

    try {
        DB = connectToDB(dbFilePath);
    }
    catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return -1;
    }

    // Start the async SQL test in a separate thread
    asyncSQLTest("SELECT * FROM book", DB);

    // Main loop to print the order book
    while (gSignalStatus != SIGINT) {
        globalOrderBook.print_order_book(); // Print the state of the order book
        std::this_thread::sleep_for(std::chrono::seconds(5)); // Pause for 5 seconds
    }

    // Cleanup and shutdown
    if (DB) {
        sqlite3_close(DB);
        DB = nullptr;
    }

    cout << "Application exiting..." << endl;
    return 0;
}
