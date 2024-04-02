#include <torch/script.h>
// IMPORTANT NOTE: PYTORCH HAS TO BE IN THE BEGINNING BEFORE ANY QT IMPORTS FOR THIS TO WORK
// #include <torch/script.h>
// #include <torch/torch.h>

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
#include "OrderBook.hpp"
#include "DBHandler.hpp"
#include "API.hpp"
#include "OrderBookWidget.hpp"
#include "CentralWidget.hpp"
#include <QApplication>
#include <QtCharts>

OrderBook globalOrderBook;
volatile std::sig_atomic_t gSignalStatus;
bool flag = false;

// Signal handler function
void signal_handler(int signal) {
  gSignalStatus = signal;
}

void asyncFunction(std::string baseSQLStatement, DBHandler *handler) {
  // Loop to continuously fetch new entries
  while (gSignalStatus != SIGINT) {
    globalOrderBook.OB_mutex.lock();
    globalOrderBook.clear_order_book();
    handler->updateOrderBookFromDB(globalOrderBook);
    globalOrderBook.OB_mutex.unlock();
    handler->emitSuccessfulUpdate();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}

int updateOrderBook(float *values) {
  for (int i = 6; i > 0; i--) {
    double price_level = API::getPrice() + i - 3;
    double new_quantity = abs(values[i - 1]);
    globalOrderBook.add_order(i, price_level, new_quantity, i <= 3);
  }
  return 0;
}

void getNextCentre(float *values, torch::Tensor &output) {
  if (!output.defined() || output.numel() < 12 || !output.is_contiguous()) {
    std::cerr << "Output tensor is not valid or does not have the expected number of elements." << std::endl;
    return;
  }
  // result of how many bids we have (denoted by negative values)
  int balance = 0;

  float temp[12];
  if (output.dim() == 2) {
    for (int64_t j = 0; j < output.size(1); ++j) {
      temp[j] = output[0][j].item<float>();

      if (temp[j] < 0)
        balance++;
    }
    std::cout << std::endl;
  } else {
    std::cout << "Tensor is not 2D. It has " << output.dim() << " dimensions." << std::endl;
  }

  // do balance - 6 to calculate the next best bid for the centre of the OB
  API::setPrice(API::getPrice() + balance - 6);

  // Store the new centre in vals
  for (int i = 2; i <= 6; ++i) {
    if (temp[i] < 0 && temp[i + 1] > 0) {
      for (int j = 0; j < 6; ++j) {
        values[j] = temp[i - 2 + j];
      }
      break;
    }
  }
}

void asyncSQLTest(std::string SQLStatement, DBHandler *handler) {
  std::thread(asyncFunction, SQLStatement, &(*handler)).detach();
}

std::string getProjectSourceDirectory() {
  std::string currFilePath = __FILE__;
  std::filesystem::path fullPath(currFilePath);
  return fullPath.parent_path().string();
}

void startServerWrapper(DBHandler &handler) {
  API::startServer(globalOrderBook, handler);
}

void generateQuantity(DBHandler *handler) {
  try {
    std::string model_path = getProjectSourceDirectory() + "/Test1.pt";
    std::cout << model_path << std::endl;
    torch::jit::script::Module model = torch::jit::load(model_path);
    model.eval();

    torch::Tensor output = torch::empty({0});
    float values[6] = {-0.7787362, -0.8905415, -0.6589257, 0.62962157, 1.2157797, 2.0017693};

    while (true) {
      if (flag) {
        getNextCentre(values, output);
      }

      flag = true;
      torch::Tensor S_t = torch::from_blob(values, torch::IntArrayRef{1, 6});
      torch::Tensor z_t = torch::randn({1, 12});
      output = model.forward({z_t, S_t}).toTensor();

      if (output.dim() == 2) {
        for (int64_t j = 0; j < output.size(1); ++j) {
          std::cout << output[0][j].item<float>() << " ";
        }
        std::cout << std::endl; // New line after each row
        std::cout << "Best bid = " << API::getPrice() << std::endl;
      } else {
        std::cout << "Tensor is not 2D. It has " << output.dim() << " dimensions." << std::endl;
      }
      globalOrderBook.clear_order_book();
      updateOrderBook(values);
      globalOrderBook.print_order_book();
      handler->emitSuccessfulUpdate();
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  }
  catch (const c10::Error &e) {
    std::cerr << "Error loading the model: " << e.what() << std::endl;
  }
}

int main(int argc, char *argv[]) {
  // Register signal handler for graceful shutdown
  std::signal(SIGINT, signal_handler);

  DBHandler handler(getProjectSourceDirectory());

  // Start the async SQL test in a separate thread
  //asyncSQLTest("SELECT * FROM book", &handler);
  std::thread serverThread(startServerWrapper, std::ref(handler));
  std::thread startGenerate(generateQuantity, &handler);

  QApplication app(argc, argv);

  CentralWidget centralWidget(&handler, &globalOrderBook);
  centralWidget.show();
  int result = app.exec();
  serverThread.join();
  startGenerate.join();

  std::cout << "Application exiting..." << std::endl;
  return result;
}
