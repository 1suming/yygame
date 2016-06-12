#ifndef __DriftingCar_Game_Table_Logic_H__
#define __DriftingCar_Game_Table_Logic_H__

#include "HNNetExport.h"
#include "HNLogicExport.h"
#include "DriftingCarMessageHead.h"
#include <array>
#include <stdexcept>
namespace DriftingCar
{
	template <typename T, typename TArray>
	void CopyArray(const T* pSou, size_t size, TArray &array)
	{
		if(size > array.size())
			throw std::out_of_range("out of range") ;
		std::copy(pSou,pSou+size, array.begin());
	}

	class GameTableUICallback;

	class GameTableLogic: public HN::HNGameLogicBase
	{
	public:
		GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool bAutoCreate);
		virtual ~GameTableLogic();	
	public:
		//�����Ϣ
		virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)override;//��Ϸ��Ϣ
		virtual void dealGameStationResp(void* object, INT objectSize)override;							//������Ϸ������Ϣ
		virtual void dealUserAgreeResp(MSG_GR_R_UserAgree* agree)override;								//��Ϸͬ����Ϣ
		virtual void dealGameStartResp(BYTE bDeskNO)override;											//��Ϸ��ʼ
		virtual void dealGameEndResp(BYTE bDeskNO)override;												//��Ϸ����	
		virtual void dealGamePointResp(void* object, INT objectSize)override;							//������Ϣ
		virtual void dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)override;			//�������
		virtual void dealUserUpResp(MSG_GR_R_UserUp * userSit, UserInfoStruct* user)override;			//�������
		virtual void dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo)override;							//��Ϸ��Ϣ
		virtual void dealUserCutMessageResp(INT userId, BYTE seatNo)override;							//�����û���Ϣ

		//��Ϸ��Ϣ
		void dealPrepareEndResp(void* object, INT objectSize);
		void dealSuperUserResp(void* object, INT objectSize);	// �����û���Ϣ
		void dealSuperSendResp(void* object, INT objectSize);	// �����û���ͻ��˷��ͽ��
		void dealUpdateBetResp(void* object, INT objectSize);	// ������ע
		void dealGameFreeResp(void* object, INT objectSize);	// ������Ϣ
		void dealBeginBetResp(void* object, INT objectSize);	// ��ʼ��ע
		void dealGameSendResp(void* object, INT objectSize);	// ��ʼ���� ����ƿ�ʼ
		void dealApplyResultResp(void* object, INT objectSize);	// ������ׯ���
		void dealLeaveResultResp(void* object, INT objectSize);	// ������ׯ���
		void dealBetResultResp(void* object, INT objectSize);	// �����ע�����Ϣ
		void dealGameOpenResp(void* object, INT objectSize);	// ������Ϣ ������Ϣ
		void dealWaitNT(void* object, INT objectSize);			// �ȴ�ׯ��

	public:
		/*
		 * 
		 */
		void requestApplyNT();									//������ׯ
		void requestApplyLeaveNT();								//������ׯ
		void requestUserBet(BYTE area, BYTE type);				//������ע
		void requestLeaveTable();								//�����뿪
		void ShowGameEndResult();								//��ʾ�������Ϣ
	protected:

		virtual void initParams();//init member only once.

		virtual void refreshParams();
		
		void clearDesk();							//�����������

		void updateNTInfo(BYTE lSeatNo, int NTCount, LLONG grade);

		void updateUserInfo(BYTE lSeatNo, LLONG grade);

		void addNT(BYTE seatNo);						//������ׯ�б���ׯ�б�����ׯ�ң�

		void addNT(const std::array<BYTE,PLAY_COUNT>&);

		void removeNT(const std::array<BYTE,PLAY_COUNT>&);

		void reloadRecord(const std::array<BYTE, HISTORY_COUNT>& records, int size, BYTE byResult);//������ʷ������¼

		void setBetEnable();							//������ע��ť������

		void setApplyButtonEnabled(BYTE NTseatNo);		//������ׯ���밴ť������

		void ShowWaitNTListCount(BYTE byseatNo);		//��ʾ��ǰ����ڵȴ��б��е�λ��

		void GetHistroyResult(int indexLogos);			//�õ���ע���

	private:
		GameTableUICallback*	_uiCallback;
	private:
		enum
		{
			Result_NON,		//û����ע
			Result_Win,		//ѹ����
			Result_Lose,	//û��ѹ��
		};
		std::array<BYTE,PLAY_COUNT> _NTWaitList;	//��ׯ�����б�	
		std::array<BYTE,HISTORY_COUNT> _historyArray;//������ʷ
		std::array<LLONG, BET_ARES> _AreaAllBetArray;//����������ע����
		std::array<LLONG, BET_ARES> _UserBetSumArray;//����ڸ�������ע����(�������)
		std::array<BYTE, SEND_COUNT> _CardStyleArray;//����
		std::array<LLONG, BET_ARES> _PreUserBetArray;//�����ע����ֵ
		std::array<INT,TIME_COUNT>	_RunTimeArray;	 //�����ʱ��
		std::array<bool, BET_ARES>  _arrayUserBetArea;//�����ע����

		LLONG _lReuslt[4];							 //������Ϣ

		BYTE _NTSeatNo;								 //ׯ����λ��
		BYTE _NTCount;								 //��ׯ����
		LLONG _NTGrade;								 //��ׯ�ɼ�							

		LLONG _SumAllBet;							 //�ܵ���ע����
		
		LLONG _UserGrade;							 //��ҳɼ�

		BYTE _GameState;							 //��Ϸ״̬
		BYTE _FreeTime;								 //����ʱ��
		
		BYTE _WinIndex;								 //����λ�� ����עλ�ò�ͬ

		BYTE _BetTime;								 //��עʱ��
		BYTE _SendTime;								 //����ʱ��
		BYTE _RemaindTime;							 //ʣ��ʱ��

		BYTE _MaxNTPeople;							 //�����ׯ����

		BYTE _byHistory;							 //0��ʾû����ע�� 1��ʾ��������ע����2��ʾû�п�����ע����
			
	};
}


#endif