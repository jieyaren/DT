#ifndef LOG_H
#define LOG_H
#include <QObject>
#include <QMessageLogContext>
//实现日志重定向
//类不能起名叫log 有个函数重名了。。。气死。找了半天连接错误
class logger: public QObject
{
	Q_OBJECT
public:
	~logger() { delete s; }
	static logger *s;
	static logger *instance()
	{
		if (s==nullptr)
			s = new logger;
		return s;
	}

public:
	void log_with_timestamp(QtMsgType type,const QString & msg);

signals:
	void log_signnal(const QString &);

private:
	logger(QObject *parent = 0):
		QObject(parent){}
};

void myMessageOutput(QtMsgType type, const QMessageLogContext & , const QString &msg);

#endif // LOG_H
