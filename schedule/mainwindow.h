#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "activitymodel.h"
#include "editactivitydialog.h"
#include "editlistsdialog.h"
#include <QMainWindow>
#include <QString>
#include <QStringListModel>
#include <QStringList>
#include <QTableView>
#include <QComboBox>
#include <QHeaderView>
#include <QAbstractTableModel>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QString currentFile;
    QTableView *table;
    ActivityModel *tableModel;
    QComboBox *roomSelector;
    QStringListModel *rooms;
    QStringListModel *teachers;
    QStringListModel *groups;
    QStringListModel *classes;
    Ui::MainWindow *ui;
    QStringList days = {"Mon","Tue","Wed","Th","Fri"};
    QStringList hours = {"8:00-8:45","8:55-9:40","9:50-11:35",
                         "11:55-12:40","12:50-13:35","13:45-14:30",
                         "14:40-15:25","15:35-16:20","16:30-17:15"};
    void saveUnderName(QString);

private slots:
    void changeActiveRoom(QString);
    void addEntry();
    void newPlan();
    void open();
    void save();
    void saveAs();
    void editLists();
    void openEditDialog(const QModelIndex &index);
};
#endif // MAINWINDOW_H
