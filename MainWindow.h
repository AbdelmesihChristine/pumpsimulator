#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include "UserProfileManager.h"
#include "BolusSafetyManager.h"
#include "HistoryManager.h"
#include "PumpController.h"
#include "CgmSimulator.h"
#include "CGMGraphWidget.h"
#include "BolusDialog.h"
#include "ProfileDialog.h"
#include "HistoryDialog.h"
#include "AlertDialog.h"
#include "WarningChecker.h"

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
    UserProfileManager* userProfileManager;
    BolusSafetyManager* bolusSafetyManager;
    HistoryManager*     historyManager;
    PumpController*     pumpController;
    CgmSimulator*       cgmSimulator;
    WarningChecker*     warningChecker;

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

    BolusDialog*   bolusDialog;
    ProfileDialog* profileDialog;
    HistoryDialog* histDialog;
    AlertDialog*   alertDialog;

    QTimer* uiRefreshTimer;
};

#endif // MAINWINDOW_H
