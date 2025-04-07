#ifndef ALERTDIALOG_H
#define ALERTDIALOG_H

#include <QDialog>

class AlertDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AlertDialog(QWidget *parent = nullptr);
};

#endif // ALERTDIALOG_H
