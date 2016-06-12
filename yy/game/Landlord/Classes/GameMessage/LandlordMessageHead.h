#ifndef __Landlord_MessageHead_H__
#define __Landlord_MessageHead_H__

#include "HNNetExport.h"

namespace landlord
{
	enum Show_Type
	{
		SHOW_CALL_LORD     = 0x01,
		SHOW_NO_CALL_LORD  = 0x02,
		SHOW_ROB_LORD      = 0x04,
		SHOW_NO_ROB_LORD   = 0x08,
		SHOW_DOUBLE        = 0x10,
		SHOW_NO_DOUBLE     = 0x20,
		SHOW_SHOW_CARD     = 0x40,
		SHOW_START         = 0x80,
	};


	enum Landlord_COMMAND
	{
		//��Ϸ��Ϣ
		GAME_NAME_ID         =  10100003, // ���� ID

		//�汾����	          
		GAME_MAX_VER         =  1,        //������߰汾
		GAME_LESS_VER        =  1,        //������Ͱ汾
        GAME_CHANGE_VER		 =	0,		  //�޸İ汾
        
        //��Ϸ�ڲ������汾��
        DEV_HEIGHT_VERSION   =  2,        //�ڲ������߰汾��(ÿ����һ��LOW_VERSIONG+1)
        DEV_LOW_VERSION	     =	0,	      //�ڲ������Ͱ汾��
        
        //��Ϸ��Ϣ
        PLAY_COUNT	         =	3,        //��Ϸ����
        
        		//��Ϸ״̬����
        GS_WAIT_SETGAME		 =	0,				//�ȴ���������״̬
        GS_WAIT_ARGEE		 =	1,				//�ȴ�ͬ������
        GS_SEND_CARD		 =	20,				//����״̬
        GS_WAIT_BACK		 =	21,				//�ȴ���ѹ����
        GS_PLAY_GAME		 =	22,				//��Ϸ��״̬
        GS_WAIT_NEXT		 =	23,				//�ȴ���һ�̿�ʼ 
        					 
        GS_FLAG_NORMAL		 =	0,				//�������
		GS_FLAG_CALL_SCORE	 =	1,				//�з�
		GS_FLAG_ROB_NT		 =	2,				//������
		GS_FLAG_ADD_DOUBLE	 =	3,				//�Ӱ�
		GS_FLAG_SHOW_CARD	 =	4,				//����
        GS_FLAG_PLAY_GAME    =  5,
	};



	const  INT  MAX_TASK_TYPE   = 4 ;  ///����������� 
	const  INT  MAX_CARD_SHAPE  = 8 ;  ///�����������
	const  INT  MAX_CARD_TYPE   = 15 ; ///������

	///��������
	enum BackCardType
	{
		TYPE_NONE        = 0 ,//ʲô���Ͷ�����
		TYPE_DOUBLE_CARD = 10 , 
		TYPE_SAME_HUA    = 11,
		TYPE_SMALL_KING  = 12 ,
		TYPE_BIG_KING    = 13 , //����
		TYPE_TRIPLE_CARD = 14 ,
		TYPE_STRAIT      = 15,
		TYPE_ROCKET      = 16
	};
	///��������
	enum LastTaskType
	{
		TYPE_LAST_NONE         = 0 ,    ///���κ����� 
		TYPE_HAVE_A_CARD       = 100 ,  ///��ĳ����
		TYPE_SOME_SHAPE        = 101 ,  ///��ĳ������
		TYPE_SINGLE_SOME_CARD  = 102 ,  ///���ĳ����
		TYPE_DOUBLE_SOME_CARD  = 103   ///���һ��ĳ����
	};

	#pragma pack(1)

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
	///��Ϸ�еı���
	struct  GameMutipleStruct
	{
		INT       sBaseMutiple ;                 ///��Ϸ�еĻ�����ע
		INT       sBackCardMutiple ;             ///���Ʊ���
		INT       sBombCount ;                   ///ը������
		INT       sSprINTMutiple ;               ///����
		INT       sCardShapeMutiple  ;           ///���ͱ�����һ��ָ�����еı�����

