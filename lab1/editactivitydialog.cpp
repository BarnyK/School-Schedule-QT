#include "editactivitydialog.h"
#include "ui_editactivitydialog.h"

EditActivityDialog::EditActivityDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditActivityDialog)
{
    ui->setupUi(this);
    classBox = ui->classBox;
    groupBox = ui->groupBox;
    teacherBox = ui->teachBox;
    roomBox = ui->roomBox;

    dModel = new QStringListModel;
    hModel = new QStringListModel;
    dModel->setStringList(days);
    hModel->setStringList(hours);
    ui->dayBox->setModel(dModel);
    ui->hourBox->setModel(hModel);

    okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDisabled(true);
    QPushButton *restoreButton = ui->buttonBox->button(QDialogButtonBox::RestoreDefaults);

    setWindowTitle("Edit Record");

    connect(classBox, &QComboBox::currentTextChanged, this, &EditActivityDialog::enableOkButton);
    connect(groupBox, &QComboBox::currentTextChanged, this, &EditActivityDialog::enableOkButton);
    connect(teacherBox, &QComboBox::currentTextChanged, this, &EditActivityDialog::enableOkButton);
    connect(restoreButton, &QPushButton::clicked, this, &EditActivityDialog::quitWithRestore);
}

EditActivityDialog::~EditActivityDialog()
{
    delete ui;
    delete dModel;
    delete hModel;
}

void EditActivityDialog::quitWithRestore(){
    done(2);
}

void EditActivityDialog::setComboBoxLists(QStringListModel *r, QStringListModel *c, QStringListModel *g, QStringListModel *t){
    classBox->setModel(c);
    groupBox->setModel(g);
    teacherBox->setModel(t);
    roomBox->setModel(r);
}
void EditActivityDialog::setComboBoxItems(int d, int h, QString r, QString g, QString c, QString t){
    int index;
    ui->dayBox->setCurrentIndex(d);
    ui->hourBox->setCurrentIndex(h);

    roomBox->setCurrentIndex(-1);
    groupBox->setCurrentIndex(-1);
    classBox->setCurrentIndex(-1);
    teacherBox->setCurrentIndex(-1);
    if(r.size()!=0){
        index = roomBox->findText(r);
        if(index!=-1){
            roomBox->setCurrentIndex(index);
        }
    }
    if(g.size()!=0){
        index = groupBox->findText(g);
        if(index!=-1){
            groupBox->setCurrentIndex(index);
            groupInitial = groupBox->currentText();
        }
    }
    if(c.size()!=0){
        index = classBox->findText(c);
        if(index!=-1){
            classBox->setCurrentIndex(index);
            classInitial = classBox->currentText();;
        }
    }
    if(t.size()!=0){
        index = teacherBox->findText(t);
        if(index!=-1){
            teacherBox->setCurrentIndex(index);
            teacherInitial = teacherBox->currentText();
        }
    }
    okButton->setDisabled(true);
}

void EditActivityDialog::disableTopBoxes(){
    ui->roomBox->setDisabled(true);
    ui->hourBox->setDisabled(true);
    ui->dayBox->setDisabled(true);
}
QString EditActivityDialog::getClass(){
    return classBox->currentText();
}

QString EditActivityDialog::getGroup(){
    return groupBox->currentText();
}

QString EditActivityDialog::getTeacher(){
    return teacherBox->currentText();
}
int EditActivityDialog::getDay(){
    return ui->dayBox->currentIndex();
}

int EditActivityDialog::getHour(){
    return ui->hourBox->currentIndex();
}

QString EditActivityDialog::getRoom(){
    return roomBox->currentText();
}

void EditActivityDialog::enableOkButton(){
    int h = ui->hourBox->currentIndex();
    int d = ui->dayBox->currentIndex();
    QString r = roomBox->currentText();
    QString c = getClass();
    QString g = getGroup();
    QString t = getTeacher();
    if(((h != -1) && (d != -1) && (r != "") && (c != "") && (g != "") && (t != "")) && (c != classInitial || g != groupInitial || t != teacherInitial)){
        okButton->setEnabled(true);
    }
}
void EditActivityDialog::disableRestoreButton(){
    ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)->setDisabled(true);
}
