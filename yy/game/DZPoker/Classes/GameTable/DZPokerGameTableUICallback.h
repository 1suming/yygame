#ifndef __DZPokerGameTableUICallback_h__
#define __DZPokerGameTableUICallback_h__

#include "HNNetExport.h"
#include "HNLogicExport.h"
#include "DZPokerGameClientMessage.h"
/************************************************************************/
/* ���ͣ��ص��ӿ�                                                        */
/* ���ܣ����������߼����յ���Ϣʱ���ý���ĺ����ӿ�                        */
/* ˵������user������û���û�������������                                */
/* -1�Ĳ������øı�                                                      */
/************************************************************************/

namespace DZPoker
{

	class GameTableUICallback  : public HN::IHNGameLogicBase
	{
	public:
		//��Ҳ�������ҽ�����ʲô����(��ס�����ơ���ע�����ơ����ơ���ʱ������)
		virtual void showUserBet(BYTE byDeskStation) = 0;
		virtual void showUserCall(BYTE byDeskStation) = 0;
		virtual void showUserAdd(BYTE byDeskStation) = 0;
		virtual void showUserCheck(BYTE byDeskStation) = 0;
		virtual void showUserFold(BYTE byDeskStation) = 0;
		virtual void showUserAllIn(BYTE byDeskStation) = 0;

		//��Ҳ�����ť����
		virtual void showBet(bool bVisible, bool bEnable, LLONG betMoney) = 0;
		virtual void showCall(bool bVisible, bool bEnable) = 0;
		virtual void showRaise(bool bVisible, bool bEnable) = 0;
		virtual void showCheck(bool bVisible, bool bEnable) = 0;
		virtual void showFold(bool bVisible, bool bEnable) = 0;
		virtual void showAllIn(bool bVisible, bool bEnable) = 0;
		virtual void showAdd(bool bVisible, bool bEnable) = 0;
		virtual void showSub(bool bVisible, bool bEnable) = 0;
		virtual void showSlider(bool bVisible, bool bEnable, LLONG max, LLONG min, LLONG current) = 0;
		virtual void setSliderValue(LLONG value) = 0;
		virtual LLONG getSliderValue() = 0;

		//��Ҳ�����ť����
		virtual void showBetMoney(bool bVisible, LLONG money) = 0;
		virtual void showCallMoney(bool bVisible, LLONG money) = 0;
		virtual void showRaiseMoney(bool bVisible, LLONG money) = 0;

		//�Զ�������ť
		virtual void showAutoCall(bool bVisible, bool bSelected) = 0;
		virtual void showAutoCallAny(bool bVisible, bool bSelected) = 0;
		virtual void showAutoCheck(bool bVisible, bool bSelected) = 0;
		virtual void showAutoCheckFold(bool bVisible, bool bSelected) = 0;

		//���µ׳�
		virtual void showPot(BYTE index) = 0;
		//������
		virtual void showHandCard(std::vector<THandCard>& handCards) = 0;
		//����
		virtual void showFlopCard(const std::vector<BYTE>& byCards) = 0;
		//ת��
		virtual void showTurnCard(BYTE card) = 0;
		//����
		virtual void showRiverCard(BYTE card) = 0;
		//�����û���Ϣ
		virtual void showUser(BYTE byDeskStation, bool bMe, bool sex) = 0;
		virtual void showUserUp(BYTE byDeskStation, bool bMe) = 0;
		virtual void showUserNickName(BYTE byDeskStation, const std::string& nickName) = 0;
		virtual void showUserMoney(BYTE byDeskStation,LLONG MONEY) = 0;
		//��ʾ������ϵ�����
		virtual void showUserCardType(BYTE seatNo, INT type) = 0;
		//��ʾׯ��
		virtual void showDealer(BYTE byDeskStation) = 0;
		//��ʾСäע
		virtual void showSmallBlind(BYTE byDeskStation) = 0;
		//��ʾ��äע
		virtual void showBigBlind(BYTE byDeskStation) = 0;
		//��ʾ��ע���루�����ϵģ� 
		virtual void showUserBetMoney(BYTE byDeskStation, LLONG money) = 0;
		//�ɽ�
		virtual void showWinPool(const std::vector<std::vector<LLONG>> &winPool) = 0;
		//��ʾ�������
		virtual void showUserHandCard(BYTE byDeskStaion, const std::vector<BYTE>& byCards) = 0;
		//��߹�����
		virtual void upCommunityCard(BYTE index) = 0;
		//����������
		virtual void upUserHandCard(BYTE byDeskStation, BYTE index) = 0;
		//��ʾ���ʣ�����ʱ��
		virtual void showUserLeftTime(BYTE byDeskStation,BYTE byTime, BYTE total, bool isMe) = 0;
		//��ʾ���ֽ�ѧ
		virtual void showHelp(bool bVisible) = 0;
		//�������Ǯ����
		virtual void showUserLackMoney(BYTE byDeskStation) = 0;
		//��������
		virtual void clearDesk() = 0;
		//�����������
		virtual void clearDeskCard() = 0;
		//�뿪����
		virtual void leaveDesk() = 0;
		//��ʾ����׼��
		virtual void showTableReady(bool bVisible) = 0;
		//ת����λ
		virtual void rotateStation(int offset) = 0;
		//��ʾ����Ի���
		virtual void showBuyMoneyDialog(BYTE deskStation, LLONG min, LLONG max, LLONG plan, LLONG total) = 0;
		//֪ͨ��ʾ
		virtual void noticeMessage(const std::string &message) = 0;
		//��������
		virtual void changeToken(bool isMe,BYTE who) = 0;
		//��ʾ������Ϣ
		virtual void showTableInfo(const std::string& tableName) = 0;
		virtual void showMyMoney(LLONG money) = 0;

		//��¼����ʱ
		virtual void isWaitTime(bool isWait) = 0;

	public:	//����ϵ��
		//������̭
		virtual void showGameContestKick() =0;       
		//�ȴ���������
		virtual void showGameContestWaitOver() = 0;
		//��������
		virtual void showGameContestOver(MSG_GR_ContestAward* contestAward) = 0;
		//��ʾ��������
		virtual void showConstJuShu(int Index) = 0;
		//��ʾ����
		virtual void ShowConstRank(int iRankNum, int iRemainPeople) = 0;					
		//�����Լ�������	
		virtual void updateMyRankNum(int iValue) = 0;
	};

}

#endif // __DZPokerGameTableUICallback_h__
