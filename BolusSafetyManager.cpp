#include "BolusSafetyManager.h"
#include <QTime>

BolusSafetyManager::BolusSafetyManager()
    : totalDailyBolus(0.0)
{
    // lastBolusTime default invalid
}

bool BolusSafetyManager::canDeliverBolus(double amount, QString &errorMessage)
{
    if (amount <= 0) {
        errorMessage = "Bolus amount must be greater than 0.";
        return false;
    }

    if (amount > maxSingleBolus) {
        errorMessage = QString("Bolus exceeds the max single limit of %1 U.").arg(maxSingleBolus);
        return false;
    }

    if (totalDailyBolus + amount > maxDailyBolus) {
        errorMessage = QString("Daily bolus limit of %1 U reached.").arg(maxDailyBolus);
        return false;
    }

    if (lastBolusTime.isValid()) {
        int secs = lastBolusTime.secsTo(QTime::currentTime());
        if (secs < cooldownMinutes * 60) {
            int waitMins = cooldownMinutes - (secs / 60);
            errorMessage = QString("Please wait %1 more minutes before another bolus.").arg(waitMins);
            return false;
        }
    }

    // Passed all checks
    return true;
}

void BolusSafetyManager::recordBolus(double amount)
{
    totalDailyBolus += amount;
    lastBolusTime = QTime::currentTime();
}
