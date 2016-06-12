#ifndef _GoldenFlower_Upgrade_Message_H__
#define _GoldenFlower_Upgrade_Message_H__

#include "HNBaseType.h"
#include <string>

namespace goldenflower
{
	//��Ϸ��Ϣ
	enum GoldenFlower_COMMAND
	{
		GAME_NAME_ID  =  12101106,	    // ���� ID
		GAME_KIND_ID  =  2,				// ���� ID
	};

	

	enum 
	{
		//��Ϸ��Ϣ
		PLAY_COUNT					=	6,								//��Ϸ����
		MAX_CARD_COUNT              =   3,
		E_CHOUMA_COUNT              =   4,

		//��Ϸ״̬����
		GS_WAIT_SETGAME				=	0,				//�ȴ���������״̬
		GS_WAIT_ARGEE				=	1,				//�ȴ�ͬ������
		GS_SEND_CARD				=	20,				//����״̬
		GS_PLAY_GAME				=	21,				//��Ϸ��״̬
		GS_WAIT_NEXT				=	23,				//�ȴ���һ�̿�ʼ 		
		
		
		TYPE_OPEN_LOSE              =    0x06,            //����ʧ��
		TYPE_OPEN_WIN               =    0x07,            //����ʧ��
		TYPE_REACH_LIMITE_NEXT      =    0x08,            //��һ����ע��ﵽ����
		TYPE_REACH_LIMITE_SELF      =    0x09,            //�Լ���ע��ﵽ����
		TYPE_COMPARE_CARD           =    0x09,           //���Ʋ���

		//��ע����
		STATE_ERR					=	0x00,			//����״̬                                                      
		STATE_NORMAL				=	0x01,			//����״̬
		STATE_LOOK					=	0x02,			//������״̬
		STATE_WAITE_OPEN			=	0x03,			//��ע���� �ȴ�����
		STATE_GIVE_UP				=	0x04,			//����״̬
		INVALID_SEAT_NO				=   255,		    //��Ч�����Ӻ�


		ACTION_LOOK					=	0x00,			//����
		ACTION_FOLLOW				=	0x01,			//��ע
		ACTION_ADD					=	0x02,			//��ע
		ACTION_OPEN					=	0x03,			//����
		ACTION_GIVEUP				=	0x04,			//����
		ACTION_WIN					=	0x05,			//Ӯ��
		ACTION_LOST					=	0x06,			//����
		ACTION_NO					=	0x07,			//��

		
		SH_THREE                    = 7, //����
		SH_SAME_HUA_CONTINUE        = 6, //ͬ��˳
		SH_SAME_HUA                 = 5, //ͬ��
		SH_CONTIUE                  = 4, //˳��
		SH_DOUBLE                   = 3, //����
		SH_SPECIAL                  = 1, //����235
		SH_OTHER                    = 2, //����
		SH_ERROR                    = 0, //����
		
		UD_VF_CALL				    =	 0x01,  //���Ը�ע
		UD_VF_RAISE				    =	 0x02,  //���Լ�ע
		UD_VF_BET				    =	 0x04,  //������ע
		UD_VF_FOLD				    =	 0x08,  //��������
		UD_VF_CHECK				    =	 0x10,  //���Թ���
		UD_VF_ALLIN				    =	 0x20,  //����ȫ��
		UD_VF_CHECKMANDATE		    =	 0x40,  //���Թ����й�
		UD_VF_CALLMANDATE		    =	 0x80,  //���Ը�ע�й�
		UD_VF_CALLANYMANDATE	    =	 0x100, //���Ը��κ�ע�й�
		UD_VF_PASSABANDONMANDATE    =	 0x200, //���Թ���/�����й�
		
		C_VF_FOLLOW   = 0x01,  //can follow
		C_VF_LOOK     = 0x02,  //can look
		C_VF_ADD      = 0x04,  //can add
		C_VF_COMPARE  = 0x08,  //can compare
		C_VF_GIVEUP   = 0x10,  //can give up
	};
	
