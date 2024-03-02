#ifndef CandlestickWidget_hpp
#define CandlestickWidget_hpp

#include <stdio.h>
#include <QApplication>
#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "OrderBook.hpp"
#include "DBHandler.hpp"
#include <mutex>
#include <list>
#include <thread>
#include <chrono>
#include <float.h>

#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QCandlestickSeries>
#include <QCandlestickSet>
#include <QStringList>
#include <QChartView>
#include <QChart>
#include <QDateTime>

using bestAskBidPair = std::pair<double,double>;

struct Candlestick
{
    double high;
    double low;
    double open;
    double close;
    int seconds;

    Candlestick(double h = DBL_MIN, double l = DBL_MAX, double o = -1.0, double c = -1.0, int s = 0) : high(h), low(l), open(o), close(c), seconds(s) {}
};

class CandlestickWidget : public QWidget
{
    Q_OBJECT
private:
    int _updateRate = 10;
    int counter = 3;
    OrderBook *orderBook;
    
    Candlestick currentStick;
    
    QStringList xAxesList;
    
    QtCharts::QCandlestickSeries *candlestickSeries;
    QtCharts::QChart *chart;
    QtCharts::QChartView *chartView;
    
    std::vector<int> getLol();
    
    void setupSeries();
    void setupChart();
    void setupAxes();
    void setupChartView();
    void setWidgetProperties();
    void addLayoutToThisWidget();
    void addChartViewToLayout(QLayout *layout);
    void updateCurrentStick(bestAskBidPair abp);
    void appendCurrentStickToGraph();
    void resetCurrentStick();
    void resizeYAxis();
    
    bestAskBidPair getBestAskBid();
    double getBestAsk();
    double getBestBid();
    
public:
    CandlestickWidget(DBHandler *handler, OrderBook *orderBook);
    
public slots:
    void updateInformation();
}
;

#endif
