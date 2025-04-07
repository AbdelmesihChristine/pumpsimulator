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

    // The same widget you had, now placed inside a QDialog
    bolusWidget = new BolusDeliveryWidget(profileMgr, pumpCtrl, sim, this);
    layout->addWidget(bolusWidget);

    setLayout(layout);

    // optional: set a fixed size or let it be resizable
    //setFixedSize(400, 300);
}
