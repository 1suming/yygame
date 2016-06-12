#include "GoldenFlowerGameUserMessageBox.h"
#include <string>
#include "HNLobbyExport.h"

namespace goldenflower
{
	static const INT USERDATA_BOX = 10;    //������Ͽ�

	static const char* MESSAGE_BOX_BG		= "goldenflower/userinfo_messagebox/table_userinfo_bg.png";
	static const char* HEAD_BOX				= "goldenflower/userinfo_messagebox/head_box.png";

	GameUserMessageBox* GameUserMessageBox::createMessageBox(cocos2d::Node* parent, INT userID, std::string headImage)
	{
		auto pRet = new GameUserMessageBox();
		if (pRet && pRet->init(parent, userID, headImage))
		{
			pRet->autorelease();
			return pRet;
		}
		delete pRet;
		pRet = nullptr;
		return pRet;
	}

	bool GameUserMessageBox::init(cocos2d::Node* parent, INT userID, std::string headImage)
	{
		if ( !HNDialogBase::init(parent)) return false;

		Size winSize = Director::getInstance()->getWinSize();

		//����һ������¼�
		auto MyListener = EventListenerTouchOneByOne::create();
		MyListener->setSwallowTouches(true);//��ֹ�������´���
		MyListener->onTouchBegan = [&](Touch* touch, Event* event)
		{
			auto sp = this->getChildByTag(USERDATA_BOX);
			Rect rect = Rect(0, 0, sp->getContentSize().width, sp->getContentSize().height);        
			auto pos = sp->convertToNodeSpace(touch->getLocation());
			if (!rect.containsPoint(pos))
			{
				hide();
			}
			return true;
		};
		//�ѵ��������ӵ�����������,��Ϊֻ������һ������,һ������ֻ�ܰ󶨵�һ��������,������������Ҫʹ�ü������¡
		_eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, this);

		//�����Ϣ�ṹ��
		UserInfoStruct* LogonResult = UserInfoModule()->findUser(userID);
		if (nullptr == LogonResult)
		{
			return true;
		}

		//������Ͽ�
		auto userDataBox = Sprite::create(MESSAGE_BOX_BG);
		userDataBox->setPosition(Vec2(winSize.width/2, winSize.height/2));
		addChild(userDataBox, 10, USERDATA_BOX);
		_userDataBoxRect = userDataBox->getBoundingBox();

		//���ͷ��
		auto userIcon = Sprite::create(headImage);
		userIcon->setPosition(Vec2(userDataBox->getContentSize().width*0.12, userDataBox->getContentSize().height*0.82));
		userDataBox->addChild(userIcon, 2);

		/*
		//���ͷ�񱳾���
		auto headBox = Sprite::create(HEAD_BOX);
		headBox->setPosition(Vec2(userDataBox->getContentSize().width*0.12, userDataBox->getContentSize().height*0.82));
		userDataBox->addChild(headBox, 3);*/

		char str[128];

		//����ǳ�
		sprintf(str, "%s", LogonResult->nickName);
		//auto uNickName = createLabel(gFontConfig_18, GBKToUtf8(str), colorGold);
		auto uNickName = Label::createWithSystemFont(GBKToUtf8(str), "", 18);

		uNickName->setAnchorPoint(Vec2(0, 0.5));
		uNickName->setPosition(Vec2(userDataBox->getContentSize().width*0.40, userDataBox->getContentSize().height*0.89));
		userDataBox->addChild(uNickName, 2);

		//����Ա�(const std::string& text, float fontSize, Color3B color = colorGold);
		Label* uSex = nullptr;
		if (LogonResult->bBoy) uSex = createLabel(GBKToUtf8("��"), 18, colorGold);
		else uSex = createLabel(GBKToUtf8("Ů"), 18, colorGold);
		uSex->setAnchorPoint(Vec2(0, 0.5));
		uSex->setPosition(Vec2(userDataBox->getContentSize().width*0.77, userDataBox->getContentSize().height*0.89));
		userDataBox->addChild(uSex, 2);

		//��Ҿ���
		sprintf(str, "%d", LogonResult->dwPoint);
		auto uEXP = createLabel(str, 18, colorGold);
		uEXP->setAnchorPoint(Vec2(0, 0.5));
		uEXP->setPosition(Vec2(userDataBox->getContentSize().width*0.40, userDataBox->getContentSize().height*0.71));
		userDataBox->addChild(uEXP, 2);

		//���ʤ��
		sprintf(str, "%d", LogonResult->uWinCount);
		auto uWinNum = createLabel(str, 18, colorGold);
		uWinNum->setAnchorPoint(Vec2(0, 0.5));
		uWinNum->setPosition(Vec2(userDataBox->getContentSize().width*0.77, userDataBox->getContentSize().height*0.71));
		userDataBox->addChild(uWinNum, 2);

		//��Ҹ���
		sprintf(str, "%d", LogonResult->uLostCount);
		auto uLostNum = createLabel(str, 18, colorGold);
		uLostNum->setAnchorPoint(Vec2(0, 0.5));
		uLostNum->setPosition(Vec2(userDataBox->getContentSize().width*0.40, userDataBox->getContentSize().height*0.529));
		userDataBox->addChild(uLostNum, 2);

		//��Һ;�
		sprintf(str, "%d", LogonResult->uMidCount);
		auto uMidNum = createLabel(str, 18, colorGold);
		uMidNum->setAnchorPoint(Vec2(0, 0.5));
		uMidNum->setPosition(Vec2(userDataBox->getContentSize().width*0.77, userDataBox->getContentSize().height*0.529));
		userDataBox->addChild(uMidNum, 2);

		//����ǩ��
		sprintf(str, "%s", LogonResult->szSignDescr);
		auto uSignDescr = createLabel(GBKToUtf8(str), 18, colorGold);
		uSignDescr->setAnchorPoint(Vec2(0, 0.5));
		uSignDescr->setPosition(Vec2(userDataBox->getContentSize().width*0.12, userDataBox->getContentSize().height*0.335));
		userDataBox->addChild(uSignDescr, 2);

		return true;
	}

}


