//
//  OrderBookWidget.hpp
//  DBHandler
//
//  Created by Shreyas Honnalli on 11/12/2023.
//

#ifndef OrderBookWidget_hpp
#define OrderBookWidget_hpp

#include <stdio.h>
#include <QApplication>
#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include "OrderBook.hpp"
#include "DBHandler.hpp"
#include <mutex>
#include <list>
#include <thread>
#include <chrono>

class OrderBookWidget : public QWidget
{
    Q_OBJECT
private:
    QTableWidget *tableWidget;
    QVBoxLayout *layout;
    OrderBook *orderBook;

public:
    OrderBookWidget(DBHandler* handler, OrderBook* orderBook);
    ~OrderBookWidget();

public slots:
    // Slot to update the table when the order book is updated
    void updateTable();
};

#endif /* OrderBookWidget_hpp */
