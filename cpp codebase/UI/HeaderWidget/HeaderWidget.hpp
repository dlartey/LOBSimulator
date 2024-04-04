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

class HeaderWidget : public QWidget
{
    Q_OBJECT
private:
    QLabel* titleLabel;
    QComboBox* modelType;
    QPushButton* startSimulation;
    QPushButton* cancelSimulation;
    
    void initialiseLayoutAndAddButtons();
    void setupButtons();
    void setupLogoAndCompany();
    void connectButtons();
    void setupModelType();

    void startSim();
    void cancelSim();
    
public:
    HeaderWidget();
signals:
    void startSimulationClicked();
    void cancelSimulationClicked();
};

#endif
