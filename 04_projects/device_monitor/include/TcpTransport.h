#ifndef TCPTRANSPORT_H
#define TCPTRANSPORT_H

#include "Transport.h"

class QTcpSocket;

class TcpTransport: public Transport
{
    Q_OBJECT
public:
    explicit TcpTransport(QObject* parent = nullptr);
    bool open() override;
    void close() override;
    bool isConnected() const override;
    void sendByte(const QByteArray& chunk) override;
    void setHost(const QString& host);
    void setPort(quint16 port);
private:
    QTcpSocket* socket_ = nullptr;
    QString host_ = "127.0.0.1";
    quint16 port_ = 8888;

};
#endif // TCPTRANSPORT_H
