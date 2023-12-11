//
//  OrderBookWidget.cpp
//  DBHandler
//
//  Created by Shreyas Honnalli on 11/12/2023.
//

#include "OrderBookWidget.hpp"

OrderBookWidget::OrderBookWidget (DBHandler* handler, OrderBook* orderBookParam) {
    orderBook = orderBookParam;
    tableWidget = new QTableWidget(this);
    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(3);
    tableWidget->setHorizontalHeaderLabels({"ID", "Price", "Quantity"});

    // Set up the layout
    layout = new QVBoxLayout(this);
    layout->addWidget(tableWidget);

    // Connect to the orderBookUpdated signal
    connect(handler, &DBHandler::orderBookUpdated, this, &OrderBookWidget::updateTable);
}

OrderBookWidget::~OrderBookWidget() {
    
}

void OrderBookWidget::updateTable()
{
    std::lock_guard<std::mutex> lock(orderBook->OB_mutex);
    if (orderBook->empty()) return;
    std::vector<Order> orders;
    for(auto pl: orderBook->getAsks()){
        std::list<Order> ordersForPL = pl.second;
        for(Order o: ordersForPL) orders.push_back(o);
    }
    
    tableWidget->clearContents();
    tableWidget->setRowCount((int)orders.size());
    
    for (int i = 0; i < orders.size(); ++i) {
            QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(orders[i].id));
            QTableWidgetItem *priceItem = new QTableWidgetItem(QString::number(orders[i].price));
            QTableWidgetItem *quantityItem = new QTableWidgetItem(QString::number(orders[i].quantity));

            tableWidget->setItem(i, 0, idItem);
            tableWidget->setItem(i, 1, priceItem);
            tableWidget->setItem(i, 2, quantityItem);
        }
//    std::cout << orderBook->getOrderCount() << std::endl;
}
