#ifndef ACTIVITYMODEL_H
#define ACTIVITYMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QMap>
#include <QJsonObject>
#include <QJsonArray>

struct Activity{
    Activity(){}
    Activity(int d,int s,QString g, QString c, QString t, QString r)
        : day{d}, slot{s}, group{g}, aclass{c}, teacher{t}, room{r}{}
    Activity(QJsonObject&);
    int day;
    int slot;
    QString group;
    QString aclass;
    QString teacher;
    QString room;
    QJsonObject toJsonObject();
};

Q_DECLARE_METATYPE(Activity)

class ActivityModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ActivityModel();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool removeData(const QModelIndex&, QString room, int role);
    void changeActiveRoom(QString room);
    QString getActiveRoom();
    QList<Activity> getAllActivities();
    void setJsonArray(QJsonArray);
private:
    QString activeRoom;
    QMap<QString, QList<Activity>> activities;
    QStringList days = {"Mon","Tue","Wed","Th","Fri"};
    QStringList hours = {"8:00-8:45","8:55-9:40","9:50-11:35",
                         "11:55-12:40","12:50-13:35","13:45-14:30",
                         "14:40-15:25","15:35-16:20","16:30-17:15"};

    int activityExists(int day, int slot, QString room);
};

#endif // ACTIVITYMODEL_H
