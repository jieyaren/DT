#include "user_define_msg.h"
#include "define.h"


struct MSG_STRING
{
	UINT32 msg;
	const char * msg_name;
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

const MSG_STRING llc_list_info[] =
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







//
// 用户定义消息导出接口
//
//
#define IS_X_STRING(pids,x) ((pids) == std::string ((#x)))
#define IS_LLC(pids) IS_X_STRING(pids,LLC)
#define IS_MM(pids) IS_X_STRING(pids,MM)
#define IS_SAME_CONST_CHAR_STAR(x,y) ( (x) == std::string ((y)))
//比较尴尬。如果类型是string 就需要手动转qstringlist 或者都转成string 要是都是const char*就需要都转成string

UINT32 getMsgCode(std::string pids, std::string  iterms)
{
	if (IS_LLC(pids))
		for (auto v : llc_list_info)
		{
			if (IS_SAME_CONST_CHAR_STAR(v.msg_name,iterms))
				return v.msg;
		}
	else if (IS_MM(pids))
	{}

	return -1;
}



QStringList getMsgList(std::string pids)
{
	QStringList CommandcomBoxList;
	if (IS_LLC(pids))
	{
		for (auto v : llc_list_info)
			CommandcomBoxList << v.msg_name;
	}
	else if (IS_MM(pids))
	{
	}
	return CommandcomBoxList;
}