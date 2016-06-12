#ifndef TRACTORUpgradeMessage_h__
#define TRACTORUpgradeMessage_h__
#include "HNBaseType.h"

namespace TRACTOR
{

	//ͬ¥ ��ͨ��/��Ƶ�� ����  release/��Ƶ��  ��������������������

	//��Ϸ��Ϣ
	enum 
	{
		GAME_NAME_ID			=			12111006,		// ���� ID
	};


	//�汾����
	enum
	{
		//GAME_MAX_VER		=			1,								//������߰汾
		//GAME_LESS_VER		=			1,								//������Ͱ汾
		//GAME_CHANGE_VER		=			0,								//�޸İ汾

	//��Ϸ��Ϣ

		PLAY_COUNT			=			6,							//��Ϸ����
		MAX_CARD_COUNT      =            3,

	//��Ϸ״̬����
		GS_WAIT_SETGAME		=			0,				//�ȴ���������״̬
		GS_WAIT_ARGEE		=			1,				//�ȴ�ͬ������
		GS_SEND_CARD		=			20,				//����״̬
		GS_WAIT_BACK		=			21,				//�ȴ���ѹ����
		GS_PLAY_GAME		=			22,				//��Ϸ��״̬
		GS_WAIT_NEXT		=			23,				//�ȴ���һ�̿�ʼ 

	//��ע����

		TYPE_GIVE_UP		=			0x00,			//����
		TYPE_NOTE			=			0x01,			//��ע
		TYPE_ADD			=			0x02,			//��ע
		TYPE_FOLLOW			=			0x03,			//��ע
		TYPE_OPENCARD		=			0x04,			//����
		TYPE_LOOKCARD		=			0x05,			//����
		TYPE_NORMAL         =            0x06,            //����״̬
		
		TYPE_OPEN_LOSE      =            0x06 ,           //����ʧ��
		TYPE_OPEN_WIN       =            0x07,            //����ʤ��
		TYPE_REACH_LIMITE_NEXT     =     0x08 ,           //��һ����ע��ﵽ����
		TYPE_REACH_LIMITE_SELF     =     0x09 ,           //�Լ���ע��ﵽ����
		TYPE_COMPARE_CARD          =     0x10  ,          //���Ʋ���
		
		SH_THREE              =  7, //����
		SH_SAME_HUA_CONTINUE  =  6, //ͬ��˳
		SH_SAME_HUA           =  5, //ͬ��
		SH_CONTIUE            =  4 ,//˳��
		SH_DOUBLE             =  3, //����
		SH_SPECIAL            =  1 ,//����235
		SH_OTHER              =  2, //����
		SH_ERROR              =  0, //����
	};
	/********************************************************************************/

	//��Ϸ���ݰ�

	/********************************************************************************/
#pragma pack(push ,1)
	//�����û����
	struct tagUserResult
	{
		BYTE bCountNotePeople;		     //��ǰδ���������
		INT bAddTime;				     //Ѻע����
		BYTE iNowBigNoteStyle;		     //��ע����
		BYTE iNowBigPeople;
		BYTE iCutOutLastpeople;
		BYTE iCutOutLastType;
		BYTE iOutPeople;			     //��ע��
		INT iNowBigNote;			     //��ǰ��ע
		INT iTotalNote;				     //����ע
		INT iUserStation[PLAY_COUNT];    //�����ע����
		INT iFirstOutPeople;             //��һ��ע���
		LLONG iUserNote[PLAY_COUNT];     //�û�������ע
		bool bIsFirstNote;
		tagUserResult()
		{
			memset(this,0,sizeof(tagUserResult));
		}
	};
	//��������

	struct lookCardResult
	{
		BYTE             bDeskStation;
		BYTE				iUserCardCount;				//�û����ϵ�����
		BYTE				iUserCard[5];				//�û����ϵ���
		lookCardResult()
		{
			memset(this,0,sizeof(lookCardResult));
		}
	};


