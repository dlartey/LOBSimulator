#include "CentralWidget.hpp"

CentralWidget::CentralWidget(DBHandler *handler, OrderBook *orderBook, QWidget *parent) : QWidget(parent), orderBookWidget(handler, orderBook)
{
    this->setStyleSheet("background-color: #000000;");
    QHBoxLayout *horizontalLayout = new QHBoxLayout(this);

    QWidget *emptyWidget = new QWidget(this);
    emptyWidget->setFixedSize(200, 200); // Set the fixed size as needed
    emptyWidget->setStyleSheet("background-color: #000000;");

    horizontalLayout->addWidget(&orderBookWidget);
}