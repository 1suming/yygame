#ifndef __BJLGameTableUICallback_h__
#define __BJLGameTableUICallback_h__

#include "HNBaseType.h"
#include "BJLMessageHead.h"
#include "HNNetExport.h"
#include "HNLogicExport.h"

/************************************************************************/
/* ���ͣ��ص��ӿ�                                                        */
/* ���ܣ����������߼����յ���Ϣʱ���ý���ĺ����ӿ�                        */
/* ˵������user������û���û�������������                                */
/* -1�Ĳ������øı�                                                      */
/************************************************************************/
namespace BJL
{
	class GameTableUICallback : public HN::IHNGameLogicBase
	{
	public:
		virtual void leaveTheGame() = 0;
		
		// ��һ����ׯ�� ��Ϸֱ�ӽ�����ע
		virtual void OnHandleBegin(S_C_GameBegin * pGameBeginData) = 0;
		
		// ����
		virtual void showSettlement(S_C_GameResult* pGameResult) = 0; 

		// ��ʾ��ʾ��Ϣ
		virtual void IShowNoticeMessage(const std::string& msg) = 0;

		// �����ϡ���ׯ��ť
		virtual void IApplyNT(bool isNT) = 0;

		// ������ׯ�б�
		virtual void IUpdateNTList() = 0;

		// ��ʾׯ����Ϣ
		virtual void IUpdateNTInfo(const std::string name, LLONG money) = 0;

		// ��ʾ�Լ���ע��
		virtual void IDisplayMyBetMoney(int noteArea, LLONG money) = 0;

		// ������ҽ��
		virtual void IUpdateUserMoney(LLONG money) = 0;

		// ��ʾ�����ע����
		virtual void IPlayNoteAnimation(BYTE noteType, int noteArea) = 0;

		// ���ó��밴ť�Ƿ����
		virtual void ISetBetEnable(bool enabled[], int count) = 0;

		// ��������ʱ
		virtual void IStartTimer(int second) = 0;

		// ��ָ��������ע����
		virtual void IAreaBet(int area, LLONG betMoney) = 0;

		// ��ʼ����
		virtual void IInitCard(BYTE cards[][3], int playerCount, int NTCount, int playerCardType[], int NTCardType[]) = 0;

		// ��ʾ��
		virtual void IShowCards(bool animate) = 0;

		// ����
		virtual void ISendCard(bool animate) = 0;

		// ��ʾ��ע
		virtual void IShowBetHint(bool visible) = 0;

		// ��ʾ�ȴ�ʱ��
		virtual void IShowWaitHint(bool visible) = 0;

		// ��Ϸ��ʼ
		virtual void IGameStart() = 0;

		// ��ʾ�н�����
		virtual void IShowWinArea(bool visible) = 0;

		// �������
		virtual void IShowOpenBoard(bool visible) = 0;

		//����ʱͼ�ƶ���λ��
		virtual void SetImageTimeMoveNewPostion() = 0;

		//����ʱͼ�ƶ�����λ��
		virtual void SetImageTimeMoveOldPostion() = 0;
	};
}


#endif // __BJLGameTableUICallback_h__