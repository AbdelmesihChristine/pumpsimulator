#include "PumpController.h"
#include "HistoryRecord.h"
#include <QDebug>

PumpController::PumpController(UserProfileManager* profileMgr,
                               HistoryManager* histMgr,
                               BolusSafetyManager* safetyMgr,
                               CgmSimulator* cgmSim,
                               QObject* parent)
    : QObject(parent),
      userProfileManager(profileMgr),
      historyManager(histMgr),
      safetyManager(safetyMgr),
      cgmSimulator(cgmSim)
{
    // Connect CGM updates to our controlIQ logic
    connect(cgmSimulator, &CgmSimulator::bgUpdated,
            this, &PumpController::onCgmUpdated);
}

bool PumpController::requestBolus(double totalBolus,
                                  const QString& notes,
                                  double extendedFrac,
                                  int durationHrs)
{
    QString errorMsg;
    if (!safetyManager->canDeliverBolus(totalBolus, errorMsg)) {
        // signal to UI
        return false;
    }

    double immediate = totalBolus;
    double extended  = 0.0;
    if (extendedFrac > 0.0 && extendedFrac < 1.0) {
        immediate = totalBolus * (1.0 - extendedFrac);
        extended  = totalBolus * extendedFrac;
    }

    // Record immediate portion
    safetyManager->recordBolus(immediate);
    historyManager->addRecord({
        cgmSimulator->getSimTimeStr(),
        RecordType::ManualBolus,
        immediate,
        notes + " (Immediate portion)"
    });

    // Extended portion
    if (extended > 0.0) {
        // For demonstration, we add it all at once
        safetyManager->recordBolus(extended);
        historyManager->addRecord({
            cgmSimulator->getSimTimeStr(),
            RecordType::ManualBolus,
            extended,
            QString("Extended portion over %1hr").arg(durationHrs)
        });
    }

    return true;
}

void PumpController::onCgmUpdated(double newBg)
{
    // 1) Log CGM reading
    historyManager->addRecord({
        cgmSimulator->getSimTimeStr(),
        RecordType::CgmReading,
        0.0,
        QString("BG= %1 mmol/L").arg(newBg, 0, 'f', 1)
    });

    // 2) ControlIQ check
    runControlIQ(newBg);
}

void PumpController::runControlIQ(double currentBg)
{
    auto lastSix = cgmSimulator->getLastSixReadings();
    if (lastSix.size() < 6) {
        return; // Not enough data for 30min trend yet
    }
    double first = lastSix.front();  // 30 min ago
    double predicted = currentBg + (currentBg - first);

    // If predicted < 3.9 => reduce or suspend basal
    if (predicted < 3.9) {
        // e.g. Basal suspended
        historyManager->addRecord({
            cgmSimulator->getSimTimeStr(),
            RecordType::Other,
            0.0,
            QString("Basal suspended by Control-IQ (predBG= %1)").arg(predicted,0,'f',1)
        });
        // In real system, you'd set the actual basal rate to 0.
        return;
    }

    // If predicted >=14 => deliver auto correction
    if (predicted >= 14.0) {
        // A naive approach: deliver 1u.
        // Real logic could be e.g. (predicted - target)/CF etc.
        deliverAutoBolus(1.0, QString("Auto correction (predBG= %1)").arg(predicted,0,'f',1));
        return;
    }

    // If predicted >=10 => increase basal
    if (predicted >= 10.0) {
        // e.g. Increase basal by some factor
        historyManager->addRecord({
            cgmSimulator->getSimTimeStr(),
            RecordType::Other,
            0.0,
            QString("Basal increased by Control-IQ (predBG= %1)").arg(predicted,0,'f',1)
        });
    }
    // else predicted is 3.9-10 => do nothing
}

void PumpController::deliverAutoBolus(double units, const QString& reason)
{
    QString errorMsg;
    if (!safetyManager->canDeliverBolus(units, errorMsg)) {
        // can't deliver auto-bolus
        historyManager->addRecord({
            cgmSimulator->getSimTimeStr(),
            RecordType::Warning,
            0.0,
            QString("Auto-bolus blocked: %1").arg(errorMsg)
        });
        return;
    }
    safetyManager->recordBolus(units);
    historyManager->addRecord({
        cgmSimulator->getSimTimeStr(),
        RecordType::AutoBolus,
        units,
        reason
    });
}
