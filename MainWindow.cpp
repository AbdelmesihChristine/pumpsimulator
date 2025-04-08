#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateTime>
#include <QIcon>
#include <QFrame>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    userProfileManager  = new UserProfileManager(this);
    bolusSafetyManager  = new BolusSafetyManager();
    historyManager      = new HistoryManager();
    cgmSimulator        = new CgmSimulator(this);
    pumpController      = new PumpController(
                              userProfileManager,
                              historyManager,
                              bolusSafetyManager,
                              cgmSimulator,
                              this);

    // Initialize warning checker
    warningChecker = new WarningChecker(historyManager, cgmSimulator, this);

    // For demonstration, set reservoir to 300U, or 200U, etc.
    warningChecker->setInsulinLevel(4.0);
    warningChecker->setBatteryLevel(8);

    // Battery
    batteryBarsWidget = new QWidget(this);
    QHBoxLayout* batteryBarsLayout = new QHBoxLayout(batteryBarsWidget);
    batteryBarsLayout->setSpacing(2);
    for(int i=0; i<5; i++){
        QFrame* bar = new QFrame(this);
        bar->setFixedSize(8, 20);
        bar->setStyleSheet("background-color: #00ff00;");
        batteryBarsLayout->addWidget(bar);
    }
    batteryTextLabel = new QLabel("100%", this);

    // Insulin
    insulinBarsWidget = new QWidget(this);
    QHBoxLayout* insulinBarsLayout = new QHBoxLayout(insulinBarsWidget);
    insulinBarsLayout->setSpacing(2);
    for(int i=0; i<5; i++){
        QFrame* bar = new QFrame(this);
        bar->setFixedSize(8, 20);
        bar->setStyleSheet("background-color: #3399ff;");
        insulinBarsLayout->addWidget(bar);
    }
    insulinTextLabel = new QLabel("300U", this);

    timeLabel = new QLabel("--:--", this);
    timeLabel->setAlignment(Qt::AlignCenter);

    bolusButton   = new QPushButton(QIcon(":/icons/icons/drop.png"), "Bolus", this);
    profileButton = new QPushButton(QIcon(":/icons/icons/setting.png"), "Profiles", this);
    historyButton = new QPushButton(QIcon(":/icons/icons/history.png"), "History", this);
    alertButton   = new QPushButton(QIcon(":/icons/icons/warning.png"), "Alerts", this);

    cgmGraphWidget = new CGMGraphWidget(cgmSimulator, this);

    connect(bolusButton,   &QPushButton::clicked, this, &MainWindow::openBolusDialog);
    connect(profileButton, &QPushButton::clicked, this, &MainWindow::openProfiles);
    connect(historyButton, &QPushButton::clicked, this, &MainWindow::openHistory);
    connect(alertButton,   &QPushButton::clicked, this, &MainWindow::openAlerts);

    bolusDialog   = new BolusDialog(userProfileManager, pumpController, cgmSimulator, this);
    profileDialog = new ProfileDialog(userProfileManager, this);
    histDialog    = new HistoryDialog(historyManager, this);
    alertDialog = new AlertDialog(historyManager, this);

    QWidget* central = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    QHBoxLayout* topLayout = new QHBoxLayout();
    QVBoxLayout* batteryLayout = new QVBoxLayout();
    batteryLayout->addWidget(batteryBarsWidget);
    batteryLayout->addWidget(batteryTextLabel);

    QVBoxLayout* insulinLayout = new QVBoxLayout();
    insulinLayout->addWidget(insulinBarsWidget);
    insulinLayout->addWidget(insulinTextLabel);

    topLayout->addLayout(batteryLayout);
    topLayout->addStretch();
    topLayout->addWidget(timeLabel);
    topLayout->addStretch();
    topLayout->addLayout(insulinLayout);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(bolusButton);
    buttonLayout->addWidget(profileButton);
    buttonLayout->addWidget(historyButton);
    buttonLayout->addWidget(alertButton);

    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(cgmGraphWidget);

    central->setLayout(mainLayout);
    setCentralWidget(central);

    setWindowTitle("t:slim X2 Pump Simulation");

    // Start CGM + Warnings
    cgmSimulator->start();
    warningChecker->startMonitoring();

    setStyleSheet(R"(
        QWidget {
            background-color: #1c1b1b;
            color: #eeeeee;
        }
    )");

    // Refresh UI elements every second
    uiRefreshTimer = new QTimer(this);
    connect(uiRefreshTimer, &QTimer::timeout, this, &MainWindow::updateTime);
    uiRefreshTimer->start(1000);

    updateTime();
}

MainWindow::~MainWindow()
{
}

void MainWindow::openBolusDialog()
{
    bolusDialog->exec();
}

void MainWindow::openProfiles()
{
    profileDialog->exec();
}

void MainWindow::openHistory()
{
    histDialog->updateTable();
    histDialog->exec();
}

void MainWindow::openAlerts()
{
    alertDialog->updateAlerts();
    alertDialog->exec();
}

void MainWindow::updateTime()
{
    // Real time for demonstration
    QDateTime now = QDateTime::currentDateTime();
    timeLabel->setText(now.toString("hh:mm AP\nddd, dd MMM"));

    // Show battery level
    int battery = warningChecker->getBatteryLevel();
    batteryTextLabel->setText(QString("%1%").arg(battery));

    // Show insulin level
    double ins = warningChecker->getInsulinLevel();
    insulinTextLabel->setText(QString("%1U").arg(ins,0,'f',0));
}
