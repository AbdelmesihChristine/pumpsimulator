#ifndef CGMSIMULATOR_H
#define CGMSIMULATOR_H

#include <QObject>
#include <QTimer>

class CgmSimulator : public QObject
{
    Q_OBJECT
public:
    explicit CgmSimulator(QObject* parent = nullptr);

    double getCurrentBg() const;
    void start();

signals:
    void bgUpdated(double newBg);

private slots:
    void onTimerTick();

private:
    double currentBg;
    QTimer updateTimer;
};

#endif // CGMSIMULATOR_H
