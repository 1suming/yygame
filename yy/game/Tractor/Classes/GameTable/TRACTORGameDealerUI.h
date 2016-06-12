#ifndef __TRACTORGameDealerUI_h__
#define __TRACTORGameDealerUI_h__


#include "HNNetExport.h"
#include <array>
#include "TRACTORMessageHead.h"
#include "TRACTORGameClientMessage.h"

namespace TRACTOR
{
	typedef std::function<void()> callBackFunc;
	class PokerCard;
	class DrawEllipse;
	class GameChipUI;

	/*
	* �ɹ��࣬ר�Ŵ���ɹٴ��������
	*/

	class GameDelearUI: public HNLayer
	{
	public:
		/*�����ɹ�*/
		static GameDelearUI* create();
		/*���úɹ�λ�ã���ʼ��һ�Σ�*/
		void setDealerPoint(cocos2d::Vec2 point);
		/*������������ҵĵ㣨��ʼ��һ�Σ�*/
		void setTablePoint(std::vector<cocos2d::Vec2>& points);
		/*���������λ�ĵ㣨��ʼ��һ�Σ�*/
		void setSeatPoint(std::vector<cocos2d::Vec2>& points);
		/*����ׯ��λ�ã�ׯ�ұ仯����Ҫ�޸ģ�*/
		void setButtonStation(BYTE buttonStation);
		/*���ù�����λ��*/
		void setCommunityPoint(const std::vector<cocos2d::Vec2>& points);

		//�����ע
		void betMoney(BYTE seatNo, LLONG money);
		//�ƶ�ׯ��
		void dealButton(BYTE dealerStation);

		//����ҷ�����
		void dealHandCard(const std::vector<THandCard>& handCard);

		// recycle card suite
		void recycleCardSuite(float delta);
		// recycle chips to winner
		void recycleChips(BYTE winnerSeatNo);


		//������
		void dealFlopCard(const std::vector<BYTE>& flopCard);
		//��ת��
		void dealTurnCard(BYTE turnCard);
		//������
		void dealRiverCard(BYTE riverCard);
		//����
		void dealCollectCard();
		/* ������ɽ������ճ��룬���ɽ���
		* ����ֵ�����ʾ�н�
		*/
		void dealWinPool(const std::vector<std::vector<LLONG>> &winPool);
		/*����*/
		void clear();
		/* play player fold animation.
		* @param seatNo player seatNo in view.
		*/
		void dealFold(BYTE seatNo);
	public:
		bool init();
	protected:
		GameDelearUI();
		virtual ~GameDelearUI();
		cocos2d::Vec2 getButtonPoint();
		cocos2d::Vec2 getSeatPoint(BYTE seatNo);
		cocos2d::Vec2 getTablePoint(BYTE seatNo);
		void collectChip();
		void sendPrize();

		void dispatchHandCard();
		void showUserHandCard(float delta);

		void moveFlopCard();
		void dispatchCommunityCard(int type);
		void openCommunityCard(cocos2d::Node* pNode);
		cocos2d::Action* getFlopAction();
		cocos2d::Action* getTurnAction();
		cocos2d::Action* getRiverAction();
	private:
		std::vector<cocos2d::Node*> _communityCard;//������
		std::vector<cocos2d::Vec2> _communityCardPoints;//������λ��
		std::vector<GameChipUI*> _tableChip;//ÿ��������ϵĳ���
		std::vector<cocos2d::Vec2> _seatPoints;//ÿ�������λ�ϵĵ�
		std::vector<cocos2d::Vec2> _tablePoints;//ÿ��������ϵĵ�
		cocos2d::Vec2 _dealerPoint;//�ɹٵ�λ��λ��
		std::vector<std::vector<LLONG>> _winPool;//���Ӯ�Ľ��
		std::vector<THandCard> _handCard;//�������Ƶ������λ��
		BYTE _curCount;
	private:
		cocos2d::Node* _dealer;
		BYTE _buttonStation;//ׯ��λ�ú�
		bool seatNoOk(BYTE seatNo);

		PokerCard* _cardSuite[52]; // card suite
		static const int TOTAL_CARDS = 52; // total cards

		DrawEllipse* _ellipse; // a ellipse area used for chips pool
		cocos2d::Layer* _chipsPool; // chips pool for chip recycle
	};
}


#endif // __TRACTORGameDealerUI_h__
