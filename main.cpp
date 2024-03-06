#include <torch/script.h>
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
#include "API.hpp"
#include "OrderBookWidget.hpp"
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
        handler->emitSuccessfulUpdate();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void getNextCentre(float *values, torch::Tensor *output)
{
    float *data = (*output).data_ptr<float>();
    for (int i = 2; i < 9; i++)
    {
        // if centre
        if (data[i] < 0 && data[i + 1] > 0)
        {
            values[0] = data[i - 2];
            values[1] = data[i - 1];
            values[2] = data[i];
            values[3] = data[i + 1];
            values[4] = data[i + 2];
            values[5] = data[i + 3];
            break;
        }
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

void startServerWrapper() {
    API::startServer(globalOrderBook);
}

//int main(int argc, char *argv[])
//{
//    // Register signal handler for graceful shutdown
//    std::signal(SIGINT, signal_handler);
//    std::thread serverThread(startServerWrapper);
//    DBHandler handler(getProjectSourceDirectory());
//    // Start the async SQL test in a separate thread
//    asyncSQLTest("SELECT * FROM book", &handler);
//    QApplication app(argc, argv);
//    OrderBookWidget obw(&handler, &globalOrderBook);
//    obw.show();
////    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
//    int result = app.exec();
//    serverThread.join();
//
//    std::cout << "Application exiting..." << std::endl;
//    return result;
//}

int main(int argc, char *argv[])
{
    /*
    // Register signal handler for graceful shutdown
    std::signal(SIGINT, signal_handler);

    DBHandler handler(getProjectSourceDirectory());

    // Start the async SQL test in a separate thread
    asyncSQLTest("SELECT * FROM book", &handler);
    QApplication app(argc, argv);
    OrderBookWidget obw(&handler, &globalOrderBook);
    obw.show();
    //    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    int result = app.exec();

    std::cout << "Application exiting..." << std::endl;
    */


    try
    {
        std::string currFilePath = __FILE__;
        std::filesystem::path fullPath(currFilePath);
        std::string model_path = fullPath.parent_path().string();
        model_path += "/BatchSize64Base1000Epochs.pt";
        std::cout << model_path << std::endl;
        torch::jit::script::Module model = torch::jit::load(model_path);
        model.eval();
        float values[6];
        torch::Tensor output;
        for (int i = 0; i < 70000; i++)
        {
            if (i == 0)
            {
                values[0] = -0.7787362;
                values[1] = -0.8905415;
                values[2] = -0.6589257;
                values[3] = 0.62962157;
                values[4] = 1.2157797;
                values[5] = 2.0017693;
            }
            else
                getNextCentre(values, &output);
            torch::Tensor S_t = torch::from_blob(values, torch::IntArrayRef{1, 6});
            torch::Tensor z_t = torch::randn({1, 12});
            output = model.forward({z_t, S_t}).toTensor();

            if (output.dim() == 1) {
                for (int64_t i = 0; i < output.size(0); ++i) {
                    std::cout << output[i].item<float>() << " ";
                }
                std::cout << std::endl;
            } else {
                std::cout << "Tensor is not 1D. It has " << output.dim() << " dimensions." << std::endl;
            }
        }
    }
    catch (const c10::Error &e)
    {
        std::cerr << "Error loading the model: " << e.what() << std::endl;
    }
    return 0;
}