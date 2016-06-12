#include "WaterMarginGameUserBox.h"
#include <string>
//#include "PlatformConfig.h"

namespace WaterMargin
{
	GameUserMessageBox::GameUserMessageBox()
		:_iMoney(nullptr)
	{

	}

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
			Size s = this->getContentSize();
			HNLOG("wide = %d,hight = %d", s.width, s.height);
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

		//����json�ļ�

		auto node = CSLoader::createNode("WaterMargin/tableui/userBox.csb");
		auto widgt = (Layout*)node->getChildByName("Panel_39");
		//auto widgt = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("WaterMargin/tableui/userBox.json");
		widgt->setAnchorPoint(Vec2(0.5, 0.5));
		this->addChild(node);
		widgt->setPosition(Vec2(winSize.width/2, winSize.height/2));
		auto bg = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(widgt, "bg"));
		auto iName = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(bg, "iName"));
		iName->setString(LogonResult->nickName);

		_iMoney = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(bg, "iMoney"));
	
		return true;
	}
}