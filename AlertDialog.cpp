#include "AlertDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

AlertDialog::AlertDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Alerts");

    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* label = new QLabel("No alerts at this time.", this);
    QPushButton* closeBtn = new QPushButton("Close", this);

    layout->addWidget(label);
    layout->addWidget(closeBtn);

    connect(closeBtn, &QPushButton::clicked, this, &AlertDialog::accept);

    setLayout(layout);
}
