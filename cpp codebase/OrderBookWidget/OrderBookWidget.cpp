//
//  OrderBookWidget.cpp
//  DBHandler
//
//  Created by Shreyas Honnalli on 11/12/2023.
//

#include "OrderBookWidget.hpp"

OrderBookWidget::OrderBookWidget(DBHandler *handler, OrderBook *orderBookParam)
{
    orderBook = orderBookParam;
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    bidsLabel = new QLabel("Bids", centralWidget);
    mainLayout->addWidget(bidsLabel);

    bidsTableWidget = new QTableWidget(centralWidget);
    initializeTable(bidsTableWidget, {"ID", "Price", "Quantity"});
    mainLayout->addWidget(bidsTableWidget);

    asksLabel = new QLabel("Asks", centralWidget);
    mainLayout->addWidget(asksLabel);

    asksTableWidget = new QTableWidget(centralWidget);
    initializeTable(asksTableWidget, {"ID", "Price", "Quantity"});
    mainLayout->addWidget(asksTableWidget);

    // Connect to the orderBookUpdated signal
    connect(handler, &DBHandler::orderBookUpdated, this, &OrderBookWidget::updateBothTables);
}

void OrderBookWidget::initializeTable(QTableWidget *tableWidget, const QStringList &headers)
{
    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(headers.size());
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

OrderBookWidget::~OrderBookWidget()
{
}

void OrderBookWidget::updateTable(std::vector<Order> &newOrders, QTableWidget *tableWidget)
{
    tableWidget->clearContents();
    tableWidget->setRowCount((int)newOrders.size());

    for (int i = 0; i < newOrders.size(); ++i)
    {
        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(newOrders[i].id));
        QTableWidgetItem *priceItem = new QTableWidgetItem(QString::number(newOrders[i].price));
        QTableWidgetItem *quantityItem = new QTableWidgetItem(QString::number(newOrders[i].quantity));
        tableWidget->setItem(i, 0, idItem);
        tableWidget->setItem(i, 1, priceItem);
        tableWidget->setItem(i, 2, quantityItem);
    }
}

std::vector<Order> OrderBookWidget::getNewOrdersFromOrderbook(bool is_bid)
{
    std::vector<Order> orderList;

    std::map<double, OrderList> mapStructure;
    if (is_bid)
        mapStructure = orderBook->getBids();
    else
        mapStructure = orderBook->getAsks();
    for (auto priceLevel : mapStructure)
    {
        std::list<Order> ordersForPL = priceLevel.second;
        for (Order o : ordersForPL)
            orderList.push_back(o);
    }
    return orderList;
}

void OrderBookWidget::updateBothTables()
{
    std::lock_guard<std::mutex> lock(orderBook->OB_mutex);
    if (orderBook->empty())
        return;

    std::vector<Order> bids = getNewOrdersFromOrderbook(true);
    std::vector<Order> asks = getNewOrdersFromOrderbook(false);

    updateTable(bids, bidsTableWidget);
    updateTable(asks, asksTableWidget);
}
