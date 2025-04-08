#ifndef CGMSIMULATOR_H
#define CGMSIMULATOR_H

#include <QObject>
#include <QTimer>
#include <deque>

/**
 * @brief Simulates CGM readings every 1 second => 5 simulated minutes.
 * Also keeps the last 6 readings for 30min (prediction).
 */
class CgmSimulator : public QObject
{
    Q_OBJECT
public:
    explicit CgmSimulator(QObject* parent = nullptr);

    double getCurrentBg() const;
    void start();
    QString getSimTimeStr() const;

    /**
     * @brief Return the last 6 readings in chronological order
     * (oldest first).
     */
    std::deque<double> getLastSixReadings() const;

signals:
    void bgUpdated(double newBg);

private slots:
    void onTimerTick();

private:
    double currentBg;
    QTimer updateTimer;
    int totalSimMinutes;

    // Rolling queue of the last 6 BG readings
    std::deque<double> lastSix;

    void pushReading(double bg);
};

#endif // CGMSIMULATOR_H
