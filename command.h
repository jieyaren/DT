#ifndef COMMAND_H
#define COMMAND_H
#include "index.h"
#include "define.h"

#include <QDebug>
#include <QtEndian>

class CommandExecSession : public QObject
{
	Q_OBJECT
	DebugStruct command[index::value];

	static CommandExecSession *s;
	int current_handle;
	CommandExecSession(QObject *parent = 0) : QObject(parent),current_handle(0) {}
	void emit_command()
	{
		emit udp_send_signal(command[current_handle]);
	}

signals:
	void udp_send_signal(const DebugStruct&);
public:
	~CommandExecSession() { delete s; }
    void Exec(const tsc_pid& pid,const quint32 msg_code,
			quint16 datalen=100, size_t CurrentCount = 1,size_t AllCount=1)
    {
		if (command[current_handle].CurrentCount == 0)
		{
			current_handle = index::instance().alloc();
		}
		
		command[current_handle].MsgType = qToBigEndian<quint32>(msg_code);
		command[current_handle].ReceiverPid = pid;
		command[current_handle].DataLen = qToBigEndian<quint16> (datalen);
		command[current_handle].Handle = qToBigEndian<quint32>(current_handle);
		command[current_handle].CurrentCount = CurrentCount;
		command[current_handle].MsgCount = AllCount;
		qInfo() << "[CMD]"
			<< " PID " << pid
			<< " MsgCode " << msg_code
			<< " Handle " << current_handle
			<< " MsgCount " << AllCount
			<< " CurrentCount " << CurrentCount
			<< " DataLen " << datalen;
		emit_command();
		command[current_handle].CurrentCount--;

    }
    quint8 * command_msg_ptr()
    {
        return command[current_handle].MsgBody;
    }
	quint16& msg_len()
	{
		return command[current_handle].DataLen;
	}


	static CommandExecSession* instance()
	{
		if (s == nullptr)
			s = new CommandExecSession;
		return s;
	}
};


#endif // COMMAND_H
