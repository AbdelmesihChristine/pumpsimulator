#ifndef BOLUSDIALOG_H
#define BOLUSDIALOG_H

#include <QDialog>
#include "BolusDeliveryWidget.h"

class BolusDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BolusDialog(UserProfileManager* profileMgr,
                         PumpController* pumpCtrl,
                         CgmSimulator* sim,
                         QWidget *parent = nullptr);

private:
    BolusDeliveryWidget* bolusWidget;
};

#endif // BOLUSDIALOG_H