	// Audio effect
	static const char*	GOLD_ADD_LADY			=	"goldenflower/sound/add_lady.wav";
	static const char*	GOLD_ADD_MAN			=	"goldenflower/sound/add_man.wav";
	
	static const char*	GOLD_FOLLOW_LADY		=	"goldenflower/sound/follow_lady.wav";
	static const char*	GOLD_FOLLOW_MAN			=	"goldenflower/sound/follow_man.wav";
	
	static const char*	GOLD_GIVEUP_LADY		=	"goldenflower/sound/giveup_lady.wav";
	static const char*	GOLD_GIVEUP_MAN			=	"goldenflower/sound/giveup_man.wav";
	
	static const char*	GOLD_LOOK_LADY			=	"goldenflower/sound/look_lady.wav";
	static const char*	GOLD_LOOK_MAN			=	"goldenflower/sound/look_man.wav";
	
	static const char*	GOLD_PK_LADY			=	"goldenflower/sound/pk_lady.wav";
	static const char*	GOLD_PK_MAN				=	"goldenflower/sound/pk_man.wav";
	
	static const char*	GOLD_PK_THUNDER			=	"goldenflower/sound/pk_thunder.wav";
	static const char*	GAME_FAPAI				=	"goldenflower/sound/fapai.mp3";
	static const char*	GAME_SHOUCHOUMA			=    "goldenflower/sound/shouchouma.mp3";
	static const char*	GAME_XIAZHU				=	"goldenflower/sound/xiazhu.mp3";


	
	/********************************************************************************/

	//��Ϸ���ݰ�

	/********************************************************************************/

	//�����û����
	#pragma pack(1)
	
	/*------------------------S��C����Ҫ�õ�������------------------------------------*/
	struct	TGameBaseData
	{
		/*����������ļ����ò���*/
		BYTE				m_byThinkTime;			//��Ϸ˼��ʱ��
		BYTE				m_byBeginTime;			//��Ϸ��ʼʱ��	

		int					m_iGuoDi;				//����ֵ	
		int					m_iDiZhu;				//��ע     
		LLONG				m_i64DingZhu;			//��ע--����ע���ﵽ���ֵ ��ǿ�ƿ���		
//		LLONG				m_i64ShangXian;			//������ע����(�� ���ÿ�������ע�����ܳ������ֵ)  ��ע�ģ�����ʾΪ��ע��
		int					m_iBaseRatio;			//���䱶��

		/*ÿ����Ϸ����/��ʼ��Ҫ���õ�����*/
		BYTE				m_byNtPeople;           //ׯ��λ��
		BYTE				m_byCurrHandleDesk;		//��ǰ�������û�
		bool				m_bCanLook;				//�Ƿ���Կ���	
		bool				m_bCanFollow;			//�Ƿ���Ը�ע	
		bool				m_bCanAdd[E_CHOUMA_COUNT];				//�Ƿ���Լ�ע	
		bool				m_bCanOpen;				//�Ƿ���Ա���	
		bool				m_bCanGiveUp;			//�Ƿ��������	

		int					m_iUserState[PLAY_COUNT];	//��¼�û�״̬	

		LLONG				m_i64ZongXiaZhu;				//��ע	
		LLONG				m_i64XiaZhuData[PLAY_COUNT];	//�����ע����
	//	LLONG				m_i64GuessXiaZhu[PLAY_COUNT];	//��ҵ���ע

		BYTE				m_byUserCard[PLAY_COUNT][MAX_CARD_COUNT];	//�û����ϵ��˿�
		BYTE				m_byUserCardCount[PLAY_COUNT];				//�û������˿���Ŀ

		bool				m_bFirstNote;                       //�Ƿ�Ϊ��һ����ע
		int					m_iAddNoteTime;							//��ע�������������ͻ����ж��Ƿ���Ա���
		LLONG				m_i64CurrZhuBase;					//��ǰ��ע�Ļ���


