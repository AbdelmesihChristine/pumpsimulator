#ifndef PUMPCONTROLLER_H
#define PUMPCONTROLLER_H

#include <QObject>
#include <QString>
#include "BolusHistoryManager.h"
#include "BolusSafetyManager.h"
#include "UserProfileManager.h"

/**
 * @brief The PumpController class acts as the "application logic"
 *        or "domain controller" for delivering boluses,
 *        referencing the user profile, safety limits, and so on.
 */
class PumpController : public QObject
{
    Q_OBJECT
public:
    explicit PumpController(UserProfileManager* profileMgr,
                            BolusHistoryManager* historyMgr,
                            BolusSafetyManager* safetyMgr,
                            QObject* parent = nullptr);

    /**
     * @brief Request a bolus (either immediate or extended).
     * @param totalBolus   The total insulin units to deliver.
     * @param notes        Any user notes.
     * @param extendedFrac The fraction (0 to 1) of the total to
     *                     deliver extended. e.g., 0.4 means 40%.
     * @param durationHrs  How many hours to spread the extended portion.
     *
     * This method:
     * 1) checks safety constraints,
     * 2) records immediate portion,
     * 3) (optionally) schedules extended portion, or delivers it all at once for demonstration.
     */
    bool requestBolus(double totalBolus,
                      const QString& notes,
                      double extendedFrac = 0.0,
                      int durationHrs = 0);

signals:
    /**
     * @brief Emitted when a bolus is successfully delivered
     *        (or partially delivered in extended).
     *
     * Use this to update the UI if needed.
     */
    void bolusDelivered(double immediate, double extended);

    /**
     * @brief Emitted if a safety check fails or something else
     *        prevents the bolus from being delivered.
     */
    void bolusFailed(const QString& errorMsg);

private:
    UserProfileManager* userProfileManager;
    BolusHistoryManager* bolusHistoryManager;
    BolusSafetyManager* safetyManager;
};


#endif // PUMPCONTROLLER_H
