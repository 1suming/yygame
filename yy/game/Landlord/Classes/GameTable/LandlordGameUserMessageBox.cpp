#include "LandlordGameUserMessageBox.h"
#include <string>
#include "HNLobbyExport.h"

namespace landlord
{
	static const int USERDATA_BOX = 10;    //������Ͽ�

	static const char* MESSAGE_BOX_BG = "landlord/userinfo_messagebox/table_userinfo_bg.png";
	static const char* HEAD_BOX		= "landlord/userinfo_messagebox/head_box.png";
	static const char* MEN_HEAD		= "landlord/userinfo_messagebox/men_head.png";
	static const char* WOMEN_HEAD	= "landlord/userinfo_messagebox/women_head.png";


	GameUserMessageBox* GameUserMessageBox::createMessageBox(cocos2d::Node* parent, INT userID)
	{
		auto pRet = new GameUserMessageBox();
		if (pRet && pRet->init(parent, userID))
		{
			pRet->autorelease();
			return pRet;
		}
		delete pRet;
		pRet = nullptr;
		return pRet;
	}

	bool GameUserMessageBox::init(cocos2d::Node* parent, INT userID)
	{
		if ( !HNDialogBase::init(parent)) return false;

		Size winSize = Director::getInstance()->getWinSize();

		//����һ������¼�
		auto MyListener = EventListenerTouchOneByOne::create();
		MyListener->setSwallowTouches(true);//��ֹ�������´���
		MyListener->onTouchBegan = [&](Touch* touch, Event* event)
		{
			//auto target = static_cast<Sprite*>(event->getCurrentTarget());//��ȡ�ĵ�ǰ������Ŀ��       
			//Point locationInNode = target->convertToNodeSpace(touch->getLocation());
			auto sp = this->getChildByTag(USERDATA_BOX);

			Size s = this->getContentSize();
			Rect rect = Rect(0, 0, s.width, s.height);        
			if (rect.containsPoint(touch->getLocation()))//�жϴ������Ƿ���Ŀ��ķ�Χ��
			{
				if (_userDataBoxRect.containsPoint(touch->getLocation())) return true;
				hide();
				return true;
			}
			else
				return false;
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
		auto userIcon = Sprite::create(LogonResult->bBoy ? MEN_HEAD : WOMEN_HEAD);
		userIcon->setPosition(Vec2(userDataBox->getContentSize().width*0.12, userDataBox->getContentSize().height*0.82));
		userDataBox->addChild(userIcon, 2);

		//���ͷ�񱳾���
		auto headBox = Sprite::create(HEAD_BOX);
		headBox->setPosition(Vec2(userDataBox->getContentSize().width*0.12, userDataBox->getContentSize().height*0.82));
		userDataBox->addChild(headBox, 3);

		char str[128];

		//����ǳ�
		sprintf(str, "%s", LogonResult->nickName);
		auto uNickName = createLabel(GBKToUtf8(str), 18, colorWhite);
		uNickName->setAnchorPoint(Vec2(0, 0.5));
		uNickName->setPosition(Vec2(userDataBox->getContentSize().width*0.4, userDataBox->getContentSize().height*0.89));
		userDataBox->addChild(uNickName, 2);

		//����Ա�
		Label* uSex = nullptr;
		if (LogonResult->bBoy) uSex = createLabel(GBKToUtf8("��"), 18, colorWhite);
		else uSex = createLabel(GBKToUtf8("Ů"), 18, colorWhite);
		uSex->setAnchorPoint(Vec2(0, 0.5));
		uSex->setPosition(Vec2(userDataBox->getContentSize().width*0.76, userDataBox->getContentSize().height*0.89));
		userDataBox->addChild(uSex, 2);
		//��Ҿ���
		sprintf(str, "%d", LogonResult->dwPoint);
		auto uEXP = createLabel(str, 18, colorWhite);
		uEXP->setAnchorPoint(Vec2(0, 0.5));
		uEXP->setPosition(Vec2(userDataBox->getContentSize().width*0.4, userDataBox->getContentSize().height*0.71));
		userDataBox->addChild(uEXP, 2);

		//���ʤ��
		sprintf(str, "%d", LogonResult->uWinCount);
		auto uWinNum = createLabel(str, 18, colorWhite);
		uWinNum->setAnchorPoint(Vec2(0, 0.5));
		uWinNum->setPosition(Vec2(userDataBox->getContentSize().width*0.76, userDataBox->getContentSize().height*0.71));
		userDataBox->addChild(uWinNum, 2);

		//��Ҹ���
		sprintf(str, "%d", LogonResult->uLostCount);
		auto uLostNum = createLabel(str, 18, colorWhite);
		uLostNum->setAnchorPoint(Vec2(0, 0.5));
		uLostNum->setPosition(Vec2(userDataBox->getContentSize().width*0.4, userDataBox->getContentSize().height*0.529));
		userDataBox->addChild(uLostNum, 2);

		//��Һ;�
		sprintf(str, "%d", LogonResult->uMidCount);
		auto uMidNum = createLabel(str, 18, colorWhite);
		uMidNum->setAnchorPoint(Vec2(0, 0.5));
		uMidNum->setPosition(Vec2(userDataBox->getContentSize().width*0.76, userDataBox->getContentSize().height*0.529));
		userDataBox->addChild(uMidNum, 2);

		//����ǩ��
		sprintf(str, "%s", LogonResult->szSignDescr);
		auto uSignDescr = createLabel(GBKToUtf8(str), 18, colorWhite);
		uSignDescr->setAnchorPoint(Vec2(0, 0.5));
		uSignDescr->setPosition(Vec2(userDataBox->getContentSize().width*0.12, userDataBox->getContentSize().height*0.335));
		userDataBox->addChild(uSignDescr, 2);

		return true;
	}
}



