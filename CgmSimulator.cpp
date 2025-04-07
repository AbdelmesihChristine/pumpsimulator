#include "CgmSimulator.h"
#include <QRandomGenerator>
#include <QtMath>

CgmSimulator::CgmSimulator(QObject* parent)
    : QObject(parent)
    , currentBg(7.0) // start at BG=7 mmol/L
{
    connect(&updateTimer, &QTimer::timeout, this, &CgmSimulator::onTimerTick);
}

double CgmSimulator::getCurrentBg() const
{
    return currentBg;
}

void CgmSimulator::start()
{
    // update every 3 seconds for demonstration
    updateTimer.start(3000);
}

void CgmSimulator::onTimerTick()
{
    // random walk +/- 0.2
    double delta = (QRandomGenerator::global()->bounded(20) - 10) / 50.0;
    currentBg += delta;
    if (currentBg < 3.0) currentBg = 3.0;
    if (currentBg > 15.0) currentBg = 15.0;

    emit bgUpdated(currentBg);
}
