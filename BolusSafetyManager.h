#ifndef BOLUSSAFETYMANAGER_H
#define BOLUSSAFETYMANAGER_H

#include <QString>
#include <QTime>

/**
 * @brief A simple manager for safety checks:
 *        - max single bolus
 *        - max daily bolus
 *        - time-based cooldown
 */
class BolusSafetyManager
{
public:
    BolusSafetyManager();

    bool canDeliverBolus(double amount, QString &errorMessage);
    void recordBolus(double amount);

private:
    const double maxSingleBolus = 10.0;
    const double maxDailyBolus  = 30.0;
    const int cooldownMinutes   = 10;

    double totalDailyBolus;
    QTime lastBolusTime;
};

#endif // BOLUSSAFETYMANAGER_H
