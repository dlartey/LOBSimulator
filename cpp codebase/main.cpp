#include <torch/script.h>
// IMPORTANT NOTE: PYTORCH HAS TO BE IN THE BEGINNING BEFORE ANY QT IMPORTS FOR THIS TO WORK

#include <iostream>
#include <filesystem>
#include <thread>
#include <future>
#include <csignal>
#include <string>
#include "OrderBook.hpp"
#include "DBHandler.hpp"
#include "API.hpp"
#include "CentralWidget.hpp"
#include <QApplication>
#include <QtCharts>
#include <nlohmann/json.hpp>
#include <httplib.h>

OrderBook globalOrderBook;
volatile std::sig_atomic_t gSignalStatus;

void signal_handler(int signal) { gSignalStatus = signal; }

std::string getProjectSourceDirectory() {
  std::string currFilePath = __FILE__;
  std::filesystem::path fullPath(currFilePath);
  return fullPath.parent_path().string();
}

void startServerWrapper(DBHandler &handler) { API::startServer(globalOrderBook, handler); }

int main(int argc, char *argv[]) {
  std::signal(SIGINT, signal_handler);
  DBHandler handler(getProjectSourceDirectory());

  std::thread serverThread(startServerWrapper, std::ref(handler));

  httplib::Client cli("localhost:8080");
  nlohmann::json body;
  auto res = cli.Post("/submit", body.dump(), "application/json");

  QApplication app(argc, argv);

  CentralWidget centralWidget(&handler, &globalOrderBook);
  centralWidget.show();
  int result = app.exec();
  serverThread.join();

  std::cout << "Application exiting..." << std::endl;
  return result;
}
