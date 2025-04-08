#include "CGMGraphWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

CGMGraphWidget::CGMGraphWidget(CgmSimulator* simulator, QWidget *parent)
    : QWidget(parent)
    , cgmSimulator(simulator)
    , timeCounter(0)
{
    chart = new QChart();
    series = new QLineSeries();
    axisX = new QValueAxis();
    axisY = new QValueAxis();

    axisX->setTitleText("Time (sec)");
    axisY->setTitleText("BG (mmol/L)");
    axisX->setRange(0, 60);
    axisY->setRange(2, 16);

    chart->addSeries(series);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    chart->setBackgroundBrush(QBrush(QColor("#1c1b1b")));

    axisX->setGridLineColor(QColor("#555555"));
    axisY->setGridLineColor(QColor("#555555"));
    axisX->setLabelsBrush(QBrush(Qt::white));
    axisY->setLabelsBrush(QBrush(Qt::white));
    axisX->setTitleBrush(QBrush(Qt::white));
    axisY->setTitleBrush(QBrush(Qt::white));
    series->setColor(QColor("#00ccff"));

    chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);

    rangeComboBox = new QComboBox(this);
    rangeComboBox->addItem("1h (12s)", 12);  // 12 real seconds = 1 sim hour
    rangeComboBox->addItem("3h (36s)", 36);  // 36 real seconds
    rangeComboBox->addItem("6h (72s)", 72);  // 72 real seconds
    connect(rangeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CGMGraphWidget::onRangeChanged);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(new QLabel("BG Graph:", this));
    mainLayout->addWidget(rangeComboBox);
    mainLayout->addWidget(chartView);

    setLayout(mainLayout);

    if (cgmSimulator) {
        connect(cgmSimulator, &CgmSimulator::bgUpdated, this, &CGMGraphWidget::updateGraph);
    }
}

void CGMGraphWidget::updateGraph(double bgValue)
{
    // Each real second -> timeCounter++,
    // so in 1 second we move 1 on X axis
    timeCounter++;
    series->append(timeCounter, bgValue);

    // Keep the chart auto-scrolling if beyond the initial range
    if (timeCounter > axisX->max()) {
        double rangeSize = axisX->max() - axisX->min();
        axisX->setRange(timeCounter - rangeSize, timeCounter);
    }
}

void CGMGraphWidget::onRangeChanged(int index)
{
    // We'll interpret the user data (12, 36, 72) as how many real seconds to show
    int maxSec = rangeComboBox->currentData().toInt();
    axisX->setRange(0, maxSec);
}
