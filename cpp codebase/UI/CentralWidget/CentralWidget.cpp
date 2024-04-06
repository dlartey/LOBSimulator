#include "CentralWidget.hpp"
#include <QSplitter>

CentralWidget::CentralWidget(DBHandler *handler, OrderBook *orderBook, QWidget *parent)
    : QWidget(parent), orderBookWidget(handler, orderBook), candlestickWidget(handler, orderBook), headerWidget(handler, orderBook)  {

    setWidgetProperties();
    QVBoxLayout *mainLayout = new QVBoxLayout(this); // Use QVBoxLayout to arrange header and content vertically

    mainLayout->addWidget(&headerWidget); // Adds the header widget directly to the main layout

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this); // Use QSplitter for dynamic resizing
    splitter->addWidget(&candlestickWidget);
    splitter->addWidget(&orderBookWidget);
    splitter->setStretchFactor(0, 1); // Adjust these factors to change initial sizes
    splitter->setStretchFactor(1, 1);

    scrollArea.setWidgetResizable(true); // Make scroll area content responsive
    scrollArea.setWidget(splitter); // Add splitter to scroll area

    mainLayout->addWidget(&scrollArea); // Add scroll area to main layout
}

void CentralWidget::setWidgetProperties() {
    QRect screenGeometry = QApplication::desktop()->screenGeometry(); // Responsive to screen size
    int width = screenGeometry.width() * 0.4; // Use 80% of screen width
    int height = screenGeometry.height() * 0.4; // Use 80% of screen height
    this->setGeometry(0, 0, width, height);
    this->setStyleSheet("background-color: #292934;");
}

