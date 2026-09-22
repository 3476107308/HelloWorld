#include "TcpTransport.h"
#include <QTcpSocket>

TcpTransport::TcpTransport(QObject* parent):Transport(parent)
{
    socket_ = new QTcpSocket(this);
    connect(socket_,&QTcpSocket::connected,this,[this](){
        emit openSucceeded();
    });
    connect(socket_,&QTcpSocket::disconnected,this,[this](){
        emit closed();
    });
    connect(socket_,&QTcpSocket::errorOccurred,this,[this]{emit openError(socket_->errorString());});
    connect(socket_,&QTcpSocket::readyRead,this,[this]{emit receiveByte(socket_->readAll());});

}

bool TcpTransport::open()
{
    if(socket_->state() != QAbstractSocket::UnconnectedState)
        return false;

    socket_->connectToHost(host_,port_);
    return true;
}
void TcpTransport::close()
{
    socket_->disconnectFromHost();
}

bool TcpTransport::isConnected()const
{
    return socket_->state() == QAbstractSocket::ConnectedState;
}

void TcpTransport::sendByte(const QByteArray& chunk)
{
    if(!isConnected())
    {
        emit sendFailed(QStringLiteral("尚未连接，无法发送"));
        return;
    }
    socket_->write(chunk);
}

void TcpTransport::setHost(const QString& host)
{
    host_ = host;
}

void TcpTransport::setPort(quint16 port)
{
    port_ = port;
}
