#ifndef BOLUSDELIVERYWIDGET_H
#define BOLUSDELIVERYWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>

#include "PumpController.h"
#include "UserProfileManager.h"
#include "CgmSimulator.h"

class BolusDeliveryWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param profileMgr    Access to active profile for calculations
     * @param pumpCtrl      Controller that handles final bolus logic
     * @param cgmSim        If not null, used for auto-populating BG
     */
    BolusDeliveryWidget(UserProfileManager* profileMgr,
                        PumpController* pumpCtrl,
                        CgmSimulator* cgmSim = nullptr,
                        QWidget *parent = nullptr);

private slots:
    void onCalculateBolus();
    void onDeliverBolus();
    void onCgmBgUpdated(double newBg);

private:
    double calculateSuggestedBolus(double bgVal, double carbsVal, double iobVal);

    // References (not owned)
    UserProfileManager* userProfileManager;
    PumpController*     pumpController;
    CgmSimulator*       cgmSimulator;

    // UI elements
    QLineEdit* bgInput;         // user or CGM BG
    QLineEdit* carbsInput;      // user carbs
    QLineEdit* iobInput;        // user IOB
    QLineEdit* suggestedBolus;  // read-only
    QCheckBox* extendedCheck;
    QSpinBox*  extendedPercent;
    QSpinBox*  extendedHours;

    QPushButton* calcButton;
    QPushButton* deliverButton;
};

#endif // BOLUSDELIVERYWIDGET_H
