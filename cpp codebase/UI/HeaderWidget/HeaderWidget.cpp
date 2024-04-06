#include "HeaderWidget.hpp"
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>

HeaderWidget::HeaderWidget(DBHandler *handler, OrderBook *ob) : handler(handler), orderBook(ob){
  initialiseLayoutAndAddButtons();
  this->setStyleSheet("background-color: rgba(12, 83, 45, 90); border-radius: 20px;");
}

HeaderWidget::~HeaderWidget() {}

void HeaderWidget::setupButtons() {
    QFont buttonFont("Arial", 10);
    QString buttonStyle = "QPushButton { "
                          "color: white; "
                          "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #555555, stop: 1 #333333);"
                          "border: 1px solid #555; "
                          "border-radius: 10px; "
                          "padding: 5px; "
                          "min-width: 80px; "
                          "min-height: 24px; "
                          "} "
                          "QPushButton:hover { "
                          "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #666666, stop: 1 #444444);"
                          "} "
                          "QPushButton:pressed { "
                          "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #555555, stop: 1 #222222);"
                          "}";

    startSimulation = new QPushButton("Start Simulation");
    cancelSimulation = new QPushButton("Cancel Simulation");

    startSimulation->setFont(buttonFont);
    cancelSimulation->setFont(buttonFont);
    startSimulation->setStyleSheet(buttonStyle);
    cancelSimulation->setStyleSheet(buttonStyle);

    connectButtons();
}

void HeaderWidget::setupLogoAndCompany() {
    QFont titleFont("Roboto", 18, QFont::Bold);
    titleLabel = new QLabel("   UNIVERSITY OF LEEDS STOCK EXCHANGE   ");
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("QLabel { color : white; }");
    titleLabel->setAlignment(Qt::AlignCenter);

    QPixmap imagePixmap("../../resources/UoLSE_Logo.png");
    imageLabel = new QLabel;
    imageLabel->setPixmap(imagePixmap.scaled(150, 80, Qt::KeepAspectRatio));
}

void HeaderWidget::setupModelType() {
    modelType = new QComboBox(this);
    modelType->addItem("GAN Model");
    modelType->addItem("Pan's Model");
    modelType->setGeometry(QRect(QPoint(200, 50), QSize(200, 40)));
    modelType->setFont(QFont("Arial", 10));
    modelType->setStyleSheet("QComboBox { color: white; background-color: #444444; border-radius: 5px; padding: 3px; }"
                             "QComboBox::drop-down { border: none; }"
                             "QComboBox::down-arrow { image: url(/path/to/arrow.png); width: 14px; height: 14px; }");
}

void HeaderWidget::initialiseLayoutAndAddButtons() {
    QVBoxLayout* container = new QVBoxLayout(this);
    QHBoxLayout* buttonContainer = new QHBoxLayout;
    QHBoxLayout* logoAndCompanyContainer = new QHBoxLayout;

    setupLogoAndCompany();
    setupButtons();
    setupModelType();

    buttonContainer->addWidget(modelType);
    buttonContainer->addWidget(startSimulation);
    buttonContainer->addWidget(cancelSimulation);

    logoAndCompanyContainer->addWidget(imageLabel);
    logoAndCompanyContainer->addStretch(1);
    logoAndCompanyContainer->addWidget(titleLabel);
    logoAndCompanyContainer->addStretch(1);

    container->addLayout(logoAndCompanyContainer);
    container->addLayout(buttonContainer);
}

void HeaderWidget::connectButtons() {
    connect(startSimulation, &QPushButton::clicked, this, &HeaderWidget::startSim);
    connect(cancelSimulation, &QPushButton::clicked, this, &HeaderWidget::cancelSim);
}

void HeaderWidget::startSim() {
    std::string option = modelType->currentText().toStdString();

    if (option == "GAN Model"){
      cancelSim();
      Gan::startServer(handler, orderBook);
    }
}

void HeaderWidget::cancelSim(){
    Gan::cancelRequested = true;
    if (Gan::startGenerate.joinable()) { Gan::startGenerate.join(); }
}
