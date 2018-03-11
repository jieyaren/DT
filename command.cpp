#include "command.h"
#include "udp.h"
#include <QtEndian>

CommandExecSession * CommandExecSession::s = nullptr;

void CommandExecSession::exec_command(int ind)
{
   //拼消息
   if(Udp::instance()->is_ipvalid())
   {
		qDebug()<<"["<<ind<<"]"<<" ip="<<Udp::instance()->ip()<<" exec command";
		DebugStruct msg;
		msg.Handle = ind;
		msg.MsgType = command[ind].msg_code;
		msg.ReceiverPid = command[ind].pid;
		qDebug()
			<<"["<<msg.Handle<<"]"
			<<"PID "<<msg.ReceiverPid
			<<"msgcode "<<msg.MsgType;

		if (!command[ind].json.isEmpty())
		{
			auto buffer = msg.MsgBody;
			for (auto obj : command[ind].json)
			{	
				if (obj.toObject().isEmpty())
					continue;

				auto value = obj.toObject().take("value").toString().toInt();
				auto value_bytes = obj.toObject().take("bytes").toString().toInt();
				switch (value_bytes)
				{
				case sizeof quint32:
					value = qToBigEndian<quint32>(value);
					break;
				case sizeof quint16:
					value = qToBigEndian<quint16>(value);
				case sizeof quint8:
				default:
					break;
				}
				memcpy(buffer, &value, value_bytes);
				buffer += value_bytes;
				msg.DataLen += value_bytes;
			}
		}
	
		emit udp_send_signal(msg);
   }
   else
   {
	   qDebug()<<"[%4.4X] ip Invalid"<<Udp::instance()->ip()<<" Please Check it";
   }

}
