#include "DZPokerGameCardType.h"

namespace DZPoker
{
	static const char* CARD_TYPE_BG		= "dzpoker/table/card_typebg_new.png";
	static const char* CARD_TYPE		= "dzpoker/table/display_card_type_new.png";

	GameCardType::GameCardType()
	{
	}

	GameCardType::~GameCardType()
	{
	}

	bool GameCardType::init()
	{
		if (!HNLayer::init())
		{
			return false;
		}

		//����
		auto bg = Sprite::create(CARD_TYPE_BG);
		auto size = bg->getContentSize();
		this->setContentSize(size);
		bg->setPosition(size.width/2, size.height/2);
		bg->ignoreAnchorPointForPosition(false);
		this->addChild(bg, 1);

		auto dispatcher = Director::getInstance()->getEventDispatcher();
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(GameCardType::onTouchBegan, this);
		dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		//�ʼ�ͬ��˳
		Label* royalFlush = createLabel(GBKToUtf8("�ʼ�ͬ��˳"), 20);
		royalFlush->setColor(colorGold);
		royalFlush->setAnchorPoint(Vec2(0, 0.5));
		royalFlush->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.95));
		bg->addChild(royalFlush, 2);

		//ͬ��˳
		auto straightFlush = createLabel(GBKToUtf8("ͬ��˳"), 20);
		straightFlush->setColor(colorGold);
		straightFlush->setAnchorPoint(Vec2(0, 0.5));
		straightFlush->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.85));
		bg->addChild(straightFlush, 2);

		//����
		auto fourOfAKind = createLabel(GBKToUtf8("����"), 20);
		fourOfAKind->setColor(colorGold);
		fourOfAKind->setAnchorPoint(Vec2(0, 0.5));
		fourOfAKind->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.75));
		bg->addChild(fourOfAKind, 2);

		//��«
		auto fullhouse = createLabel(GBKToUtf8("��«"), 20);
		fullhouse->setColor(colorGold);
		fullhouse->setAnchorPoint(Vec2(0, 0.5));
		fullhouse->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.65));
		bg->addChild(fullhouse, 2);

		//ͬ��
		auto flush = createLabel(GBKToUtf8("ͬ��"), 20);
		flush->setColor(colorGold);
		flush->setAnchorPoint(Vec2(0, 0.5));
		flush->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.55));
		bg->addChild(flush, 2);

		//˳��
		auto straight = createLabel(GBKToUtf8("˳��"), 20);
		straight->setColor(colorGold);
		straight->setAnchorPoint(Vec2(0, 0.5));
		straight->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.45));
		bg->addChild(straight, 2);

		//����
		auto threeOfAkind = createLabel(GBKToUtf8("����"), 20);
		threeOfAkind->setColor(colorGold);
		threeOfAkind->setAnchorPoint(Vec2(0, 0.5));
		threeOfAkind->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.35));
		bg->addChild(threeOfAkind, 2);

		//����
		auto twoPairs = createLabel(GBKToUtf8("����"), 20);
		twoPairs->setColor(colorGold);
		twoPairs->setAnchorPoint(Vec2(0, 0.5));
		twoPairs->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.25));
		bg->addChild(twoPairs, 2);

		//һ��
		auto onePair = createLabel(GBKToUtf8("һ��"), 20);
		onePair->setColor(colorGold);
		onePair->setAnchorPoint(Vec2(0, 0.5));
		onePair->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.15));
		bg->addChild(onePair, 2);

		//����
		auto highCard = createLabel(GBKToUtf8("����"), 20);
		highCard->setColor(colorGold);
		highCard->setAnchorPoint(Vec2(0, 0.5));
		highCard->setPosition(Vec2(bg->getContentSize().width * 0.6, size.height * 0.05));
		bg->addChild(highCard, 2);

		auto cardtype = Sprite::create(CARD_TYPE);
		cardtype->setAnchorPoint(Vec2(0.0f, 0.5f));
		cardtype->setPosition(10, size.height/2);
		this->addChild(cardtype, 2);

		return true;
	}

	bool GameCardType::onTouchBegan(Touch *touch, Event *unused_event)
	{
		auto pos = this->getPosition();
		if (pos.x < 0)
		{
			return false;
		}
		auto size = this->getContentSize();
		auto des_pos = Vec2(-size.width,0.0f);
		this->stopAllActions();
		auto mveTo = MoveTo::create(0.2f,des_pos);
		auto funcall = CallFuncN::create(CC_CALLBACK_1(GameCardType::moveByAnimationCallback,this));
		auto seq = Sequence::create(mveTo,funcall, nullptr);
		this->runAction(seq);
		return false;
	}

	void GameCardType::moveByAnimationCallback(Node* sender)
	{
		sender->setVisible(false);
	}

}
