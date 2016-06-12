#ifndef __DriftingCar_MessageHead_H__
#define __DriftingCar_MessageHead_H__

#include "HNNetExport.h"

namespace DriftingCar
{
	//��ϷID
	enum Game_ID
	{
		NAME_ID = 31010100
	};

	//��Ϸָ��ID
	enum CMD_ID
	{
		ASS_GAME_FREE			=	53,	  //������Ϣ
		ASS_BEGIN_BET			=	54,	  //��ʼ��ע
		ASS_GAME_SEND			=	55,	  //��ʼ����
		ASS_UPDATE_BET			=	56,	  //������ע
		ASS_APPLY_NT			=	57,	  //������ׯ��Ϣ
		ASS_APPLY_RESULT		=	58,	  //������ׯ�����Ϣ
		ASS_APPLY_LEAVE			=	59,	  //������ׯ��Ϣ
		ASS_LEAVE_RESULT		=	60,	  //������ׯ�����Ϣ
		ASS_USER_BET			=	61,	  //�����ע��Ϣ
		ASS_BET_RESULT			=	62,	  //�����ע�����Ϣ
		ASS_GAME_OPEN           =   63,   //����
		ASS_SUPER_RESULT		=	64,	  //�����û����ƽ����Ϣ��
	
		ASS_SUPER_USER			=	65,	  //�����û���Ϣ
		ASS_PREPARE_END			=	66,	  //��Ϸ׼���׶��û��˳�
		ASS_WAITE_NT		    =   67,   //��ׯ�ȴ���Ϣ
		ASS_SHOW_ROBOTBET       =   68,   //��ʾ��������ע��Ϣ
	};

	//��Ϸ״̬
	enum Game_State
	{
		GS_WAIT_SETGAME			=	0,	 //�ȴ���������״̬
		GS_WAIT_ARGEE			=	1,	 //�ȴ�ͬ������
		GS_FREE_STATUS			=	20,	 //��Ϸ���н׶�
		GS_BET_STATUS			=	21,	 //��Ϸ��ע�׶�
		GS_SEND_STATUS			=	22,	 //���ƽ׶�
		GS_WAIT_NEXT			=	23,	 //�ȴ���һ�̿�ʼ�׶�
	};

	enum Game_Const
	{
		PLAY_COUNT				=	180, //��Ϸ����
		SEND_COUNT				=	32,	 //��������
		BET_ARES				=	8,	 //��ע������
		BET_STYLE				=	6,	 //��������
		HISTORY_COUNT			=   72,  //��ʷ��¼
		TIME_COUNT				=   100, //�����ʱ��
		CAR_LOGO_COUNT			=   32,  //��������
		NON_NT					=   255, //û��������ׯ
	};


//	//��ע��������
//	static	const	int		G_iArePeiLv[BET_ARES] = {40,30,20,10,5,5,5,5};
//	//��������ֵ
//	static	const	int		G_iChouMaValue[BETSTYLE] = {100,1000,10000,100000,1000000,10000000};

	#pragma pack(1)

	//������ׯ��Ϣ
	struct	ApplyNtMsg
	{
		BYTE	byDeskStation;		//������λ��
		ApplyNtMsg()
		{
			memset(this,-1,sizeof(ApplyNtMsg));

		}
	};
	//�����û����ƵĽ��
	struct	SuperUserControlReslut
	{
		BYTE	bSuperDeskStation;	//�����û���λ��
		BYTE    bySuperReslut;		//�����û����ƵĽ��
		
		SuperUserControlReslut()
		{
			memset(this,-1,sizeof(SuperUserControlReslut));

		}
	};
	//�����û���ͷ��˷��ͽ��
	struct	SuperUserControlStart
	{
		BYTE	byDeskStation;		//�����û���λ��
		BYTE    bySuperStart;		//�����û����Ƶ����
		SuperUserControlStart()
		{
			memset(this,-1,sizeof(SuperUserControlStart));

		}
	};

