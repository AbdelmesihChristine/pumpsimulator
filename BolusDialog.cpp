#include "BolusDialog.h"
#include <QVBoxLayout>

BolusDialog::BolusDialog(UserProfileManager* profileMgr,
                         PumpController* pumpCtrl,
                         CgmSimulator* sim,
                         QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Manual Bolus");
    QVBoxLayout* layout = new QVBoxLayout(this);

    bolusWidget = new BolusDeliveryWidget(profileMgr, pumpCtrl, sim, this);
    layout->addWidget(bolusWidget);
    setLayout(layout);
}
