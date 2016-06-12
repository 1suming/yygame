#ifndef __BJLResultCard_h__
#define __BJLResultCard_h__

#include "HNNetExport.h"
#include "HNUIExport.h"

#include "BJLMessageHead.h"
#include "BJLGameLogic.h"
#include "BJLPokerCard.h"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>

using namespace cocostudio;
using namespace ui;
using namespace std;

namespace BJL
{
	class GameTableUI;

	class ResultCard
	{
	public:
		ResultCard(GameTableUI* gameTableUi);
		~ResultCard();

	public:
		bool dispatchCard();
		bool dispatchCardWithoutAnimation();
		void showCardBorad();
		void showCardBoradWithOutAnimation();
		void calculatePoint();								//������Ϸ���͵���
		void hideCardBorad();

		// ��ȡ����
		void initCard(BYTE card[2][3], BYTE dealerCardNum, BYTE leisureCardNum,int dealerCardtype[5],int leisureCardtype[5] );
		void initNewGame();

	public:
		BYTE getCardFileValue(const WORD& card);


	private:
		GameTableUI* _gameTableUi;
		Node* _csLoader;
		Node* _panel;
		PokerCard* _spriteCard[2][3];
		ImageView* background;
		ImageView* leisure;
		ImageView* dealer;
		ImageView* leisure_win;
		ImageView* dealer_win;
		ImageView* dealer_leisure;
		
		//�ƶ�ͼƬ
		ImageView* cardsss;

		ImageView* cardTest[2];		//��ͼƬ �����ڲ��Ŷ���ʱȷ��λ��  �ֶ�û���ã�  0.0

	private:
		BYTE _cards[2][3];				   //��ֵ
		Vec2 destinationPositon[2][3];     //Ŀ��λ��
		int _dealerCardtype[5];			   //ׯ������
		int _leisureCardtype[5];           //�м�����
		BYTE    _bSendCardIndex;            //��������
		BYTE _dealerCardNum;
		BYTE _leisureCardNum;
		Sprite* spArr[2];
	private:
		void cardFlipCallback(Node* pNode);
		void dispatchCardAnimation(PokerCard* pokerCard , BYTE state);
		BYTE countTheAnswer();
	};
}

#endif // __BJLResultCard_h__
