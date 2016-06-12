#ifndef __BJLGameTableLogic_h__
#define __BJLGameTableLogic_h__

#include "BJLMessageHead.h"
#include "BJLGameTableUICallback.h"
#include "HNNetExport.h"
#include "HNLogicExport.h"

using namespace std;

namespace BJL
{
	class GameTableLogic : public HNGameLogicBase
	{
	public:
		// ���캯��
		GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool autoCreate);

		// ��������
		virtual ~GameTableLogic();

	public:
		// ����վ��
		void requestStandUp();
		
		// ������ע
		void requestBet(int betType, int betArea);

		// ������ׯ����ׯ
		void requestBeNT(bool isNT);

		// ����������������߸ոս��뷿��ʱ����ָ��������ݰ�
		void requestGameData();

		// ����ׯ���б�
		vector<string> requestNTList();

		// ����·��
		std::vector<LuziData> getRecords();

		// ��ȡ����Ӯ
		int* getWinArea();

		// ��ȡ����ʱ��
		int getFreeTime();

		// ��ȡ��Ϸ״̬
		BYTE getGameStatus();

		// �Ƿ���ׯ�һ���������ׯ���б���
		bool isInNTList(BYTE seatNo);

	public:
		// �����ϡ���ׯ��Ϣ
		void sendBeNT(bool isNT);

		// ������ע��Ϣ
		void sendBet(BYTE deskNo, int betType, int betArea);

	public:
		// ͬ�⿪ʼ
		virtual void dealUserAgreeResp(MSG_GR_R_UserAgree* agree) override;

		// ��Ϸ��ʼ
		virtual void dealGameStartResp(BYTE bDeskNO) override;

		// ��Ϸ����
		virtual void dealGameEndResp(BYTE bDeskNO) override;

		// �������
		virtual void dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;

		// ���վ��
		virtual void dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;

		// ��Ϸ��Ϣ
		virtual void dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo) override;

		// ��Ϸ״̬
		virtual void dealGameStationResp(void* object, INT objectSize) override;

		// ��Ϸ��Ϣ
		virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize) override;

	public:
		// ������ׯ
		void dealApplyNTResp(void* object, INT objectSize);

		// ��ׯ�ȴ�
		void dealNoNTWaitResp(void* object, INT objectSize);

		// ��Ϸ��ʼ��ע
		void dealNoticeBetResp(void* object, INT objectSize);

		// ��Ϸ��ע���
		void dealBetResultResp(void* object, INT objectSize);

		// ��ʼ����
		void dealSendCardResp(void* object, INT objectSize);

		// ��ʾ�н�����
		void dealShowWinAreaResp(void* object, INT objectSize);

		// ��Ϸ����
		void dealGameEndResp(void* object, INT objectSize);
	
	private:
		// ����ص��ӿ�
		GameTableUICallback* _uiCallback;

		// ׯ����λ��
		BYTE _NTSeatNo;

		// ׯ�ҽ��
		LLONG _NTMoney;

		// ׯ��Ӯ�Ľ��
		LLONG _NTWinMoney;

		// ׯ�Ұ���
		LLONG _NTPlayCount;

		// ��ׯ�б�
		BYTE  _NTList[PLAY_COUNT];	            

		// ��ҽ��
		LLONG _userMoney;

		// �����Ӯ
		LLONG _userWin;

		// ����ܵ÷�
		LLONG _userTotalScore;

		// �������÷�
		LLONG _userAreaScore[BET_ARES];

		// ���������
		float _userPercent;

		// ׯ�����ٽ��
		LLONG _NTMinLimit;

		// �����ע����
		LLONG _maxBet;

		// ��Ϸ״̬
		BYTE _gameStatus;

		// ��ע���
		BYTE _betSeatNo;
		
		// ��ע����
		int	_betArea;

		// ��ע����
		LLONG _betMoney;

		// ���ѵ�ǰ��ע��
		LLONG _roundTotaBetMoney;
		
		// ��ʵ��ҵ���עֵ
		LLONG _areaBet[BET_ARES];

		// ����ÿ�������µ�ע��
		LLONG _roundAreaBet[BET_ARES];

		// �����ע���
		LLONG _userAreaBet[BET_ARES];
		
		// ÿ���������¶���ע
		LLONG _areaMaxBet[BET_ARES];

		// ��ҵ���,0Ϊ�У�1Ϊׯ
		BYTE _userCard[2][3];

		// ׯ������,Ԫ��0ǰ�����Ƶ�ֵ��Ԫ��1����ֵ��Ԫ��2������Ԫ��3���ӣ�Ԫ��4��
		int _NTCardType[5];				

		// �м�����
		int _playerCardType[5];				
		
		// ��Ϸ��ӮǮ���� ֵ=0 ��ʾû�н�  ����0��ʱ�� ��ʾ�н���  0�У�1��������2�ж��ӣ�3ׯ��4ׯ������5ׯ���ӣ�6�ͣ�7ͬ���
		int _winArea[BET_ARES];

		// ·����Ϣ
		LuziData _records[MAXCOUNT];

		// ��עʱ��
		int _betTime;

		// ����ʱ��
		int _openTime;

		// ����ʱ��
		int _freeTime;

		// ��ʾ����ʱ��
		int _showWinTime;
	};
}

#endif // __BJLGameTableLogic_h__