		TGameBaseData()
		{
			InitAllData();
		}

		void	InitAllData()
		{
			m_byThinkTime	= 15;			
			m_byBeginTime	= 15;		

			m_iGuoDi	= 100;				
			m_iDiZhu	= 100;			
			m_i64DingZhu= 1000;					
			//m_i64ShangXian=10000;	

			m_byNtPeople = 255;

			InitSomeData();
		}

		/*ÿ����Ϸ����/��ʼ��Ҫ���õ�����*/
		void	InitSomeData()
		{
			m_byCurrHandleDesk	= 255;		
			m_bCanLook	= false;				
			m_bCanFollow= false;			
		//	m_bCanAdd	= false;				
			m_bCanOpen	= false;				
			m_bCanGiveUp= false;

			memset(m_bCanAdd,0,sizeof(m_bCanAdd));

			memset(m_iUserState,STATE_ERR,sizeof(m_iUserState));
			m_i64ZongXiaZhu = 0;	
			memset(m_i64XiaZhuData,0,sizeof(m_i64XiaZhuData));
			//memset(m_i64GuessXiaZhu,0,sizeof(m_i64GuessXiaZhu));

			memset(m_byUserCard,0,sizeof(m_byUserCard));
			memset(m_byUserCardCount,0,sizeof(m_byUserCardCount));

			m_bFirstNote	= true;
			m_iAddNoteTime	= 0;							
			m_i64CurrZhuBase= 0;					
		}
	};
	/*--------------------------------------------------------------------------------*/
	/********************************************************************************/
	//��Ϸ���ݰ�
	/********************************************************************************/
	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/

	/*-----------------------------------------------------------------------------*/
	//����������Ϸ״̬���ݰ��� �ȴ�������ҿ�ʼ ��
	struct S_C_GameStation_WaiteAgree
	{
		BYTE		byThinkTime;			//��Ϸ˼��ʱ��
		BYTE		byBeginTime;			//��Ϸ��ʼʱ��

		bool		bAgree[PLAY_COUNT];		//���׼��״̬

		int			iGuoDi;					//����ֵ	
		int			iDiZhu;					//��ע      

		LLONG		i64ShangXian;			//������ע����	��ע
		LLONG		i64DingZhu;				//��ע

		S_C_GameStation_WaiteAgree()
		{
			memset(this,0,sizeof(S_C_GameStation_WaiteAgree));
		}
	};
	/*-----------------------------------------------------------------------------*/
	//����������Ϸ״̬���ݰ�	�� ��Ϸ���ڷ���״̬ ��
	struct S_C_GameStation_SendCard
	{
		BYTE		byThinkTime;			//��Ϸ˼��ʱ��
		BYTE		byBeginTime;			//��Ϸ��ʼʱ��

		BYTE		byNtStation;					//ׯ��λ��
		BYTE		byUserCardCount[PLAY_COUNT];	//�û����ϵ�����

		int			iUserState[PLAY_COUNT];		//���״̬

		int			iGuoDi;						//����ֵ	
		int			iDiZhu;						//��ע      
		//LLONG		i64ShangXian;				//������ע����	��ע
		LLONG		i64DingZhu;					//��ע

		LLONG		i64AllZongZhu;				//��ע
		LLONG		i64XiaZhuData[PLAY_COUNT];	//�û���ǰ��ע��Ϣ
		S_C_GameStation_SendCard()
		{
			memset(this,0,sizeof(S_C_GameStation_SendCard));
		}
	};
	/*-----------------------------------------------------------------------------*/
	//����������Ϸ״̬���ݰ�	�� ��Ϸ��״̬ ��
	struct S_C_GameStation_PlayGame 
	{
		bool		bCanLook;				//�ɷ���
		bool		bCanFollow;				//�ɷ��ע
		bool		bCanAdd[E_CHOUMA_COUNT];//�ɷ��ע
		bool		bCanOpen;				//�ɷ����
		bool		bCanGiveUp;				//�ɷ�����


