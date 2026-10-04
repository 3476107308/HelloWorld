#ifndef IOWORKER_H
#define IOWORKER_H

#include <QString>
#include <QObject>
#include <QByteArray>

#include "ConnectionConfig.h"

class TcpTransport;


class IoWorker: public QObject
{
    Q_OBJECT
public:
    explicit IoWorker();

public slots:
    void start();
    void openConnection(const ConnectionConfig& cfg);
    void closeConnection();
    void sendData(const QByteArray& data);

signals:
    void dataReceived(const QByteArray& data);
    void connectionOpened();
    void connectionFailed(const QString& reason);
    void connectionClosed();
    void sendFailed(const QString& reason);

private:
    TcpTransport* transport_ = nullptr;
};

#endif // IOWORKER_H
