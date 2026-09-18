#include "TcpTransport.h"

TcpTransport::TcpTransport(QObject* parent):Transport(parent){}

bool TcpTransport::open()
{
    return false;
}
void TcpTransport::close()
{

}

bool TcpTransport::isConnected()const
{
    return false;
}

void TcpTransport::sendByte(const QByteArray& chunk)
{

}