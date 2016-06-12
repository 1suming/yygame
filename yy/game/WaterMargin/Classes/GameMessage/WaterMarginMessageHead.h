#ifndef __HN_WaterMargin_UpgradeMessage_h__
#define __HN_WaterMargin_UpgradeMessage_h__

#include "HNNetExport.h"

namespace WaterMargin
{
	//��ϷID
	enum Game_ID
	{
		NAME_ID = 33000401,
	};

	//�汾����
#define GAME_MAX_VER					1						//������߰汾
#define GAME_LESS_VER					1						//������Ͱ汾
#define GAME_CHANGE_VER					0						//�޸İ汾

	//֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME
	//#define GAME_TABLE_NAME					"MatchTable"

	//��Ϸ�ڲ������汾��
#define DEV_HEIGHT_VERSION				3				//�ڲ������߰汾��(ÿ����һ��LOW_VERSIONG+1)
#define DEV_LOW_VERSION					1				//�ڲ������Ͱ汾��

	//��Ϸָ��ID
	enum CMD_ID
	{
		// ���ݰ���������ʶ
		ASS_VREB_CHECK					=  179,				//�û�����
		/**************************����ˡ���>�ͻ�����Ϣ**************************/
		ASS_S_GAME_BEGIN				=  	51,				//��Ϸ��ʼ

		ASS_S_BUY_COIN_RESULT			=  	60,				//�Ϸ��·ֽ����Ϣ
		ASS_S_ADD_NOTE_RESULT			=  	61,				//��Ҽ�ע���
		ASS_S_ROLL_RESULT				=  	62,				//�������
		SUB_S_MALI_START				=  	63,				//������ʼ
		SUB_S_GETBIBEI_RESULT			=  	64,				//��ȡ�ȱ����
		ASS_S_GETSCORE_RESULT			=  	65,				//��ȡ�÷ֽ��
		ASS_S_BIBEI_RESULT				=  	66,				//�ȱ�ѹ�ֽ��

		/**************************�ͻ��ˡ���>�������Ϣ**************************/
		ASS_C_BUY_COIN					=  	80,				//�Ϸ��·���Ϣ	
		ASS_C_ADD_NOTE					=  	81,				//��Ҽ�ע
		ASS_C_START_ROLL				=  	82,				//������Ϸ����
		ASS_C_GET_MALI					=  	83,				//��ȡ����
		ASS_C_GET_BIBEI					=  	84,				//��ȡ�ȱ�
		ASS_C_GET_SCORE					=  	85,				//��ȡ�÷�
		ASS_C_BIBEI						=  	86				//�ȱ�ѹ��

	};

#define SUPER_PLAYER                    10                         //�������
#define MAX_NAME_INFO                   256

	//��Ϸ״̬
	enum Game_State
	{
		GS_WAIT_SETGAME					= 0,				//�ȴ���������״̬
		GS_WAIT_ARGEE					= 1,				//�ȴ�ͬ������
		GS_PLAY_GAME					= 20,				//����״̬
		GS_WAIT_BACK					= 21,				//�ȴ���ѹ����
		GS_WAIT_NEXT					= 23				//�ȴ���һ�̿�ʼ 
	};

	enum Game_Const
	{
		PLAY_COUNT				    =   1,              //��Ϸ����
	};


	/********************************************************************************/

	//��Ϸ���ݰ�
	/********************************************************************************/  
	/*-------------------------------------------------------------------------------*/
	#pragma pack(1)
	//��Ϸ״̬�������ݰ�
	struct		GameStation_Base
	{
		BYTE				iVersion;						//��Ϸ�汾��
		BYTE				iVersion2;						//��Ϸ�汾��


		BYTE			byGameStation;					//��Ϸ״̬
		int				iLineCount;						//��ע����
		LLONG			i64UserMoney;					//�û��Ľ����

		GameStation_Base()
		{
			memset(this, 0 , sizeof(GameStation_Base));
		}
	};
	/*-------------------------------------------------------------------------------*/
	//��Ϸ��ʼ���ݰ�
	struct		GameBegin_Data
	{
		bool	bStart;
		GameBegin_Data()
		{
			memset(this, 0 , sizeof(GameBegin_Data));
		}
	};
	/*-------------------------------------------------------------------------------------*/
	//�����ӵ�
	struct		CMD_C_BuyCoin
	{
		bool			bAddOrMove;		//�ϻ�������

