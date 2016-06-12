// ******************************************************************************************************
//  UpgradeMessage   version:  1.0   ��  date: 03/29/2011
//  --------------------------------------------------------------------------------------------------

//	�¿���˹�˿ˣ���Ϸ������-�ͻ���Э��\
//  NameID��10100008

//  --------------------------------------------------------------------------------------------------
//  Copyright (C) 2011 - All Rights Reserved
// ******************************************************************************************************
// 
// ******************************************************************************************************
#ifndef __DZPokerUpgradeMessage_h__
#define __DZPokerUpgradeMessage_h__

#include <cstring>
#include "HNBaseType.h"

namespace DZPoker
{

	enum DZPoker_COMMAND
	{
		UD_VF_CALL				=	0x01,  //���Ը�ע
		UD_VF_RAISE				=	0x02,  //���Լ�ע
		UD_VF_BET				=	0x04,  //������ע
		UD_VF_FOLD				=	0x08,  //��������
		UD_VF_CHECK				=	0x10,  //���Թ���
		UD_VF_ALLIN				=	0x20,  //����ȫ��
		UD_VF_CHECKMANDATE		=	0x40,  //���Թ����й�
		UD_VF_CALLMANDATE		=	0x80 , //���Ը�ע�й�
		UD_VF_CALLANYMANDATE	=	0x100, //���Ը��κ�ע�й�
		UD_VF_PASSABANDONMANDATE =	0x200, //���Թ���/�����й�

		MAX_ZORDER				=	500,
	};

	enum DZPoker_COMMAND1
	{
		//��Ϸ��Ϣ����
		//////////////////////////////////////////////////////////////////////////
		//�ļ����ֶ���
		GAME_NAME_ID			=		10100008	,	// ���� ID

		PLAY_COUNT				=		8			,	// ��Ϸ����

		PUBLIC_CARD_COUNT		=		5			,	// ��������
	
		MAX_NAME_INFO          =         256 ,

		//��Ϸ���ݳ������
		MAX_CARDS_NUM			=		216		,		// �ĸ��Ƶ���������Ҳ����֧����������
		MAX_HANDCARDS_NUM		=		52		,		// ���ϳ��е�����������
		MAX_510K_CARDS_NUM		=		48		,		// �ĸ���5/10/K����������
		MAX_FIRST_CARDS			=		1		,		// ��������
		MAX_DEAL_CARDS			=		2		,		// ��������
		MAX_THREE_CARDS			=		3		,		// ��������
		MAX_BACKCARDS_NUM		=		16		,		// ���ĵ�������

		TYPE_PLAY_GAME			=		10000	,		// �����
		SUPER_PLAYER            =       10      ,		// �������
		//******************************************************************************************
		//���ݰ���������ʶ
		//******************************************************************************************

		ASS_UG_USER_SET				=	50		,		//�û�������Ϸ
		ASS_UG_USER_SET_CHANGE		=	51	,			//������ø���

		ASS_GAME_BEGIN				=	120		,		//������Ϸ��ʼ
		ASS_SYS_DESK_CFG			=	121		,		//ȷ����ע�����Ϣ
		ASS_CFG_DESK				=	122		,		//������õ�ע
		ASS_DESK_CFG				=	123		,		//ȷ����ע�����Ϣ
		ASS_AGREE_DESK_CFG			=	124    ,         //ͬ���ע
		ASS_BET						=	125    ,         //�����ע��Ϣ
		ASS_SEND_A_CARD				=	130		,		//��1������Ϣ
		ASS_SEND_3_CARD				=	131		,		//��3������Ϣ
		ASS_SEND_4_5_CARD			=	132		,		//��4,5������Ϣ
		ASS_SEND_CARD_FINISH		=	133		,		//���ƽ���
		ASS_BETPOOL_UP				=	134		,		//�߳ظ���
		ASS_BET_REQ					=	135		,		//��ע����
		ASS_BET_INFO				=	136		,		//��ע��Ϣ
		ASS_NEW_USER				=	137		,		//��һ����Ҳ���
		ASS_PASS_USER				=	138		,		//���Ƶ���Ҳ���
		ASS_SUB_MONEY                =   139   ,          //����������
		ASS_COMPARE_CARD              =  140  ,           //������Ϣ
	
