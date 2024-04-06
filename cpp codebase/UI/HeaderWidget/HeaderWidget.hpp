#ifndef HeaderWidget_hpp
#define HeaderWidget_hpp

#include <stdio.h>
#include <iostream>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <list>
#include <thread>
#include <chrono>
#include "OrderBook.hpp"
#include "DBHandler.hpp"
#include "GAN.hpp"

class HeaderWidget : public QWidget
{
    Q_OBJECT
private:
    DBHandler *handler;

    QLabel* titleLabel;
    QLabel* imageLabel;
    QComboBox* modelType;
    QPushButton* startSimulation;
    QPushButton* cancelSimulation;
    
    void initialiseLayoutAndAddButtons();
    void setupButtons();
    void setupLogoAndCompany();
    void connectButtons();
    void setupModelType();

public:
    OrderBook *orderBook;
    HeaderWidget(DBHandler *handler, OrderBook *orderBook);
    ~HeaderWidget();

public slots:
    void startSim();
    void cancelSim();
};

#endif
