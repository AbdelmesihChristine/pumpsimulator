#ifndef ALERTDIALOG_H
#define ALERTDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include "HistoryManager.h"

class AlertDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AlertDialog(HistoryManager* historyMgr, QWidget *parent = nullptr);
    void updateAlerts();

private:
    HistoryManager* historyManager;
    QTableWidget* table;
};

#endif // ALERTDIALOG_H
