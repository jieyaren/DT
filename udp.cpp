#include "define.h"
#include "udp.h"
#include <QByteArray>
#include <QtEndian>
Udp * Udp::s = nullptr;

bool Udp::setIP(const QString& s)
{
	return ipinfo.setAddress(s);
}

void Udp::on_UdpMessage_received()
{
	qDebug()<<"[UDP] Received Message!";
	while (udpsocket_r->hasPendingDatagrams()) {
		QByteArray datagram;
		datagram.resize(udpsocket_r->pendingDatagramSize());
		udpsocket_r->readDatagram(datagram.data(), datagram.size());

		DebugStruct *msg =(DebugStruct*) datagram.data();

		qInfo()<<"["<< QString(msg->Handle).toInt(nullptr, 16)<<"]"
				<<"PID=["<< msg->ReceiverPid <<"]"
			<< "msgcode=[" << QString(msg->MsgType).toInt(nullptr, 16) << "]";
//if json exist
// json it
//else
		qDebug()<<datagram.data();
		index::instance().free(msg->Handle);
	}
}

void Udp::sendMessage(const DebugStruct& msg)
{
	quint8 t = msg.CurrentCount;
	DebugStruct tmp(msg);

	tmp.Handle = qToBigEndian<quint32>(tmp.Handle);
	tmp.DataLen = qToBigEndian<quint16>(tmp.DataLen);
	tmp.MsgType = qToBigEndian<quint32>(tmp.MsgType);

	while (t--)
	{
		tmp.CurrentCount = t;
		udpsocket_t->writeDatagram((const char *)(&tmp),50, ipinfo, TSC_ExMAC_SignalRecvPort);
		qDebug() << "udp meg len" << tmp.DataLen;
		qDebug() << "[UDP] Datagram sending to ip " << ipinfo;
	}	
}