		BYTE      sAddGameMutiple[PLAY_COUNT] ;  /// ��Ϸ�мӱ�
		BYTE      sRobNtMutiple[PLAY_COUNT] ;    ///����������
		BYTE      sMingPaiMutiple[PLAY_COUNT] ;  ///ÿ���˵����Ʊ���

		GameMutipleStruct()
		{
			sBaseMutiple = 1 ; 
			sBackCardMutiple = 1 ; 
			sBombCount = 0 ; 
			sSprINTMutiple = 1;
			sCardShapeMutiple = 100 ; 

			memset(sAddGameMutiple , 0 , sizeof(sAddGameMutiple)) ; 
			memset(sRobNtMutiple , 0 , sizeof(sRobNtMutiple)) ; 
			memset(sMingPaiMutiple , 0 , sizeof(sMingPaiMutiple)) ; 
		};
		void  IniData(INT  iBaseMutiple)
		{
			sBaseMutiple = iBaseMutiple ; 
			sBombCount = 0 ; 
			sBackCardMutiple = 1 ; 
			sSprINTMutiple = 1;
			sCardShapeMutiple = 100; 

			memset(sAddGameMutiple , 0 , sizeof(sAddGameMutiple)) ; 
			memset(sRobNtMutiple , 0 , sizeof(sRobNtMutiple)) ; 
			memset(sMingPaiMutiple , 0 , sizeof(sMingPaiMutiple)) ; 
		}
		///��ȡ���������
		INT  GetMingMaxMutiple(void)
		{		
			INT iMingMutiple = std::max(std::max(sMingPaiMutiple[0] , sMingPaiMutiple[1]) ,sMingPaiMutiple[2]) ; 

			return (iMingMutiple >0?iMingMutiple :1) ; 
		}
		///��ȡ������������
		INT  GetRobNtMutiple(void)
		{
			INT  iRobMutiple = 0 ; 
			INT  iRobCount   = 0 ; 

			for(INT  i = 0  ; i <PLAY_COUNT ; i++)
			{
				iRobCount += sRobNtMutiple[i] ; 
			}

			iRobMutiple = pow(2.0, iRobCount) ; 

			return iRobMutiple ; 
		}
		///��ȡ��������
		INT  GetPublicMutiple(void)
		{
			INT  iBombMutiple = pow(2.0 , sBombCount) ; 

			INT  iGameMutiple  = sBaseMutiple*sBackCardMutiple*iBombMutiple*sSprINTMutiple*sCardShapeMutiple*GetRobNtMutiple()*GetMingMaxMutiple()/100 ;

			return iGameMutiple ; 

		}
	};


	/********************************************************************************/
	//��Ϸ���ݰ�

	/********************************************************************************/
	//�û��������ݰ� �������������
	struct OutCardStruct
	{
		INT					iCardCount;						//�˿���Ŀ
		BYTE				iCardList[45];					//�˿���Ϣ
	};

	//������ҳ���
	struct ReplaceOutCardStruct
	{
		BYTE				bDeskStation;					//������Ƶ�λ��
		INT					iCardCount;						//�˿���Ŀ
		BYTE				iCardList[45];					//�˿���Ϣ
	};

	//�û��������ݰ� ������ͻ��ˣ�
	struct OutCardMsg
	{
		INT					iNextDeskStation;				//��һ������
		BYTE				iCardCount;						//�˿���Ŀ
		BYTE				bDeskStation;					//��ǰ������	
		BYTE				iCardList[45];					//�˿���Ϣ
	};
	//��һ��
	struct NewTurnStruct
	{
		BYTE				bDeskStation;					//����
		BYTE				bReserve;						//����
	};
	//��Ϸ״̬���ݰ�	�� �ȴ���������״̬ ��
	struct GameStation_1
	{
		//��Ϸ�汾
		BYTE				iVersion;						//��Ϸ�汾��
		BYTE				iVersion2;						//��Ϸ�汾��
		//��Ϸ��Ϣ
	};

