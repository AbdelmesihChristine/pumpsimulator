#include "CgmSimulator.h"
#include <QRandomGenerator>
#include <QtMath>

CgmSimulator::CgmSimulator(QObject* parent)
    : QObject(parent)
    , currentBg(7.0)  // Start around normal BG
    , totalSimMinutes(0)
{
    connect(&updateTimer, &QTimer::timeout, this, &CgmSimulator::onTimerTick);
}

double CgmSimulator::getCurrentBg() const
{
    return currentBg;
}

void CgmSimulator::start()
{
    // 1 real second => 5 simulated minutes
    // So in 72 real seconds => 6 hours pass
    updateTimer.start(1000);
}

QString CgmSimulator::getSimTimeStr() const
{
    int hours = totalSimMinutes / 60;
    int mins  = totalSimMinutes % 60;
    return QString("%1:%2")
            .arg(hours, 2, 10, QLatin1Char('0'))
            .arg(mins, 2, 10, QLatin1Char('0'));
}

std::deque<double> CgmSimulator::getLastSixReadings() const
{
    return lastSix;
}

void CgmSimulator::onTimerTick()
{
    totalSimMinutes += 5;

    // random walk or random generation
    double delta = (QRandomGenerator::global()->bounded(20) - 10) / 50.0;
    currentBg += delta;
    if (currentBg < 2.5)  currentBg = 2.5;
    if (currentBg > 18.0) currentBg = 18.0;

    pushReading(currentBg);
    emit bgUpdated(currentBg);
}

void CgmSimulator::pushReading(double bg)
{
    lastSix.push_back(bg);
    // Keep only last 6
    while (lastSix.size() > 6) {
        lastSix.pop_front();
    }
}
