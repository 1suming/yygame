#ifndef __TRACTORGameClientMessage_h__
#define __TRACTORGameClientMessage_h__


#include "TRACTORMessageHead.h"
#include <string>
#include "HNBaseType.h"

namespace TRACTOR
{
	/************************************************************************/
	/* �ͻ��˶������Ϣ�ṹ�����ṩ�ͻ���ʹ��                                  */
	/************************************************************************/
	enum 
	{
		UD_VF_CALL			=		0x01,  //���Ը�ע
		UD_VF_RAISE			=		0x02,  //���Լ�ע
		UD_VF_BET			=		0x04,  //������ע
		UD_VF_FOLD			=		0x08,  //��������
		UD_VF_CHECK			=		0x10,  //���Թ���
		UD_VF_ALLIN			=		0x20,  //����ȫ��
		UD_VF_CHECKMANDATE	=		0x40,  //���Թ����й�
		UD_VF_CALLMANDATE		=	0x80,  //���Ը�ע�й�
		UD_VF_CALLANYMANDATE	=	0x100, //���Ը��κ�ע�й�
		UD_VF_PASSABANDONMANDATE=	0x200, //���Թ���/�����й�

		C_VF_FOLLOW  =  0x01,  //can follow
		C_VF_LOOK    =  0x02,  //can look
		C_VF_ADD     =  0x04,  //can add
		C_VF_COMPARE =  0x08,  //can compare
		C_VF_GIVEUP  =  0x10,  //can give up
	};


	// Audio effect
	static const  char*  GOLD_BACKGROUND_MUSIC		=	"tractor/sound/bgm.wav";
	static const  char*  GOLD_SEND_POKER			=	"tractor/sound/sendpoker.wav";
	static const  char*  GOLD_RECYCLE_CHIP			=	"tractor/sound/recyclechips.wav";		
	static const  char*  GOLD_ADD_LADY				=	"tractor/sound/add_lady.wav";
	static const  char*  GOLD_ADD_MAN				=	"tractor/sound/add_man.wav";		
	static const  char*  GOLD_FOLLOW_LADY			=	"tractor/sound/follow_lady.wav";
	static const  char*  GOLD_FOLLOW_MAN			=	"tractor/sound/follow_man.wav";		
	static const  char*  GOLD_GIVEUP_LADY			=	"tractor/sound/giveup_lady.wav";
	static const  char*  GOLD_GIVEUP_MAN			=	"tractor/sound/giveup_man.wav";		
	static const  char*  GOLD_LOOK_LADY				=	"tractor/sound/look_lady.wav";
	static const  char*  GOLD_LOOK_MAN				=	"tractor/sound/look_man.wav";		
	static const  char*  GOLD_PK_LADY				=	"tractor/sound/pk_lady.wav";
	static const  char*  GOLD_PK_MAN				=	"tractor/sound/pk_man.wav";
	static const  char*  GOLD_PK_THUNDER			=	"tractor/sound/pk_thunder.wav";
	static const  char*  GOLD_PK_WIN				=	"tractor/sound/pk_win.wav";
	static const  char*  GOLD_PK_BGM				=	"tractor/sound/pk_bgm.wav";
	static const  char*  GOLD_OVER					=	"tractor/sound/gameover.wav";
	static const  char*  GOLD_WIN					=	"tractor/sound/gamewin.wav";
	static const  char*  GOLD_WARNING				=	"tractor/sound/warning.wav";


	struct PlayerInfo
	{

	};

	//game result data node.
	struct TGameResult
	{
		std::string szName;
		LLONG llMoney;			//��Ӯ�Ľ��
		LLONG i64Money;			//����ǰ��ҵ�ǰ���ϵĽ��
		BYTE szType;
		BYTE viewSeatNo;
		TGameResult()
			:llMoney(0)
		{

		}
	};

	//���������Ϣ
	struct THandCard
	{
		BYTE bySeatNo;
		BYTE byCards[3];

		THandCard()
		{
			bySeatNo = INVALID_DESKNO;
			memset(byCards,0,sizeof(byCards));
		}
	};

	struct TableInfo
	{
		INT                 iOffset;
		BYTE                byDeskNo;                       //���ӱ��
		BYTE                byMeSeatNo;                     //�ҵ���λ��
		bool                bAutoCreate;                    //�Զ�����������
		BYTE                byGameStation;                  //��Ϸ״̬
		BYTE                byWaitTime;                     //׼���ȴ�ʱ��
		BYTE				iThinkTime;	       		        //��Ϸ˼��ʱ��
		BYTE				iBeginTime;				        //��Ϸ��ʼʱ��
		BYTE				iAllCardCount;			        //�˿���Ŀ
		INT					iRoomBasePoint;			        //���䱶��
		INT					iRunPublish;				        //���ܿ۷�
		BYTE                bEXFlag1[12];                   //zht 2011-4-12, ��ʹ�õ��ֶΣ�ռλ��
		LLONG				iLimitNote;				        //��ע�ⶥֵ   ��ע / ��ע
		LLONG				iThisTurnLimit;				    //������ע   ��ע / ��ע
		INT					iBaseNote;				        //��ע	       ��ע
		INT                 iGuoDi;                         //����
		LLONG               iLimtePerNote;                  //��ע���ޣ���ע��Ҫ * 2
		INT                bShowIdOrPlay;                  //��ʾ��һ���id
		INT                 iFrontNote;                     //ǰλ�����Ч��ע��
		bool                bPlayer[PLAY_COUNT];            //�Ƿ���һ���;�����ս��(�Ƿ������Ϸ������Ҫͨ�����״̬�ж�)
		BYTE				iUserCardCount[PLAY_COUNT];		//�û����ϵ�����
		BYTE				iUserCard[PLAY_COUNT][5];		//�û����ϵ���
		bool                bSuperUser[PLAY_COUNT];         //�Ƿ񳬼����
		LLONG               iPlayerNote[PLAY_COUNT];         //�ܹ���ע����
		LLONG               iLastNote[PLAY_COUNT];		        //��һ����ע����
		LLONG               iTotalNote;                     //����ע
		bool                bOpenLose[PLAY_COUNT];          //��ұ����Ƿ�����
		bool                bMing[PLAY_COUNT];              //����״̬
		bool                bIsGiveUp[PLAY_COUNT];          //����Ƿ����
		BYTE                byWin[PLAY_COUNT];
		CHAR                strCity[PLAY_COUNT][50];        //���ڳ���
		CHAR                strUserID[PLAY_COUNT][50];      //���ID
		bool                bFirstGame;                     //�Ƿ��һ����ע
		BYTE                byNtPeople;                     //ׯ��
		BYTE                byOutCardPeople;                //���ڳ����û�
		BYTE                byFirstOutPeople;               //��һ�����Ƶ����
		tagUserResult       tag;
		BYTE                byTimeOutCount;                 //��ʱ����
		BYTE                byComLoseStation;               //���Ƹ���
		INT                 iNoteTimes;                     //��ע��������ע����һȦ���ܱ��ƣ�
	};

}
#endif // __TRACTORGameClientMessage_h__