		BYTE		byThinkTime;			//��Ϸ˼��ʱ��
		BYTE		byBeginTime;			//��Ϸ��ʼʱ��

		BYTE		byNtStation;					//ׯ��λ��
		BYTE		byCurrHandleDesk;				//��ǰ���������
		BYTE		byUserCardCount[PLAY_COUNT];				//�û����ϵ�����
		BYTE		byUserCard[PLAY_COUNT][MAX_CARD_COUNT];		//�û����ϵ���

		int			iUserState[PLAY_COUNT];		//���״̬
		int         iRemainderTime;             //ʣ��ʱ��

		int			iGuoDi;						//����ֵ	
		int			iDiZhu;						//��ע      
		LLONG		i64ShangXian;				//������ע����	��ע
		LLONG		i64DingZhu;					//��ע

		LLONG		i64AllZongZhu;				//��ע
		LLONG		i64XiaZhuData[PLAY_COUNT];	//�û���ǰ��ע��Ϣ

		S_C_GameStation_PlayGame()
		{
			memset(this,0,sizeof(S_C_GameStation_PlayGame));
		}
	};


	/*-----------------------------------------------------------------------------*/
	//��Ϸ��ʼ
	struct	S_C_GameBegin
	{
		BYTE                byNtStation;                      //ׯ�� 

		int                 iGuoDi;                         //���ֹ���ֵ
		int					iDiZhu;							//���ֵ�ע

		int					iUserState[PLAY_COUNT];			//���״̬

		//LLONG				i64ShangXian;					//������ע���� (�� ���ÿ�������ע�����ܳ������ֵ) ��ע�ģ�����ʾΪ��ע��
		LLONG				i64DingZhu;						//���ֶ�ע -����ע����	�ﵽ���� ��Ҫ����	

		LLONG				i64ZongXiaZhu;					//��ǰ����ע;
		LLONG				i64XiaZhuData[PLAY_COUNT];		//�����ע����

		S_C_GameBegin()
		{
			memset(this,0,sizeof(S_C_GameBegin));
		}
	};
	/*-----------------------------------------------------------------------------*/
	//�������ݰ�
	struct S_C_SendCard
	{
		BYTE		byCard[PLAY_COUNT][MAX_CARD_COUNT];				//������
		BYTE		byCardCount[PLAY_COUNT];                        //������
		BYTE		bySendCardTurn[PLAY_COUNT*MAX_CARD_COUNT];		//����˳��--��¼���Ƿ��Ƶ�λ��  255��ʾ����Ҫ����

		S_C_SendCard()
		{
			memset(this,0,sizeof(S_C_SendCard));
			memset(bySendCardTurn,255,sizeof(bySendCardTurn));
		}

	};
	/*-----------------------------------------------------------------------------*/
	//��Ϸ��ʼ���ݰ�
	struct S_C_BeginPlay
	{
		BYTE		byCurrHandleStation;			//���Ƶ�λ��
		bool		bLook;		//�Ƿ�ɿ���
		bool		bFollow;	//�Ƿ�ɸ�ע
		bool		bAdd[E_CHOUMA_COUNT];		//�Ƿ�ɼ�ע
		bool		bOpen;		//�Ƿ�ɱ���
		bool		bGiveUp;	//�Ƿ������

		S_C_BeginPlay()
		{
			memset(this,0,sizeof(S_C_BeginPlay));
		}
	};
	/*-----------------------------------------------------------------------------*/
	//֪ͨ����
	struct S_C_NoticeAction
	{
		BYTE				byCurrHandleStation;	//���Ƶ�λ��
		bool				bCanLook;				//�Ƿ���Կ���	
		bool				bCanFollow;				//�Ƿ���Ը�ע	
		bool				bCanAdd[E_CHOUMA_COUNT];				//�Ƿ���Լ�ע	
		bool				bCanOpen;				//�Ƿ���Ա���	
		bool				bCanGiveUp;				//�Ƿ��������	