	struct GameStationBase
	{
		BYTE bGameStation;
		GameStationBase()
		{
			memset(this,0,sizeof(GameStationBase));
		}
	};
	//��Ϸ״̬���ݰ�	�� �ȴ���������״̬ ��
	struct GameStation_1 : GameStationBase
	{
		//��Ϸ�汾
		BYTE				iVersion;						//��Ϸ�汾��
		BYTE				iVersion2;						//��Ϸ�汾��
		GameStation_1()
		{
			memset(this,0,sizeof(GameStation_1));
		}
	};

	//��Ϸ״̬���ݰ�	�� �ȴ�������ҿ�ʼ ��
	struct GameStation_2 : GameStationBase
	{
		BYTE                bEXFlag[12];                    //zht 2011-4-12, ��ʹ�õ��ֶΣ�ռλ��
		BYTE				iVersion;						//��Ϸ�汾��
		BYTE				iVersion2;						//��Ϸ�汾��
		BYTE				iThinkTime;						//��Ϸ˼��ʱ��
		BYTE				iBeginTime;						//��Ϸ��ʼʱ��
		BYTE				iAllCardCount;				    //�˿���Ŀ
		INT					iRoomBasePoint;					//���䱶��
		INT					iRunPublish;					//���ܿ۷�
		BYTE                bEXFlag1[12];                    // ��ʹ�õ��ֶΣ�ռλ��
		//   ���������Ϣ
		LLONG				iLimitNote;						//��ע�ⶥֵ   ��ע / ��ע
		INT					iBaseNote;						//��ע	       ��ע
		INT                 iGuoDi;                         //����
		LLONG               iLimtePerNote;                  //��ע���ޣ���ע��Ҫ * 2
		INT                bShowIdOrPlay;                  //��ʾ��һ�����ʾid
		GameStation_2()
		{
			memset(this,0,sizeof(GameStation_2));
		}
	};

	//��Ϸ״̬���ݰ�	�� ��Ϸ���ڷ���״̬ ��
	struct GameStation_3 : GameStationBase
	{
		BYTE                bEXFlag[12];                    //zht 2011-4-12, ��ʹ�õ��ֶΣ�ռλ��
		BYTE				iVersion;						//��Ϸ�汾��
		BYTE				iVersion2;						//��Ϸ�汾��
		BYTE				iThinkTime;						//��Ϸ����
		BYTE				iBeginTime;						//��Ϸ����
		BYTE				iAllCardCount;					//�˿���Ŀ
		BYTE				iBeenPlayGame;					//�Ѿ���Ϸ�ľ���
		BYTE				iSysCardCount;					//��ʣ��������
		BYTE                bEXFlag1[12];                   //zht 2011-4-12, ��ʹ�õ��ֶΣ�ռλ��
		BYTE				iUserCardCount[PLAY_COUNT];		//�û����ϵ�����
		BYTE				iUserCard[PLAY_COUNT][5];		//�û����ϵ���	
		INT					iUpGradePeople;					//ׯ��λ��
		INT					iRoomBasePoint;					//���䱶��
		INT					iRunPublish;					//���ܿ۷�

		LLONG			    iLimitNote;						//��ע�ⶥֵ   ��ע / ��ע
		INT					iBaseNote;						//��ע	       ��ע
		INT                 iGuoDi;                         //����
		LLONG               iLimtePerNote;                  //��ע���ޣ���ע��Ҫ * 2
		LLONG				iTotalNote[PLAY_COUNT];			//�û���ǰ��ע
		INT                bShowIdOrPlay;
		CHAR                strCityText[PLAY_COUNT][50];    //������������ַ
		CHAR                strUserIDText[PLAY_COUNT][50];  //����������ID
		bool                bSuperUser[PLAY_COUNT];         //������Ƿ��ǳ������
		INT                bPlayer[PLAY_COUNT];            //�Ƿ���һ���;�����ս��
		GameStation_3()
		{
			memset(this,0,sizeof(GameStation_3));
		}
	};

