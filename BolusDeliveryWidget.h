#ifndef BOLUSDELIVERYWIDGET_H
#define BOLUSDELIVERYWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QRadioButton>
#include "PumpController.h"
#include "UserProfileManager.h"
#include "CgmSimulator.h"

class BolusDeliveryWidget : public QWidget {
    Q_OBJECT

public:
    BolusDeliveryWidget(UserProfileManager* profileMgr,
                        PumpController* pumpCtrl,
                        CgmSimulator* cgmSim = nullptr,
                        QWidget *parent = nullptr);

private slots:
    void onCalculateBolus();
    void onDeliverBolus();
    void onCgmBgUpdated(double newBg);
    void onToggleBgSource();

private:
    double calculateSuggestedBolus(double bgVal, double carbsVal, double iobVal);

    UserProfileManager* userProfileManager;
    PumpController*     pumpController;
    CgmSimulator*       cgmSimulator;

    QRadioButton* useManualBgRadio;
    QRadioButton* useCgmBgRadio;

    QLineEdit* bgInput;
    QLineEdit* carbsInput;
    QLineEdit* iobInput;
    QLineEdit* suggestedBolus;
    QCheckBox* extendedCheck;
    QSpinBox*  extendedPercent;
    QSpinBox*  extendedHours;

    QPushButton* calcButton;
    QPushButton* deliverButton;
};

#endif // BOLUSDELIVERYWIDGET_H
