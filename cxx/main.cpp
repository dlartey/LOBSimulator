#include <filesystem>
#include <thread>
#include <csignal>
#include <string>
#include "OrderBook.hpp"
#include "API.hpp"
#include "CentralWidget.hpp"
#include <QApplication>
#include <QtCharts>
#include <QSplashScreen>
#include <QTimer>

OrderBook globalOrderBook;
volatile std::sig_atomic_t gSignalStatus;
void signal_handler(int signal) { gSignalStatus = signal; }

std::string getProjectSourceDirectory() {
  std::string currFilePath = __FILE__;
  std::filesystem::path fullPath(currFilePath);
  return fullPath.parent_path().string();
}

void startServerWrapper() { API::startServer(globalOrderBook); }

int main(int argc, char *argv[]) {
    std::signal(SIGINT, signal_handler);
    QApplication app(argc, argv);

    std::string logo = getProjectSourceDirectory() +"/resources/images/UoLSE_Logo.png";
    QPixmap pixmap = QPixmap(logo.c_str());
    // Declare the QPixmap variable outside the if-else structure to ensure its scope covers the entire function

    QSplashScreen splash(pixmap);
    splash.show();
    app.processEvents(); // Ensures that the splash screen is displayed immediately

    // Optional: Add a progress bar or label to display loading status
    QLabel loadingLabel(&splash);
    loadingLabel.setText("Initializing...");
    loadingLabel.setStyleSheet("color: white;");
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

    std::thread serverThread(startServerWrapper);
    CentralWidget centralWidget(&globalOrderBook);
    splash.finish(&centralWidget); // Close the splash screen

    centralWidget.showMaximized();
    int result = app.exec();
    return result;
}