	//��Ϸ״̬���ݰ�	�� ��Ϸ��״̬ ��
	struct GameStation_4 : GameStationBase
	{
		BYTE                bEXFlag[12];                    //zht 2011-4-12, ��ʹ�õ��ֶΣ�ռλ��
		BYTE				iVersion;						//��Ϸ�汾��
		BYTE				iVersion2;						//��Ϸ�汾��
		BYTE				iThinkTime;						//��Ϸ����
		BYTE				iBeginTime;						//��Ϸ����
		BYTE				iAllCardCount;					//�˿���Ŀ
		BYTE				iBeenPlayGame;					//�Ѿ���Ϸ�ľ���
		BYTE				SysCard[28];					//��ʣ�µ���
		BYTE				iSysCardCount;					//��ʣ��������
		BYTE                bEXFlag2[12];                   //zht 2011-4-12, ��ʹ�õ��ֶΣ�ռλ��
		LLONG				iLimitNote;						//��ע�ⶥֵ   ��ע / ��ע
		INT					iBaseNote;						//��ע	       ��ע
		INT                 iGuoDi;                         //����
		LLONG               iLimtePerNote;                  //��ע���ޣ���ע��Ҫ * 2
		INT                 iCurNote;                       //��һλ�����עֵ���Ѱ�ע����
		INT					iRoomBasePoint;					//���䱶��
		INT					iRunPublish;					//���ܿ۷�
		BYTE                bNtPeople;                      //ׯ��λ��
		BYTE				iUpGradePeople;					//��עλ��
		BYTE				iOutCardPeople;					//���ڳ����û�
		BYTE				iFirstOutPeople;				//�ȳ��Ƶ��û�
		LLONG				iTotalNote[PLAY_COUNT];			//�û���ǰ��ע
		LLONG               iThisGameNote[PLAY_COUNT];		//������Ϸ��ע��(��ע)
		LLONG				iTotalGameNote[PLAY_COUNT];
		INT                iMing[PLAY_COUNT];              //����״̬
		bool                bOpenLose[PLAY_COUNT];          //��ұ����Ƿ�����
		bool                bIsGiveUp[PLAY_COUNT];                      //����Ƿ����
		bool                bIsFirstNote;                   //�Ƿ�Ϊ��һ����ע
		INT				    iTimeRest;						// ��ʱ��ʵ��ʣ�µ�ʱ�䣬���ڱ�ʾ��������ʱʣ��ʱ��ֵ
		CHAR                strCityText[PLAY_COUNT][50];    //������������ַ
		CHAR                strUserIDText[PLAY_COUNT][50];  //����������ID
		bool                bSuperUser[PLAY_COUNT];         //������Ƿ��ǳ������
		tagUserResult		tag;
		BYTE				iUserCardCount[PLAY_COUNT];	    //�û����ϵ�����
		BYTE				iUserCard[PLAY_COUNT][5];	    //�û����ϵ���
		bool                bOpenLoser[PLAY_COUNT];
		INT                bPlayer[PLAY_COUNT];
		BYTE                byWin[PLAY_COUNT];
		INT                bShowIdOrPlay;
		GameStation_4()
		{
			memset(this,0,sizeof(GameStation_4));
		}
	};

	//��Ϸ״̬���ݰ�	�� �ȴ����̿�ʼ״̬  ��
	struct GameStation_5 : GameStationBase
	{
		INT                 bShowIdOrPlay;
		bool                bSuperUser[PLAY_COUNT];             //������Ƿ��ǳ������
		//��Ϸ�汾
		BYTE				iVersion;						//��Ϸ�汾��
		BYTE				iVersion2;						//��Ϸ�汾��
		BYTE				iBeenPlayGame;					//�Ѿ���Ϸ�ľ���
		BYTE                bEXFlag[12];                    //��ʹ�õ��ֶΣ�ռλ��
		//��Ϸ��Ϣ
		BYTE				iThinkTime;						//��Ϸ����
		BYTE				iBeginTime;						//��Ϸ����
		BYTE				iAllCardCount;				//�˿���Ŀ
		BYTE                bEXFlag2[12];                    //��ʹ�õ��ֶΣ�ռλ��
		INT					iRoomBasePoint;					//���䱶��
		INT					iRunPublish;					//���ܿ۷�
		INT					iBaseNote;						//��ע	��ע
		INT                 iGuoDi;                         //����
		LLONG				iLimitNote;						//��ע�ⶥֵ   ��ע / ��ע
		LLONG               iLimtePerNote;                   //��ע���ޣ���ע��Ҫ * 2
		//����״̬����
		INT					iUpGradePeople;					//ׯ��λ��
		GameStation_5()
		{
			memset(this,0,sizeof(GameStation_5));
		}
	};

