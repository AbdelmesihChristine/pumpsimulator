#include "MainWindow.h"
#include <QDateTime>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //
    // 1) Create back-end managers
    //
    userProfileManager  = new UserProfileManager(this);
    bolusSafetyManager  = new BolusSafetyManager();
    bolusHistoryManager = new BolusHistoryManager();
    cgmSimulator        = new CgmSimulator(this);
    pumpController      = new PumpController(userProfileManager,
                                             bolusHistoryManager,
                                             bolusSafetyManager,
                                             this);

    //
    // 2) Create main UI controls
    //

    // Battery section
    batteryBarsWidget = new QWidget(this);
    QHBoxLayout* batteryBarsLayout = new QHBoxLayout(batteryBarsWidget);
    batteryBarsLayout->setSpacing(2);
    for (int i = 0; i < 5; ++i) {
        QFrame* bar = new QFrame(this);
        bar->setFixedSize(8, 20);
        bar->setStyleSheet("background-color: #00ff00; border-radius: 2px;");
        batteryBarsLayout->addWidget(bar);
    }
    batteryTextLabel = new QLabel("100%", this);
    batteryTextLabel->setStyleSheet("color: #00ff00; font-weight: bold;");

    // Insulin section
    insulinBarsWidget = new QWidget(this);
    QHBoxLayout* insulinBarsLayout = new QHBoxLayout(insulinBarsWidget);
    insulinBarsLayout->setSpacing(2);
    for (int i = 0; i < 5; ++i) {
        QFrame* bar = new QFrame(this);
        bar->setFixedSize(8, 20);
        bar->setStyleSheet("background-color: #3399ff; border-radius: 2px;");
        insulinBarsLayout->addWidget(bar);
    }
    insulinTextLabel = new QLabel("235U", this);
    insulinTextLabel->setStyleSheet("color: #3399ff; font-weight: bold;");

    // Time label
    timeLabel = new QLabel("--:--", this);
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setStyleSheet("font-size: 24px; font-weight: bold;");

    // Buttons
    bolusButton   = new QPushButton(QIcon(":/icons/icons/drop.png"), "Bolus", this);
    profileButton = new QPushButton(QIcon(":/icons/icons/settings.png"), "Profiles", this);
    historyButton = new QPushButton(QIcon(":/icons/icons/file.png"), "History", this);
    alertButton   = new QPushButton(QIcon(":/icons/icons/alert.png"), "Alerts", this);

    bolusButton->setIconSize(QSize(32, 32));
    profileButton->setIconSize(QSize(32, 32));
    historyButton->setIconSize(QSize(32, 32));
    alertButton->setIconSize(QSize(32, 32));

    cgmGraphWidget = new CGMGraphWidget(cgmSimulator, this);

    //
    // 3) Connect signals
    //
    connect(bolusButton,   &QPushButton::clicked, this, &MainWindow::openBolusDialog);
    connect(profileButton, &QPushButton::clicked, this, &MainWindow::openProfiles);
    connect(historyButton, &QPushButton::clicked, this, &MainWindow::openHistory);
    connect(alertButton,   &QPushButton::clicked, this, &MainWindow::openAlerts);

    //
    // 4) Create dialogs
    //
    bolusDialog    = new BolusDialog(userProfileManager, pumpController, cgmSimulator, this);
    profileDialog  = new ProfileDialog(userProfileManager, this);
    historyDialog  = new HistoryDialog(bolusHistoryManager, this);
    alertDialog    = new AlertDialog(this);

    //
    // 5) Layout the UI
    //
    QWidget* central = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    // Top Bar Layout
    QHBoxLayout* topLayout = new QHBoxLayout();
    QVBoxLayout* batteryLayout = new QVBoxLayout();
    batteryLayout->addWidget(batteryBarsWidget);
    batteryLayout->addWidget(batteryTextLabel);
    batteryLayout->setAlignment(Qt::AlignLeft);

    QVBoxLayout* insulinLayout = new QVBoxLayout();
    insulinLayout->addWidget(insulinBarsWidget);
    insulinLayout->addWidget(insulinTextLabel);
    insulinLayout->setAlignment(Qt::AlignRight);

    topLayout->addLayout(batteryLayout);
    topLayout->addStretch();
    topLayout->addWidget(timeLabel);
    topLayout->addStretch();
    topLayout->addLayout(insulinLayout);

    // Navigation buttons
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

    setWindowTitle("Insulin Pump Home");

    //
    // 6) Start CGM
    //
    cgmSimulator->start();

    //
    // 7) Dark theme
    //
    setStyleSheet(R"(
        QWidget {
            background-color: #1c1b1b;
            color: #eeeeee;
            font-family: 'Segoe UI', sans-serif;
        }
        QPushButton {
            background-color: #2a2a2a;
            color: #ffffff;
            border: none;
            border-radius: 12px;
            padding: 10px 20px;
            text-align: middle;       /* Align text left */
            padding-left: 32px;     /* Add space between icon and text */
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #3a3a3a;
        }
        QLabel {
            color: #eeeeee;
            font-weight: bold;
        }
    )");

    //
    // 8) Update clock
    //
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
    timer->start(1000);
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
    historyDialog->exec();
}

void MainWindow::openAlerts()
{
    alertDialog->exec();
}

void MainWindow::updateTime()
{
    QDateTime now = QDateTime::currentDateTime();
    QString timeText = now.toString("h:mm AP\nddd, dd MMM");
    timeLabel->setText(timeText);
}
