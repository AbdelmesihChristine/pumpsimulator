#ifndef CGMGRAPHWIDGET_H
#define CGMGRAPHWIDGET_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QComboBox>
#include "CgmSimulator.h"

QT_CHARTS_USE_NAMESPACE

class CGMGraphWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CGMGraphWidget(CgmSimulator* simulator, QWidget *parent = nullptr);

private slots:
    void updateGraph(double bgValue);
    void onRangeChanged(int index);

private:
    CgmSimulator* cgmSimulator;
    QChartView* chartView;
    QLineSeries* series;
    QChart* chart;
    QValueAxis* axisX;
    QValueAxis* axisY;
    QComboBox*  rangeComboBox;

    int timeCounter; // increments by 1 each second
};

#endif // CGMGRAPHWIDGET_H
