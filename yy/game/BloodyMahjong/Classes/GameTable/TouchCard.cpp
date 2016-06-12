#include "TouchCard.h"

namespace XZMJ
{


	TouchCard::TouchCard(void)
	{
		_runSp = Sprite::createWithSpriteFrameName("pengAction1.png");
		this->addChild(_runSp);
		_runSp->setVisible(false);
	}


	TouchCard::~TouchCard(void)
	{
	}

	void TouchCard::run()
	{
		_pool->addSomeOutCards(3, _cardNumber);   // ���ӵ���

		// ��������
		_pool->afterAction(2);

		// ���Ŷ���
		uiAction("pengAction");
	}

}