#include "user_define_msg.h"
#include "define.h"


struct MSG_STRING
{
	UINT32 msg;
	const char * msg_name;
};

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define VALUE_STRING_PAIR(v) {v, #v}


// LLC������Ϣ��
typedef enum LLC_Remote_Debug_Enum
{
	RE_DBG_Query_Channel_LINK_LOG = 0x00000001,  //��ѯ�ŵ���·�仯��¼		
	RE_DBG_Query_Channel_USE_LOG = 0x00000002,  //��ѯ�ŵ�ʹ�ü�¼
	RE_DBG_Query_QUEUE_STATE_LOG = 0x00000003,  //��ѯ�ŵ�����״̬
	RE_DBG_Query_LLC_SYS_PARAM = 0x00000004,  //��ѯLLCȫ�ֲ���
	RE_DBG_Query_GPS_INFO = 0x00000005,  //��ѯGPS���ض�����ϸ��Ϣ
	RE_DBG_Query_ADJ_Info = 0x00000006,  //��ѯ��վ��Ϣ
	RE_DBG_SET_Print_Flag = 0x00000007,  //����LLC��ӡ����
	RE_DBG_SET_Fun_Cfg = 0x00000008,  //����LLC��������

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
// �û�������Ϣ�����ӿ�
//
//
#define IS_X_STRING(pids,x) ((pids) == std::string ((#x)))
#define IS_LLC(pids) IS_X_STRING(pids,LLC)
#define IS_MM(pids) IS_X_STRING(pids,MM)
#define IS_SAME_CONST_CHAR_STAR(x,y) ( (x) == std::string ((y)))
//�Ƚ����Ρ����������string ����Ҫ�ֶ�תqstringlist ���߶�ת��string Ҫ�Ƕ���const char*����Ҫ��ת��string

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