	//�û�ͬ����Ϸ
	struct UserArgeeGame
	{
		BYTE				iBeginStation;					//��ʼλ��
		BYTE				iEndStation;					//����λ��
		BYTE				iPlayCount;						//��Ϸ����
		BYTE				iCardCount;						//�˿���Ŀ
		UserArgeeGame()
		{
			memset(this,0,sizeof(UserArgeeGame));
		}
	};

	//��Ϸ��ʼ
	struct BeginUpgradeStruct
	{
		INT                 bIsShowIdOrPlay;                  //��ʾid�������
		bool                bSuperUser[PLAY_COUNT];           //������Ƿ��ǳ������
		CHAR                strCityText[PLAY_COUNT][50];
		CHAR                strUserIDText[PLAY_COUNT][50];
		BYTE                bNtStation;                      //ׯ�� 
		BYTE				AllCardCount;					 //һ��ʹ�õ�����
		BYTE				SysCard[54];					 //��ǰ��
		INT					iBaseNote;						 //��ע	
		INT                 iGuoDi;                          //����
		LLONG				iLimitNote;						 //���ע��
		LLONG               iLimtePerNote;                   //��ע���ޣ���ע��Ҫ * 2   
		BeginUpgradeStruct()
		{
			memset(this,0,sizeof(BeginUpgradeStruct));
		}
	};
	//�ռ����붯��
	struct CollectJetonStruct
	{
		BYTE bDeskStation;
		INT JetonTotal;
		CollectJetonStruct()
		{
			memset(this,0,sizeof(CollectJetonStruct));
		}
	};
	//�������ݰ�
	struct SendCardStruct
	{
		BYTE bCard[PLAY_COUNT][MAX_CARD_COUNT];	//�Ʊ��
		BYTE bCardCount[PLAY_COUNT];     //������
		SendCardStruct()
		{
			memset(this,0,sizeof(SendCardStruct));
		}
	};
	//��Ϸ��ʼ���ݰ�
	struct BeginPlayStruct
	{
		BYTE				iOutDeskStation;				//���Ƶ�λ��
		BYTE                bNtPeople;                      //ׯ��λ��
		BYTE				byUserData[PLAY_COUNT][2];		//ȷ������Ƿ�����˺���ҵ��ƴ�С������ֻ����������)
		BeginPlayStruct()
		{
			memset(this,0,sizeof(BeginPlayStruct));
		}
	};

	//�û��������ݰ� �������������
	struct OutCardStruct
	{
		BYTE				iCardList[39];					//�˿���Ϣ
		INT					iCardCount;						//�˿���Ŀ
		OutCardStruct()
		{
			memset(this,0,sizeof(OutCardStruct));
		}
	};



	//�û��������ݰ� ������ͻ��ˣ�
	struct OutCardMsg
	{
		BYTE				iCardCount;						//�˿���Ŀ
		BYTE				bDeskStation;					//��ǰ������
		BYTE				iCardList[39];					//�˿���Ϣ
		INT					iNextDeskStation;				//��һ������
		OutCardMsg()
		{
			memset(this,0,sizeof(OutCardMsg));
		}
	};

