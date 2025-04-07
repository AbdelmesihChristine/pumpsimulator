#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include "BolusHistoryManager.h"

/**
 * @brief A dialog to display the bolus history in a table view.
 */
class HistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HistoryDialog(BolusHistoryManager* manager, QWidget* parent = nullptr);

private:
    void updateTable();

    BolusHistoryManager* historyManager;
    QTableWidget* table;
};

#endif // HISTORYDIALOG_H