		ASS_CALLMANDATE				 =   141	,			//��ע�й�
		ASS_CHECKMANDATE             =   142    ,         //�����й�
		ASS_CALLANYMANDATE           =   143	,			//���κ�ע�й�
		ASS_PASSABANDONMANDATE      =    144    ,         //���ƻ������й�

		ASS_USER_LEFT_DESK         =     145    ,         //����뿪����

		ASS_SORT_OUT				=	170		,		//����ͼƬ

		ASS_TOKEN					=	186		,		//������Ϣ
		ASS_RESULT					=	191		,		//�غ����
		ASS_SHOW_IN_MONEY			=	192		,		//��ʾ������
		ASS_LOOK_CARD				=	193		,		//����
		ASS_CAN_SHOW_CARD			=	194		,		//��չʾ�Ƶ���Ϣ
		ASS_SHOW_CARD				=	195		,		//չʾ�Ƶ���Ϣ
	

		//�쳣����Ϣ
		ASS_NO_PLAYER				=	155		,		//û����ҽ�����Ϸ
		ASS_AGREE					=	156		,		//����Ƿ�ͬ�����Ϣ
		ASS_CFG_DESK_TIMEOUT		=	157		,		//���õ�ע��ʱ
		ASS_NOT_ENOUGH_MONEY		=	158		,		//��ҽ�Ҳ������������Ϣ
		ASS_MONEY_LACK				=	159		,		//��ҽ�Ҳ������ʾ
	
		ASS_NEXT_ROUND_REQ			=	150		,		//�����һ�غ�����
		ASS_NEW_ROUND_BEGIN   		=	151		,		//��һ�غϿ�ʼ

		//��������Ϣ
		ASS_FINISH_ROUND_REQ		=	180		,		//����������Ϸ
		S_C_IS_SUPER_SIG            =   181     ,		//�Ƿ��ǳ���
	};
	
	#define TAG_INIT(tagName) \
		tagName() { Clear();} \
		void Clear()

	//******************************************************************************************
	// ��Ϸ״̬����
	//******************************************************************************************
	enum
	{
		GS_WAIT_SETGAME			=		0,	//�ȴ���������״̬
		GS_WAIT_ARGEE			=		1,	//�ȴ�ͬ������
		GS_CONFIG_NOTE			=		20, //���õ�ע״̬
		GS_AGREE_NOTE			=		21,	//ͬ���ע״̬
		GS_PLAY_GAME			=		25,	//��Ϸ����״̬
		GS_WAIT_NEXT_ROUND		=		28,	//�ȴ���һ�غϽ���״̬
	};


	/******************************************************************************************
	*									�¿���˹�˿�״̬����									  *
	*******************************************************************************************/

	#define GS_USER_STAKE				30	// �����ע��״̬

	//******************************************************************************************
	// ��ע����
	enum emType
	{
		ET_UNKNOWN 		= 0,	//δ֪����
		ET_BET			= 1,	//��ע
		ET_CALL			= 2,	//��ע
		ET_RAISE 		= 3,	//��ע
		ET_CHECK		= 4,	//����
		ET_FOLD    		= 5,    //����
		ET_ALLIN		= 6,	//ȫ��
		ET_AUTO			= 7,    //�Զ���ע
	};

	// ��������
	enum emTypeCard
	{
		UNKNOWN			= 0,	//δ֪����
		SEND_A_CAND		= 1,	//�·�һ������
		SEND_3_CAND		= 2,	//�·� 3 ������
		SEND_4_5_CAND   = 3,	//�·� 4, 5 ������
	};

	// ����������
	enum emTypeMoney
	{
		NT_UNKNOWN	= 0,	//δ֪����
		NT_LOAD		= 1,	//ȷ��������
		NT_EXIT		= 2,    //�˳���Ϸ
		NT_ADD		= 3,    //���ӽ��
		NT_SUB		= 4,    //���ٽ��
	};