		S_C_NoticeAction()
		{
			memset(this,0,sizeof(S_C_NoticeAction));
		}
	};
	/*-----------------------------------------------------------------------------*/
	//�û�����
	struct C_S_LookCard
	{
		BYTE	byDeskStation;	
		C_S_LookCard()
		{
			memset(this,255,sizeof(C_S_LookCard));
		}
	};

	//��������
	struct S_C_LookCardResult
	{	
		BYTE            byDeskStation;				//���λ��
		BYTE			byUserCardCount;			//�û����ϵ�����
		BYTE			byUserCard[MAX_CARD_COUNT];	//�û����ϵ���
		int				iUserState;					//���״̬
		S_C_LookCardResult()
		{
			memset(this,-1,sizeof(S_C_LookCardResult));
			byUserCardCount = 0;
		}
	};
	/*-----------------------------------------------------------------------------*/
	//�û���ע
	struct C_S_UserGenZhu
	{
		BYTE	byDeskStation;		//���λ��
	};
	/*-----------------------------------------------------------------------------*/
	//�û���ע���
	struct S_C_UserGenZhuResult
	{

		BYTE		byDeskStation;		//���λ��
		int			iUserState;			//��λ����ҵ�״̬
		LLONG		i64FollowNum;		//��ע�Ĵ�С
		LLONG		i64UserZongZhu;		//��ҵ�����ע
		LLONG		i64AllZongZhu;		//��������ע

		S_C_UserGenZhuResult()
		{
			memset(this,0,sizeof(S_C_UserGenZhuResult));
		}
	};
	/*-----------------------------------------------------------------------------*/
	//�û���ע
	struct C_S_JiaZhu
	{

		BYTE		byDeskStation;	
		int			iAddType;			//��ע����

		C_S_JiaZhu()
		{
			memset(this,0,sizeof(C_S_JiaZhu));
		}
	};
	/*-----------------------------------------------------------------------------*/
	//�û���ע���
	struct S_C_JiaZhuResult
	{
		BYTE		byDeskStation;	
		int			iUserState;			//��λ����ҵ�״̬
		LLONG		i64AddNum;			//��ע�Ĵ�С
		LLONG		i64UserZongZhu;		//��ҵ�����ע
		LLONG		i64AllZongZhu;		//��������ע

		S_C_JiaZhuResult()
		{
			memset(this,0,sizeof(S_C_JiaZhuResult));
		}
	};
	/*-----------------------------------------------------------------------------*/
	//�û����ơ����ƽ��
	struct C_S_UserGiveUp
	{
		BYTE	byDeskStation;	
	};

	struct S_C_UserGiveUpResult
	{
		BYTE	byDeskStation;	
		int		iUserState;					//���״̬
	};
	/*-----------------------------------------------------------------------------*/
	//�û�����
	struct C_S_UserBiPai
	{
		BYTE	byDeskStation;		//�������Ƶ����
		BYTE	byBeDeskStation;	//�����������

		C_S_UserBiPai()
		{
			memset(this, 255, sizeof(C_S_UserBiPai));
		}
	};
	/*-----------------------------------------------------------------------------*/
	//�û�����
	struct S_C_UserBiPaiResult
	{
		BYTE	byDeskStation;		//�������Ƶ����
		BYTE	byBeDeskStation;	//�����������

		BYTE	byWinDesk;			//����Ӯ�����
		BYTE	byLostDesk;			//����������

		int		iUserStation[PLAY_COUNT];			//��¼�û�״̬

		LLONG		i64AddNum;			//������Ҫ��ע����
		LLONG		i64UserZongZhu;		//��ҵ�����ע
		LLONG		i64AllZongZhu;		//��������ע



		S_C_UserBiPaiResult()
		{
			memset(this,0,sizeof(S_C_UserBiPaiResult));
		}
	};
	/*-----------------------------------------------------------------------------*/
	//��Ϸ����ͳ�����ݰ�
	struct S_C_GameEnd
	{
		BYTE				byWiner;							//Ӯ��

