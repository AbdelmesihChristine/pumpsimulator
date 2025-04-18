#ifndef PROFILEDIALOG_H
#define PROFILEDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QListWidgetItem>
#include "UserProfileManager.h"

class ProfileDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ProfileDialog(UserProfileManager* mgr, QWidget *parent = nullptr);

private slots:
    void onAddProfile();
    void onEditProfile();
    void onDeleteProfile();
    void onProfileActivated(QListWidgetItem* item);

private:
    void refreshProfileList();
    bool getProfileInput(UserProfile &profile, const QString &title);

    UserProfileManager* profileManager;
    QListWidget* profileList;
    QPushButton* addButton;
    QPushButton* editButton;
    QPushButton* deleteButton;
};

#endif // PROFILEDIALOG_H
