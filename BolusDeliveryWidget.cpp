#include "BolusDeliveryWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QDoubleValidator>
#include "UserProfile.h"

BolusDeliveryWidget::BolusDeliveryWidget(UserProfileManager* profileMgr,
                                         PumpController* pumpCtrl,
                                         CgmSimulator* cgmSim,
                                         QWidget *parent)
    : QWidget(parent)
    , userProfileManager(profileMgr)
    , pumpController(pumpCtrl)
    , cgmSimulator(cgmSim)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // BG Input
    QHBoxLayout* bgLayout = new QHBoxLayout();
    bgLayout->addWidget(new QLabel("BG (mmol/L):"));
    bgInput = new QLineEdit(this);
    bgInput->setValidator(new QDoubleValidator(0, 30, 1, this));
    bgLayout->addWidget(bgInput);
    mainLayout->addLayout(bgLayout);

    // If cgmSimulator is available, connect signal to auto-update
    if (cgmSimulator) {
        connect(cgmSimulator, &CgmSimulator::bgUpdated,
                this, &BolusDeliveryWidget::onCgmBgUpdated);
    }

    // Carbs Input
    QHBoxLayout* carbsLayout = new QHBoxLayout();
    carbsLayout->addWidget(new QLabel("Carbs (g):"));
    carbsInput = new QLineEdit(this);
    carbsInput->setValidator(new QDoubleValidator(0, 1000, 1, this));
    carbsLayout->addWidget(carbsInput);
    mainLayout->addLayout(carbsLayout);

    // IOB Input
    QHBoxLayout* iobLayout = new QHBoxLayout();
    iobLayout->addWidget(new QLabel("Insulin on Board (U):"));
    iobInput = new QLineEdit(this);
    iobInput->setValidator(new QDoubleValidator(0, 100, 1, this));
    iobLayout->addWidget(iobInput);
    mainLayout->addLayout(iobLayout);

    // Suggested Bolus
    QHBoxLayout* bolusLayout = new QHBoxLayout();
    bolusLayout->addWidget(new QLabel("Suggested Bolus (U):"));
    suggestedBolus = new QLineEdit(this);
    suggestedBolus->setReadOnly(true);
    bolusLayout->addWidget(suggestedBolus);
    mainLayout->addLayout(bolusLayout);

    // Extended Bolus Options
    extendedCheck = new QCheckBox("Extended Bolus?", this);
    mainLayout->addWidget(extendedCheck);

    QHBoxLayout* extendLayout = new QHBoxLayout();
    extendLayout->addWidget(new QLabel("Extended %:"));
    extendedPercent = new QSpinBox(this);
    extendedPercent->setRange(0,100);
    extendedPercent->setValue(40); // default 40%
    extendLayout->addWidget(extendedPercent);

    extendLayout->addWidget(new QLabel("Duration (hrs):"));
    extendedHours = new QSpinBox(this);
    extendedHours->setRange(1, 24);
    extendedHours->setValue(3); // default 3 hours
    extendLayout->addWidget(extendedHours);

    mainLayout->addLayout(extendLayout);

    // Buttons
    calcButton = new QPushButton("Calculate Bolus", this);
    deliverButton = new QPushButton("Deliver Bolus", this);
    mainLayout->addWidget(calcButton);
    mainLayout->addWidget(deliverButton);

    connect(calcButton, &QPushButton::clicked, this, &BolusDeliveryWidget::onCalculateBolus);
    connect(deliverButton, &QPushButton::clicked, this, &BolusDeliveryWidget::onDeliverBolus);

    setLayout(mainLayout);
}

/**
 * @brief Basic formula from your notes:
 *   Food Bolus = carbsVal / carbRatio
 *   Correction Bolus = max( (bgVal - targetBG)/correctionFactor , 0 )
 *   totalBeforeIOB = Food + Correction
 *   final = totalBeforeIOB - IOB
 */
double BolusDeliveryWidget::calculateSuggestedBolus(double bgVal, double carbsVal, double iobVal)
{
    // Retrieve the active profile
    UserProfile prof = userProfileManager->getActiveProfile();

    // Food bolus
    double foodBolus = 0.0;
    if (prof.carbRatio > 0) {
        foodBolus = carbsVal / prof.carbRatio;
    }

    // Correction bolus
    double correction = 0.0;
    double diff = bgVal - prof.targetGlucose;
    if (diff > 0 && prof.correctionFactor > 0) {
        correction = diff / prof.correctionFactor;
    }

    double totalBeforeIOB = foodBolus + correction;
    double finalBolus = totalBeforeIOB - iobVal;
    if (finalBolus < 0) {
        finalBolus = 0;
    }
    return finalBolus;
}

void BolusDeliveryWidget::onCalculateBolus()
{
    bool okBG, okCarbs, okIOB;
    double bgVal    = bgInput->text().toDouble(&okBG);
    double carbsVal = carbsInput->text().toDouble(&okCarbs);
    double iobVal   = iobInput->text().toDouble(&okIOB);

    if (!okBG || !okCarbs || !okIOB) {
        QMessageBox::warning(this, "Invalid Input", "Please enter valid BG, Carbs, and IOB values.");
        return;
    }

    double suggestion = calculateSuggestedBolus(bgVal, carbsVal, iobVal);
    suggestedBolus->setText(QString::number(suggestion, 'f', 2));
}

void BolusDeliveryWidget::onDeliverBolus()
{
    bool ok;
    double total = suggestedBolus->text().toDouble(&ok);
    if (!ok || total <= 0) {
        QMessageBox::warning(this, "Invalid Bolus", "Calculated bolus is invalid or zero.");
        return;
    }

    // Check if extended is selected
    double extFrac = 0.0;
    int durHrs = 0;
    if (extendedCheck->isChecked()) {
        extFrac = extendedPercent->value() / 100.0;
        durHrs  = extendedHours->value();
    }

    QString notes = QString("BG=%1, Carbs=%2, IOB=%3")
        .arg(bgInput->text())
        .arg(carbsInput->text())
        .arg(iobInput->text());

    // Ask the PumpController to deliver
    if (!pumpController->requestBolus(total, notes, extFrac, durHrs)) {
        // The PumpController will emit bolusFailed() with an error if it fails
        // But we can also show an alert here if we want.
        return;
    }

    // If it succeeded, you could optionally clear fields:
    // bgInput->clear();
    // carbsInput->clear();
    // iobInput->clear();
    // suggestedBolus->clear();
    // ...
    QMessageBox::information(this, "Bolus Requested",
        QString("Total Bolus: %1 U \n(Extended? %2%)").arg(total).arg(extendedCheck->isChecked()? extendedPercent->value() : 0));
}

void BolusDeliveryWidget::onCgmBgUpdated(double newBg)
{
    // If user wants auto-populate, we can just set the text
    // or you can do more advanced logic. For now, let's just set it.
    bgInput->setText(QString::number(newBg, 'f', 1));
}
