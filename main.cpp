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
#include <string>        // For std::stoi and std::stod
#include "OrderBook.hpp" // Include your OrderBook definition
#include "DBHandler.hpp"
#include <QApplication>
#include <QPushButton>
#include <QObject>

OrderBook globalOrderBook;
volatile std::sig_atomic_t gSignalStatus;

// Signal handler function
void signal_handler(int signal)
{
    gSignalStatus = signal;
}

void asyncFunction(std::string baseSQLStatement, DBHandler *handler)
{
    // Loop to continuously fetch new entries
    while (gSignalStatus != SIGINT)
    {
        globalOrderBook.OB_mutex.lock();
        globalOrderBook.clear_order_book();
        handler->updateOrderBookFromDB(globalOrderBook);
        globalOrderBook.OB_mutex.unlock();
    }
}

void asyncSQLTest(std::string SQLStatement, DBHandler *handler)
{
    std::thread(asyncFunction, SQLStatement, &(*handler)).detach();
}

std::string getProjectSourceDirectory()
{
    std::string currFilePath = __FILE__;
    std::filesystem::path fullPath(currFilePath);
    return fullPath.parent_path().string();
}

int main(int argc, char *argv[])
{
    // Register signal handler for graceful shutdown
    std::signal(SIGINT, signal_handler);

    DBHandler handler(getProjectSourceDirectory());

    // Start the async SQL test in a separate thread
    asyncSQLTest("SELECT * FROM book", &handler);

    QApplication app(argc, argv);
    QPushButton button("Say Hello");
    button.show();
    int result = app.exec();

    // while (gSignalStatus != SIGINT) {
    //     globalOrderBook.OB_mutex.lock();
    //     if (globalOrderBook.getOrderCount()) std::cout << globalOrderBook.getOrderCount() << std::endl;
    //     globalOrderBook.OB_mutex.unlock();
    //     //std::this_thread::sleep_for(std::chrono::seconds(1));
    // }
    while (gSignalStatus != SIGINT)
    {
        //         globalOrderBook.OB_mutex.lock();
        //         std::cout << globalOrderBook.getOrderCount() << " ";
        //         globalOrderBook.OB_mutex.unlock();
    }

    std::cout << "Application exiting..." << std::endl;
    return result;
}
