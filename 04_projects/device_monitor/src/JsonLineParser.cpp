#include "JsonLineParser.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

JsonLineParser::JsonLineParser(QObject* parent):QObject(parent){}

void JsonLineParser::appendData(const QByteArray& chunk)
{
    buffer_ += chunk;

    int pos;
    while((pos = buffer_.indexOf('\n')) >= 0)
    {
        QByteArray line = buffer_.left(pos);
        buffer_.remove(0,pos + 1);
        parseLine(line);
    }
    if(buffer_.size() > kMaxBuffer)
    {
        emit parseFailed(QStringLiteral("单行超限（>100KB），已丢弃并重新同步"));
        buffer_.clear();
    }
}

void JsonLineParser::parseLine(const QByteArray& line)
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