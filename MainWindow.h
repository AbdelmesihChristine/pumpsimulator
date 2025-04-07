#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtCharts/QChartView>

#include "UserProfileManager.h"
#include "BolusSafetyManager.h"
#include "BolusHistoryManager.h"
#include "PumpController.h"
#include "CgmSimulator.h"
#include "CGMGraphWidget.h"
#include "BolusDialog.h"
#include "ProfileDialog.h"
#include "HistoryDialog.h"
#include "AlertDialog.h"

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openBolusDialog();
    void openProfiles();
    void openHistory();
    void openAlerts();
    void updateTime();

private:
    // Backend managers
    UserProfileManager*   userProfileManager;
    BolusSafetyManager*   bolusSafetyManager;
    BolusHistoryManager*  bolusHistoryManager;
    PumpController*       pumpController;
    CgmSimulator*         cgmSimulator;

    // UI elements
    QLabel* batteryTextLabel;
    QWidget* batteryBarsWidget;
    QLabel* insulinTextLabel;
    QWidget* insulinBarsWidget;
    QLabel* timeLabel;

    QPushButton* bolusButton;
    QPushButton* profileButton;
    QPushButton* historyButton;
    QPushButton* alertButton;
    CGMGraphWidget* cgmGraphWidget;

    // Dialogs
    BolusDialog*   bolusDialog;
    ProfileDialog* profileDialog;
    HistoryDialog* historyDialog;
    AlertDialog*   alertDialog;

    QTimer* timer;
};

#endif // MAINWINDOW_H
