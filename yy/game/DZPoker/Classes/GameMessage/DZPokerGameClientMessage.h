#ifndef __DZPokerGameClientMessage_h__
#define __DZPokerGameClientMessage_h__

#include "DZPokerUpgradeMessage.h"
#include "HNBaseType.h"

namespace DZPoker
{
	/************************************************************************/
	/* �ͻ��˶������Ϣ�ṹ�����ṩ�ͻ���ʹ��                                  */
	/************************************************************************/

	
	// ���������Ϣ
	struct THandCard
	{
		BYTE byUser;
		BYTE byCards[2];

		THandCard()
		{
			byUser = INVALID_DESKNO;
			memset(byCards, 0x00, sizeof(byCards));
		}
	};

	struct TableInfo
	{
		INT					iOffsetStation;						// λ��ƫ����
		BYTE				bWaitTime;							// �ȴ�ʱ��
		BYTE				bGameStatus;						// ��Ϸ״̬
		BYTE				byMeStation;						// �Լ���λ��
		BYTE				byNTUser;							// ׯ��λ��
		BYTE				bySmallBlind;						// Сäעλ��
		BYTE				byBigBlind;							// ��äעλ��
		emTypeCard			nTypeCard;							// ��������
		BYTE				byHandCard[PLAY_COUNT][2];			// ��������
		INT					iHandCardCounts;					// ������
		BYTE				byCommunityCard[MAX_HANDCARDS_NUM];	// �������
		INT					iCommunityCardCounts;
		TBetPool			tagBetPool;							// ��ע�߳�
		BYTE				bEableWatch;						// �Ƿ������Թ�
		bool				bWatch;								// ����ǲ����Թ���
		TDeskCfg			tagDeskCfg;							// ����������Ϣ
		bool				bHaveUser[PLAY_COUNT];				// ���Ϊ��
		LLONG				iMoneys[PLAY_COUNT];				// ��¼ÿ����Ҵ�����
		bool				bAutoStartTimer;					// �Զ���ʼʱ����
		bool				bIsOffline;							// �Ƿ�Ϊ�����ػ����
		BYTE				byTokenUser;						// �õ����Ƶ����
		INT					iUserStation;						// �Ƿ������(�÷�����)
		bool				bAllIn[PLAY_COUNT];					// ȫ�����
		bool				bFold[PLAY_COUNT];					// �������
		LLONG				nBetMoney[PLAY_COUNT];				// ÿ�������ע��� 
		bool				bSoundPlay;							// ���ֿ���
		emType				emLastOperation[PLAY_COUNT];		// ��¼ÿ����Ҳ�������
		std::stack<BYTE>	stackLastStation;					// ��¼��һ���������
		LLONG				nCurrentMoney;						// ��ǰ��ע���
		bool				bTrusteeship[4];					// �йܱ�־ 0: �Զ���ע(ֻ��һ��)  1: �Զ����κ�ע(ÿ�ֶ���ע)  2: �Զ�����  3: �Զ�����(�ϼ����˹��ƾ͹��ƣ�û�˹��ƾ�����)
		LLONG				nCallMoney;							// ��ע���
		TResult				tagResult;							// ������Ϣ
		bool				bTuoGuan;							// �Ƿ����й�״̬
		BYTE				byBackCard[MAX_BACKCARDS_NUM];		// ��������
		INT					iBackCardNums;						// ��������
		BYTE				bDeskIndex;							// ���ӵı��
		bool				bAutoCreate;						// �Ƿ��Զ���������
	};

	// �û���Ϣ
	struct UserInfoForGame
	{
		CHAR			szNickName[61];							// �ǳ�
		LLONG			i64Money;								// �û����
		UINT     		dwUserID;								// ID ����
		UINT			dwUserIP;								// ��¼IP��ַ
		UINT			bLogoID;								// ͷ�� ID ����
		bool			bBoy;									// �Ա�
		bool            bIsRobot;								// �Ƿ������
		BYTE			bGameMaster;							// ����ȼ�
		BYTE			bDeskNO;								// ��Ϸ����
		BYTE			bDeskStation;							// ����λ��
		CHAR			szCity[61];								// ���ڳ���
	};

}


#endif // __GameClientMessage_h__
