#ifndef EDITLISTSDIALOG_H
#define EDITLISTSDIALOG_H

#include <QDialog>
#include <QList>
#include <QPushButton>
#include <QComboBox>
#include <QString>
#include <QStringListModel>
#include <QListView>
#include <QPlainTextEdit>

namespace Ui {
class EditListsDialog;
}

class EditListsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditListsDialog(QStringListModel* rooms,
                             QStringListModel* classes,
                             QStringListModel* groups,
                             QStringListModel* teachers);
    ~EditListsDialog();
public slots:
    void changeCurrentModel(int i);
    void removeSelectedRow();
    void addRow();

private:
    Ui::EditListsDialog *ui;
    QStringListModel *rooms;
    QStringListModel *classes;
    QStringListModel *groups;
    QStringListModel *teachers;
    QStringListModel *currentModel;

};

#endif // EDITLISTSDIALOG_H
