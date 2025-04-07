#include "PumpController.h"
#include "BolusRecord.h"
#include <QTimer>
#include <QDebug>

PumpController::PumpController(UserProfileManager* profileMgr,
                               BolusHistoryManager* historyMgr,
                               BolusSafetyManager* safetyMgr,
                               QObject* parent)
    : QObject(parent)
    , userProfileManager(profileMgr)
    , bolusHistoryManager(historyMgr)
    , safetyManager(safetyMgr)
{
}

bool PumpController::requestBolus(double totalBolus,
                                  const QString& notes,
                                  double extendedFrac,
                                  int durationHrs)
{
    // Safety check
    QString errorMsg;
    if (!safetyManager->canDeliverBolus(totalBolus, errorMsg)) {
        emit bolusFailed(errorMsg);
        return false;
    }

    // Split immediate vs. extended
    double immediate = totalBolus;
    double extended  = 0.0;
    if (extendedFrac > 0.0 && extendedFrac < 1.0) {
        immediate = totalBolus * (1.0 - extendedFrac);
        extended  = totalBolus * extendedFrac;
    }

    // 1) Deliver immediate portion
    safetyManager->recordBolus(immediate);
    BolusRecord immediateRecord(immediate, notes + " [Immediate]");
    bolusHistoryManager->addRecord(immediateRecord);

    // 2) Extended portion
    if (extended > 0.0) {
        // For demonstration, let's just record it at once
        // In a real pump, you'd deliver it gradually over durationHrs
        // with repeated safety checks or partial increments.

        // Possibly do another check if you want to ensure daily limit
        safetyManager->recordBolus(extended);
        BolusRecord extendedRecord(extended,
            QString("Extended %1 hrs").arg(durationHrs));
        bolusHistoryManager->addRecord(extendedRecord);
    }

    emit bolusDelivered(immediate, extended);
    return true;
}
