#include "device.h"
#include "DeviceListModel.h"

DeviceListModel::DeviceListModel(QVector<Device>& devices,QObject* parent):QAbstractListModel(parent),devices_(devices)
{}

int DeviceListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return devices_.size();
}

QVariant DeviceListModel::data(const QModelIndex& index,int role)const
{
    if(!index.isValid())
        return {};
    switch(role)
    {
    case Qt::DisplayRole:
        return QStringLiteral("%1  温度%2")
            .arg(devices_[index.row()].name_)
            .arg(devices_[index.row()].temperature_);
    case NameRole:
        return devices_[index.row()].name_;
    case TemperatureRole:
        return devices_[index.row()].temperature_;
    case VoltageRole:
        return devices_[index.row()].voltage_;
    case OnlineRole:
        return devices_[index.row()].online_;
    default:
        return {};
    }
}

void DeviceListModel::reload()
{
    beginResetModel();
    endResetModel();
}

void DeviceListModel::deviceDataChanged(int row)
{
    emit dataChanged(index(row,0),index(row,0));
}