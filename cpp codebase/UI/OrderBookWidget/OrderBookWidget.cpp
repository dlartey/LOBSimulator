//
//  OrderBookWidget.cpp
//
//  Created by Shreyas Honnalli on 11/12/2023.
//

#include "OrderBookWidget.hpp"
#include <nlohmann/json.hpp>

OrderBookWidget::OrderBookWidget(DBHandler *handler, OrderBook *orderBookParam) : orderBook(orderBookParam) {
  QVBoxLayout *mainLayout = new QVBoxLayout;

  apiButton = new QPushButton("Submit Order", this);
  apiButton->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));

  currentBalance = new QLabel(this);
  currentBalance->setText("Current Balance = 100000");
  currentBalance->setGeometry(QRect(10, 10, 200, 20));

  currentQuantity = new QLabel(this);
  currentQuantity->setText("Current Quantity = 0");
  currentQuantity->setGeometry(QRect(50, 50, 200, 20));

  pnl = new QLabel(this);
  pnl->setText("Overall Position = 0");
  pnl->setGeometry(QRect(100, 100, 200, 20));

  orderType = new QComboBox(this);
  orderType->setPlaceholderText("Order Type");
  orderType->addItem("IOC");
  orderType->addItem("FOK");
  orderType->setGeometry(QRect(QPoint(200, 50), QSize(200, 40)));

  bidAsk = new QComboBox(this);
  bidAsk->setPlaceholderText("Bid or Ask");
  bidAsk->addItem("Bid");
  bidAsk->addItem("Ask");
  bidAsk->setGeometry(QRect(QPoint(200, 50), QSize(200, 40)));

  price = new QLineEdit(this);
  price->setPlaceholderText("Price");
  price->setGeometry(QRect(QPoint(100, 50), QSize(200, 40)));
  price->setValidator(new QDoubleValidator(price));

  quantity = new QLineEdit(this);
  quantity->setPlaceholderText("Quantity");
  quantity->setGeometry(QRect(QPoint(200, 50), QSize(200, 40)));
  quantity->setValidator(new QDoubleValidator(quantity));

  asksTableWidget = new QTableWidget;
  bidsTableWidget = new QTableWidget;

  initializeTable(asksTableWidget, {"ID", "Price", "Quantity"});
  initializeTable(bidsTableWidget, {"ID", "Price", "Quantity"});

  mainLayout->addWidget(asksTableWidget);
  mainLayout->addWidget(bidsTableWidget);
  mainLayout->addWidget(apiButton);
  mainLayout->addWidget(orderType);
  mainLayout->addWidget(bidAsk);
  mainLayout->addWidget(price);
  mainLayout->addWidget(quantity);
  mainLayout->addWidget(currentQuantity);
  mainLayout->addWidget(currentBalance);
  mainLayout->addWidget(pnl);

  this->setLayout(mainLayout);

  // Connect to the orderBookUpdated signal
  connect(apiButton, &QPushButton::clicked, this, &OrderBookWidget::invokeAPI);
  connect(handler, &DBHandler::orderBookUpdated, this, &OrderBookWidget::updateBothTables);
  connect(handler, &DBHandler::orderBookUpdated, this, &OrderBookWidget::updateBalance);
  connect(handler, &DBHandler::orderBookUpdated, this, &OrderBookWidget::updateQuantity);
  connect(handler, &DBHandler::orderBookUpdated, this, &OrderBookWidget::updatePnl);
}

void OrderBookWidget::initializeTable(QTableWidget *tableWidget, const QStringList &headers) {
  tableWidget->verticalHeader()->setVisible(false);
  tableWidget->setRowCount(0);
  tableWidget->setColumnCount(headers.size());
  tableWidget->setHorizontalHeaderLabels(headers);
  tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  tableWidget->setFixedWidth(300);
}

void OrderBookWidget::invokeAPI() {
  httplib::Client cli("localhost:8080");

  nlohmann::json body;
  body["price"] = price->text().toFloat();
  body["quantity"] = quantity->text().toFloat();
  body["orderType"] = orderType->currentText().toStdString();

  if (bidAsk->currentText().toStdString() == "Bid"){ body["bidAsk"] = true; } else {body["bidAsk"] = false; }

  auto res = cli.Post("/submit", body.dump(), "application/json");

  if (res && res->status == 200) {
    std::cout << "Response from API: " << res->body.c_str() << std::endl;
  } else {
    std::cout << "Failed to call API" << std::endl;
  }
}

OrderBookWidget::~OrderBookWidget() {
}

void OrderBookWidget::updateTable(std::vector<Order> &newOrders, QTableWidget *tableWidget) {
  tableWidget->clearContents();
  tableWidget->setRowCount((int) newOrders.size());

  for (int i = 0; i < newOrders.size(); ++i) {
    QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(newOrders[i].id));
    QTableWidgetItem *priceItem = new QTableWidgetItem(QString::number(newOrders[i].price));
    QTableWidgetItem *quantityItem = new QTableWidgetItem(QString::number(newOrders[i].quantity));
    tableWidget->setItem(i, 0, idItem);
    tableWidget->setItem(i, 1, priceItem);
    tableWidget->setItem(i, 2, quantityItem);
  }
  addColoursToTables();
}

std::vector<Order> OrderBookWidget::getNewOrdersFromOrderbook(bool is_bid) {
  std::vector<Order> orderList;

  std::map<double, OrderList> mapStructure;
  if (is_bid)
    mapStructure = orderBook->getBids();
  else
    mapStructure = orderBook->getAsks();
  for (auto priceLevel : mapStructure) {
    std::list<Order> ordersForPL = priceLevel.second;
    for (Order o : ordersForPL)
      orderList.push_back(o);
  }
  return orderList;
}

void OrderBookWidget::updateBothTables() {
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

void OrderBookWidget::addColoursToTables() {
  QBrush redBrush(Qt::red);

  for (int row = 0; row < asksTableWidget->rowCount(); ++row) {
    for (int col = 0; col < asksTableWidget->columnCount(); ++col) {
      QTableWidgetItem *item = asksTableWidget->item(row, col);
      if (item) {
        item->setForeground(redBrush);
      }
    }
  }

  QBrush greenBrush(Qt::green);

  for (int row = 0; row < bidsTableWidget->rowCount(); ++row) {
    for (int col = 0; col < bidsTableWidget->columnCount(); ++col) {
      QTableWidgetItem *item = bidsTableWidget->item(row, col);
      if (item) {
        item->setForeground(greenBrush);
      }
    }
  }
}

void OrderBookWidget::updateBalance() { currentBalance->setText("Current Balance = " + QString::number(API::getBalance(), 'f')); }

void OrderBookWidget::updateQuantity() { currentQuantity->setText("Current Quantity = " + QString::number(API::getQuantity(), 'f')); }

void OrderBookWidget::updatePnl() { pnl->setText("Overall Balance = " + QString::number(API::getPnl(), 'f')); }
