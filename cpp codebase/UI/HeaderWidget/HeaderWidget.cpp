#include "HeaderWidget.hpp"


HeaderWidget::HeaderWidget()
{
    initialiseLayoutAndAddButtons();
}

void HeaderWidget::setupButtons()
{
    startSimulation = new QPushButton("Start simulation");
    cancelSimulation = new QPushButton("Cancel simulation");
    connectButtons();
}

void HeaderWidget::setupLogoAndCompany()
{
    titleLabel = new QLabel("UNIVERSITY OF LEEDS STOCK EXCHANGE");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont font("Helvetica", 16);
    titleLabel->setFont(font);
}

void HeaderWidget::setupModelType(){
  modelType = new QComboBox(this);
  modelType->addItem("GAN model");
  modelType->addItem("Pan's Model");
  modelType->setGeometry(QRect(QPoint(200, 50), QSize(200, 40)));
}

void HeaderWidget::initialiseLayoutAndAddButtons()
{
    QVBoxLayout* container = new QVBoxLayout;
    QHBoxLayout* buttonContainer = new QHBoxLayout;
    QHBoxLayout* logoAndCompanyContainer = new QHBoxLayout;

    setupLogoAndCompany();
    setupButtons();
    setupModelType();
    buttonContainer->addWidget(modelType);
    buttonContainer->addWidget(startSimulation);
    buttonContainer->addWidget(cancelSimulation);
    //TODO: ADD LOGO HERE
    logoAndCompanyContainer ->addWidget(titleLabel);
    
    container->addLayout(logoAndCompanyContainer);
    container->addLayout(buttonContainer);
    this->setLayout(container);
}

void HeaderWidget::connectButtons()
{
    connect(startSimulation, &QPushButton::clicked, this, &HeaderWidget::startSim);
    connect(cancelSimulation, &QPushButton::clicked, this, &HeaderWidget::cancelSim);
}

void HeaderWidget::startSim()
{
    std::cout << "Start clicked!\n";
    emit startSimulationClicked();
}

void HeaderWidget::cancelSim()
{
    std::cout << "Cancel clicked!\n";
    emit cancelSimulationClicked();
}