	// �й�״̬
	enum emToken
	{
		TO_UNKNOWN					= 0,	// δ֪����
		TO_CALLMANDATE				= 1,	// ��ע�йܰ�ť
		TO_CHECKMANDATE				= 2,    // �����йܰ�ť
		TO_CALLANYMANDATE			= 3,    // ���κ�ע��ť
		TO_PASSABANDONMANDAT		= 4,    // ����/�����йܰ�ť
	};

	#pragma pack(1)

	//֪ͨ������Ϸ��
	typedef struct tagTCfgDesk
	{
		//��ע���
		struct TDizhu
		{
			bool	bCanSet;		//��ҿ����Ƿ�������ã��·�ʱ��
			INT		iLowers[5];		//���5����ѡ�ĵ�עֵ���·�ʱ��
			bool    bSelects[5];	//��5��ֵ�Ŀ�ѡ����
			BYTE	bySelect;		//��Ҵ�iValuesѡ������һ����עֵ���Ϸ�ʱ��

			void Clear()
			{
				bCanSet = false;	//��ҿ����Ƿ��������
				memset(iLowers,0,sizeof(iLowers));		 //���������ѡ�ĵ�עֵ
				memset(bSelects,false,sizeof(bSelects)); //������ֵ�Ŀ�ѡ����
				bySelect = 255;	//��Ҵ�iValuesѡ������һ����עֵ
			}

		} dz;

		TAG_INIT(tagTCfgDesk)
		{
			dz.Clear();
		}
	} TCfgDesk;

	//�й���������
	typedef struct tagTDeskCfg
	{
		//��ע���
		struct TDizhu
		{
			LLONG	iLowers[5];						// ���5����ѡ�ĵ�עֵ
			LLONG	iUppers[5];						// ���5����עֵ
			LLONG	iRoomMultiple;                  // ���䱶��
			LLONG	iLower;							// ϵͳ�Զ����õĵ�עֵ
			LLONG	iUpper;							// ϵͳ�Զ����õĶ�עֵ
			LLONG	iSmallBlindNote;				// Сäע���õĵ�עֵ
			LLONG	iBigBlindNote;					// ��äע���õĵ�עֵ

			// ����������
			bool    bIsRoomSubMoney;                // �˷����Ƿ���Ҫ�����Ҳſ���
			LLONG	iSubMinMoney;                   // ��������ʹ�����
			LLONG	iSubMaxMoney;                   // ��������ߴ����� 
			LLONG	iSubPlanMoney;					// ��ǰ׼��������
			LLONG	iMinusMoney;					// �޸�ע��Ӽ����
			LLONG	iMinMoney;						// ��С��ע���
			LLONG	iUntenMoney;					// ���ڶ��ٽ��ʱ���� 

			bool iUserClose;						// һ�ֺ�10���ڽ����ǿ��
			INT iUserCloseTime;						// �رտͻ���ʱ��
			INT iUserInMoney;						// ��Ϸ������ʱ��

			emTypeMoney nType;						// ���������

			void Clear()
			{
				memset(iLowers, 0, sizeof(iLowers));	// ���5����ѡ�ĵ�עֵ
				memset(iUppers, 0, sizeof(iUppers));	// ���5����עֵ
				iLower = 0;							// ϵͳ�Զ����õĵ�עֵ
				iUpper = 0;							// ϵͳ�Զ����õĶ�עֵ
				iSmallBlindNote = 0;				// Сäע���õĵ�עֵ
				iBigBlindNote = 0;					// ��äע���õĵ�עֵ
				nType = NT_UNKNOWN;					// δ֪����
			}

		} dz;

		//ʱ�����
		struct TTime
		{
			BYTE   byOperate;    	 // ��Ҳ�����ʱ��

			void Clear()
			{
				byOperate = 0;	 	 // ��Ҳ�����ʱ��
			}
		} Time;

		//�������
		 struct TRule
		{
			INT  nPokeNum;			//ʹ�ö��ٷ��˿�
			void Clear()
			{
				nPokeNum = 1;		//ʹ�ö��ٷ��˿�
			}

		} Rule;

		//��������
		bool bShowUserInfo;			// �Ƿ���ʾ��Һ���ݣ������ڱ�����

		TAG_INIT(tagTDeskCfg)
		{
			dz.Clear();
			Time.Clear();
			Rule.Clear();
		}
	} TDeskCfg;

