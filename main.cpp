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

// Define your mutex and OrderBook instance globally
OrderBook globalOrderBook;
volatile std::sig_atomic_t gSignalStatus;

using namespace std;

// Signal handler function
void signal_handler(int signal) {
    gSignalStatus = signal;
}


int callback(void*, int argc, char** argv, char** azColName) {
    std::lock_guard<std::mutex> guard(orderBookMutex); // Ensure thread safety

    int id;
    double price, quantity;
    bool is_bid;

    // Parse the data. This assumes that your database columns are in the order of:
    // id, price, quantity, is_bid (where is_bid is 0 for asks and 1 for bids)
    if (argc == 4) {
        id = std::stoi(argv[0]);
        price = std::stod(argv[1]);
        quantity = std::stod(argv[2]);
        is_bid = std::stoi(argv[3]) == 1; // Assuming 1 for bid and 0 for ask

        // Add the order to the global order book
        globalOrderBook.add_order(id, price, quantity, is_bid);
    }

    return 0;
}


void asyncFunction(string SQLStatement, sqlite3* DB){
    while(true){
        sqlite3_exec(DB, SQLStatement.c_str(), callback, nullptr, NULL);
        cout << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }
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
