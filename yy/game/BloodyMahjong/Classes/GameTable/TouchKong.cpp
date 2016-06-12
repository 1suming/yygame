#include "TouchKong.h"

namespace XZMJ
{

	TouchKong::TouchKong(void)
	{
		_runSp = Sprite::createWithSpriteFrameName("gangAction1.png");
		this->addChild(_runSp);
		_runSp->setVisible(false);
	}


	TouchKong::~TouchKong(void)
	{
	}

	void TouchKong::run()
	{
		_pool->addSomeOutCards(1, _cardNumber,  false);   // ���ӵ���

		// ��������
		_pool->afterAction(1);

		// ���Ŷ���
		uiAction("gangAction");
	}

}