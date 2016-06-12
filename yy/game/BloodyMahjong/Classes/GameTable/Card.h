#ifndef _CARD_H_
#define _CARD_H_

#include "cocos2d.h"
#include "XZMJ_MessageHead.h"
#include "ResourceLoader.h"

namespace XZMJ
{

#define CREATE_CARD(__className) \
	static __className * create(mahjongCreateType type,   sitDir sitNo, INT number = -999) \
	{ \
	auto p = new __className; \
	if (p && p->init(type, sitNo, number)) \
	{ \
	p->autorelease(); \
	return p; \
	} \
	delete p; \
	return nullptr; \
}

	class Card :
		public cocos2d::Node
	{
	public:
		Card(void);
		~Card(void);

		virtual bool init(mahjongCreateType _type, sitDir sitNo, INT number) = 0;         // ��ʼ�����Ƶ�����
	
		// set ����
		void setCardPos(const Vec2& pos);																	// ���ÿ��Ƶ�λ��
		void setCardZorder(const int zorder);																// ���ÿ��Ƶ�localZorder
		void setCardVisible(const bool& bs = true);														// ���ÿ�����ʾ
		void setCardOwner(const sitDir& dir);																// ���ÿ��Ƶ�ʹ����
		void setCardEnableTouch(bool touch = true);                                                   // ���ÿ��ƵĴ�������
		void setCardTouchEvent();                                                                                // ���ÿ��ƵĴ�������

		// get ����
		const Size& getCardSize();																				// ��ȡ���Ƶĳߴ�
		Sprite* getCardSprite();																						// ��ȡ���Ƶľ���
		const Vec2& getCardPos() { return _uiSp->getPosition(); }								    // ��ȡ���Ƶ�λ��
		bool isCardVisible();																							// ��ȡ�����Ƿ���ʾ
		const sitDir& getCardOwner() { return _sitNo; };													// ��ȡ���Ƶ�ʹ����
		const INT& getCardNumber();																			// ��ȡ���Ƶĵ���
		const mahjongColor& getCardColor();															    // ��ȡ���ƵĻ�ɫ
		const INT& getCardZorder();                                                                            // ��ȡ���Ƶ�localZoderֵ
		const INT& getCardSumNumber() { return _sum; }                                           // ��ȡ���Ƶı���ֵ

	protected:
		cocos2d::Sprite* _uiSp;																						// ������ʾ
		mahjongCreateType _type;																				// ��������
		INT _number;																									// ����
		sitDir _sitNo;																										// ʹ����
		mahjongColor _color;																						// ��ɫ    
		Size _size;																											// �ߴ�   
		bool _canTouch;																								// ����
		INT _cardZorder;																								// localZoder
		INT _sum;                                                                                                         // ����ֵ
		Vec2 _startPos;																									// ��ʼ����λ��
		INT _startZorder;																								// ��ʼzorderֵ
		Color3B _startColor;																							// ��ʼ��ɫ
	};

}
#endif // _CARD_H_