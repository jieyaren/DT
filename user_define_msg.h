#ifndef USER_DEFINE_MSG_H
#define USER_DEFINE_MSG_H

#include <tuple>
#include <string>
#include <list>
#include <QStringList>
#include <QString>
#include <QtGlobal>
typedef quint32 UINT32;
typedef quint16 UINT16;
typedef quint8  UINT8;
UINT32 getMsgCode(std::string pids, std::string  iterms);
QStringList getMsgList(std::string pids);


//-------------------------------------------------------------------------------------------
//用户定义结构体 开始
#pragma pack(push,1)

//LLC的GPS周期上拉表中的用户上拉信息结构体
typedef struct  LLC_GPSPOLL_MSParam_Stru
{
	UINT32    MSAddress;
	UINT32    MsgHandle;
	UINT8     AB;
	UINT8     GPS_Type; //上拉的GPS 数据格式，GPSU/GPS2U/GPS3U
	UINT8	  Ms_Priority;
	UINT32    Begin_Time;
	UINT32    PolledTime_Last;     // 上次发送的时间单位,DPOLL信令发送前,PolledTime_Last = PolledTime_Curr
	UINT32    PolledTime_Curr;     // 本次发送的时间单位,DPOLL信令发送后,PolledTime_Curr = 系统发送的时间
	//动态调整GPS订阅周期功能2018/3/19
	UINT32    Subscribe_Cycle;     // 用户原始订阅周期
	UINT8     Speed_Status;        //  0:静止 1:慢速 2:快速  0xFF:未知
	UINT32    Status_Count;        // 状态累计值
}LLC_GPSPOLL_MSParam_Stru;
#define GPSPOLL_UINTTIME_USERCOUNT_MAX     480      // 单位周期中，每个元素包含用户数量最大值，（由最大周期决定）
//LLC的GPS周期上拉表的信息结构体
typedef struct  LLC_GPSPOLL_CycleTime_Stru
{
	UINT32                      PollTime_Cycle;                                  // 用户上拉周期
	UINT32						SavedCount;										 // 已存相同周期终端数
	UINT32                      MSMAX_Count;                                     // 最大用户数,最多GPSPOLL_UINTTIME_USERCOUNT_MAX
	UINT32                      MSPOLLed_Index;                                  // 应该发送DPOLL信令的用户索引,单个节点内的控制
	LLC_GPSPOLL_MSParam_Stru    MS_GPSPOLL[GPSPOLL_UINTTIME_USERCOUNT_MAX];      // 保存这个时间单位内的用户信息

}LLC_GPSPOLL_CycleTime_Stru;


//用户定义结构体 结束
//-------------------------------------------------------------------------------------------
#pragma pack(pop)

#endif // USER_DEFINE_MSG_H
