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
#include <QApplication>
#include <QSplashScreen>
#include <QProgressBar>
#include <QTimer>
#include <QCoreApplication>

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
    QApplication app(argc, argv);
    
    qDebug() << "Current dir:" << QDir::currentPath();

    QPixmap pixmap; // Declare the QPixmap variable outside the if-else structure to ensure its scope covers the entire function
    
    // If running on Xcode, the directory is usually the Debug folder
    if (QDir::currentPath().contains("Debug")) {
        pixmap = QPixmap("../../resources/UoLSE_Logo.png");
    } else {
        pixmap = QPixmap("COMP5530M/cpp codebase/resources/UoLSE_Logo.png");
    }

    QSplashScreen splash(pixmap);
    splash.show();
    app.processEvents(); // Ensures that the splash screen is displayed immediately

    // Optional: Add a progress bar or label to display loading status
    QLabel loadingLabel(&splash);
    loadingLabel.setText("Initializing...");
    loadingLabel.setStyleSheet("color: white;"); // Customize as needed
    loadingLabel.setAlignment(Qt::AlignBottom);
    loadingLabel.setGeometry(splash.geometry());

    // This is where you'd perform your loading tasks
    // For demonstration, let's simulate a delay
    for(int i = 0; i < 100; i += 20) {
        // Update loading status here
        loadingLabel.setText(QString("Loading... %1%").arg(i));
        QThread::sleep(1); // Simulate time-consuming task
        app.processEvents();
    }

    DBHandler handler(getProjectSourceDirectory());

    std::thread serverThread(startServerWrapper, std::ref(handler));

    httplib::Client cli("localhost:8080");
    nlohmann::json body;
    auto res = cli.Post("/submit", body.dump(), "application/json");

    CentralWidget centralWidget(&handler, &globalOrderBook);
    splash.finish(&centralWidget); // Close the splash screen
//    centralWidget.show();
    centralWidget.showMaximized();

    int result = app.exec();
    // Your application cleanup follows

    return result;
}
