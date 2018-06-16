#include "user_define_msg.h"
#include "define.h"


struct MSG_STRING
{
	MSG_STRING(const UINT32&  Msg, const char * Msg_name)
		:msg(Msg),msg_name(std::string (Msg_name))
	{}

	bool is_same_msg(const std::string& Msg)
	{
		return Msg == msg_name;
	}

	UINT32 msg;
	std::string msg_name;
};

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define VALUE_STRING_PAIR(v) {v, #v}


// LLC定义消息码
typedef enum LLC_Remote_Debug_Enum
{
	RE_DBG_Query_Channel_LINK_LOG = 0x00000001,  //查询信道链路变化记录		
	RE_DBG_Query_Channel_USE_LOG = 0x00000002,  //查询信道使用记录
	RE_DBG_Query_QUEUE_STATE_LOG = 0x00000003,  //查询信道队列状态
	RE_DBG_Query_LLC_SYS_PARAM = 0x00000004,  //查询LLC全局参数
	RE_DBG_Query_GPS_INFO = 0x00000005,  //查询GPS辅控订阅详细信息
	RE_DBG_Query_ADJ_Info = 0x00000006,  //查询临站信息
	RE_DBG_SET_Print_Flag = 0x00000007,  //设置LLC打印开关
	RE_DBG_SET_Fun_Cfg = 0x00000008,  //设置LLC功能配置

}LLC_Remote_Debug_Enum;

static const MSG_STRING llc_list_info[] =
{
	VALUE_STRING_PAIR(RE_DBG_Query_Channel_LINK_LOG),
	VALUE_STRING_PAIR(RE_DBG_Query_Channel_USE_LOG),
	VALUE_STRING_PAIR(RE_DBG_Query_QUEUE_STATE_LOG),
	VALUE_STRING_PAIR(RE_DBG_Query_LLC_SYS_PARAM),
	VALUE_STRING_PAIR(RE_DBG_Query_GPS_INFO),
	VALUE_STRING_PAIR(RE_DBG_Query_ADJ_Info),
	VALUE_STRING_PAIR(RE_DBG_SET_Print_Flag),
	VALUE_STRING_PAIR(RE_DBG_SET_Fun_Cfg)
};


// 	"code" : "0x05000002",
// 	"name" : "SD_PRINT_Single_MS_GPS",
// 	"items" : [
// {
// 	"name":"MSI",
// 		"bytes" : "4",
// 		"value" : 0
// }]
// 
// "code":"0x05000003",
// "name" : "SD_Cancle_Single_MS_GPS",
// "items" : [
// 	{
// 		"name":"MSI",
// 
// 			"bytes" : "4",
// 			"value" : 0
// 	}]

// SD定义消息码
typedef enum SD_Remote_Debug_Enum
{
	RT_SD_PRINT_GPS				= 0x05000000,
	RT_SD_Cancel_NOT_MATCH_GPS	= 0x05000001,
	SD_PRINT_Single_MS_GPS		= 0x05000002,
	SD_Cancle_Single_MS_GPS		= 0x05000003,
} SD_Remote_Debug_Enum;


static const MSG_STRING sd_list_info[] =
{
	VALUE_STRING_PAIR(RT_SD_PRINT_GPS),
	VALUE_STRING_PAIR(RT_SD_Cancel_NOT_MATCH_GPS),
	VALUE_STRING_PAIR(SD_PRINT_Single_MS_GPS),
	VALUE_STRING_PAIR(SD_Cancle_Single_MS_GPS)
};



//
// 用户定义消息导出接口
//
//
#define IS_X_STRING(pids,x) ((pids) == std::string ((#x)))
#define IS_LLC(pids) IS_X_STRING(pids,LLC)
#define IS_MM(pids) IS_X_STRING(pids,MM)
#define IS_SD(pids) IS_X_STRING(pids,SD)

//根据消息码遍历命令列表项，返回消息码
UINT32 getMsgCode(std::string pids, std::string  iterms)
{
	if (IS_LLC(pids))
	{
		for (auto v : llc_list_info)
		{
			if (v.is_same_msg(iterms))
				return v.msg;
		}
	}
	else if (IS_MM(pids))
	{}
	else if (IS_SD(pids))
	{
		for (auto v : sd_list_info)
		{
			if (v.is_same_msg(iterms))
				return v.msg;
		}
	}
	return -1;
}


//获取更新的命令列表
QStringList getMsgList(std::string pids)
{
	QStringList CommandcomBoxList;
	if (IS_LLC(pids))
	{
		for (auto v : llc_list_info)
			CommandcomBoxList << v.msg_name.c_str();
	}
	else if (IS_SD(pids))
	{
		for (auto v : sd_list_info)
			CommandcomBoxList << v.msg_name.c_str();
	}
	else if (IS_MM(pids))
	{
	}
	return CommandcomBoxList;
}