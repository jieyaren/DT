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

		qDebug()<<datagram.data();
		index::instance().free(msg->Handle);
	}
}

void Udp::sendMessage(const DebugStruct& msg)
{	
	DebugStruct tmp(msg);
	auto datalen = qToBigEndian<quint16>(tmp.DataLen);
	//大于2000要分片
	auto len = datalen > 2000 ? MSG_BODY_LENGTH : datalen;

	if (datalen > 2000)
	{
		tmp.DataLen = qToBigEndian<quint16>(MSG_BODY_LENGTH);
	}

	udpsocket_t->writeDatagram((const char *)(&tmp), len, ipinfo, TSC_ExMAC_SignalRecvPort);

	qInfo() << "[UDP] sending ip " << ipinfo.toString()
		<< " datalen " << datalen
		<< " send time " << msg.CurrentCount;

}