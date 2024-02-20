// IMPORTANT NOTE: PYTORCH HAS TO BE IN THE BEGINNING BEFORE ANY QT IMPORTS FOR THIS TO WORK
#include <torch/script.h>
#include <torch/torch.h>

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
#include "CentralWidget.hpp"
#include <QApplication>
#include <QtCharts>

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
        handler->emitSuccessfulUpdate();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
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

    CentralWidget centralWidget(&handler, &globalOrderBook);
    centralWidget.show();
    int result = app.exec();
    return result;
}
