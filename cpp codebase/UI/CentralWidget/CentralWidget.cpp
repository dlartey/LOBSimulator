#include "CentralWidget.hpp"

CentralWidget::CentralWidget(DBHandler *handler, OrderBook *orderBook, QWidget *parent)
    : QWidget(parent), orderBookWidget(handler, orderBook), candlestickWidget(handler, orderBook) {
  QWidget *allContent = new QWidget;

  setWidgetProperties();
  addOBAndCandleStickToMainWidget(allContent);
  addMainWidgetToScrollArea(allContent);
  addScrollAreaToThisWidget();
}

void CentralWidget::setWidgetProperties() {
  this->setGeometry(0, 0, 1400, 1140);
  this->setStyleSheet("background-color: #ffffff;");
}

void CentralWidget::addOBAndCandleStickToMainWidget(QWidget *mainWidget) {
  QHBoxLayout *horizontalLayout = new QHBoxLayout;
  horizontalLayout->addWidget(&candlestickWidget);
  horizontalLayout->addWidget(&orderBookWidget);
  mainWidget->setLayout(horizontalLayout);
}

void CentralWidget::addMainWidgetToScrollArea(QWidget *mainWidget) {
  //scrollArea.setWidgetResizable(true);
  scrollArea.setWidget(mainWidget);
}

void CentralWidget::addScrollAreaToThisWidget() {
  QLayout *layout = new QVBoxLayout;
  layout->addWidget(&headerWidget);
  layout->addWidget(&scrollArea);
  this->setLayout(layout);
}
