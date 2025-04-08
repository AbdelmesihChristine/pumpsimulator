#ifndef PUMPCONTROLLER_H
#define PUMPCONTROLLER_H

#include <QObject>
#include "UserProfileManager.h"
#include "HistoryManager.h"
#include "BolusSafetyManager.h"
#include "CgmSimulator.h"

/**
 * @brief Controls insulin delivery:
 * - Manual Bolus
 * - Automatic (Control-IQ) adjustments
 */
class PumpController : public QObject
{
    Q_OBJECT
public:
    explicit PumpController(UserProfileManager* profileMgr,
                            HistoryManager* histMgr,
                            BolusSafetyManager* safetyMgr,
                            CgmSimulator* cgmSim,
                            QObject* parent = nullptr);

    bool requestBolus(double totalBolus,
                      const QString& notes,
                      double extendedFrac = 0.0,
                      int durationHrs = 0);

public slots:
    /**
     * @brief Called whenever CGM updates.
     * We log the reading and run Control-IQ logic for auto adjustments.
     */
    void onCgmUpdated(double newBg);

private:
    UserProfileManager* userProfileManager;
    HistoryManager*     historyManager;
    BolusSafetyManager* safetyManager;
    CgmSimulator*       cgmSimulator;

    /**
     * @brief Basic function to do controlIQ logic:
     * predict BG in 30min using last 6 readings,
     * then adjust basal or auto-bolus if needed.
     */
    void runControlIQ(double currentBg);

    /**
     * @brief Actually do an automatic bolus
     */
    void deliverAutoBolus(double units, const QString& reason);
};

#endif // PUMPCONTROLLER_H
