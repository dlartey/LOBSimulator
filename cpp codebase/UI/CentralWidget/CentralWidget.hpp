#ifndef CentralWidget_hpp
#define CentralWidget_hpp

#include <stdio.h>
#include <QApplication>
#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <mutex>
#include <list>
#include <thread>
#include <chrono>
#include "OrderBookWidget.hpp"

class CentralWidget : public QWidget
{
    Q_OBJECT
private:
    OrderBookWidget orderBookWidget;

public:
    explicit CentralWidget(DBHandler *handler, OrderBook *orderBook, QWidget *parent = nullptr);
};

#endif /* OrderBookWidget_hpp */