	//��Ϸ״̬���ݰ�	�� �ȴ�������ҿ�ʼ ��
	struct GameStation_2
	{
		BYTE				iVersion;						//��Ϸ�汾��
		BYTE				iVersion2;						//��Ϸ�汾��
		bool                bUserReady[PLAY_COUNT] ;        ///����Ƿ���׼��

		BYTE				iBegINTime;						//��ʼ׼��ʱ��
		BYTE				iThinkTime;						//����˼��ʱ��
		BYTE				iCallScoreTime;					//�зּ�ʱ
		BYTE                iRobNTTime ;                    //������ʱ��
		BYTE				iAddDoubleTime;					//�Ӱ�ʱ��

		INT                 iGameMutiple   ;             ///��Ϸ�еı���
		INT                 iAddDoubleLimit;             //�ӱ�����
		INT                 iGameMaxLimit ;              ///��Ϸ�����Ӯ

		UINT				iCardShape;						//��������
		//��Ϸ����
		UINT				iDeskBasePoINT;					//���������
		UINT				iRoomBasePoINT;					//���䱶��
		LLONG				iRunPublish;					//���ܿ۷�

	};


	//��Ϸ״̬���ݰ�	�� �ȴ���Ѻ����״̬ ��
	struct GameStation_3
	{
		BYTE				iVersion;						//��Ϸ�汾��
		BYTE				iVersion2;						//��Ϸ�汾��

		BYTE				iBackCount;						//������
		BYTE				iBegINTime;						//��ʼ׼��ʱ��
		BYTE				iThinkTime;						//����˼��ʱ��
		BYTE                iRobNTTime ;                    //������ʱ��
		BYTE				iCallScoreTime;					//�зּ�ʱ
		BYTE				iAddDoubleTime;					//�Ӱ�ʱ��

		BYTE				iCallScorePeople;				//��ǰ�з���
		BYTE				iGameFlag;						//�зֱ��
		BYTE				iCallScoreResult;				//���еķ�

		INT					iUpGradePeople;					//ׯ��λ��
		INT                 iCurOperator ;                 ///��ǰ��������

		UINT				iDeskBasePoINT;					//���������
		UINT				iRoomBasePoINT;					//���䱶��

		UINT				iCardShape;						//��������
		INT                 iGameMutiple   ;             ///��Ϸ�еı���
		INT                 iAddDoubleLimit;             //�ӱ�����
		INT                 iGameMaxLimit ;              ///��Ϸ�����Ӯ

		bool				bAuto[PLAY_COUNT];				//�й����
		bool				bCanleave[PLAY_COUNT];			//�ܷ���˳�
		BYTE				iCallScore[PLAY_COUNT];			//���ҽз����
		INT 				iRobNT[PLAY_COUNT];				//���������
		INT                 iUserDoubleValue[PLAY_COUNT] ;  ///��Ҽӱ����
		BYTE				iUserCardCount[PLAY_COUNT];		//�û������˿���Ŀ
		BYTE				iUserCardList[195];				//�û����ϵ��˿�
		BYTE                iGameBackCard[12] ;                 ///����
		BYTE 				iBackCardCount;						//��������
		LLONG				iRunPublish;					//���ܿ۷�

		GameMutipleStruct   gameMutiple ;                      ///��Ϸ����  
		GameTaskStruct      gameTask ;                         ///��Ϸ����


	};

	//��Ϸ״̬���ݰ�	�� ��Ϸ��״̬ ��
	struct GameStation_4	
	{
		bool				bIsLastCard;						//�Ƿ�����������

		BYTE				iVersion;							//��Ϸ�汾��
		BYTE				iVersion2;							//��Ϸ�汾��
		BYTE				iBackCount;							//������

		BYTE				iBegINTime;							//��ʼ׼��ʱ��
		BYTE				iThinkTime;							//����˼��ʱ��
		BYTE                iRobNTTime ;                    //������ʱ��
		BYTE				iCallScoreTime;						//�зּ�ʱ
		BYTE				iAddDoubleTime;						//�Ӱ�ʱ��

		BYTE				bIsPass;							//�Ƿ񲻳�

		INT					iRunPublish;						//���ܿ۷�	
		INT					iBase;								//��ǰը������
		INT					iUpGradePeople;						//ׯ��λ��

		INT 				iCallScoreResult;					//�зֽ��
		INT					iOutCardPeople;						//���ڳ����û�
		INT					iFirstOutPeople;					//�ȳ��Ƶ��û�
		INT					iBigOutPeople;						//�ȳ��Ƶ��û�

		UINT				iDeskBasePoINT;						//���������
		UINT				iRoomBasePoINT;						//���䱶��

		INT                 iGameMutiple   ;             ///��Ϸ�еı���
		UINT				iCardShape;							//��������
		INT                 iAddDoubleLimit;             //�ӱ�����
		INT                 iGameMaxLimit ;              ///��Ϸ�����Ӯ

		BYTE				iAwardPoINT[PLAY_COUNT];			//����
		BYTE				iPeopleBase[PLAY_COUNT];			//�Ӱ�
		BYTE				iRobNT[PLAY_COUNT];					//������


		bool				bAuto[PLAY_COUNT];					//�й����
		bool				bCanleave[PLAY_COUNT];				//�ܷ���˳�

		BYTE				iUserCardCount[PLAY_COUNT];			//�û������˿���Ŀ
		BYTE				iUserCardList[195];					//�û����ϵ��˿�

		BYTE				iBaseOutCount;						//���Ƶ���Ŀ
		BYTE                iBaseCardList[45] ;                //�����ϵ���

		BYTE				iDeskCardCount[PLAY_COUNT];			//�����˿˵���Ŀ
		BYTE                iDeskCardList[PLAY_COUNT][45] ;    ///�����ϵ���

		BYTE				iLastCardCount[PLAY_COUNT];			//�����˿˵���Ŀ
		BYTE				iLastOutCard[PLAY_COUNT][45];		//���ֵ��˿�

		bool                bPass[PLAY_COUNT] ;                //����
		bool                bLastTurnPass[PLAY_COUNT] ;         //��һ�ֲ���

		BYTE                iGameBackCard[12] ;                 ///����
		BYTE 				iBackCardCount;						//��������

		GameMutipleStruct   gameMutiple ;                      ///��Ϸ����  
		GameTaskStruct      gameTask ;                         ///��Ϸ����

	};

	//��Ϸ״̬���ݰ�	�� �ȴ����̿�ʼ״̬  ��
	struct GameStation_5
	{
		BYTE				iVersion;							//��Ϸ�汾��
		BYTE				iVersion2;							//��Ϸ�汾��
		BYTE				iBegINTime;							//��ʼ׼��ʱ��
		BYTE				iThinkTime;							//����˼��ʱ��
		BYTE                iRobNTTime ;                    //������ʱ��
		BYTE				iCallScoreTime;						//�зּ�ʱ
		BYTE				iAddDoubleTime;						//�Ӱ�ʱ��

		INT                 iGameMutiple   ;               ///��Ϸ�еı���
		INT                 iAddDoubleLimit;             //�ӱ�����
		INT                 iGameMaxLimit ;              ///��Ϸ�����Ӯ

		bool                bUserReady[PLAY_COUNT] ;     ///���׼��
		UINT				iCardShape;							//��������

		UINT				iDeskBasePoINT;						//���������
		UINT				iRoomBasePoINT;						//���䱶��
		LLONG				iRunPublish;						//���ܿ۷�
	};

	//�û�ͬ����Ϸ
	struct UserArgeeGame
	{
		BYTE				iPlayCount;							//��Ϸ����
		BYTE				iCardCount;							//�˿���Ŀ
	};
	///���ƿ�ʼ
	struct  UserMingStruct
	{
		bool    bStart  ;       ///�Ƿ�Ϊ��ʼ
		bool    bMing ;         ///����Ƿ�����
		BYTE    bDeskStaion ;   ///��ҵ�λ��
		BYTE    byCardCount ;   ///����ʱ����˿˵�����
	};
	/*----------------------------------------------------------------------*/
	//��Ϸ��ʼ
	struct	GameBeginStruct
	{
		BYTE				iPlayLimit;							//��Ϸ�ܾ���
		BYTE				iBeenPlayGame;						//�Ѿ����˶��پ�
		BYTE                byUserMingBase[PLAY_COUNT] ;        ///������Ʊ������
		UINT				iCardShape;							//��������

		GameBeginStruct()
		{
			memset(this, 0, sizeof(GameBeginStruct));
		}
	};
	/*----------------------------------------------------------------------*/
	//��Ϸ׼�����ݰ�
	struct	GamePrepareStruct
	{
		BYTE				byCard;									//����
		GamePrepareStruct()
		{
			memset(this,255,sizeof(GamePrepareStruct));
		}
	};
	/*----------------------------------------------------------------------*/
	//�������ݰ�
	struct	SendCardStruct
	{
		BYTE				bDeskStation;							//���
		BYTE				bCard;									//�Ʊ��
	};
	/*----------------------------------------------------------------------*/
	//�l�������Ɣ���
	struct	SendAllStruct
	{
		BYTE             iUserCardCount[PLAY_COUNT];		//��������
		BYTE             iUserCardList[108];				//���ƶ���

		SendAllStruct()
		{
			memset(this,0,sizeof(SendAllStruct));
		}
	};
	/*----------------------------------------------------------------------*/
	//���ƽ���
	struct	SendCardFinishStruct
	{
		BYTE		bReserve;	
		BYTE		byBackCardList[12];						//��������
		BYTE		byUserCardCount[PLAY_COUNT];			//�û������˿���Ŀ
		BYTE		byUserCard[PLAY_COUNT][45];				//�û����ϵ��˿�
		SendCardFinishStruct()
		{
			memset(this,0,sizeof(SendCardFinishStruct));
		}
	};
	/*----------------------------------------------------------------------*/
	//�������ݰ�
	struct BackCardExStruct
	{
		BYTE				iGiveBackPeople;				//�������
		BYTE				iBackCardCount;					//�˿���Ŀ
		BYTE				iBackCard[12];					//��������

		GameTaskStruct      gameTask ;                      //��Ϸ����  

	};
	//��Ϸ��ʼ���ݰ�
	struct BeginPlayStruct
	{
		BYTE				iOutDeskStation;				//���Ƶ�λ��
	};


	//�û�˦��ʧ�����ݰ� ������ͻ��ˣ�
	struct OutShowCardStruct
	{
		BYTE				iCardCount;						//�˿���Ŀ
		BYTE				bDeskStation;					//��ǰ������
		BYTE				iResultCount;					//����˿���Ŀ
		BYTE				iCardList[78];					//�˿���Ϣ
	};

	//���濪ʼ
	struct ReplaceAgreeStruct
	{
		BYTE bDeskStation;									
	};

	//�з����ݰ�
	struct CallScoreStruct
	{	
		BYTE				bDeskStation;							//��ǰ�з���
		INT					iValue;									//�з�����
		bool 				bCallScoreflag;							//�зֱ��								
	};
	/*----------------------------------------------------------------------*/
	//������
	struct	RobNTStruct
	{
		BYTE   byDeskStation;		//����������
		BYTE   byRobCount ;         //�������������
		INT    iValue;				//���������(0-�е���״̬ 1-������״̬)
		RobNTStruct()
		{
			memset(this, 0, sizeof(RobNTStruct));
		}
	};
	/*----------------------------------------------------------------------*/
	//����
	struct ShowCardStruct
	{
		BYTE bDeskStation;										//����
		BYTE iCardList[54];					                   //�˿���Ϣ
		BYTE iCardCount;						              //�˿���Ŀ
		INT iValue;											  //����
		INT iBase;											  //���� 20081204
	};

	//��������
	struct ReplaceShowCardStruct
	{
		BYTE bDeskStation;										//����
		BYTE bRepDeskStation;									//����λ��
		INT iValue;												//����
	};
	//�ӱ�
	struct AddDoubleStruct
	{
		BYTE bDeskStation;										//�ӱ�λ��
		INT iValue;												//�ӱ����
	};

