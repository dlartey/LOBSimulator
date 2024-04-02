//
//  OrderBookWidget.cpp
//
//  Created by Shreyas Honnalli on 11/12/2023.
//

#include "OrderBookWidget.hpp"

OrderBookWidget::OrderBookWidget(DBHandler *handler, OrderBook *orderBookParam) : orderBook(orderBookParam)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    asksTableWidget = new QTableWidget;
    bidsTableWidget = new QTableWidget;
    
    initializeTable(asksTableWidget, {"ID", "Price", "Quantity"});
    initializeTable(bidsTableWidget, {"ID", "Price", "Quantity"});
    
    mainLayout->addWidget(asksTableWidget);
    mainLayout->addWidget(bidsTableWidget);
    
    this->setLayout(mainLayout);

    // Connect to the orderBookUpdated signal
    connect(handler, &DBHandler::orderBookUpdated, this, &OrderBookWidget::updateBothTables);
}

void OrderBookWidget::initializeTable(QTableWidget *tableWidget, const QStringList &headers)
{
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(headers.size());
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tableWidget->setFixedWidth(300);
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
    addColoursToTables();
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
    reverse(bids.begin(), bids.end());
    reverse(asks.begin(), asks.end());
    updateTable(bids, bidsTableWidget);
    updateTable(asks, asksTableWidget);
}

void OrderBookWidget::addColoursToTables()
{
    QBrush redBrush(Qt::red);

    for (int row = 0; row < asksTableWidget->rowCount(); ++row)
    {
        for (int col = 0; col < asksTableWidget->columnCount(); ++col)
        {
            QTableWidgetItem *item = asksTableWidget->item(row, col);
            if (item)
            {
                item->setForeground(redBrush);
            }
        }
    }

    QBrush greenBrush(Qt::green);

    for (int row = 0; row < bidsTableWidget->rowCount(); ++row)
    {
        for (int col = 0; col < bidsTableWidget->columnCount(); ++col)
        {
            QTableWidgetItem *item = bidsTableWidget->item(row, col);
            if (item)
            {
                item->setForeground(greenBrush);
            }
        }
    }
}
