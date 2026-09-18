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
private:
    QTcpSocket* socket_ = nullptr;
};
#endif // TCPTRANSPORT_H