	//����з����ݰ�
	struct ReplaceCallScoreStruct
	{	
		BYTE		bDeskStation;								//����з���
		INT		    NowCallScorePeople;							//��ǰ�з���
		INT			CallScore;									//�з�����
		bool 		CallScoreFlag;								//�зֱ��								
	};

	//����
	struct AwardPoINTStruct
	{
		BYTE	iAwardPoINT;									//����
		BYTE	bDeskStation;									//����
		INT		iBase;											//����
	};

	//�й����ݽṹ
	struct AutoStruct
	{
		BYTE bDeskStation;
		bool bAuto;
	};

	//�������й�
	struct UseAIStation
	{
		BYTE bDeskStation;					//����
		INT bState;						//״̬
	};

	//�ھ����
	struct ChampionStruct
	{
		BYTE bDeskStation ;
	};

	//�û��������ݰ� ������ͻ��ˣ�
	struct BatchOutCardMsg
	{
		BYTE				iCardCount;						//�˿���Ŀ
		BYTE				iCardList[156];					//�˿���Ϣ
	};

	////����
	//struct PropStruct
	//{
	//	BYTE				sendDeskStation;				//����λ��
	//	BYTE				recvDeskStation;				//����λ��
	//	INT					iValue;
	//};
	//��Ϸ����ͳ�����ݰ�
	struct GameEndStruct
	{
		bool                bFinishTask ;                    //�Ƿ����������
		BYTE				iUpGradeStation;				//ׯ��λ��
		BYTE			    iUserCardCount[PLAY_COUNT];	    //�û������˿���Ŀ
		BYTE			    iUserCard[PLAY_COUNT][45];		//�û����ϵ��˿�

		GameMutipleStruct   gameMutiple ; 

		LLONG  			iTurePoINT[PLAY_COUNT];			//��ҵ÷�
		LLONG				iChangeMoney[8];				//��ҽ��

		GameEndStruct()
		{
			memset(this , 0 ,sizeof(GameEndStruct)) ; 
		}

	};

	//��Ϸ����ͳ�����ݰ�
	struct GameCutStruct
	{
		INT					iRoomBasePoINT;					//����
		INT					iDeskBasePoINT;					//���汶��
		INT					iHumanBasePoINT;				//��ͷ����

		INT					bDeskStation;					//�˳�λ��
		LLONG				iChangeMoney[8];				//��ҽ��
		LLONG				iTurePoINT[PLAY_COUNT];			//ׯ�ҵ÷�

	};

	struct HaveThingStruct
	{
		BYTE pos;
		char szMessage[61];
	};

	struct LeaveResultStruct
	{
		BYTE bDeskStation;
		INT bArgeeLeave;
	};

	struct MessageStruct	//�ͻ��˵�������
	{
		char Message[200];
	};

	struct StopResultStruct
	{
		bool bArgee;
	};

	//��Ϸ����
	struct GameFinishNotify
	{
		CHAR name[PLAY_COUNT][61];
		//	INT	iBasePoINT;
		BYTE iStyle;			//��Ϸ�����Ƿ�Ϊǿ��,������������
		INT	iBasePoINT;			//ϵͳ����
		INT	iDeskBasePoINT;		//���������
		INT iUpGradePeople;		//�f��
		INT iGrade2;			//�f������
		LLONG	iWardPoINT[PLAY_COUNT];
		LLONG iAwardPoINT[PLAY_COUNT];	//����
		LLONG iTotalScore[PLAY_COUNT];	//�ܻ���
		LLONG iMoney[PLAY_COUNT];			//��������Ǯ��
		BYTE iGameStyle;					//��Ϸ����
	};

	struct UserleftDesk
	{
		BYTE bDeskStation;                //�������
	};

#pragma pack()
	//-------------------------------------------------------------------------------
	//������Ϣ�ṹ��
	struct	SuperUserMsg
	{
		BYTE	byDeskStation;	
		bool	bIsSuper;
		SuperUserMsg()
		{
			memset(this,0,sizeof(SuperUserMsg));
		}
	};
	//-------------------------------------------------------------------------------
	/********************************************************************************/

