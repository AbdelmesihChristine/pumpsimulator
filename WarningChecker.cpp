#include "WarningChecker.h"
#include "HistoryRecord.h"
#include <QMessageBox>

WarningChecker::WarningChecker(HistoryManager* hist, CgmSimulator* cgm, QObject* parent)
    : QObject(parent),
      history(hist),
      cgmSimulator(cgm),
      batteryLevel(100),
      insulinReservoir(200.0) // default
{
    connect(&checkTimer, &QTimer::timeout, this, &WarningChecker::onCheck);
}

void WarningChecker::startMonitoring()
{
    checkTimer.start(30000); // every 30 real seconds
}

void WarningChecker::stopMonitoring()
{
    checkTimer.stop();
}

void WarningChecker::onCheck()
{
    // 1) Deplete battery by 1% for demonstration
    if (batteryLevel > 0) {
        batteryLevel -= 1;
    }

    // 2) Check battery thresholds
    if (batteryLevel == 5) {
        logWarning("Battery critically low!");
    } else if (batteryLevel == 20) {
        logWarning("Battery low!");
    }

    // 3) Check insulin reservoir
    // For demonstration, we only reduce reservoir
    // whenever we do a bolus in PumpController => not here
    if (insulinReservoir <= 5) {
        logWarning("Insulin critically low!");
    } else if (insulinReservoir <= 20) {
        logWarning("Insulin low!");
    }

    // 4) Check BG for critical
    if (cgmSimulator) {
        double bg = cgmSimulator->getCurrentBg();
        if (bg < 3.9) {
            logWarning(QString("BG critically low (%.1f)!" ).arg(bg));
        } else if (bg > 13.9) {
            logWarning(QString("BG critically high (%.1f)!" ).arg(bg));
        }
    }
}

void WarningChecker::logWarning(const QString& msg)
{
    if (!history || !cgmSimulator) return;

    history->addRecord({
        cgmSimulator->getSimTimeStr(),
        RecordType::Warning,
        0.0,
        msg
    });

    showDarkWarning("Pump Warning", msg);
}

void WarningChecker::showDarkWarning(const QString& title, const QString& text)
{
    QMessageBox box(QMessageBox::Warning, title, text, QMessageBox::Ok);
    box.setStyleSheet(R"(
        QMessageBox {
            background-color: #2a2a2a;
        }
        QLabel {
            color: white;
            font-weight: bold;
        }
        QPushButton {
            background-color: #444;
            color: white;
            border: 1px solid #666;
            padding: 6px;
            border-radius: 4px;
        }
        QPushButton:hover {
            background-color: #555;
        }
    )");
    box.exec();
}