	//ĳ����Ƿ����ע����Ϣ��
	typedef struct tagTAgree 
	{
		BYTE byUser; 				// ��Ϣ���������
		bool bAgree; 				// �Ƿ�ͬ��
		bool bAllAgree; 			// ��ǰ�Ƿ�������Ҷ�ͬ����Ϸ

		TAG_INIT(tagTAgree)
		{
			byUser = 255;			// ��Ϣ���������
			bAgree = true;			// �Ƿ�ͬ��
			bAllAgree = false;		// ��ǰ�Ƿ�������Ҷ�ͬ����Ϸ
		}
	} TAgree;

	//������ע��
	typedef struct tagTBet
	{
		emType	nType;				// ��ע����
		BYTE	byUser;				// ��ע�����
		BYTE	bUserman[8];			// �������
		LLONG	nMoney;				// ���
		LLONG	nBetMoney;			// ��ע�������

		TAG_INIT(tagTBet)
		{ 
			nType = ET_UNKNOWN;		// ��ע����
			byUser = 255;			// ��ע�����
			//bUserman = 255;		// �������
			memset(bUserman, 0, sizeof(bUserman));
			nMoney = 0;				// ��ҵ�ѡ��
			nBetMoney = 0;			// ��ע�������
		}
	} TBet;

	// ������
	typedef struct tagTSubMoney
	{
		BYTE	bDeskStation;
		LLONG	nMoney[PLAY_COUNT];
		bool	bReady[PLAY_COUNT];       // ׼��״̬

		tagTSubMoney()
		{
			memset(nMoney, 0, sizeof(nMoney));
			memset(bReady, 0, sizeof(bReady));
			bDeskStation = 255;
		}
	} TSubMoney;


	// �������ݰ�
	typedef struct tagTCompareCard
	{
		INT  nCardKind[PLAY_COUNT];		 // �������
		BYTE bCards[PLAY_COUNT][5];		 // ���������ɵ�������
		BYTE bCardsEx[PLAY_COUNT][2];    // ʣ��2��û�õ���
		BYTE bHandCards[PLAY_COUNT][2];  // �������
		INT nCardCount;                  // ���������
		INT nHandCardCount[PLAY_COUNT];  // �����������

		tagTCompareCard()
		{
			memset(this, 0, sizeof(tagTCompareCard));
			memset(bHandCards, 255, sizeof(bHandCards));
		}
	} TCompareCard;

	//�����·������ݸ��ͻ��˵����ݰ�
	typedef struct tagTCards
	{
		emTypeCard		nTypeCard;					//��������
		BYTE   			byUser;						//���Ʒ��������
		BYTE			byCards[PLAY_COUNT][MAX_DEAL_CARDS];	//������
		BYTE			byPubCards[5];				//������������
		INT				iCardsNum;					//����������Ƶ�����

		TAG_INIT(tagTCards)
		{
			nTypeCard	= UNKNOWN;					//��������
			byUser		= 255;						//���Ʒ��������
			memset(byCards,0,sizeof(byCards));		//������
			iCardsNum = 0;							//��������Ƶ�����
		}
	} TCards;

	//���ư������������û����л������Ƶȵ�
	typedef struct tagTToken
	{
		BYTE	  byUser;									// �������
		BYTE	  byVerbFlag;								// ��������־
		INT		  iSelPoints[5];							// ��עֵѡ���
		emToken	  nemToken;									// �й�״̬����
		bool	  bNewTurn;                                 // �Ƿ�Ϊ�µ�һ�ֿ�ʼ
		bool	  bNewTurns;                                // �Ƿ�Ϊ�µ�һ�ֿ�ʼ
		INT		  nTurnNums;								// ��ǰ��Ϸ���Ȧ��
		LLONG   nCallMoney;                               // ��ǰ���Ը�ע�Ľ��
		INT       iPassedTime;                               //�Ѿ����˵�ʱ�䣨������

		TAG_INIT(tagTToken)
		{
			nemToken = TO_UNKNOWN;						// δ֪����
			byUser = 255;								// �������
			byVerbFlag = 0;								// ��������־
			bNewTurn = false;
			bNewTurns = false;
			nCallMoney = 0;
			nTurnNums = 0;
			::memset(iSelPoints,0,sizeof(iSelPoints));	// ��עֵѡ���
			iPassedTime = 0;
		}
	} TToken;

