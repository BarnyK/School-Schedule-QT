#ifndef EDITACTIVITYDIALOG_H
#define EDITACTIVITYDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QComboBox>
#include <QString>
#include <QStringListModel>

namespace Ui {
class EditActivityDialog;
}

class EditActivityDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditActivityDialog(QWidget *parent = nullptr);
    ~EditActivityDialog();
    void setComboBoxLists(QStringListModel *, QStringListModel *, QStringListModel *, QStringListModel *);
    void setLabels(QString room, QString day , QString slot);
    void setComboBoxItems(int day, int hour, QString room, QString group, QString aclass, QString teacher);
    QString getClass();
    QString getGroup();
    QString getTeacher();
    QString getRoom();
    int getHour();
    int getDay();
    void disableTopBoxes();
    void disableRestoreButton();

private slots:
    void enableOkButton();
private:
    Ui::EditActivityDialog *ui;
    QPushButton *okButton;
    QComboBox *groupBox;             // Boxes
    QComboBox *classBox;
    QComboBox *teacherBox;
    QComboBox *roomBox;
    QString groupInitial = "";       // Initial values of boxes
    QString classInitial = "";
    QString teacherInitial = "";
    QStringListModel *dModel;
    QStringListModel *hModel;

    void quitWithRestore();

    QStringList days = {"Mon","Tue","Wed","Th","Fri"};
    QStringList hours = {"8:00-8:45","8:55-9:40","9:50-11:35",
                         "11:55-12:40","12:50-13:35","13:45-14:30",
                         "14:40-15:25","15:35-16:20","16:30-17:15"};
};

#endif // EDITACTIVITYDIALOG_H
