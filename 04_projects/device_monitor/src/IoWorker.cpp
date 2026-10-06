#include "IoWorker.h"
#include "TcpTransport.h"
#include "Transport.h"

#include <QDebug>
#include <QThread>

IoWorker::IoWorker()
{

}

void IoWorker::start()
{
    qDebug() << "[IoWorker::start] thread =" << QThread::currentThread();
    transport_ = new TcpTransport(this);
    connect(transport_,&Transport::receiveByte,this,[this](const QByteArray& data){
        emit dataReceived(data);
    });
    connect(transport_,&Transport::openSucceeded,this,[this](){
        emit connectionOpened();
    });
    connect(transport_,&Transport::openError,this,[this](const QString& reason){
        emit connectionFailed(reason);
    });
    connect(transport_,&Transport::closed,this,[this](){
        emit connectionClosed();
    });
    connect(transport_,&Transport::sendFailed,this,[this](const QString& reason){
        emit sendFailed(reason);
    });
}

void IoWorker::openConnection(const ConnectionConfig& cfg)
{
    transport_->setHost(cfg.host);
    transport_->setPort(cfg.port);
    transport_->open();
}

void IoWorker::closeConnection()
{
    transport_->close();
}

void IoWorker::sendData(const QByteArray& data)
{
    transport_->sendByte(data);
}