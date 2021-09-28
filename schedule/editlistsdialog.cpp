#include "editlistsdialog.h"
#include "ui_editlistsdialog.h"

EditListsDialog::EditListsDialog(
        QStringListModel* aRooms,
        QStringListModel* aClasses,
        QStringListModel* aGroups,
        QStringListModel* aTeachers) :
    QDialog(),
    ui(new Ui::EditListsDialog)
{
    ui->setupUi(this);
    rooms = aRooms;
    classes = aClasses;
    groups = aGroups;
    teachers = aTeachers;

    currentModel = rooms;
    ui->listView->setModel(currentModel);

    connect(ui->listSelect, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &EditListsDialog::changeCurrentModel);
    connect(ui->removeButton, &QPushButton::clicked, this, &EditListsDialog::removeSelectedRow);
    connect(ui->addButton, &QPushButton::clicked, this, &EditListsDialog::addRow);
    connect(ui->addTextEdit, &QLineEdit::returnPressed, this, &EditListsDialog::addRow);
}

EditListsDialog::~EditListsDialog()
{
    delete ui;
}

void EditListsDialog::changeCurrentModel(int i){
    switch(i){
    case 0:
        currentModel = rooms;
        break;
    case 1:
        currentModel = classes;
        break;
    case 2:
        currentModel = groups;
        break;
    case 3:
        currentModel = teachers;
        break;
    }
    ui->listView->setModel(currentModel);
}

void EditListsDialog::removeSelectedRow(){
    QModelIndexList indexes = ui->listView->selectionModel()->selectedRows();
    if(indexes.size() > 0){
        currentModel->removeRow(indexes[0].row());
    }
}

void EditListsDialog::addRow(){
    QString value = ui->addTextEdit->text();
    if(value.size() > 0){
        currentModel->insertRow(currentModel->rowCount());
        QModelIndex i = currentModel->index(currentModel->rowCount()-1,0);
        currentModel->setData(i, value);
        ui->addTextEdit->clear();
    }
}
