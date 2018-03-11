#ifndef DEFINE_H
#define DEFINE_H
#include <cstdlib>
#include "index.h"
#include <QDebug>

enum tsc_pid {VOS,LLC, CC, MM, RT, RM, VSW, SD, PD, BDBS = 32};

enum UDP_DATA_TYPE {
	REMOTE_DEBUG_REQ = 0x70,//TSC远程调试消息
	REMOTE_DEBUG_RSP = 0x71
};

//整个消息结构体要小于2000
#define MSG_BODY_LENGTH				(1985)

//DS-6210_TSC_概要设计_R6.5_TSC调试对外接口
//pack1
//} __attribute__ ((aligned (1)));
#pragma pack(push,1)
struct DebugStruct {
	quint8 DataType;
	quint32 Handle;
	quint32 MsgType;
	quint8 ReceiverPid;
	quint8 MsgCount;//总发送次数 默认1
	quint8 CurrentCount;//当前发送次数 1
	quint16 DataLen;// 数组字节长度默认0
	quint8 MsgBody[MSG_BODY_LENGTH];//消息内容
	DebugStruct()
		:DataType(REMOTE_DEBUG_REQ),
		Handle(index::value),
		MsgType(-1),
		ReceiverPid(LLC),
		MsgCount(1),
		CurrentCount(1),
		DataLen(0)
	{
		memset(&MsgBody, 0xff, sizeof(MsgBody));
	}

	DebugStruct(const DebugStruct& m)
	{
		DataType=m.DataType;
		Handle = m.Handle;
		MsgType = m.MsgType;
		ReceiverPid = m.ReceiverPid;
		MsgCount = m.MsgCount;//总发送次数 默认1
		CurrentCount = m.CurrentCount;//当前发送次数 忽略
		DataLen = m.DataLen;
		memcpy(&MsgBody, &m.MsgBody, sizeof MsgBody);
	}

	/*
	~DebugStruct()
	{
		HexDump((char *)MsgBody, MSG_BODY_LENGTH, int(MsgBody));
	}*/
	void HexDump(char *buf, int len, int addr) {
		int i, j, k;
		char binstr[80];

		for (i = 0; i < len; i++) {
			if (0 == (i % 16)) {
				sprintf(binstr, "%08x -", i + addr);
				sprintf(binstr, "%s %02x", binstr, (unsigned char)buf[i]);
			}
			else if (15 == (i % 16)) {
				sprintf(binstr, "%s %02x", binstr, (unsigned char)buf[i]);
				sprintf(binstr, "%s  ", binstr);
				for (j = i - 15; j <= i; j++) {
					sprintf(binstr, "%s%c", binstr, ('!' < buf[j] && buf[j] <= '~') ? buf[j] : '.');
				}
				//printf("%s\n", binstr);
				qDebug() << binstr;
			}
			else {
				sprintf(binstr, "%s %02x", binstr, (unsigned char)buf[i]);
			}
		}
		if (0 != (i % 16)) {
			k = 16 - (i % 16);
			for (j = 0; j < k; j++) {
				sprintf(binstr, "%s   ", binstr);
			}
			sprintf(binstr, "%s  ", binstr);
			k = 16 - k;
			for (j = i - k; j < i; j++) {
				sprintf(binstr, "%s%c", binstr, ('!' < buf[j] && buf[j] <= '~') ? buf[j] : '.');
			}
			//printf("%s\n", binstr);
			qDebug() << binstr;
		}
	}
};
#pragma pack(pop)

#define DEBUG_TOOL_VERSION "DT_0.0.00.001" "["__DATE__"_"__TIME__"]"
//接收端口 
#define TSC_ExMAC_SignalRecvPort	(5503)
//绑定端口 随便写的
#define LOCAL_PORT					(9000)

#endif // DEFINE_H