	//��Ϸ����ͳ�����ݰ�
	struct GameEndStruct
	{
		BYTE				iUpGradeStation;						//ׯ��λ��
		BYTE				bCard[PLAY_COUNT][MAX_CARD_COUNT];		//�Ʊ��
		BYTE				iCardShape[PLAY_COUNT];
		INT					iUserState[PLAY_COUNT];					//���״̬(��ǰ����,������)
		LLONG				iTurePoint[PLAY_COUNT];					//ׯ�ҵ÷�
		LLONG				iChangeMoney[8];
		GameEndStruct()
		{
			memset(this,0,sizeof(GameEndStruct));
		}
	};

	//��Ϸ����ͳ�����ݰ�
	struct GameCutStruct
	{
		INT					bDeskStation;					//�˳�λ��
		LLONG					iTurePoint[PLAY_COUNT];					//ׯ�ҵ÷�
		LLONG					iChangeMoney[8];
	};

	struct HaveThingStruct
	{
		BYTE pos;
		CHAR szMessage[61];
	};

	struct LeaveResultStruct
	{
		BYTE bDeskStation;
		INT bArgeeLeave;
	};

	struct MessageStruct	//�ͻ��˵�������
	{
		CHAR Message[200];
	};

	struct StopResultStruct
	{
		bool bArgee;
	};
	//�������ݰ�
	struct SetCardStruct
	{
		BYTE bCardCount;//������
		BYTE bPlayCount;//�����
		INT  nPlayCardCount[8]; //ÿ�����������
		BYTE bPlayCard[8][60]; // ÿ����ҵ���
		SetCardStruct()
		{
			memset(this,0,sizeof(SetCardStruct));
		}
	};
	/*
	//��ť����
	enum BUTTON_TYPE
	{
	BUTTON_TYPE_NONE = -1,
	BUTTON_TYPE_GIVEUP,//����
	BUTTON_TYPE_100NOTE,//100ע
	BUTTON_TYPE_200NOTE,//200ע
	BUTTON_TYPE_400NOTE,//400ע
	BUTTON_TYPE_ADD,	//��ע
	BUTTON_TYPE_FOLLOW,//����
	BUTTON_TYPE_SHUTTLE,//����
	};*/

	//�û�������Ϣ
	struct tagUserProcess
	{
		BYTE iVrebType;//������İ�ť
		INT  iNote;		//��ע��
		INT  bMing;     //���Ʒ�?
		tagUserProcess()
		{
			memset(this,0,sizeof(tagUserProcess));
		}
	};

	struct tagCompareCard :  public tagUserProcess 
	{
		BYTE byComparedStation ;  //���Ա��Ƶ����   
	};

	struct tagCompareCardResult
	{
		BYTE iNt;		//������
		BYTE iNoNt;     //������
		BYTE iLoster;	//����
		BYTE bWinner;		// Add By ZXD 20090820 ʤ���ߣ������ƽ�������һ�����ϴﵽ��ע���ޣ����ʱ�ͻ�����Ҫ֪��ʤ������˭
		INT iCurNote;		//��ǰ�����ע��
		INT iNote;          //��ǰ��Чע��
		BYTE bAddStyle;   //����
		bool bGameFinish;
		tagCompareCardResult()
		{
			memset(this,0,sizeof(tagCompareCardResult));
		}
	};

	//��������û�������Ϣ
	struct SupersedeUserNote
	{
		BYTE bDeskStation;//�����λ��
		BYTE iVrebType;//������İ�ť
		INT  iNote;		//��ע��
		SupersedeUserNote()
		{
			memset(this,0,sizeof(SupersedeUserNote));
		}
	};
	//�Q���Ɣ����Y��
	struct ChangeBackStruct
	{
		BYTE bDeskStation;//λ��
		BYTE bSourceCard;//Դ��
		BYTE bResultCard;//�Y����
		INT  bExtVal;	//�Ƿ���Q�ɹ�
		ChangeBackStruct()
		{
			memset(this,0,sizeof(ChangeBackStruct));
		}
	};

