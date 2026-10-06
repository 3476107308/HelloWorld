#ifndef TELEMETRYRECORD_H
#define TELEMETRYRECORD_H

#include <QDateTime>
#include <QString>

struct TelemetryRecord
{
    QDateTime ts_;
    QString id_;
    double temperature_ = 0.0;
    double voltage_ = 0.0;
};

#endif // TELEMETRYRECORD_H