		BYTE				byCard[PLAY_COUNT][MAX_CARD_COUNT];	//������
		BYTE				byCardShape[PLAY_COUNT];			//����

		int					iOperateCount[PLAY_COUNT];			//��������
		LLONG				i64UserScore[PLAY_COUNT];			//����ҵ÷�-��˰ǰ
		LLONG				i64ChangeMoney[PLAY_COUNT];			//����ҽ�ұ仯-��˰��

		S_C_GameEnd()
		{
			memset(this,0,sizeof(S_C_GameEnd));
		}
	};
	/*-----------------------------------------------------------------------------*/
	//��Ϸ����ͳ�����ݰ�
	struct GameCutStruct
	{
		BYTE					byDeskStation;					//�˳�λ��
		LLONG					iTurePoint[PLAY_COUNT];					//ׯ�ҵ÷�
		LLONG					iChangeMoney[PLAY_COUNT];
		GameCutStruct()
		{
			memset(this,0,sizeof(GameCutStruct));
		}
	};
	//-------------------------------------------------------------------------------
	//������Ϣ�ṹ��
	struct	S_C_SuperUserProof
	{
		BYTE	byDeskStation;	
		bool	bIsSuper;
		S_C_SuperUserProof()
		{
			memset(this,0,sizeof(S_C_SuperUserProof));
			byDeskStation = 255;
		}
	};
	//������Ϣ�ṹ��
	struct	C_S_SuperUserSet
	{
		BYTE	byDeskStation;	
		BYTE	byMaxDesk;		//�趨�������
		BYTE	byMinDesk;		//�趨��С�����
		C_S_SuperUserSet()
		{
			memset(this,255,sizeof(C_S_SuperUserSet));
		}
	};
	//�������ý��
	struct	S_C_SuperUserSetResult
	{
		bool	bSuccess;		//�Ƿ��趨�ɹ�
		BYTE	byDeskStation;	
		BYTE	byMaxDesk;		//�趨�������
		BYTE	byMinDesk;		//�趨��С�����
		S_C_SuperUserSetResult()
		{
			memset(this,255,sizeof(S_C_SuperUserSetResult));
			bSuccess = false;
		}
	};
	/*-----------------------------------------------------------------------------*/
	#pragma pack()

		/********************************************************************************/

		// ���ݰ���������ʶ

		/********************************************************************************/
	enum
	{
		S_C_GAME_BEGIN = 50,			//��Ϸ��ʼ		
		S_C_SEND_CARD = 51,			//��ʼ����
		S_C_GAME_PLAY = 52,			//��ʼ��Ϸ		
		S_C_CALL_ACTION = 53,			//֪ͨ��Ҳ���	
		S_C_GAME_END = 54,			//��Ϸ����

		S_C_SUPER_PROOF = 70,			//����У��
		C_S_SUPER_SET = 71,			//��������
		S_C_SUPER_SET_RESULT = 72,			//�������ý��

		C_S_LOOK_CARD = 80,			//��ҿ���
		S_C_LOOK_CARD_RESULT = 81,			//��ҿ��ƽ��
		C_S_GEN_ZHU = 82,			//��Ҹ�ע
		S_C_GEN_ZHU_RESULT = 83,			//��Ҹ�ע���
		C_S_JIA_ZHU = 84,			//��ע
		S_C_JIA_ZHU_RESULT = 85,			//��ע���
		C_S_BI_PAI = 86,			//��ұ���
		S_C_BI_PAI_RESULT = 87,			//��ұ��ƽ��
		C_S_GIVE_UP = 88,			//�������
		S_C_GIVE_UP_RESULT = 89,			//������ƽ��
	};
                            
	//�Զ�����Ϣ�ṹ        
	//game result data node.
	struct TGameResult
	{
		std::string szName;
		LLONG llMoney;
		std::string szType;
		TGameResult()
		{
			llMoney = 0;
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

}


#endif