	// �����ע�߳ظ��°�
	typedef struct tagTBetPool
	{
		LLONG iBetPools[8];			//��ע�߳�

		TAG_INIT(tagTBetPool)
		{
			::memset(iBetPools, 0, sizeof(iBetPools));  //�߳س�ʼ��
		}
	} TBetPool;

	//����ṹ��
	typedef struct tagTResult
	{
		INT		nbyUserID[PLAY_COUNT];				//Ӯ��ID
		CHAR	szName[8][61];						//Ӯ���ǳ�
		LLONG	nBetPools[8];					//������ע��
		LLONG	nUserBet[PLAY_COUNT][8];		//ÿ�������ע����
		bool	bWinBetPools[PLAY_COUNT][8];		//Ӯ����ע��
		LLONG	nWinPoolsMoney[PLAY_COUNT][8];	//Ӯ����ע�ؽ��
		LLONG	nScore[PLAY_COUNT];     		//��Ӯ����
		LLONG	nMoney[PLAY_COUNT];     		//ʵ����Ӯ���
		LLONG	nMoneyEx[PLAY_COUNT];          //��Ӯ��� 
		LLONG	nSubMoney[PLAY_COUNT];  		//��ǰÿ����Ҵ�����
		LLONG	nSelfMoney[PLAY_COUNT]; 		//�Լ��Ľ�Ҹ���: �ͻ��˻�ȡ�Ľ���п��ܻ�û��ˢ��, ���������﷢��ȥ
		bool	bWin[PLAY_COUNT];       			//��¼Ӯ��
		bool	bExit;                  			//true:�����Ҫ�˳�, false:����Ҫ�˳�
		BYTE	bOrder[PLAY_COUNT];                //�������, �����������

		tagTResult()
		{
			::memset(this, 0, sizeof(tagTResult));
			::memset(nUserBet, 0, sizeof(nUserBet));
			::memset(nMoneyEx, 0, sizeof(nMoneyEx));
			::memset(bOrder, 255, sizeof(bOrder)); 
		}
	} TResult;

	//�µ�һ�غ������
	typedef struct tagTNextRoundReq
	{
		enum emType
		{
			UNKNOWN		= 0,	//δ֪����
			READY		= 1,	//׼����
			EXIT		= 2,    //���˳���Ϸ
		};

		emType nType;			//���������

		TAG_INIT(tagTNextRoundReq)
		{
			nType = UNKNOWN;	//δ֪����
		}
	} TNextRoundReq;

	//�µ�һ�غ���Ϣ��
	typedef struct tagTNextRoundBeginInfo
	{
		BYTE byNTUser;		//ׯ��λ��
		BYTE bSmallBlind;	//Сäע
		BYTE bBigBlind;		//��äע

		TAG_INIT(tagTNextRoundBeginInfo)
		{
			byNTUser = 255;
			bSmallBlind = 255;
			bBigBlind = 255;
		}
	} TNextRoundBeginInfo;

	//û�����������֪ͨ
	typedef struct tagTNoPlayer
	{
		bool bGameFinished;	//������ɢ��

		TAG_INIT(tagTNoPlayer)
		{
			bGameFinished = false;	//������ɢ��
		}
	} TNoPlayer;

	// ��ҽ���
	typedef struct tagTMoneyLack
	{
		INT nbyUser;			// �������

		TAG_INIT(tagTMoneyLack)
		{
			nbyUser = 255;
		}
	} TMoneyLack;

