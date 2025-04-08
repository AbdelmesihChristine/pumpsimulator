#ifndef BOLUSSAFETYMANAGER_H
#define BOLUSSAFETYMANAGER_H

#include <QString>
#include <QTime>

/**
 * @brief Checks bolus safety constraints
 *        such as max single bolus, daily limit, and cooldown.
 */
class BolusSafetyManager
{
public:
    BolusSafetyManager();

    bool canDeliverBolus(double amount, QString &errorMessage);
    void recordBolus(double amount);

private:
    double maxSingleBolus = 10.0;
    double maxDailyBolus  = 30.0;
    int cooldownMinutes   = 10;

    double totalDailyBolus;
    QTime lastBolusTime;
};

#endif // BOLUSSAFETYMANAGER_H
