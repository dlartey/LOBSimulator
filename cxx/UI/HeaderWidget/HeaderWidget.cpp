#include "HeaderWidget.hpp"
#include <QFont>
#include <QPixmap>
#include <QDir>
#include <QFontDatabase>

HeaderWidget::HeaderWidget(DBHandler *handler, OrderBook *orderBook, QWidget *parent) :
QWidget(parent), handler(handler), orderBook(orderBook) {
    initialiseLayoutAndAddButtons();
    this->setStyleSheet("background-color: rgba(12, 83, 45, 90); border-radius: 20px;");
}

HeaderWidget::~HeaderWidget() {
  if (newsTimer) { newsTimer->stop(); }
}

void HeaderWidget::initialiseLayoutAndAddButtons() {
  QVBoxLayout *container = new QVBoxLayout(this);
  QHBoxLayout *buttonContainer = new QHBoxLayout;
  QHBoxLayout *logoAndCompanyContainer = new QHBoxLayout;

  setupLogoAndCompany();
  setupButtons();
  setupModelType();
  setupNewsTicker();

  buttonContainer->addWidget(modelType);
  buttonContainer->addWidget(startSimulation);
  buttonContainer->addWidget(cancelSimulation);

  logoAndCompanyContainer->addWidget(imageLabel);
  logoAndCompanyContainer->addStretch(1);
  logoAndCompanyContainer->addWidget(titleLabel);
  logoAndCompanyContainer->addStretch(1);
  logoAndCompanyContainer->addWidget(newsTicker);

  container->addLayout(logoAndCompanyContainer);
  container->addLayout(buttonContainer);
}

std::string HeaderWidget::getProjectSourceDirectory() {
  std::string currFilePath = __FILE__;
  std::filesystem::path fullPath(currFilePath);
  return fullPath.parent_path().parent_path().parent_path().string();
}

void HeaderWidget::setupLogoAndCompany() {
  std::string fontPath = getProjectSourceDirectory() +"/resources/Nexa-Heavy.ttf";
  int id = QFontDatabase::addApplicationFont(fontPath.c_str());

  QString family = QFontDatabase::applicationFontFamilies(id).at(0);
  QFont titleFont(family, 18);
  // QFont titleFont("Helvetica", 24, QFont::Bold);
  titleLabel = new QLabel("University of Leeds Stock Exchange");
  titleLabel->setFont(titleFont);
  titleLabel->setAlignment(Qt::AlignCenter);
  titleLabel->setStyleSheet("QLabel { color : white; }");

  std::string logoPath = getProjectSourceDirectory() +"/resources/UoLSE_Logo.png";
  QPixmap imagePixmap = QPixmap(logoPath.c_str());
  imageLabel = new QLabel;
  imageLabel->setPixmap(imagePixmap.scaled(200, 200, Qt::KeepAspectRatio));
}

void HeaderWidget::setupButtons() {
  std::string fontPath = getProjectSourceDirectory() +"/resources/TangoSans.ttf";
  int id = QFontDatabase::addApplicationFont(fontPath.c_str());

    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont buttonFont(family, 10);

    QString buttonStyle = "QPushButton { color: white; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                          " stop: 0 #555555, stop: 1 #333333); border: 1px solid #555; border-radius: 10px; "
                          "padding: 5px; min-width: 80px; min-height: 24px; } "
                          "QPushButton:hover { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, "
                          "stop: 0 #666666, stop: 1 #444444); } QPushButton:pressed { background-color: "
                          "qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #555555, stop: 1 #222222); }";

    startSimulation = new QPushButton("Start Simulation");
    cancelSimulation = new QPushButton("Cancel Simulation");
    startSimulation->setFont(buttonFont);
    cancelSimulation->setFont(buttonFont);
    startSimulation->setStyleSheet(buttonStyle);
    cancelSimulation->setStyleSheet(buttonStyle);
    connectButtons();
}

void HeaderWidget::setupModelType() {
  std::string fontPath = getProjectSourceDirectory() +"/resources/TangoSans.ttf";
  int id = QFontDatabase::addApplicationFont(fontPath.c_str());

    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont modelTypeFont(family, 10);

    modelType = new QComboBox(this);
    modelType->addItem("GAN Model");
    modelType->addItem("Agent Based Model");
    modelType->setFont(modelTypeFont);
    modelType->setStyleSheet("QComboBox { color: white; background-color: #444444; border-radius: 5px; padding: 3px; }"
                             " QComboBox::drop-down { border: none; }");
}

void HeaderWidget::setupNewsTicker() {
    int rightMargin = -800; // Margin from the right edge
    int widgetWidth = 320;  // Width of the newsTicker
    int widgetHeight = 30;  // Height of the newsTicker
    int yPos = 45;          // Vertical position from the top

  newsTicker = new QLabel(this);
  // Set background color, text color, and round corners directly on the newsTicker
  newsTicker->setStyleSheet(
      "QLabel { color: white; font-size: 10pt; background-color: rgba(12, 83, 45, 90); "
      "border-radius: 10px; padding: 5px; }");

  // Calculate x-position from the right edge
  int xPos = this->width() - widgetWidth - rightMargin;

    // Set geometry with the calculated x-position
    newsTicker->setGeometry(QRect(xPos, yPos, widgetWidth, widgetHeight));

    newsItems << "Ranaex Exchange Unveils RNAC: A Pioneering Leap in Cryptocurrency"
              << "Lessons From The FTX Trial: Regulating CEXs May Not Be Enough To Prevent Bad Actors | CryptoInfoNet"
              << "Potential Shiba Inu Rally as Selling Slows, Uniswap & AI Altcoin Strengthen Position"
              << "Global regulators propose new rules for Bitcoin transactions."
              << "FIU Notifies Fresh Money Laundering, Terror Financing Reporting Alerts For Capital Markets"
              << "Greenhaven Road Capital Main Fund Q1 2024 Investor Letter"
              << "Digging into Decentralised Exchanges: Benefits and Challenges"
              << "Institutional Crypto Investors Choose ETFSwap (ETFS) As The Better Alternative To Binance And ByBit";
    newsTicker->setText(newsItems.first());

  newsTimer = new QTimer(this);
  connect(newsTimer, &QTimer::timeout, this, &HeaderWidget::updateNewsTicker);
  newsTimer->start(3000);
}

void HeaderWidget::updateNewsTicker() {
  currentNewsIndex = (currentNewsIndex + 1) % newsItems.size();
  newsTicker->setText(newsItems.at(currentNewsIndex));
  newsTicker->setAlignment(Qt::AlignCenter);
}

void HeaderWidget::connectButtons() {
  connect(startSimulation, &QPushButton::clicked, this, &HeaderWidget::startSim);
  connect(cancelSimulation, &QPushButton::clicked, this, &HeaderWidget::cancelSim);
}

void HeaderWidget::startSim() {
  std::string option = modelType->currentText().toStdString();
  cancelSim();

  if (option == "GAN Model") {
    Gan::startServer(handler, orderBook);
  } else {
    Abm::startServer(handler, orderBook);
  }
}

void HeaderWidget::cancelSim() {
  Gan::cancelRequested = true;
  Abm::cancelRequested = true;
  if (Gan::startGenerate.joinable()) { Gan::startGenerate.join(); }
  if (Abm::startGenerate.joinable()) { Abm::startGenerate.join(); }
}
