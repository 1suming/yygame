#ifndef __GoldenFlowerGame_Table_Logic_H__
#define __GoldenFlowerGame_Table_Logic_H__

#include "GoldenFlowerMessageHead.h"

#include "HNNetExport.h"
#include "HNLogicExport.h"

namespace goldenflower
{
	class GameTableUICallback;

	/*
	 * GoldenFolower
	 * game table logic deal unit.
	 */
	class GameTableLogic: public HN::HNGameLogicBase
	{
	public:
		// ���캯��
		GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool bAutoCreate);
	
		// ��������
		virtual ~GameTableLogic();

	protected:
		// ������Ϸ��Ϣ
		virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize);

		// ��Ϸ��ʼ
		void dealGameBeginResp(void* object, INT objectSize);

		// ������Ϣ
		void dealSendCardResp(void* object, INT objectSize);

		// ��ʼ��Ϸ
		void dealGamePlayResp(void* object, INT objectSize);

		// ֪ͨ��Ҷ���
		void dealCallActionResp(void* object, INT objectSize);

		// ��Ϸ��������Ϸ����Ϣ��
		void dealGameEndResp(void* object, INT objectSize);

		// ����У��
		void dealSuperProofResp(void* object, INT objectSize);

		// �������ý��
		void dealSuperSetResultResp(void* object, INT objectSize);

		// ��ҿ��ƽ��
		void dealLookCardResultResp(void* object, INT objectSize);

		// ��Ҹ�ע���
		void dealGenZhuResultResp(void* object, INT objectSize);

		// ��ע���
		void dealJiaZhuResultResp(void* object, INT objectSize);

		// ��ұ��ƽ��
		void dealBiPaiResultResp(void* object, INT objectSize);

		// ������ƽ��
		void dealGiveUpResultResp(void* object, INT objectSize);

		// ���ͬ����Ϸ��ƽ̨��Ϣ��
		virtual void dealUserAgreeResp(MSG_GR_R_UserAgree* agree);

		// ��Ϸ��ʼ��ƽ̨��Ϣ��
		virtual void dealGameStartResp(BYTE bDeskNO);

		// ��Ϸ������ƽ̨��Ϣ��
		virtual void dealGameEndResp(BYTE deskNo);

		// ������£�ƽ̨��Ϣ��
		virtual void dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);
		
		//����Ŷ�����
		virtual void dealQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& user);

		// ���վ��ƽ̨��Ϣ��
		virtual void dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);

		// ��Ϸ��Ϣ��ƽ̨��Ϣ��
		virtual void dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo);

		// ��Ϸ״̬��Ϣ��ƽ̨��Ϣ��
		virtual void dealGameStationResp(void* object, INT objectSize);

		// ��Ϸ״̬��Ϣ���ȴ�ͬ�⣩
		void dealWaitAgreeResp(S_C_GameStation_WaiteAgree* pData);

		// ��Ϸ״̬��Ϣ�����ƣ�
		void dealSendCardResp(S_C_GameStation_SendCard* pData);

		// ��Ϸ״̬��Ϣ����Ϸ�У�
		void dealPlayGameResp(S_C_GameStation_PlayGame* pData);

		//--------------------------�����ӿ�---------------------
		//������Ϣ�㲥
		virtual void dealGameContestNotic(MSG_GR_I_ContestInfo* contestInfo) override;
		//�û�������Ϣ
		virtual void dealGameUserContset(MSG_GR_ContestChange* contestChange) override;
		 //������̭
		virtual void dealGameContestKick() override;
		 //�ȴ���������
		virtual void dealGameContestWaitOver() override;
		 //��������
		virtual void dealGameContestOver(MSG_GR_ContestAward* contestAward) override;

	

	public:
		//�����Ŷ�
		void sendQueue();
		// վ��
		void sendStandUp();
	
		// ����
		void sendUserSit(BYTE byDeskStation);

		// ׼��
		void sendUserReady();

		//������Ϸ
		void enterGame();

		// �����û�
		void loadUsers();

		// ��������
		void clearDesk();

		// ����Ƿ��û�
		void clearInvalidUser();

		// ��ȡ��С����
		LLONG getMinBuy();

		// ��ȡ�����
		LLONG getMaxBuy();

		// ��ȡ����ֵ
		LLONG getPlanBuy();

		// ��ȡ�ҵ���λ��
		BYTE getMeStation();
	
		// ��ע
		void sendNote(LLONG note);

		// ��ע
		void sendAdd(LLONG note);

		//��ע�ı���
		void sendAddBet(int multiple);

		// ��ס
		void sendFollow();

		// ����
		void sendGiveUp();

		// ����
		void sendLook();

		// ��ʾ����ѡ��
		void compareCardReq();

		// ����
		void sendCompare(BYTE seatNo);

		// ��ʾ������ť
	protected:
		// ����ص�
		GameTableUICallback*	_uiCallback;

		// ������Ϣ
		//TableInfo				_tableInfo;

		// ��ǰ��ע��
		LLONG                   _curNote;


		// ����Ƿ�����
		bool playingGame(int state);

		// ������ʼ��
		void initParams();

		// ��������
		void refreshParams();

		// ֹͣ���еȴ�
		void stopAllWait();

		// ��ʼ������
		void initGuoDi();

		// ��ʾ�����Ϣ
		void showUserInfo();

		// ��ʾ����Ի���
		void showBuyMoney(LLONG min, LLONG max, LLONG plan);

		// ��ȡ��һ����ҵ�λ��
		BYTE getNextUserStation(BYTE byStation);

		// ��ȡ��һ����ҵ�λ��
		BYTE getLastUserStation(BYTE byStation);

	private:
		
		// ׯ����λ��
		BYTE                _NTSeatNo;                     

		// ���ֹ���ֵ
		int                 _GuoDi;

		// ���ֵ�ע
		int					_DiZhu;							

		// ���״̬
		int					_UserState[PLAY_COUNT];			

		//������ע���� (�� ���ÿ�������ע�����ܳ������ֵ) ��ע�ģ�����ʾΪ��ע��
		//LLONG				_ShangXian;					

		//���ֶ�ע -����ע����	�ﵽ���� ��Ҫ����	
		LLONG				_DingZhu;						

		//��ǰ����ע;
		LLONG				_ZongXiaZhu;	

		//��ǰ��������ע;
		LLONG				_MyZongXiaZhu;

		//�����ע����
		LLONG				_XiaZhuData[PLAY_COUNT];

		// ��Ϸ״̬
		BYTE                _gameStation;

		// ˼��ʱ�䣨׼����
		BYTE                _thinkTime;

		// ��ʼʱ��
		BYTE                _beginTime;

		// ͬ��״̬
		bool                _agree[PLAY_COUNT];

		// �û�������
		BYTE                _userCardCount[PLAY_COUNT];

		// �û�������
		BYTE                _userCard[PLAY_COUNT][MAX_CARD_COUNT];
		
		// ��ǰ���������
		BYTE		        _CurrHandleSeatNo;

		// �ɷ���
		bool		        _CanLook;

		// �ɷ��ע
		bool		        _CanFollow;		

		// �ɷ��ע
		bool		        _CanAdd[E_CHOUMA_COUNT];

		// �ɷ����
		bool		        _CanOpen;	

		// �ɷ�����
		bool		        _CanGiveUp;			

		public:
			std::string _userName[PLAY_COUNT];                                     //����ǳ�
		public:
			//��¼�Ƿ�Ϊ������׼��
			bool _isReadyQueue;
			// �Ƿ������
			bool	_bContestRoom;
			//�Ƿ�������� 
			bool	_bContestEnd;

	};
}



#endif