	/******************************************************************************************
	*																						  *
	*******************************************************************************************/
	//��Ϸ״̬���Ļ���
	typedef struct tagTGSBase 
	{
		/*-GSBase��������-*/
		INT  iEvPassTime;			//��ǰ�¼������ĵ�ʱ�䣨�룩
		INT	 iVerbTime;				//��Ϸ�и�������˼��ʱ��
		bool bBoy[PLAY_COUNT];  	//����Ա�
		bool bHaveUser[PLAY_COUNT]; //���ϸ�λ����û�����
		BYTE bGameStatus;			//��Ϸ״̬

		/*-�����������-*/
		tagTDeskCfg tagDeskCfg;

		TAG_INIT(tagTGSBase)
		{
			::memset(this, 0, sizeof(tagTGSBase));
		}
	} TGSBase;

	//��Ϸ״̬�����ȴ�ͬ��/δ��ʼʱ
	typedef struct tagTGSWaitAgree : public TGSBase 
	{
		bool	bReady[PLAY_COUNT];               //�Ƿ�׼��
		LLONG	nSubMoney[PLAY_COUNT];            //ÿ����Ҵ���Ľ��

		TAG_INIT(tagTGSWaitAgree)
		{
			::memset(this, 0, sizeof(tagTGSWaitAgree));
		}
	} TGSWaitAgree;

	//��Ϸ״̬������Ϸ��
	typedef struct tagTGSPlaying : public TGSBase 
	{
		BYTE  byHandCard[PLAY_COUNT][2];	    //������ϵ��˿�
		INT   nHandCardNums;		            //��������˿���Ŀ

		BYTE  byCards[5];                       //�����Ϲ�����
		INT   nCardsCount;                      //���湫������
		INT   nID[5];							//�����ť
		LLONG   nBetMoney[PLAY_COUNT][8];     //��ע���
		bool  bNoBet[PLAY_COUNT];               //��¼������ע�����λ��
		bool  bGiveUp[PLAY_COUNT];              //��¼���������λ��

		BYTE  byTokenUser;		                //�õ����Ƶ��û�
		BYTE  byNTUser;					        //ׯ��
		BYTE  bySmallBlind;                     //Сä��
		BYTE  byBigBlind;                       //������
		LLONG	  nCallMoney;					//��ע���

		BYTE  byVerbFlag;                       //��������
		bool  bNewTurn;                         //�µ�һ�ֲ���
		bool  bNewTurns;                        //�µ�һ�ֲ���

		LLONG   nBetPools[8];			        //��ע�߳ؽ��
		LLONG   nSubMoney[PLAY_COUNT];        //ÿ����Ҵ���Ľ��

		INT   iUserStation ;

		TAG_INIT(tagTGSPlaying)
		{
			::memset(this,0,sizeof(tagTGSPlaying));
		}
	} TGSPlaying;


	//��Ϸ״̬�����ȴ���һ�غ�
	typedef struct tagTGSWaitNextRound : public TGSBase 
	{
		LLONG   nSubMoney[PLAY_COUNT];            //ÿ����Ҵ���Ľ��

		TAG_INIT(tagTGSWaitNextRound)
		{
			::memset(this, 0, sizeof(tagTGSWaitNextRound));
		}
	} TGSWaitNextRound;


	typedef struct tagUserLeftDeskStruct
	{
		BYTE    bDeskStation; 
		bool    bClearLogo ;
	} UserLeftDeskStruct;


	//����
	typedef struct tagUserLookCardStruct
	{
		BYTE    bDeskStation; 

		tagUserLookCardStruct()
		{
			bDeskStation = 255;
		}
	} UserLookCardStruct;

	//չʾ��
	typedef struct tagShowCardStruct
	{
		BYTE	byDeskStation;
		BYTE    byCardList[2]; 
		INT		iCardCount;

		tagShowCardStruct()
		{
			::memset(this,0,sizeof(tagShowCardStruct));
			byDeskStation = 255;
		}
	} ShowCardStruct;

	#pragma pack()
}

#endif // __DZPokerUpgradeMessage_h__