	// ���ݰ���������ʶ
	//warning:79��IDԤ���������ͻ���,����ʹ��
	/********************************************************************************/

	enum Net_Cmd
	{
		ASS_UG_USER_SET				=	50,				//������Ϸ
		ASS_GAME_BEGIN				=	51,				//��Ϸ��ʼ
		ASS_GAME_PREPARE			=	52,				//׼������
		
		ASS_SEND_CARD				=	53,				//������Ϣ
		ASS_SEND_CARD_MSG			=	54,				//���ƹ����д�����Ϣ
		ASS_SEND_ALL_CARD			=	55,				//����������(һ�·���ȫ��)
		ASS_SEND_FINISH				=	56,				//�������
		ASS_CALL_SCORE				=	57,				//�з�
		ASS_CALL_SCORE_RESULT		=	58,				//�зֽ��
		ASS_CALL_SCORE_FINISH		=	59,				//�зֽ���
		ASS_REPLACE_CALL_SCORE		=	60,				//����з�
		
		ASS_ROB_NT					=	61,				//������
		ASS_ROB_NT_RESULT			=	62,				//���������
		ASS_REPLACE_ROB_NT			=	63,				//����������
		ASS_GAME_MULTIPLE			=	64,				//��Ϸ����(���������ӱ�)
		ASS_ROB_NT_FINISH			=	65,				//���������
		
		ASS_BACK_CARD				=	66,				//��������
		ASS_BACK_CARD_EX			=	67,				//��չ��������(������ҿɼ�)
		ASS_ADD_DOUBLE				=	68,				//�ӱ�
		ASS_ADD_DOUBLE_RESULT		=	69,				//�ӱ����
		ASS_REPLACE_ADD_DOUBLE		=	70,				//����Ӱ�
		ASS_ADD_DOUBLE_FINISH		=	71,				//�Ӱ�����
		ASS_SHOW_CARD				=	72,				//����
		ASS_SHOW_CARD_RESULT		=	73,				//���ƽ��
		ASS_REPLACE_SHOW_CARD		=	74,				//��������
		ASS_SHOW_CARD_FINISH		=	75,				//���ƽ���
		
		ASS_GAME_PLAY				=	76,				//��ʼ��Ϸ
		ASS_OUT_CARD				=	77,				//�û�����
		ASS_OUT_CARD_RESULT			=	78,				//���ƽY��
		
		ASS_SUPER_USER				=	79,				//������Ϣ
		
		
		ASS_REPLACE_OUT_CARD		=	80,				//�������(79���������ͻ��˷�����)
		ASS_ONE_TURN_OVER			=	81,				//һ�����(ʹ�ͻ�����һ�ֿ���)
		ASS_NEW_TURN				=	82,				//��һ�ֿ�ʼ
		ASS_AWARD_POINT				=	83,				//����(ը�����)
		
		ASS_CONTINUE_END			=	84,				//��Ϸ����
		ASS_NO_CONTINUE_END			=	85,				//��Ϸ����
		ASS_NO_CALL_SCORE_END		=	86,				//���˽з�
		ASS_CUT_END					=	87,				//�û�ǿ���뿪
		ASS_SAFE_END				=	88,				//��Ϸ��ȫ����
		ASS_TERMINATE_END			=	89,				//�������
		ASS_AHEAD_END				=	90,				//��ǰ����
		ASS_AUTO					=	91,				//�û��й�
		ASS_HAVE_THING				=	92,				//����
		ASS_LEFT_RESULT				=	93,	     		//�����뿪���
		ASS_MESSAGE					=	94,				//������Ϣ
		ASS_AI_STATION				=	95,				//�������й�(���߻���)
		ASS_REPLACE_GM_AGREE_GAME	=	96,				//������ҿ�ʼ
		ASS_USER_LEFTDESK           =   97,               //����뿪���ӻ����
	};

}


/********************************************************************************/
#endif
