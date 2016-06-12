#include "Card.h"
#include "GameManager.h"

namespace XZMJ
{

	Card::Card(void)
	{
		_uiSp = nullptr;
		_type = mahjongCreateType::DI_BEI_HENG;   
		_number = -999;
		_sitNo = sitDir::MID_DIR;
		_color = mahjongColor::TIAO;
		_canTouch = false;
	}


	Card::~Card(void)
	{
	}


	bool Card::init(mahjongCreateType type, sitDir sitNo, INT number)
	{
		if (!Node::init())
		{
			return false;
		}
		return true;
	}

	const Size& Card::getCardSize()
	{
		return _size;
	}

	void Card::setCardPos(const Vec2& pos)
	{
		_uiSp->setPosition(pos);
	}

	void Card::setCardZorder(const int zorder)
	{
		_uiSp->setLocalZOrder(zorder);
		_cardZorder = zorder;
	}

	void Card::setCardVisible(const bool& bs)
	{
		_uiSp->setVisible(bs);
	}

	bool Card::isCardVisible()
	{
		return _uiSp->isVisible();
	}

	Sprite* Card::getCardSprite()
	{
		return _uiSp;
	}

	void Card::setCardOwner(const sitDir& dir)
	{
		_sitNo = dir;
	}


	const INT& Card::getCardNumber()
	{
		return _number;
	}
		
	const mahjongColor& Card::getCardColor()
	{
		return _color;
	}

	void Card::setCardTouchEvent()
	{
		auto listener =EventListenerTouchOneByOne::create();
		listener->onTouchBegan = [&](Touch* t, Event* e)->bool{
			if (!_canTouch)            // �Ƿ��ܴ���
			{
				return false;
			}
			if (!_uiSp->getBoundingBox().containsPoint((t->getLocation())))   // �Ƿ��Ʒ�Χ��
			{
				return false;
			}

			// ȡ�����Ƶĳ�ʼ״̬
			_startPos = _uiSp->getPosition();					// ��ʼ�����
			_startZorder = _uiSp->getLocalZOrder();		// zֵ
			_startColor = _uiSp->getColor();					// ��ɫ

			// �ı���״̬
			_uiSp->setScale(1.4f);
			_uiSp->setLocalZOrder(99999);
			auto pool = (SouthMahjongCardPool *)(GameManager::getInstance()->getUserCardPool().at(0));
			if (_color == mahjongColor(GameManager::getInstance()->getQueMen()) || pool->hasNoQueColor())   // �Ƿ��ܴ��
			{
				_uiSp->setColor(Color3B(200, 255, 195));   // ����ɫ
			}
			else
			{
				_uiSp->setColor(Color3B(250, 180, 180));   // ����ɫ
			}
			return true;
		};
		listener->onTouchMoved = [&](Touch* t, Event* e){
			if (_canTouch)
			{
				_uiSp->setPositionY(_uiSp->getPositionY() + t->getDelta().y);           // �ƶ�ѡ�е���
				auto pool = (SouthMahjongCardPool *)(GameManager::getInstance()->getUserCardPool().at(0));

				// �жϳ�������
				float needMoveLength = 60.0f;  // ��y��������Ҫ�ƶ��ľ���
				if (abs(_uiSp->getPositionY() - _startPos.y) > needMoveLength
					&& (_color == mahjongColor(GameManager::getInstance()->getQueMen()) || pool->hasNoQueColor()))							// �ﵽ����������������˷�����Ϣ
				{
					GameManager::getInstance()->getUserCardPool().at(0)->removeCardMsg(this);
				}
			}
		};
		listener->onTouchEnded = [&](Touch* t, Event* e){

			// ��ԭ�Ƶ���ʼ״̬
				if (_uiSp->isVisible())
				{
					_uiSp->setPosition(_startPos);
				}
				else
				{
					_uiSp->setPosition(Vec2(-1000, -1000));
				}
				_uiSp->setScale(1.0f);
				_uiSp->setLocalZOrder(_startZorder);
				_uiSp->setColor(_startColor);
		};
		listener->onTouchCancelled = [&](Touch* t, Event* e){};
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _uiSp);
	}

	void Card::setCardEnableTouch(bool touch)
	{
		_canTouch = touch;
	}

	const INT& Card::getCardZorder()
	{
		return _cardZorder;
	}

}