		CMD_C_BuyCoin()
		{
			memset(this, 0, sizeof(CMD_C_BuyCoin));
		}
	};
	/*-------------------------------------------------------------------------------------*/
	//�����ӵ���Ŀ
	struct		CMD_S_BulletCount
	{
		BYTE			byDeskStation;		//���·���λ��
		int				iUserScore;			//��������ܷ���
		int				iAllCellScore;		//�����ע��
		LLONG			i64UserMoney;		//������ϵĽ��
		CMD_S_BulletCount()
		{
			memset(this, 0, sizeof(CMD_S_BulletCount));
		}
	};
	/*-------------------------------------------------------------------------------*/
	//��ע����
	struct	CMD_S_AddScore
	{
		//��ǰ��ע
		BYTE		byDeskStation;
		bool		bUp;
		int			iAddScore;	//��Ҽ�ע��
		int			iUserScore;	//��ҵ��Ϸ�����
		CMD_S_AddScore()
		{
			memset(this,0,sizeof(CMD_S_AddScore));
			byDeskStation = 255;
		}
	};
	/*-------------------------------------------------------------------------------*/
	//��ʼ����
	struct	CMD_C_StartRoll
	{
		//��ǰ��ע
		BYTE		byDeskStation;
		CMD_C_StartRoll()
		{
			memset(this,-1,sizeof(CMD_C_StartRoll));

		}
	};
	/*-------------------------------------------------------------------------------*/
	//������Ϣ
	struct CMD_S_TypeScroll
	{
		//��ǰ�Ϸ�
		int                            iUserScore;
		//��Ӯ����
		int                            iWinScore;
		//ͼ������
		int                            iTypeImgid[15];

		CMD_S_TypeScroll()
		{
			memset(this,0,sizeof(CMD_S_TypeScroll));
		}
		//
	};
	/*-------------------------------------------------------------------------------*/
	//С������Ϸ
	struct CMD_S_MaLi
	{
		//�Ƿ����Ͽ�ʼС����
		bool                           m_NowBegin;
		//��������
		int                            m_GameCount;
		//4��ͼ
		int                            m_ThreeTypeId[4];
		//תתͼ
		int                            m_TypeIndex;
		//��ǰ��
		int                            m_AddScore;

		CMD_S_MaLi()
		{
			memset(this,0,sizeof(CMD_S_MaLi));
		}
	};
	/*-------------------------------------------------------------------------------*/
	//С������Ϸ
	struct CMD_C_GetMaLi
	{
		BYTE		byDeskStation;

		CMD_C_GetMaLi()
		{
			byDeskStation = 255;
		}
	};
	/*-------------------------------------------------------------------------------*/
	//��ȡ�ȱ�
	struct CMD_C_GetBiBei
	{
		BYTE		byDeskStation;

		CMD_C_GetBiBei()
		{
			byDeskStation = 255;
		}
	};
	/*-------------------------------------------------------------------------------*/
	//�ȱ�����
	struct CMD_S_BiBeiData
	{
		//�ȱ�����
		int                            iBiBeiData[10];
		CMD_S_BiBeiData()
		{
			memset(this,0,sizeof(CMD_S_BiBeiData));
		}
	};
	/*-------------------------------------------------------------------------------*/
	//��ȡ�÷�
	struct CMD_C_GetScore
	{
		BYTE			byDeskStation;
		int             iGetType;

		CMD_C_GetScore()
		{
			memset(this,0,sizeof(CMD_C_GetScore));	
		}
	};
	/*-------------------------------------------------------------------------------*/
	//Ӯȡ����
	struct CMD_S_WinScore
	{
		//��ǰӮ��
		int                            iWinScore;
		//��÷�ʽ
		int                            iGameState;
		CMD_S_WinScore()
		{
			memset(this,0,sizeof(CMD_S_WinScore));
		}
	};
	/*-------------------------------------------------------------------------------*/
	//��ұȱ�
	struct CMD_C_BiBeiStart
	{
		//�����С���,0,��,1,С,2,��
		int		iMaxOrOther;
		//�ȱ���ʽ,0,�ȱ�,1,��ȱ�,2,ȫ�ȱ�
		int		iBiBeiType;
		CMD_C_BiBeiStart()
		{
			memset(this,0,sizeof(CMD_C_BiBeiStart));
		}
	};
	/*-------------------------------------------------------------------------------*/
	//�ȱ����
	struct CMD_S_BiBeiRelult
	{
		//����һ
		int			iS1;
		//���Ӷ�
		int			iS2;
		//ͼ������
		int			iWinScore;
		CMD_S_BiBeiRelult()
		{
			memset(this,0,sizeof(CMD_S_BiBeiRelult));
		}
	};

#pragma pack()
}
	//-------------------------------------------------------------------------------

	/********************************************************************************/
#endif