	//�����û�Ѻע
	struct NoteResult
	{
		BYTE bAddStyle;		          //��ע����
		BYTE iOutPeople;	          //��ע��
		INT iCurNote;		          //��ǰ�����ע��
		INT iNote;                    //��ǰ��Чע��
		BYTE bNextDeskStation;
		LLONG  iLimiteNote;           //�����ע
		NoteResult()
		{
			memset(this,0,sizeof(NoteResult));
		}
	};
	struct GameFinishNotify
	{
		CHAR name[PLAY_COUNT][61];
		//int dwUserID[PLAY_COUNT];         //��ӡuserid
		INT	iBasePoint;
		BYTE iStyle;			//��Ϸ�����Ƿ�Ϊǿ��,������������
		INT	iWardPoint[PLAY_COUNT];
		//LLONG iTotalPoint[4];
		BYTE iCardShape[PLAY_COUNT];
		LLONG iMoney[PLAY_COUNT];
		GameFinishNotify()
		{
			memset(this,0,sizeof(GameFinishNotify));
		}
	};
	//�����û����ƵĽ��
	struct	SuperUserControlReslut
	{
		BYTE    bWinStation;		//��λ��
		BYTE    bLoseStation;		//Ӯλ��
		SuperUserControlReslut()
		{
			memset(this,-1,sizeof(SuperUserControlReslut));
		}
	};
	//-------------------------------------------------------------------------------
	/// ֪ͨ�ͻ����Ƿ�Ϊ�����ͻ���״̬��Ϣ�ṹ
	typedef struct SuperUserState
	{
		BYTE byDeskStation;      /**< ���λ�� */
		bool bEnable;           /**< �Ƿ�ͨ */
		SuperUserState()
		{
			memset(this,0,sizeof(SuperUserState));
		}
	}SUPERSTATE;


	/*
	//�����������
	struct tagUserGiveUp
	{
	bool bShowOnly;		//����ʾ
	bool bAddFlag;	//�ӱ����	
	BYTE bAddTime;	//Ѻע����
	BYTE bAddStyle;	//��ע����	
	BYTE iGiveUpPeo;//������
	BYTE iOutPeople;//��ע��
	//	tagButtonStatus ButtonPara;//��ť����
	};
	*/
	/********************************************************************************/

	// ���ݰ���������ʶ

	/********************************************************************************/
	enum{
		ASS_UG_USER_SET			=		50,				//�û�������Ϸ
		ASS_BEGIN_UPGRADE		=		51,				//������Ϸ��ʼ
		ASS_SEND_CARD			=		52,				//������Ϣ
		ASS_SEND_FINISH			=		54,				//�������
		ASS_GAME_PLAY			=		59	,			//��ʼ��Ϸ
		ASS_NEW_TURN			=		64	,			//��һ�ֿ�ʼ
		ASS_CONTINUE_END		=		65	,			//��Ϸ����
		ASS_NO_CONTINUE_END		=		66	,			//��Ϸ����
		ASS_CUT_END				=		67	,			//�û�ǿ���뿪
		ASS_SALE_END			=		68	,			//��Ϸ��ȫ����
		
		
		ASS_VREB_CHECK			=		179	,			//�û�����
		
		ASS_SHUTTLE				=		80	,			//����
		ASS_ADD_NOTE			=		82	,			//��ע
		ASS_GIVE_UP				=		86	,			//����
		ASS_MODIFY_PLAY_NOTE	=		90	,			//�����û�����ע
		ASS_NOTE				=		91	,			//��ע
		ASS_NOTE_RESULT			=		92	,			//��ע���
		ASS_COLLECT_JETON		=		94	,			//�ռ�����
		ASS_BIPAI_RESULT        =        95  ,            //���ƽ��
		ASS_GM_SUPERCLIENT		=		100	,			//�����͑���
		ASS_FINISH_COMPARE      =        101 ,            
		ASS_AI_WIN              =        102 ,            //�����˱�Ӯ
		ASS_SET_TEST_CARD       =        120 ,            //�������趨��
		ASS_SUPER_RESULT        =        121 ,            //���˿��ƽ����Ϣ
		ASS_SUPER_USER          =        122  ,           //������֤��Ϣ
	};
}

#pragma pack(pop)
#endif // TRACTORUpgradeMessage_h__
/********************************************************************************/