	//׼���׶��˳��ṹ
	struct GamePrepareStageQuit
	{
		BYTE				byNtStation;				//ׯ��λ��
		BYTE				byNtWaitList[PLAY_COUNT];	//ׯ���б�
		BYTE                byNtcount;					//��ׯ����
		LLONG             i64NtScore;					//ׯ�ҳɼ�
		GamePrepareStageQuit()
		{
			memset(this,0,sizeof(GamePrepareStageQuit));
			memset(byNtWaitList,255,sizeof(byNtWaitList));
			byNtStation=255;
		}
	};
//-------------------------------------------------------------------------------
//��ׯ�ȴ�
struct S_C_WaiteNtMsg
{
	bool	bWait;
	S_C_WaiteNtMsg()
	{
		memset(this,0,sizeof(S_C_WaiteNtMsg));
	}
};
	//-------------------------------------------------------------------------------
	//��ʼ��ע��Ϣ��
	struct BeginBetMsg
	{
		BYTE	byNtStation;	            //ׯ��λ��
		BYTE    byNtCount;                  //ׯ����ׯ����
		BYTE    byGameStation;              //��Ϸ״̬
		BYTE    byNtWaitList[PLAY_COUNT];   //ׯ���б�
		BYTE	byCardStyle[SEND_COUNT];    //������

		LLONG   i64NtScore;                 //ׯ�ҳɼ�
		BeginBetMsg()
		{
			memset(this,0,sizeof(BeginBetMsg));
			memset(byNtWaitList,255,sizeof(byNtWaitList));
			memset(byCardStyle,-1,sizeof(byCardStyle));
			byNtStation=255;
		
		}
	};
	//������Ϣ��(��ά���飬0��ʾ��ҵģ�1��ʾׯ�ҵ�)
	struct BeginAward
	{
		BYTE	  bDeskStation;       //��λ��
		BYTE	  byLuziList[72];     //·��
		LLONG     i64PlayerGrade[2];  //��ҳɼ�
		LLONG     i64PlayCoin[2];     //�����Ӯ���
		LLONG     i64ReturnMoney[2];  //�����Ľ����
		BeginAward()
		{
			memset(this,0,sizeof(BeginAward));
			memset(byLuziList,255,sizeof(byLuziList));
		}
	};
	//������Ϣ��
	struct BeginSendCardMsg
	{
		BYTE byGameStation;					//��Ϸ״̬
		BYTE byWinPrize;					//����λ�� ��ʱ�� С������С���󱼳ۣ�С������ڣ�С
		BeginSendCardMsg()
		{
			memset(this,0,sizeof(BeginSendCardMsg));
			byWinPrize=-1;

		}
	};
	//������ʷ��ע
	struct TUpdateHistoryBet
	{
		LLONG   i64AreaBetAllSum[BET_ARES];//�����������ע
		LLONG	i64UserBetSum[BET_ARES];   //����ڸ����������ע
		LLONG	i64SumAllBet;//����ע��
		TUpdateHistoryBet()
		{
			memset(this,0,sizeof(i64AreaBetAllSum));
		}
	};
	//������Ϣ��
	struct BeginFreeMsg
	{
		bool	bFlag;						//���б�ʶ
		BYTE	byGameStation ;				//��ǰ����Ϸ״̬
		BYTE	byFreeTime;					//����ʱ��
		BYTE    byNtStation;				//ׯ��λ��
		BYTE    byPlayStation;				//���λ��
		BYTE    byNtcount;					//ׯ����ׯ����
		BYTE    byWaitNtList[PLAY_COUNT];	//��ׯ�б�
		LLONG   i64NtScore;					//ׯ�ҳɼ�
		BeginFreeMsg()
		{
			memset(this,0,sizeof(BeginFreeMsg));
			byNtStation = 255;
			memset(byWaitNtList, 255, sizeof(byWaitNtList));
		}
	};
	//������ׯ�����Ϣ
	struct	ApplyNtResultMsg
	{
		bool    bNtFlag;							//��û��ׯ�ҵ��������ׯ�ı�־
		bool	bApplay;							//�Ƿ���Ϊ���û�дﵽ��ׯ��Ҷ���ׯ���ɹ�
		BYTE	byDeskStation;						//������λ��
		BYTE	byWaitNtList[PLAY_COUNT];			//������ׯ�б�
		BYTE	byNtStation;						//ׯ�ҵ�λ��
		BYTE    byNtcount;							//ׯ����ׯ����
		BYTE    bGameStation;
		LLONG   i64NtScore;							//ׯ�ҳɼ�
		ApplyNtResultMsg()
		{
			memset(this,-1,sizeof(ApplyNtResultMsg));
			memset(byWaitNtList,255,sizeof(byWaitNtList));
			byNtStation=255;
			bApplay=false;
			bNtFlag=false;
		}
	};
	//������ׯ��Ϣ��
	struct	ApplyLiveNtMsg
	{
		BYTE	byDeskStation;						//������λ��
		ApplyLiveNtMsg()
		{
			memset(this,-1,sizeof(ApplyLiveNtMsg));
		}
	};
	//������ׯ�����Ϣ��
	struct	ApplyLiveNtResultMsg
	{
		BYTE    byNtStation;                //ׯ��λ��
		BYTE	byDeskStation;		        //������λ��
		BYTE    byWaitNtList[PLAY_COUNT];	//��ׯ�б�
		BYTE    byNtcount;                  //ׯ����ׯ����
		LLONG   i64NtScore;                 //ׯ�ҳɼ�
		ApplyLiveNtResultMsg()
		{
			memset(this,-1,sizeof(ApplyLiveNtResultMsg));
			memset(byWaitNtList,255,sizeof(byWaitNtList));
			byNtStation=255;
			byDeskStation=255;
			byNtcount=0;
			i64NtScore=0;
		}
	};

