#include "log.h"
#include <QString>
#include <QDate>

logger * logger::s = nullptr;


void logger::log_with_timestamp(QtMsgType type,const QString & msg)
{
    QString type_string;
    switch (type) {
    case QtDebugMsg:
        type_string= "[DEBUG]";
        break;
    case QtInfoMsg:
        type_string= "[INFO ]";
        break;
    case QtWarningMsg:
        type_string= "[WARN ]";
        break;
    case QtCriticalMsg:
        type_string= "[CRITC]";
        break;
    case QtFatalMsg:
        type_string= "[FATAL]";
        break;
    }
    QString newLog = QDateTime::currentDateTime().toString(QLatin1String("MM-dd hh:mm:ss:zzz"))
           + type_string + QLatin1Char(' ') + msg;
    emit log_signnal(newLog);
}

void myMessageOutput(QtMsgType type, const QMessageLogContext & , const QString &msg)
{
    logger::instance()->log_with_timestamp(type,msg);
}
