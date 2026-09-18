#include "MessageParser.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

MessageParser::MessageParser(QObject* parent):QObject(parent){}

void MessageParser::appendData(const QByteArray& chunk)
{
    buffer_ += chunk;

    int pos;
    while((pos = buffer_.indexOf('\n')) >= 0)
    {
        QByteArray line = buffer_.left(pos);
        buffer_.remove(0,pos + 1);
        parseLine(line);
    }
}

void MessageParser::parseLine(const QByteArray& line)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(line,&err);

    if(err.error != QJsonParseError::NoError) {
        emit parseFailed(err.errorString());
        return;
    }
    if(!doc.isObject()) {
        emit parseFailed(QStringLiteral("not a json object"));
        return;
    }
    emit messageReceived(doc.object());
}