	//�����ע
	struct	UserBetMsg
	{
		BYTE	byDeskStation;		//��ע��λ��
		BYTE	byBetArea;			//��ע����
		BYTE	byChouMaType;		//��ע��������
		UserBetMsg()
		{
			memset(this,0,sizeof(UserBetMsg));
		}
	};
	//��Ϸ״̬�������ݰ�
	struct	GameStation_Base
	{
		BYTE		bGameStation;		           //��Ϸ״̬
		BYTE		byFreeTime;					   //����ʱ��
		BYTE		byBetTime;			           //��עʱ��
		BYTE		bySendTime;	                   //����ʱ��
		BYTE		byRemaindTime;		           //ʣ��ʱ��
		BYTE		byNtStation;			       //ׯ��λ��
		BYTE		byMaxNtPeople;		           //�����ׯ����

		BYTE		byLuziList[72];		           //·�ӣ�������¼��
		BYTE		byWaitNtList[PLAY_COUNT];	   //��ׯ�б�
		int			iNtPlayCount;		           //ׯ����ׯ����
		int			iApplyNtLimite;		           //��ׯ����
		LLONG		i64NtGrade;		               //ׯ�ҳɼ�
		LLONG       i64PlayerGrade;			       //�e�ҳɿ�
		LLONG       i64AreaBetLimite;       	   //�����ע����
		GameStation_Base()
		{
			memset(this,0, sizeof(GameStation_Base));
		}
	};
	//-------------------------------------------------------------------------------
	//��ע���(����ע)
	struct	UserBetResultMsg
	{
		bool		bDynamicBet;		           //�Ƿ�̬��ע
		BYTE		byBetDenied;				   //��ע״̬ 0��ʾ�ɹ� 1��ʾׯ�Ҳ�����2��ʾ�Լ�������3��ʾ��ע����
		BYTE	    byDeskStation;		           //��ע��λ��
		BYTE		byBetArea;			           //��ע����
		BYTE		byChouMaType;		           //��ע��������
		BYTE	    byBetNumFlag;		           //��ע100W��500W����500W�ı�־
		LLONG       i64SumAllBet;                  //����ע
		LLONG       i64UserBetCount[BET_ARES];     //�����ÿ���������ע���
		LLONG		i64AreaBetAllSum[BET_ARES];	   //�����������ע
		UserBetResultMsg()
		{
			memset(this,0,sizeof(UserBetResultMsg));
		}
	};
	///��Ϸ����ṹ
	struct GameTaskStruct
	{
		BYTE     byBackCardType ; 
		BYTE     byTaskType ; 
		BYTE     bySpecifyShape ; 
		BYTE     bySpecifyCard ;
		BYTE     byBackCardMutiple ;  ///���Ʊ���

		GameTaskStruct()
		{
			memset(this , 0 , sizeof(GameTaskStruct)) ; 
		}
		void IniData(void)
		{
			memset(this , 0 ,sizeof(GameTaskStruct)) ; 
		}

	};

	#pragma	pack()
}


/********************************************************************************/
#endif
