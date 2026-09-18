#ifndef DEVICE_H
#define DEVICE_H

#include <QString>

struct Device{
    QString id_;
    QString name_;
    bool online_ = false;
    double temperature_ = 0.0;
    double voltage_ = 0.0;
    bool alarm_ = false;
};

#endif // DEVICE_H