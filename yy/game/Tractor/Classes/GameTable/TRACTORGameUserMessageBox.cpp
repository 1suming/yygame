#include "TRACTORGameUserMessageBox.h"
#include <string>
#include "HNLobbyExport.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace ui;

namespace TRACTOR
{
	static const INT USERDATA_BOX			= 10;    //������Ͽ�
	static const char*  CSB_FILE			= "tractor/userinfo/userinfo.csb"	;

	static const char* MEN_HEAD				= "tractor/table/men_head.png";
	static const char* WOMEN_HEAD			= "tractor/table/women_head.png";


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
		auto thisSize = this->getContentSize();
		auto wigetNode = CSLoader::createNode(CSB_FILE);
		wiget = (Widget*)wigetNode->getChildByName("Panel_24");
		//wiget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(JSON_FILE);
		wiget->setAnchorPoint(Vec2(0.5f , 0.5f));
		wiget->setPosition(Vec2(thisSize.width / 2 , thisSize.height / 2));
		this->addChild(wigetNode);
		//����һ������¼�
		auto MyListener = EventListenerTouchOneByOne::create();
		MyListener->setSwallowTouches(true);//��ֹ�������´���
		MyListener->onTouchBegan = [&](Touch* touch, Event* event)
		{
			auto sp = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(wiget , "bg"));
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
		char str[50];
		auto pic = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(wiget , "xiangkuang"));
		auto pic_size = pic->getContentSize();

		//���ͷ��
		auto userIcon = Sprite::create(LogonResult->bBoy ? MEN_HEAD : WOMEN_HEAD);
		userIcon->setAnchorPoint(Vec2(0.5f , 0.5f));
		userIcon->setPosition(Vec2(pic_size.width / 2 , pic_size.height / 2));
		pic->addChild(userIcon, 2);

		//����ǳ�
		sprintf(str, "%s", LogonResult->nickName);
		auto name_text = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(wiget , "name_0"));
		name_text->setString(GBKToUtf8(str));

		//��ҽ��
		sprintf(str, "%lld", LogonResult->i64Money);
		auto money_text = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(wiget , "money_0"));
		money_text->setString(str);
		/*
		//������Ͽ�
		auto userDataBox = Sprite::create(MESSAGE_BOX_BG);
		userDataBox->setPosition(Vec2(winSize.width/2, winSize.height/2));
		addChild(userDataBox, 10, USERDATA_BOX);
		_userDataBoxRect = userDataBox->getBoundingBox();

		//���ͷ�񱳾���
		auto headBox = Sprite::create(HEAD_BOX);
		headBox->setPosition(Vec2(userDataBox->getContentSize().width*0.2, userDataBox->getContentSize().height*0.72));
		userDataBox->addChild(headBox, 3);

		char str[128];

		//����ǳ�
		sprintf(str, GBKToUtf8("%s"), LogonResult->nickName);
		auto uNickName = createLabel(gFontConfig_18, str, colorGold);
		uNickName->setAnchorPoint(Vec2(0, 0.5));
		uNickName->setPosition(Vec2(userDataBox->getContentSize().width*0.44, userDataBox->getContentSize().height*0.89));
		userDataBox->addChild(uNickName, 2);

		//����Ա�
		Label* uSex = nullptr;
		if (LogonResult->bBoy) uSex = createLabel(gFontConfig_18, GBKToUtf8("��"), colorGold);
		else uSex = createLabel(gFontConfig_18, GBKToUtf8("Ů"), colorGold);
		uSex->setAnchorPoint(Vec2(0, 0.5));
		uSex->setPosition(Vec2(userDataBox->getContentSize().width*0.8, userDataBox->getContentSize().height*0.89));
		userDataBox->addChild(uSex, 2);
		//��Ҿ���
		sprintf(str, GBKToUtf8("%d"), LogonResult->dwPoint);
		auto uEXP = createLabel(gFontConfig_18, str, colorGold);
		uEXP->setAnchorPoint(Vec2(0, 0.5));
		uEXP->setPosition(Vec2(userDataBox->getContentSize().width*0.44, userDataBox->getContentSize().height*0.71));
		userDataBox->addChild(uEXP, 2);

		//���ʤ��
		sprintf(str, GBKToUtf8("%d"), LogonResult->uWinCount);
		auto uWinNum = createLabel(gFontConfig_18, str, colorGold);
		uWinNum->setAnchorPoint(Vec2(0, 0.5));
		uWinNum->setPosition(Vec2(userDataBox->getContentSize().width*0.8, userDataBox->getContentSize().height*0.71));
		userDataBox->addChild(uWinNum, 2);

		//��Ҹ���
		sprintf(str, GBKToUtf8("%d"), LogonResult->uLostCount);
		auto uLostNum = createLabel(gFontConfig_18, str, colorGold);
		uLostNum->setAnchorPoint(Vec2(0, 0.5));
		uLostNum->setPosition(Vec2(userDataBox->getContentSize().width*0.44, userDataBox->getContentSize().height*0.529));
		userDataBox->addChild(uLostNum, 2);

		//��Һ;�
		sprintf(str, GBKToUtf8("%d"), LogonResult->uMidCount);
		auto uMidNum = createLabel(gFontConfig_18, str, colorGold);
		uMidNum->setAnchorPoint(Vec2(0, 0.5));
		uMidNum->setPosition(Vec2(userDataBox->getContentSize().width*0.8, userDataBox->getContentSize().height*0.529));
		userDataBox->addChild(uMidNum, 2);

		//����ǩ��
		sprintf(str, GBKToUtf8("%s"), LogonResult->szSignDescr);
		auto uSignDescr = createLabel(gFontConfig_18, str, colorGold);
		uSignDescr->setAnchorPoint(Vec2(0, 0.5));
		uSignDescr->setPosition(Vec2(userDataBox->getContentSize().width*0.12, userDataBox->getContentSize().height*0.335));
		userDataBox->addChild(uSignDescr, 2);*/

		return true;
	}
}
