#include "activitymodel.h"

ActivityModel::ActivityModel() : QAbstractTableModel(){}

int ActivityModel::rowCount(const QModelIndex &/*parent*/) const{
    return 9;
}

int ActivityModel::columnCount(const QModelIndex &/*parent*/) const{
    return 5;
}

QVariant ActivityModel::data(const QModelIndex &index, int role) const{
    if(index.isValid() && (role == Qt::DisplayRole or role == Qt::EditRole)){
        for(int i=0; i < activities[activeRoom].size(); i++){
            if(activities[activeRoom][i].day == index.column() && activities[activeRoom][i].slot == index.row()){
                if(role == Qt::DisplayRole)
                    return activities[activeRoom][i].group;
                if(role == Qt::EditRole){
                    QVariant a;
                    a.setValue(activities[activeRoom][i]);
                    return a;
                }
            }
        }
    }
    return QVariant();
}

QVariant ActivityModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if (role == Qt::DisplayRole){
        if(orientation == Qt::Horizontal)
            return days[section];
        else
            return hours[section];
    }
    return QVariant();
}

bool ActivityModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(index.isValid() && role == Qt::EditRole) {
        Activity actValue = value.value<Activity>();
        int i = activityExists(actValue.day, actValue.slot, actValue.room);
        if(i  != -1){
            activities[actValue.room][i] = actValue;
        }
        else{
            activities[actValue.room].append(actValue);
        }
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
        return true;
    }
    return false;
}

bool ActivityModel::removeData(const QModelIndex &index, QString room, int role){
    // Removes data by index
    if(index.isValid() && role == Qt::EditRole){
        int i = activityExists(index.column(),index.row(), room);
        if(i != -1){
            activities[room].removeAt(i);
            emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
            return true;
        }
    }
    return false;
}

Qt::ItemFlags ActivityModel::flags(const QModelIndex &index) const{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index);
}

int ActivityModel::activityExists(int day, int slot, QString room){
    // Checks if activity exists and returns it index in QList if it does
    for(int i=0; i<activities[room].size(); i++){
        if(activities[room][i].day == day && activities[room][i].slot == slot)
            return i;
    }
    return -1;
}

void ActivityModel::changeActiveRoom(QString room){
    emit beginResetModel();
    activeRoom = room;
    emit endResetModel();
}

QList<Activity> ActivityModel::getAllActivities(){
    QList<Activity> result;
    QList<Activity> curList;
    for(QMap<QString, QList<Activity>>::const_iterator it = activities.cbegin(), end=activities.cend(); it != end; ++it){
        curList = it.value();
        for(int i = 0; i < curList.size(); i++){
            result << curList[i];
        }
    }
    return result;
}

void ActivityModel::setJsonArray(QJsonArray array){
    beginResetModel();
    activities.clear();
    QJsonObject tmp;
    for(int i=0; i < array.size(); i++){
        tmp = array[i].toObject();
        activities[tmp["room"].toString()].append( Activity(tmp));
    }
    endResetModel();
}

QJsonObject Activity::toJsonObject(){
    QJsonObject res;
    res["day"] = day;
    res["slot"] = slot;
    res["teacher"] = teacher;
    res["group"] = group;
    res["class"] = aclass;
    res["room"] = room;
    return res;
}

Activity::Activity(QJsonObject &obj){
   day = obj["day"].toInt();
   slot = obj["slot"].toInt();
   room = obj["room"].toString();
   group = obj["group"].toString();
   teacher = obj["teacher"].toString();
   aclass = obj["class"].toString();
}


