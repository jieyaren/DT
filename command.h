#ifndef COMMAND_H
#define COMMAND_H
#include "index.h"
#include "define.h"
#include <QDebug>
#include <QJsonObject>
#include <QjsonArray>
struct Command
{
	QJsonArray json;
	tsc_pid pid;
	quint32 msg_code;
	Command() {}
};

class CommandExecSession : public QObject
{
	
	Q_OBJECT

	Command command[index::value];
	//QVector<Command> command ;
	void exec_command(int ind);
	static CommandExecSession *s;
	CommandExecSession(QObject *parent = 0) : QObject(parent) {}

signals:
	void udp_send_signal(const DebugStruct&);
public:
	~CommandExecSession() { delete s; }
	void Exec(const tsc_pid& pid,const quint32 msg_code,const QJsonArray& msgjson)
	{
		int i = index::instance().alloc();
		if (i < index::value)
		{
			command[i].msg_code = msg_code;
			command[i].pid = pid;
			command[i].json = msgjson;
			exec_command(i);
		}
		else
			qWarning("CommandExeSession is Full! Please Wait ...  ");
	}

	static CommandExecSession* instance()
	{
		if (s == nullptr)
			s = new CommandExecSession;
		return s;
	}
};


#endif // COMMAND_H
