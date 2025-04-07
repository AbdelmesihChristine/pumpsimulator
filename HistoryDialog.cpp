#include "HistoryDialog.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include "BolusRecord.h"

HistoryDialog::HistoryDialog(BolusHistoryManager* manager, QWidget* parent)
    : QDialog(parent)
    , historyManager(manager)
{
    setWindowTitle("Bolus History");
    resize(400, 300);

    QVBoxLayout* layout = new QVBoxLayout(this);

    table = new QTableWidget(this);
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels({"Amount (U)", "Notes"});
    table->horizontalHeader()->setStretchLastSection(true);

    layout->addWidget(table);
    setLayout(layout);

    // Populate initial data
    updateTable();
}

void HistoryDialog::updateTable()
{
    const auto& records = historyManager->getRecords();
    table->setRowCount(static_cast<int>(records.size()));

    for (int i = 0; i < (int)records.size(); ++i) {
        const BolusRecord& rec = records[i];
        table->setItem(i, 0,
            new QTableWidgetItem(QString::number(rec.getAmount())));
        table->setItem(i, 1,
            new QTableWidgetItem(rec.getNotes()));
    }
}
