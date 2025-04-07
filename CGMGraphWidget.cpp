#include "CGMGraphWidget.h"
#include <QVBoxLayout>

CGMGraphWidget::CGMGraphWidget(CgmSimulator* simulator, QWidget *parent)
    : QWidget(parent)
    , cgmSimulator(simulator)
    , timeCounter(0)
{
    chart = new QChart();
    series = new QLineSeries();
    axisX = new QValueAxis();
    axisY = new QValueAxis();

    axisX->setTitleText("Time (s)");
    axisY->setTitleText("BG (mmol/L)");


    axisX->setRange(0, 60);
    axisY->setRange(2, 16);

    chart->addSeries(series);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    // Dark Theme + White Labels + White Titles
    chart->setBackgroundBrush(QBrush(QColor("#1c1b1b"))); // Dark background
    axisX->setGridLineColor(QColor("#555555"));            // Light gridlines
    axisY->setGridLineColor(QColor("#555555"));            // Light gridlines
    axisX->setLabelsBrush(QBrush(Qt::white));              // White axis numbers
    axisY->setLabelsBrush(QBrush(Qt::white));              // White axis numbers
    series->setColor(QColor("#00ccff"));                   // Blue glucose curve
    series->setPointsVisible(false);                       // Hide dots, smooth curve

    // New: Make Title Text white
    QFont axisFont;
    axisFont.setBold(true);
    axisFont.setPointSize(10);

    axisX->setTitleBrush(QBrush(Qt::white));
    axisY->setTitleBrush(QBrush(Qt::white));
    axisX->setTitleFont(axisFont);
    axisY->setTitleFont(axisFont);


    chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
    setLayout(layout);

    if (cgmSimulator) {
        connect(cgmSimulator, &CgmSimulator::bgUpdated,
                this, &CGMGraphWidget::updateGraph);
    }
}

void CGMGraphWidget::updateGraph(double bgValue)
{
    timeCounter += 3;
    series->append(timeCounter, bgValue);

    if (timeCounter > 60) {
        axisX->setRange(timeCounter - 60, timeCounter);
    }
}
