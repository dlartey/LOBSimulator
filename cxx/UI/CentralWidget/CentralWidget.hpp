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
#include "CandlestickWidget.hpp"
#include "HeaderWidget.hpp"

#include <QScrollArea>
#include <QDesktopWidget>

class CentralWidget : public QWidget
{
    Q_OBJECT
private:
    QScrollArea scrollArea;
    OrderBookWidget orderBookWidget;
    CandlestickWidget candlestickWidget;
    HeaderWidget headerWidget;
    
    void setWidgetProperties();
    void addOBAndCandleStickToMainWidget(QWidget *mainWidget);
    void addMainWidgetToScrollArea(QWidget *mainWidget);
    void addScrollAreaToThisWidget();

public:
    explicit CentralWidget(DBHandler *handler, OrderBook *orderBook, QWidget *parent = nullptr);
};

#endif /* OrderBookWidget_hpp */
