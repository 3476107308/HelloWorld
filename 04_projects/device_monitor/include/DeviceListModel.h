#ifndef DEVICELISTMODEL_H
#define DEVICELISTMODEL_H

#include <QAbstractListModel>
#include "device.h"
#include <QVector>

class DeviceListModel: public QAbstractListModel
{
    Q_OBJECT
public:
    enum DeviceRoles{
        NameRole = Qt::UserRole + 1,
        TemperatureRole = Qt::UserRole + 2,
        VoltageRole = Qt::UserRole + 3,
        OnlineRole = Qt::UserRole + 4
    };
    explicit DeviceListModel(QVector<Device>& devices,QObject* parent = nullptr);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index,int role = Qt::DisplayRole) const override;
    void reload();
    void deviceDataChanged(int row);

private:
    QVector<Device>& devices_;

};

#endif // DEVICELISTMODEL_H
