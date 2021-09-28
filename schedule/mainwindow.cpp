#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("School Planner");
    rooms = new QStringListModel();
    teachers = new QStringListModel();
    groups = new QStringListModel();
    classes = new QStringListModel();

    table = ui->tableView;
    table->setDisabled(true);
    tableModel = new ActivityModel;
    table->setModel(tableModel);

    roomSelector = ui->comboBox;
    roomSelector->setModel(rooms);

    connect(table, &QTableView::doubleClicked,this,&MainWindow::openEditDialog); //Test
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newPlan);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
    connect(ui->actionEdit_Lists, &QAction::triggered, this, &MainWindow::editLists);
    connect(ui->actionAddEntry, &QAction::triggered, this, &MainWindow::addEntry);
    connect(roomSelector, &QComboBox::currentTextChanged, this, &MainWindow::changeActiveRoom);
}


void MainWindow::openEditDialog(const QModelIndex &index){
    // Opens dialog window and sets up it's combo boxes
    EditActivityDialog d;
    d.setComboBoxLists(rooms, classes, groups, teachers);

    QVariant varItem = tableModel->data(index, Qt::EditRole);
    if(varItem.isValid()){
        Activity actItem = varItem.value<Activity>();
        d.setComboBoxItems(index.column(),
                           index.row(),
                           roomSelector->currentText(),
                           actItem.group,
                           actItem.aclass,
                           actItem.teacher);
    }
    else{
        d.setComboBoxItems(index.column(),
                           index.row(),
                           roomSelector->currentText(),
                           "", "", "");
    }
    d.disableTopBoxes();
    d.exec();

    if(d.result() == QDialog::Accepted){
        QString c = d.getClass();
        QString g = d.getGroup();
        QString t = d.getTeacher();
        Activity act(index.column(),index.row(),g,c,t,roomSelector->currentText());
        QVariant vAct;
        vAct.setValue(act);
        tableModel->setData(index, vAct, Qt::EditRole);
    }
    else if(d.result() == 2){
        tableModel->removeData(index,roomSelector->currentText(),Qt::EditRole);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete rooms;
    delete teachers;
    delete groups;
    delete classes;
    delete tableModel;
    delete table;
}

void MainWindow::newPlan(){
    currentFile.clear();
    rooms->setStringList({});
    teachers->setStringList({});
    groups->setStringList({});
    classes->setStringList({});
    delete tableModel;
    tableModel = new ActivityModel;
    table->setModel(tableModel);
    table->setDisabled(true);
}

void MainWindow::open(){
    QString fileName = QFileDialog::getOpenFileName(this, "Open file", "", "JSON (*.json);;All Files (*.*)");
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }
    QByteArray fileData = file.readAll();
    file.close();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(fileData));
    QJsonObject jsonData = jsonDoc.object();
    rooms->setStringList(jsonData["rooms"].toVariant().toStringList());
    groups->setStringList(jsonData["groups"].toVariant().toStringList());
    classes->setStringList(jsonData["classes"].toVariant().toStringList());
    teachers->setStringList(jsonData["teachers"].toVariant().toStringList());
    tableModel->setJsonArray(jsonData["activities"].toArray());
    roomSelector->setCurrentIndex(0);
}

void MainWindow::save(){
    if(currentFile.isEmpty()){
        this->saveAs();
    }
    else{
        this->saveUnderName(currentFile);
    }
}

void MainWindow::saveAs(){
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    this->saveUnderName(fileName);
    currentFile = fileName;
}

void MainWindow::saveUnderName(QString fileName){
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }

    QList<Activity> activities = tableModel->getAllActivities();
    QJsonArray jActivities;
    for(int i = 0; i < activities.size(); i++){
        jActivities << activities[i].toJsonObject();
    }

    QJsonObject save;
    save["rooms"] = QJsonArray::fromStringList(rooms->stringList());
    save["groups"] = QJsonArray::fromStringList(groups->stringList());
    save["classes"] = QJsonArray::fromStringList(classes->stringList());
    save["teachers"] = QJsonArray::fromStringList(teachers->stringList());
    save["activities"] = jActivities;

    QJsonDocument saveDoc(save);
    file.write(saveDoc.toJson());
    file.close();
}

void MainWindow::editLists(){
    EditListsDialog d(rooms, groups, classes, teachers);
    d.exec();
}

void MainWindow::changeActiveRoom(QString room){
    tableModel->changeActiveRoom(room);
    table->setEnabled(true);
}

void MainWindow::addEntry(){
    EditActivityDialog dialog;
    dialog.setComboBoxLists(rooms, classes, groups, teachers);
    dialog.setComboBoxItems(-1,-1,"","","","");
    dialog.disableRestoreButton();
    dialog.exec();
    if(dialog.result() == QDialog::Accepted){
        int d = dialog.getDay();
        int h = dialog.getHour();
        QString r = dialog.getRoom();
        QString c = dialog.getClass();
        QString g = dialog.getGroup();
        QString t = dialog.getTeacher();
        Activity act(d,h,g,c,t,r);
        QVariant vAct;
        vAct.setValue(act);
        QModelIndex index = tableModel->index(h,d,QModelIndex());
        tableModel->setData(index, vAct, Qt::EditRole);
    }
}
