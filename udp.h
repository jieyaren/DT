#ifndef UDP_H
#define UDP_H

#include <QHostAddress>
#include <QString>
#include <QObject>
#include <QUdpSocket>
#include "define.h"
#include "index.h"
#include "command.h"

class Udp : public QObject
{
	Q_OBJECT
	QHostAddress ipinfo;
	QUdpSocket *udpsocket_r;
	QUdpSocket *udpsocket_t;
	static Udp * s;
	Udp(QObject *parent = 0) 
		: QObject(parent),
		udpsocket_r(new QUdpSocket(this)),
		udpsocket_t(new QUdpSocket(this))
	{
		udpsocket_r->bind(QHostAddress::LocalHost,LOCAL_PORT);
		udpsocket_t->bind(LOCAL_PORT);
		connect(udpsocket_r,SIGNAL(readyRead()),this,SLOT(on_UdpMessage_received()));
		connect(CommandExecSession::instance(), SIGNAL(udp_send_signal(DebugStruct)), this, SLOT(sendMessage(DebugStruct)));
	}
private slots:
	void on_UdpMessage_received();

public slots:
	void sendMessage(const DebugStruct& msg);
public:
	~Udp()
	{
		delete udpsocket_r;
		delete udpsocket_t;
	}
	static Udp* instance()
	{
		 if (s==nullptr)
			 s = new Udp;
		 return s;
	}
	bool setIP(const QString& s);
	bool is_ipvalid() { return !ipinfo.isNull();}
	QString ip() {return ipinfo.toString();}
	
};

#endif // UDP_H
