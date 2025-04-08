#ifndef WARNINGCHECKER_H
#define WARNINGCHECKER_H

#include <QObject>
#include <QTimer>
#include "HistoryManager.h"
#include "CgmSimulator.h"

/**
 * @brief Checks battery, insulin reservoir, and BG for warnings every 30s.
 */
class WarningChecker : public QObject
{
    Q_OBJECT
public:
    explicit WarningChecker(HistoryManager* hist, CgmSimulator* cgm, QObject* parent=nullptr);

    void setBatteryLevel(int level) { batteryLevel = level; }
    int  getBatteryLevel() const { return batteryLevel; }

    void setInsulinLevel(double units) { insulinReservoir = units; }
    double getInsulinLevel() const { return insulinReservoir; }
    void showDarkWarning(const QString& title, const QString& text);

public slots:
    void startMonitoring();
    void stopMonitoring();

private slots:
    void onCheck();

private:
    HistoryManager* history;
    CgmSimulator*   cgmSimulator;
    QTimer checkTimer;

    int batteryLevel;        // 0..100%
    double insulinReservoir; // total units left in cartridge

    void logWarning(const QString& msg);
};

#endif // WARNINGCHECKER_H
