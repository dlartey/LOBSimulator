#include "CentralWidget.hpp"
#include <QSplitter>
#include <QDesktopWidget>
#include <QApplication>
#include <QScreen>

CentralWidget::CentralWidget(DBHandler *handler, OrderBook *orderBook, QWidget *parent)
    : QWidget(parent), orderBookWidget(handler, orderBook), candlestickWidget(handler, orderBook), headerWidget(handler, orderBook)  {

    setWidgetProperties();
    QVBoxLayout *mainLayout = new QVBoxLayout(this); // Use QVBoxLayout to arrange header and content vertically

    mainLayout->addWidget(&headerWidget); // Adds the header widget directly to the main layout

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this); // Use QSplitter for dynamic resizing
    splitter->addWidget(&candlestickWidget);
    splitter->addWidget(&orderBookWidget);
    splitter->setStretchFactor(0, 1); // CandlestickWidget initially takes half the available width
    splitter->setStretchFactor(1, 1); // OrderBookWidget also takes half the available width

    // Remove scrollArea and add splitter directly to layout
    mainLayout->addWidget(splitter); // Add splitter to main layout
    mainLayout->setSpacing(0); // Eliminate any space between widgets
    mainLayout->setMargin(0); // No margin around the layout
}

void CentralWidget::setWidgetProperties() {
    // Get the screen on which the application's main window is being displayed
    QScreen *screen = QApplication::primaryScreen();  // Adjust to QApplication::screens()[0] if specific screen needed
    QRect screenGeometry = screen->geometry();
    int width = screenGeometry.width();  // Use full screen width
    int height = screenGeometry.height();  // Use full screen height
    this->setGeometry(0, 0, width, height);
    this->setStyleSheet("background-color: #292934;");
}

