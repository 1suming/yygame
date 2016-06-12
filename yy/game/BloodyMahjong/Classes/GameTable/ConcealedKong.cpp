#include "ConcealedKong.h"

namespace XZMJ
{

	ConcealedKong::ConcealedKong(void)
	{
		_runSp = Sprite::createWithSpriteFrameName("gangAction1.png");
		this->addChild(_runSp);
		_runSp->setVisible(false);
	}


	ConcealedKong::~ConcealedKong(void)
	{
	}

	void ConcealedKong::run()
	{
		_pool->addSomeOutCards(4, _cardNumber, false, false);   // ���ӵ���

		// ��������
		_pool->afterAction(4);

		// ���Ŷ���
		uiAction("gangAction");
	}
}