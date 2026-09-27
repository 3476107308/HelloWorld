#ifndef CONNECTIONCONFIG_H
#define CONNECTIONCONFIG_H

#include <QString>

struct ConnectionConfig
{
    QString host = QStringLiteral("127.0.0.1");
    quint16 port = 8888;
};

#endif // CONNECTIONCONFIG_H
