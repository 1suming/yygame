#include "GoldenToadGameTableUI.h"
#include "GoldenToadGameLoading.h"
#include "GoldenToadGameSceneFishTrace.h"
#include "GoldenToadGameFishManager.h"
#include "GoldenToadGameToolbar.h"
#include "GoldenToadGameAudio.h"
#include "GoldenToadGameAdmin.h"
#include "GoldenToadGameChatDlg.h"
#include <time.h>
#include "HNUIExport.h"
#include "HNLobbyExport.h"

namespace GoldenToad
{
	static const int	outTime_tag				=   999;
	static const char*	BG_PATH					=	"goldenToad/Scene/bg/";
	static const char*	TABLEUI_PATH			=	"goldenToad/GameTableUi/Node_Table.csb";
	static const char*  Notice_Bg				=	"goldenToad/notic/control.png";
	static const char*  Notice_Bg_Stencil		=	"goldenToad/notic/Notice_Bg.png";
	static const char*  Score_number_path		=	"goldenToad/number/score_num1.png";
	static const char*  Settlemen_path			=	"goldenToad/SettlementUi/SettlementUi.csb";

	static const FLOAT	SWITCH_SCENE_TIME		=	5.5f;

	GameTableUI::GameTableUI()
		: _isAllowFire (true)
		, _isAutoFire (false)
		, _isReadyLock (false)
		, _iBullet_id (0)
		, _lockFishId (0)
		, _touchPoint (Size::ZERO)
		, _currentBg (nullptr)
		, _tableUi (nullptr)
		, _autoBtn (nullptr)
	{
		memset(_players, 0x0, sizeof(_players));
		_resultInfo = new catchResultInfo();

		//������ϷΪ�˷�ֹ�ֻ��к�̨�����ʱ����һ����㣬������Ӹ������ֶ�
		//HNSocketThread�����ȡΪtrueʱ����MDM_GM_GAME_NOTIFY��Ϣ
		Configuration::getInstance()->setValue("bControl", Value(true));
	}

	GameTableUI::~GameTableUI()
	{
		HN_SAFE_DELETE(_tableLogic);
		HN_SAFE_DELETE(_fishManager);
		HN_SAFE_DELETE(_resultInfo);
		HN_SAFE_DELETE(_fishTrace);

		//������ϷΪ�˷�ֹ�ֻ��к�̨�����ʱ����һ����㣬������Ӹ������ֶ�
		//HNSocketThread�����ȡΪtrueʱ����MDM_GM_GAME_NOTIFY��Ϣ
		//�˳�������Ϸʱ���û�������ֹ������ϷҲ������MDM_GM_GAME_NOTIFY��Ϣ
		Configuration::getInstance()->setValue("bControl", Value(false));

		//AnimationCache::getInstance()->destroyInstance();
		//ArmatureDataManager::getInstance()->removeArmatureFileInfo("goldenToad/fishAnimation/bigFishAnimation.ExportJson");		
		
		//SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("goldenToad/bullet/netImg.plist");
		//SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("goldenToad/fishAnimation/fishImg.plist");
		//SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("goldenToad/Scene/water.plist");

		//SpriteFrameCache::getInstance()->removeSpriteFrames();
		//Director::getInstance()->getTextureCache()->removeAllTextures();
		//Director::getInstance()->getTextureCache()->removeUnusedTextures();
		
	}

	void GameTableUI::onEnterTransitionDidFinish()
	{
		HNGameUIBase::onEnterTransitionDidFinish();

		//releaseUnusedRes();
	}

	GameTableUI* GameTableUI::create(BYTE bDeskIndex, bool autoCreate)
	{
		GameTableUI *tableUI = new GameTableUI();
		if (tableUI && tableUI->init(bDeskIndex, autoCreate))
		{ 
			tableUI->autorelease();
			return tableUI;
		} 
		CC_SAFE_DELETE(tableUI);
		return nullptr;
	}

	bool GameTableUI::init(BYTE bDeskIndex, bool autoCreate)
	{
		if (!HNLayer::init()) return false;

		_deskIndex		= bDeskIndex;
		_bAutoCreate	= autoCreate;

		//�첽������Դ
		auto loadingLayer = GoldenToadGameLoading::create(true);
		addChild(loadingLayer);
		loadingLayer->onCloseCallBack = [&](){
			initGameUI();
		};
		loadingLayer->loadRes();
		return true;
	}

	bool GameTableUI::initGameUI()
	{
		//����������
		enableKeyboard();
		auto winSize = Director::getInstance()->getWinSize();

		Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA8888);

		GoldenToadGameAudio::playBackgroundMusic();

		//�򿪵��
		auto disptch = Director::getInstance()->getEventDispatcher();
		auto myListener = EventListenerTouchOneByOne::create();
		myListener->onTouchBegan = CC_CALLBACK_2(GameTableUI::onTouchBegan,this);
		myListener->onTouchMoved = CC_CALLBACK_2(GameTableUI::onTouchMoved,this);
		myListener->onTouchEnded = CC_CALLBACK_2(GameTableUI::onTouchEnded,this);
		disptch->addEventListenerWithSceneGraphPriority(myListener, this);

		//����
		std::string bgName(BG_PATH);
		bgName.append("bg1.png");
		_currentBg = ImageView::create(bgName);
		float scaleY = winSize.height / _currentBg->getContentSize().height;
		_currentBg->setPosition(winSize / 2);
		_currentBg->setScaleY(scaleY);
		addChild(_currentBg);

		//tableUI����
		auto node = CSLoader::createNode(TABLEUI_PATH);
		addChild(node, PLAYER_ZORDER);

		_tableUi = dynamic_cast<Layout*>(node->getChildByName("Panel_Table"));
		_tableUi->setScaleY(scaleY);
		_tableUi->setPosition(Vec2(winSize / 2));

		//�㲥֪ͨ
		_NoticeBG = Sprite::create(Notice_Bg);
		_NoticeBG->setPosition(Vec2(winSize.width / 2, winSize.height * 0.80f));
		_NoticeBG->setCascadeOpacityEnabled(true);
		this->addChild(_NoticeBG, NOTICE_ZORDER);
		_NoticeBG->setVisible(false);

		_notic = GameNotice::createGameNotice(Notice_Bg_Stencil);
		_notic->setPosition(Vec2(_NoticeBG->getContentSize() / 2)); 
		_notic->setCallBack([this](){
			_NoticeBG->setVisible(false);
		});
		_NoticeBG->addChild(_notic);

		showNoticeMessage(GBKToUtf8("��ӭ�����λú������磡��"));

		//���˿���
		auto btnAdmin = dynamic_cast<Button*>(_tableUi->getChildByName("Button_Admin"));
		btnAdmin->addClickEventListener([=](Ref*){
			//ֹͣ��ʱ�˳�����ʱ
			gameOutTime(false);
			this->stopActionByTag(outTime_tag);

			GameAdmin* adminLayer = GameAdmin::create(_tableLogic);
			adminLayer->setPosition(winSize / 2);
			adminLayer->setName("adminLayer");
			addChild(adminLayer, MAX_ZORDER);

			adminLayer->onCloseCallBack = [this](){
				//��ʼ�´ε���ʱ
				auto seq = Sequence::create(DelayTime::create(60.f), CallFunc::create([this](){
					gameOutTime(true);
				}), nullptr);

				seq->setTag(outTime_tag);
				this->runAction(seq);
			};
			RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_UPDATE_SUPER_INFO_SIG);
		});

		//����
		Vector<Node*> children = _tableUi->getChildren();
		float scY = 720 / winSize.height;

		for (auto child : children)
		{
			if (winSize.width / winSize.height <= 1.7f)
			{
				child->setScaleY(scY);
			}
		}

		//��ʼ���������
		_fishManager = new GameFishManager(this);

		//�����ɹ��ص�
		_fishManager->onLockFishCallBack = [this](INT fishId){
			_isReadyLock = false;
			_lockFishId = fishId;
			//������̨������
			_players[_tableLogic->_MYSEATNO]->setIsLockFish(true, _lockFishId);

			//ȥ��������ʾ
			auto lockTip = dynamic_cast<ImageView*>(_tableUi->getChildByName("Image_LockTip"));
			lockTip->setVisible(false);
		};

		//�����ɹ��ص�
		_fishManager->unLockFishCallBack = [this](int fishId){
			//��������������Լ��������㣬���Լ����ڲ�������
			if (_lockFishId == fishId)
			{
				_lockFishId = INVALID_FISHID;				
			}

			for (int i = 0; i < PLAY_COUNT; i++)
			{
				_players[i]->setIsLockFish(false, fishId);
			}
		};

		_fishTrace	= new SceneFishTrace();

		//��ʼ�������̨
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			auto player = GamePlayer::createGamePlayer(i, _tableUi, _fishManager);
			player->setBatteryVisible(false);
			_players[i] = player;
			addChild(player);
		}

		auto newThread = new std::thread(&GameTableUI::asyncBuildData, this);
		newThread->detach();

		scheduleOnce(schedule_selector(GameTableUI::delayedLoadAction), 1.0f);
		//��ʼ��������
		initToolBarData();

		//��ʼ�����칦��ҳ��
		showOrHideGameChat(false);

		_tableLogic = new GameTableLogic(this, _deskIndex, _bAutoCreate);
		_tableLogic->enterGame();

		return true;
	}

	void GameTableUI::asyncBuildData()
	{
		auto winSize = Director::getInstance()->getWinSize();

		//�����㳱·������
		_fishTrace->BuildSceneKind1Trace(winSize.width, winSize.height);
		_fishTrace->BuildSceneKind2Trace(winSize.width, winSize.height);
		_fishTrace->BuildSceneKind3Trace(winSize.width, winSize.height);
		_fishTrace->BuildSceneKind4Trace(winSize.width, winSize.height);
		_fishTrace->BuildSceneKind5Trace(winSize.width, winSize.height);
	}

	void GameTableUI::delayedLoadAction(float dt)
	{
		auto winSize = Director::getInstance()->getWinSize();

		//ˮ�⶯��
		char name[24];
		auto waterAnimation = Animation::create();
		waterAnimation->setDelayPerUnit(0.1f);
		waterAnimation->setLoops(-1);
		for(int i = 1; i <= 16; i++)
		{
			sprintf(name, "water%d.png", i);
			waterAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
		}
		auto  animate = Animate::create(waterAnimation);
		auto water = Sprite::createWithSpriteFrameName(name);
		float X = winSize.width / water->getContentSize().width;
		float Y = winSize.height / water->getContentSize().height;
		water->setScale(X, Y);
		water->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
		water->setOpacity(0);
		water->runAction(Sequence::create(FadeIn::create(0.5f), animate, nullptr));
		addChild(water, WATER_ZORDER);

		//��������Ч��
		ParticleSystem *meteor = ParticleSystemQuad::create("goldenToad/particle/qipao.plist");
		meteor->setPosition(winSize / 2);
		meteor->setAutoRemoveOnFinish(true);
		addChild(meteor, 1);
	}

	void GameTableUI::initToolBarData()
	{
		auto toolBarBG = dynamic_cast<Layout*>(ui::Helper::seekWidgetByName(_tableUi, "Panel_ToolBar"));
		auto toolBar = GoldenToadToolbar::create(toolBarBG);
		addChild(toolBar);

		toolBar->_onExit = [this](){
			gameSettlementAndExit();
		};

		toolBar->_onAutoFire = [this](Button* autoBtn){
			_autoBtn = autoBtn;
			if (!_isAutoFire)
			{
				_isAutoFire = true;
				schedule(schedule_selector(GameTableUI::setAtuoFire), 0.2f);
				autoBtn->setBright(false);
			}
			else
			{
				_isAutoFire = false;
				unschedule(schedule_selector(GameTableUI::setAtuoFire));
				autoBtn->setBright(true);
			}
		};

		toolBar->_onChat = [this](){
			auto chatLayer = dynamic_cast<GameChatDlgLayer*>(this->getChildByName("chatLayer"));
			if (chatLayer)
			{
				chatLayer->showOrHideChatDlg(true);
			}
		};

		toolBar->_onHelp = [this](){
			//ֹͣ��ʱ�˳�����ʱ
			gameOutTime(false);
			this->stopActionByTag(outTime_tag);

			auto helpLayer = dynamic_cast<Layout*>(_tableUi->getChildByName("Panel_Help"));
			helpLayer->setLocalZOrder(1);
			helpLayer->setOpacity(255);
			helpLayer->setVisible(true);
			auto btnClose = dynamic_cast<Button*>(helpLayer->getChildByName("Button_Close"));
			btnClose->addClickEventListener([=](Ref*){
				helpLayer->runAction(Sequence::create(FadeOut::create(0.1f), Hide::create(), nullptr));
				//��ʼ�´ε���ʱ
				auto seq = Sequence::create(DelayTime::create(60.f), CallFunc::create([this](){
					gameOutTime(true);
				}), nullptr);

				seq->setTag(outTime_tag);
				this->runAction(seq);
			});
		};

		toolBar->_onSet = [this](){
			auto setLayer = GameSetLayer::create();
			setLayer->setName("setLayer");
			setLayer->showSet(this, 1000);
		};

		toolBar->_onLock = [this](bool bLock, bool bNormal){
			auto lockTip = dynamic_cast<ImageView*>(_tableUi->getChildByName("Image_LockTip"));
			if (!bLock)
			{
				_isReadyLock = false;
				_players[_tableLogic->_MYSEATNO]->setIsLockFish(false, _lockFishId);
				_lockFishId = INVALID_FISHID;
				lockTip->setVisible(false);
			}
			else
			{
				if (bNormal)
				{
					_isReadyLock = true;
					lockTip->setVisible(true);
				}
				else
				{
					_fishManager->setLockFish(Vec2::ZERO, BIGFISH);
				}
			}
		};
	}

	void GameTableUI::addUser(BYTE seatNo, bool bMe)
	{
		if (!isValidSeat(seatNo))	return;

		BYTE logicSeatNo = _tableLogic->viewToLogicSeatNo(seatNo);
		UserInfoStruct* userInfo = _tableLogic->getUserBySeatNo(logicSeatNo);

		_players[seatNo]->setIsMe(bMe);
		_players[seatNo]->setBatteryVisible(true);
		_players[seatNo]->setUserID(_tableLogic->getUserId(seatNo));
		_players[seatNo]->setCannonType(BULLET_KIND_1_NORMAL);
		_players[seatNo]->setCannonNum(10);
		_players[seatNo]->setGameTable(this);
	}

	void GameTableUI::removeUser(BYTE seatNo, bool bMe)
	{
		if (!isValidSeat(seatNo))	return;
		_players[seatNo]->setBatteryVisible(false);
		_players[seatNo]->setIsEnergyCannon(false);
		_players[seatNo]->setIsLockFish(false, INVALID_FISHID);
		_players[seatNo]->setCannonNum(0);
		_players[seatNo]->setUserMoney(0);
	}

	void GameTableUI::dealSetUserMoney(BYTE seatNo, LLONG value)
	{
		if (!isValidSeat(seatNo))	return;
		_players[seatNo]->setUserMoney(value);
	}

	void GameTableUI::dealSetUserCannonNum(BYTE seatNo, INT value)
	{
		if (!isValidSeat(seatNo))	return;
		_players[seatNo]->setCannonNum(value);
	}

	void GameTableUI::dealSetGameConfig(CMD_S_GameConfig* gConfig)
	{
		_fishManager->setConfig(gConfig);
		
		for (BYTE i = 0; i < PLAY_COUNT; i++)
		{
			_players[i]->setConfig(gConfig);
		}
	}

	//����Ϸʱ��Ϸ�ı���ͼ��Ϊ�˸�pcͬ��
	void GameTableUI::dealSetSceneStyle(SceneStyle Scene)
	{
		char name[12];
		std::string bgName(BG_PATH);
		sprintf(name, "bg%d.png", Scene + 1);
		bgName.append(name);
		_currentBg->loadTexture(bgName);

		playSeaweedAnimation(Scene);
		
		if (!_isAutoFire && _isAllowFire)
		{
			//��ʼ��ʱ�˳�����ʱ
			auto seq = Sequence::create(DelayTime::create(60.f), CallFunc::create([this](){
				gameOutTime(true);
			}), nullptr);

			seq->setTag(outTime_tag);
			this->runAction(seq);
		}
	}

	void GameTableUI::dealLockFishTimeOut()
	{
		_fishManager->setAllFishIsPause(false);
	}

	void GameTableUI::dealBulletTimeOut(BYTE seatNo)
	{
		_players[seatNo]->setIsEnergyCannon(false);
	}

	//����
	void GameTableUI::dealGameFire(CMD_S_UserFire* pFire, bool bMeFire)
	{
		BYTE seatNo = pFire->chair_id;
		if (seatNo >= PLAY_COUNT || seatNo < 0) return;
		if (!_players[seatNo]->batteryIsVisible()) return;

		//ת����ҿ��ڽǶ�
		float degree = 0.0f;
		Fish* LockFish = nullptr;
		if (pFire->lock_fishid > 0)
		{
			LockFish = dynamic_cast<Fish*>(_fishManager->getFishById(pFire->lock_fishid));
		}

		//��������������ڽǶ�Ϊ�ڿں�Ŀ����ĽǶȣ�����Ϊ��������������ҿ��ڽǶ�
		if (pFire->lock_fishid != INVALID_FISHID && nullptr != LockFish)
		{
			//�˴ο���������������Ҫ����������Ϣ
			_players[seatNo]->setIsLockFish(true, pFire->lock_fishid);
			//ת���ڿڵ�Ŀ����֮��Ŀ��ڽǶ�
			Vec2 cononpoint = _players[seatNo]->getCannonPoint();
			float angle = MathAide::CalcAngle(LockFish->getPositionX(), LockFish->getPositionY(), cononpoint.x, cononpoint.y);
			degree = CC_RADIANS_TO_DEGREES(angle);
		}
		else
		{
			_players[seatNo]->setIsLockFish(false, INVALID_FISHID);
			degree = CC_RADIANS_TO_DEGREES(pFire->angle);
		}
		
		//���bMeFire��ָ���ؿ��ڣ���ֹ�����ӳ٣��Լ����ڲ��ȷ�������Ϣ����ֱ����ʾ
		if (!bMeFire)
		{
			//������ҽ��
			_players[seatNo]->setUserMoney(pFire->fish_score);
		}

		if (!_players[seatNo]->getIsMe())
		{
			//��ʾ��ҿ���
			_players[seatNo]->gameFire(degree, seatNo, pFire->bullet_id, pFire->bullet_kind, pFire->bullet_mulriple, pFire->lock_fishid, pFire->bIsRobot);
		}
		else
		{
			//������ڵ����Լ�����ֻ��ʾ���ؿ�����Ϣ
			if (bMeFire)
			{
				_players[seatNo]->gameFire(degree, seatNo, pFire->bullet_id, pFire->bullet_kind, pFire->bullet_mulriple, pFire->lock_fishid);
			}
		}
	}

	//�����ͨ����Ϣ������Ϣ�����������Ŀ������
	void GameTableUI::dealAddFishInfo(void* object, WORD objectSize)
	{
		CCASSERT(objectSize % sizeof(CMD_S_FishTrace) == 0, "CMD_S_FishTrace size is error");
		if (objectSize % sizeof(CMD_S_FishTrace) != 0) return;
		
		BYTE count = objectSize / sizeof(CMD_S_FishTrace);
		CMD_S_FishTrace* pTrace = (CMD_S_FishTrace*)(object);

		auto winSize = Director::getInstance()->getWinSize();

		for (BYTE i = 0; i < count; i++)
		{
			std::vector<Vec2> initPos;
			for (BYTE j = 0;  j < pTrace->init_count; j++)
			{
				//��Ϊpc��y������Ϊ������0��������y�ᷴ������y��ȡ��
				auto pos = Vec2(pTrace->init_pos[j].x / kResolutionWidth * winSize.width,
					(1.0f - pTrace->init_pos[j].y / kResolutionHeight) * winSize.height);
				initPos.push_back(pos);
			}
			std::vector<FPointAngle> fishTraceList;
			_fishManager->addFish(pTrace->fish_kind, pTrace->fish_id, &initPos, fishTraceList, pTrace->fish_kind_ex, pTrace->trace_type);

			++pTrace;
		}
	}

	//���������������ڴ�λ�ó�����Ȧ��Ⱥ
	void GameTableUI::dealAddGroupFishInfo(CMD_S_GroupFishTrace* pFish)
	{
		if (0 == pFish->byIndex)
		{
			if (_fpCircleFishCenter[0].x == 0 && _fpCircleFishCenter[0].y == 0)
			{
				return;
			}
		}
		else
		{
			if (_fpCircleFishCenter[1].x == 0 && _fpCircleFishCenter[1].y == 0)
			{
				return;
			}
		}

		//���ݱ���Ĵ�����������λ�����ó���2Ȧ����ж�·��
		for (int i = 0; i < pFish->fish_count; ++i)
		{
			Vec2 init_pos[5];
			int center_x ;
			int center_y ;
			if (0 == pFish->byIndex)
			{
				center_x = _fpCircleFishCenter[0].x;
				center_y = _fpCircleFishCenter[0].y;
			}
			else
			{
				center_x = _fpCircleFishCenter[1].x;
				center_y = _fpCircleFishCenter[1].y;
			}

			auto winSize = Director::getInstance()->getWinSize();
			float fAngle = (float)((M_PI*2) / (pFish->fish_count));

			init_pos[0].x = static_cast<float>(center_x);
			init_pos[0].y = static_cast<float>(center_y);
			init_pos[1].x = static_cast<float>(center_x + sin((float)(i*fAngle))*500);
			init_pos[1].y = static_cast<float>(center_y + cos((float)(i*fAngle))*500);
			init_pos[2].x = static_cast<float>(center_x + sin((float)(i*fAngle))*1440);
			init_pos[2].y = static_cast<float>(center_y + cos((float)(i*fAngle))*900);

			if (true) 
			{
				init_pos[1].x = init_pos[2].x;
				init_pos[1].y = init_pos[2].y;
			}

			std::vector<Vec2> initPos;
			for (BYTE j = 0;  j < 3; j++)
			{
				auto pos = Vec2(init_pos[j].x, init_pos[j].y);
				initPos.push_back(pos);
			}
			std::vector<FPointAngle> fishTraceList;
			_fishManager->addFish(pFish->fish_kind[i], pFish->fish_id[i], &initPos, fishTraceList, pFish->fish_red_kind, TRACE_LINEAR, true);
		}
	}

	void GameTableUI::dealCatchFish(CMD_S_CatchFish* pFish)
	{
		Fish* fish = _fishManager->getFishById(pFish->fish_id);
		if (nullptr == fish) return;

		if (pFish->bullet_ion)
		{
			_players[pFish->chair_id]->setIsEnergyCannon(true);
		}

		if (_tableLogic->_MYSEATNO == pFish->chair_id)
		{
			//�Լ�����������������ͼ�1��Ϊ�˽���չʾ��
			CCASSERT( pFish->fish_kind < FISH_KIND_COUNT , "��������Խ��");
			if (pFish->fish_kind < FISH_KIND_COUNT)
			{
				_resultInfo->numberOfKindFIsh[pFish->fish_kind]++;
				_resultInfo->userGoldMoney += pFish->fish_score;
			}
		}

		//ץ������������������ֹͣ�ζ�
		if (FISH_KIND_19 == pFish->fish_kind)
		{
			_fishManager->setAllFishIsPause(true);
		}

		if (fish != nullptr)
		{
			GoldenToadGameAudio::playCatchEffect();
			//����ץ�����ҷ�������ʾ���𶯻�
			playAwardFishAnimation(pFish->chair_id, pFish->fish_score, pFish->i64UserFishScore_, fish);
			//��ʾ�������������Ƴ�
			_fishManager->removeOrSetFishDieById(pFish->fish_id, true);
		}
	}

	void GameTableUI::playAwardFishAnimation(BYTE bSeatNo, LLONG money, LLONG userMoney, Fish* fish)
	{
		//ץ���������ʹ���16��Ҫ��������Ч
		if (fish->getFishKind() >= FISH_KIND_17)
		{
			GoldenToadGameAudio::playGoldEffect();

			this->stopActionByTag(100);
			this->setPosition(Size::ZERO);
			auto shake = Shake::create(1.0f, 30, 30);
			shake->setTag(100);
			this->runAction(shake);
		}

		//�����������Ҫ��ʾbingoת��
		if ((fish->getFishKind() >= FISH_KIND_18 && fish->getFishKind() <= FISH_KIND_21) ||
			(fish->getFishKind() >= FISH_KIND_25 && fish->getFishKind() <= FISH_KIND_30)) 
		{
			if (money > 0)
			{
				_players[bSeatNo]->setBingo(money);
			}
		}

		ParticleSystem *meteor = nullptr;
		switch (fish->getFishKind())
		{
		case FISH_KIND_17:
		case FISH_KIND_18:
		case FISH_KIND_20:
			{
				meteor = ParticleSystemQuad::create("goldenToad/particle/shayu.plist");
			}
			break;
		case FISH_KIND_19:
			{
				meteor = ParticleSystemQuad::create("goldenToad/particle/ding.plist");

				auto ding = Sprite::create("goldenToad/particle/Ding_Magic.png");
				ding->setPosition(fish->getPosition());
				addChild(ding, FISH_ZORDER + 1);
				ding->runAction(Sequence::create(Blink::create(3.0f, 3), DelayTime::create(1.0f), RemoveSelf::create(true), nullptr));

				Size winSize = Director::getInstance()->getWinSize();

				auto snowParticle = ParticleSystemQuad::create("goldenToad/particle/snow.plist");
				snowParticle->setPosition(Vec2(winSize.width / 2, winSize.height + 50));
				snowParticle->setAutoRemoveOnFinish(true);
				snowParticle->setName("snowParticle");
				addChild(snowParticle, FISH_ZORDER + 2);
				snowParticle->runAction(Sequence::create(DelayTime::create(10.5f), FadeOut::create(0.5f), RemoveSelf::create(true), nullptr));
			}
			break;
		case FISH_KIND_21:
		case FISH_KIND_22:
			{
				meteor = ParticleSystemQuad::create("goldenToad/particle/jinlong.plist");
			}
			break;
		case FISH_KIND_23:
		case FISH_KIND_25:
		case FISH_KIND_26:
			{
				meteor = ParticleSystemQuad::create("goldenToad/particle/bigwin.plist");
			}
			break;
		case FISH_KIND_27:
		case FISH_KIND_28:
		case FISH_KIND_29:
		case FISH_KIND_30:
			{
				meteor = ParticleSystemQuad::create("goldenToad/particle/dasanyuan.plist");
			}
			break;
		default:
			break;
		}

		if (nullptr != meteor)
		{
			meteor->setPosition(fish->getPosition());
			meteor->setAutoRemoveOnFinish(true);
			addChild(meteor, FISH_ZORDER + 1);
		}
		
		char str[24];
		if (money > 0)
		{
			//�������ý������		
			sprintf(str, "%lld", money);
			auto moneyText = TextAtlas::create(str, Score_number_path, 54, 62, ".");
			if (fish->getFishKind() <= FISH_KIND_5)
			{
				moneyText->setScale(0.6f);
			}
			moneyText->setPosition(fish->getPosition());
			this->addChild(moneyText, FISH_ZORDER + 2);

			moneyText->runAction(Sequence::create(
				JumpBy::create(0.3f, Vec2(this->getPosition()), 15, 1), 
				DelayTime::create(0.5f),
				Spawn::create(MoveBy::create(1.0f, Vec2(0, 50)), 
				FadeOut::create(1.0f), nullptr), 
				CallFunc::create([=](){
					//GoldenToadGameAudio::playCatchEffect();
					moneyText->removeFromParent();
			}), nullptr));
		}

		Vec2 pos = _players[bSeatNo]->getMoneyPoint();
		auto eFKind = fish->getFishKind();
		Vec2 fish_pos = fish->getPosition();

		//��Ҫ���������Ͳ�ͬ������ͬ�������
		int coin_count = G_Fish_Coin_Count[eFKind];
		auto cache = SpriteFrameCache::getInstance();
		
		_players[bSeatNo]->setUserMoney(userMoney);
		//���ݻ�õĸ�����λ������������Ҷ���
		for (int i = 0; i < coin_count; ++i) 
		{
			//�������λ��
			int k = (rand() % 150) - 75;
			int s = (rand() % 150) - 75;

			std::string name("coin21_");
			if (fish->getFishKind() < FISH_KIND_5)
			{
				name = "coin11_";
			}

			Animate* animate = nullptr;
			auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(name));
			if (!animation)
			{
				animate = buildAnimation(name, 12, -1, 0.08f);
			}
			else
			{
				animate = Animate::create(animation);
			}

			auto coin = Sprite::create();
			coin->setPosition(Vec2(fish_pos.x + k, fish_pos.y + s));
			this->addChild(coin, FISH_ZORDER + 1);
			coin->runAction(Spawn::create(animate, Sequence::create(DelayTime::create(1.5f),
				MoveTo::create(0.5f, pos), CallFunc::create([=](){
					//_players[bSeatNo]->setUserMoney(userMoney);
			}), RemoveSelf::create(), nullptr), nullptr));
		}
	}

	void GameTableUI::dealCatchSweepFish(CMD_S_CatchSweepFish* pSweep)
	{
		Fish* fish = _fishManager->getFishById(pSweep->fish_id);
		if (nullptr == fish) return;

		//ץ������Ҫ�ѵ�ǰ��Ļ�������Ϣ����������
		CMD_C_CatchSweepFish pRet = _fishManager->catchSweepFish(pSweep->chair_id , pSweep->fish_id);
		_tableLogic->sendSweepFish(&pRet, sizeof(CMD_C_CatchSweepFish));

		//�����������������������Ҫ��¼������
		if (fish->getFishKind() == FISH_KIND_32)
		{
			_fishManager->getFishPositionByFishId(fish->getFishID());
			if (0 == pSweep->byIndex)
			{
				_fpCircleFishCenter[0].x = fish->getPositionX();
				_fpCircleFishCenter[0].y = fish->getPositionY();
			}
			else
			{
				_fpCircleFishCenter[1].x = fish->getPositionX();
				_fpCircleFishCenter[1].y = fish->getPositionY();
			}
		}
	}

	void GameTableUI::dealCatchSweepFishResult(CMD_S_CatchSweepFishResult* pResult)
	{
		auto winSize = Director::getInstance()->getWinSize();
		auto fish = _fishManager->getFishById(pResult->fish_id);

		if (fish != nullptr)
		{
			GoldenToadGameAudio::playGoldEffect();

			if (_tableLogic->_MYSEATNO == pResult->chair_id)
			{
				//�Լ�����������������ͼ�1��Ϊ�˽���չʾ��
				if (fish->getFishKind() < FISH_KIND_COUNT)
				{
					_resultInfo->numberOfKindFIsh[fish->getFishKind()]++;
					_resultInfo->userGoldMoney += pResult->fish_score_Sum;
				}
			}
			//��ʾ���������������Ƴ���ը����������
			_fishManager->removeOrSetFishDieById(pResult->fish_id, true);
			//����ץ�������ҷ�������ʾ���𶯻�
			playAwardFishAnimation(pResult->chair_id, pResult->fish_score_Sum, pResult->i64UserFishScore_, fish);

			if (pResult->fish_score_Sum > 0)
			{
				_players[pResult->chair_id]->setBingo(pResult->fish_score_Sum);
			}

			GoldenToadGameAudio::playBombEffect();

			ParticleSystem *meteor = nullptr;
			switch (fish->getFishKind())
			{
			case FISH_KIND_24:
				{
					meteor = ParticleSystemQuad::create("goldenToad/particle/bomb.plist");
				}
				break;
			case FISH_KIND_31:
				{
					meteor = ParticleSystemQuad::create("goldenToad/particle/xuehua.plist");
				}
				break;
			case FISH_KIND_32:
				{
					meteor = ParticleSystemQuad::create("goldenToad/particle/yanhua.plist");
				}
				break;
			default:
				break;
			}

			if (nullptr != meteor)
			{
				meteor->setPosition(fish->getPosition());
				meteor->setAutoRemoveOnFinish(true);
				addChild(meteor, FISH_ZORDER);
			}

			//ץ��ȫ��ը��������������
			for (int i = 0; i < pResult->catch_fish_count; i++)
			{
				int smallFishId = pResult->catch_fish_id[i];
				auto smallFish = _fishManager->getFishById(smallFishId);
				if (nullptr != smallFish)
				{
					//ץ�����������ʹ���������������������ͬ�������������������������Ч
					if (fish->getFishKind() == FISH_KIND_31 || fish->getFishKind() == FISH_KIND_32)
					{
						Vec2 fishPos = fish->getPosition();
						Vec2 sFishPos = _fishManager->getFishPositionByFishId(smallFishId);

						std::string name = (fish->getFishKind() == FISH_KIND_31) ? "goldenToad/particle/Light_Blue.png" : "goldenToad/particle/Light_Red.png";
						auto light = Sprite::create(name);
						auto range = sqrt(pow(sFishPos.x - fishPos.x, 2) + pow(sFishPos.y - fishPos.y, 2));
						float degree = (atan2(sFishPos.x - fishPos.x, sFishPos.y - fishPos.y) * 180 / 3.14f) - 90;
						float scale = range / light->getContentSize().width;
						light->setScaleX(0.0f);
						light->setRotation(degree);
						light->setAnchorPoint(Vec2(0, 0.5f));
						light->setPosition(fishPos);

						addChild(light, FISH_ZORDER);
						light->runAction(Sequence::create(ScaleTo::create(1.0f, scale), DelayTime::create(0.5f),
							RemoveSelf::create(true), nullptr));
					}

					//��ʾ�������������Ƴ�
					_fishManager->removeOrSetFishDieById(smallFishId, true);
					//����ץ�����ҷ�������ʾ���𶯻�
					playAwardFishAnimation(pResult->chair_id, pResult->fish_score[i], pResult->i64UserFishScore_, smallFish);
				}				
			}	
		}
	}

	void GameTableUI::dealCatchSweepFishResultEx(CMD_S_CatchSweepFishResultEx* pResult)
	{
		char str[128];
		sprintf(str, "%s%d������%s�����%s���������%lld�бҽ�������", pResult->cRoomName, pResult->iDeskID + 1, pResult->cFishName, 
			pResult->szNickName, pResult->fish_score);
		showNoticeMessage(GBKToUtf8(str));
	}

	void GameTableUI::dealSwitchScene(CMD_S_SwitchScene* pScene)
	{
		_switchScene = *pScene;

		auto winSize = Director::getInstance()->getWinSize();
		//���Ų�����Ч
		GoldenToadGameAudio::pauseBackgroundMusic();
		GoldenToadGameAudio::playWaveEffect();

		//���˶���
		char name[64];
		auto waterAnimation = Animation::create();
		waterAnimation->setDelayPerUnit(0.5f);
		waterAnimation->setLoops(-1);
		for(int i = 1; i <= 2; i++)
		{
			sprintf(name, "goldenToad/Scene/wave/wave_%02d.png", i);
			waterAnimation->addSpriteFrameWithFile(name);
		}
		auto  animate = Animate::create(waterAnimation);
		auto water = Sprite::create(name);
		float ScaleY = winSize.height/ water->getContentSize().height;
		water->setScaleY(ScaleY);
		water->setAnchorPoint(Vec2(0, 0.5f));
		water->setPosition(Vec2(winSize.width - 200, winSize.height / 2));
		water->runAction(animate);
		addChild(water, 200);

		//�л�����ͼ
		std::string bgName(BG_PATH);
		sprintf(name, "bg%d.png", pScene->Scene_Style + 1);
		bgName.append(name);

		auto backGround = ImageView::create(bgName);
		backGround->setAnchorPoint(Vec2(0 , 0.5f));
		float _xScale = winSize.width / backGround->getContentSize().width;
		float _yScale = winSize.height/ backGround->getContentSize().height;
		backGround->setScaleX(_xScale);
		backGround->setScaleY(_yScale);
		backGround->setPosition(Vec2(winSize.width, winSize.height / 2));
		addChild(backGround);

		auto oldBG = _currentBg;
		_currentBg = backGround;

		_isAllowFire = false;
		//�Ƴ�������ͼ��ײ����
		_fishManager->setSwitchScene(backGround, true);

		//��Ӳ��ź��ݱ��Ƕ���
		playSeaweedAnimation(pScene->Scene_Style);

		//����ͼ���������˶�
		auto water_move = MoveBy::create(SWITCH_SCENE_TIME, Vec2( - winSize.width, 0));
		auto bg_move = MoveBy::create(SWITCH_SCENE_TIME, Vec2( - winSize.width, 0));
		water->runAction(Sequence::create(water_move, FadeOut::create(0.2f), RemoveSelf::create(true), nullptr));

		backGround->runAction(Sequence::create(bg_move, CallFunc::create([=](){
			oldBG->removeFromParent();
			_fishManager->setSwitchScene(nullptr, false);
			_isAllowFire = true;
			switchScene();
		}), nullptr));		
	}

	//���ź��ݱ��Ƕ���
	void GameTableUI::playSeaweedAnimation(SceneStyle style)
	{
		switch (style)
		{
		case GoldenToad::SCENE_STYLE_1:
			{
				Animate* beike = buildAnimation("bk_", 24, 1, 0.1f);
				auto bkSp = Sprite::createWithSpriteFrameName("bk_01.png");
				bkSp->setPosition(Vec2(_currentBg->getContentSize().width * 0.2f, _currentBg->getContentSize().height * 0.5f));
				bkSp->runAction(RepeatForever::create(Sequence::create(beike, CallFunc::create([=](){
					auto meteor = ParticleSystemQuad::create("goldenToad/particle/beikeqipao.plist");
					meteor->setPosition(bkSp->getContentSize() / 2);
					meteor->setAutoRemoveOnFinish(true);
					bkSp->addChild(meteor);
				}), DelayTime::create(10.0f), nullptr)));
				_currentBg->addChild(bkSp, 1);

				Animate* haicao = buildAnimation("shuicao_5_", 14, -1, 0.2f);
				auto hcSp = Sprite::createWithSpriteFrameName("shuicao_5_01.png");
				hcSp->setPosition(Vec2(_currentBg->getContentSize().width * 0.8f, _currentBg->getContentSize().height * 0.35f));
				hcSp->runAction(RepeatForever::create(haicao));
				_currentBg->addChild(hcSp, 1);
			}
			break;
		case GoldenToad::SCENE_STYLE_2:
			{
				Animate* haicao1 = buildAnimation("shuicao_1_", 29, -1, 0.1f);
				auto hcSp1 = Sprite::createWithSpriteFrameName("shuicao_1_01.png");
				hcSp1->setPosition(Vec2(_currentBg->getContentSize().width * 0.18f, _currentBg->getContentSize().height * 0.45f));
				hcSp1->runAction(RepeatForever::create(haicao1));
				_currentBg->addChild(hcSp1, 1);

				Animate* haicao2 = buildAnimation("shuicao_5_", 14, -1, 0.2f);
				auto hcSp2 = Sprite::createWithSpriteFrameName("shuicao_5_01.png");
				hcSp2->setPosition(Vec2(_currentBg->getContentSize().width * 0.85f, _currentBg->getContentSize().height * 0.55f));
				hcSp2->runAction(RepeatForever::create(haicao2));
				_currentBg->addChild(hcSp2, 1);
			}
			break;
		case GoldenToad::SCENE_STYLE_3:
			{
				Animate* beike = buildAnimation("bk_", 24, 1, 0.1f);
				auto bkSp = Sprite::createWithSpriteFrameName("bk_01.png");
				bkSp->setPosition(Vec2(_currentBg->getContentSize().width * 0.35f, _currentBg->getContentSize().height * 0.36f));
				bkSp->runAction(RepeatForever::create(Sequence::create(beike, CallFunc::create([=](){
					auto meteor = ParticleSystemQuad::create("goldenToad/particle/beikeqipao.plist");
					meteor->setPosition(bkSp->getContentSize() / 2);
					meteor->setAutoRemoveOnFinish(true);
					bkSp->addChild(meteor);
				}), DelayTime::create(8.0f), nullptr)));
				_currentBg->addChild(bkSp, 1);

				Animate* haixing = buildAnimation("haixing_", 7, 2, 0.1f);
				auto hxSp = Sprite::createWithSpriteFrameName("haixing_01.png");
				hxSp->setScale(0.7f);
				hxSp->setPosition(Vec2(_currentBg->getContentSize().width * 0.55f, _currentBg->getContentSize().height * 0.72f));
				hxSp->runAction(RepeatForever::create(Sequence::create(haixing, DelayTime::create(10.0f), nullptr)));
				_currentBg->addChild(hxSp, 1);
			}
			break;
		default:
			break;
		}
	}

	//��������
	Animate* GameTableUI::buildAnimation(string name, int num, int loops, float speed)
	{
		auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(name));
		if (!animation)
		{
			animation = Animation::create();
			animation->setDelayPerUnit(speed);
			animation->setLoops(loops);
			for (int i = 1; i <= num; i++)
			{
				std::string temp(name);
				temp.append(StringUtils::format("%02d.png", i));
				animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(temp));
			}
			AnimationCache::getInstance()->addAnimation(animation, name);
		}

		auto animate = Animate::create(animation);
		return animate;
	}

	void GameTableUI::switchScene()
	{
		GoldenToadGameAudio::resumeBackgroundMusic();
		GoldenToadGameAudio::playBackgroundMusic();

		switch (_switchScene.scene_kind)
		{
		case SCENE_KIND_1:
			{
				assert(_switchScene.fish_count == arraysize(_fishTrace->scene_kind_1_trace_));
				if (_switchScene.fish_count != arraysize(_fishTrace->scene_kind_1_trace_))
					return;

				int index = 0;
				for (int i = 0; i < _switchScene.fish_count; ++i) 
				{
					FishKind fish_kind_ex[5] = {FISH_KIND_1};
					if (_switchScene.fish_kind[i] == FISH_KIND_31)
					{
						for(int k=0; k<5; k++)
						{
							fish_kind_ex[k] = _switchScene.fish_kindEx[index];
						}
						index++;
					}

					//�������ɵ��㳱·�����ݸ���ǰ��
					std::vector<FPointAngle> fishTraceList;
					std::copy(_fishTrace->scene_kind_1_trace_[i].begin(), _fishTrace->scene_kind_1_trace_[i].end(), std::back_inserter(fishTraceList));
					//����������
					_fishManager->addFish(_switchScene.fish_kind[i], _switchScene.fish_id[i], nullptr, fishTraceList, fish_kind_ex);
				}
			}
			break;
		case SCENE_KIND_2:
			{
				assert(_switchScene.fish_count == arraysize(_fishTrace->scene_kind_2_trace_));
				if (_switchScene.fish_count != arraysize(_fishTrace->scene_kind_2_trace_)) 
					return;

				for (int i = 0; i < _switchScene.fish_count; ++i) 
				{
					FishKind fish_kind_ex[5] = {FISH_KIND_1};
					//�������ɵ��㳱·�����ݸ���ǰ��
					std::vector<FPointAngle> fishTraceList;
					std::copy(_fishTrace->scene_kind_2_trace_[i].begin(), _fishTrace->scene_kind_2_trace_[i].end(), std::back_inserter(fishTraceList));
					//����������
					auto fish = _fishManager->addFish(_switchScene.fish_kind[i], _switchScene.fish_id[i], nullptr, fishTraceList, fish_kind_ex);
					//�㳱2Ҫ����������С���ε���Ļ�еȴ����㾭������������ֹͣʱ��
					if (nullptr != fish)
					{
						if (i < 200) 
						{
							fish->setFishStop(_fishTrace->scene_kind_2_small_fish_stop_index_[i], _fishTrace->scene_kind_2_small_fish_stop_count_);
						} 
						else 
						{
							fish->setFishStop(_fishTrace->scene_kind_2_big_fish_stop_index_, _fishTrace->scene_kind_2_big_fish_stop_count_);
						}
					}
				}
			}
			break;
		case SCENE_KIND_3:
			{
				assert(_switchScene.fish_count == arraysize(_fishTrace->scene_kind_3_trace_));
				if (_switchScene.fish_count != arraysize(_fishTrace->scene_kind_3_trace_)) 
					return;

				int index = 0;
				for (int i = 0; i < _switchScene.fish_count; ++i) 
				{
					FishKind fish_kind_ex[5] = {FISH_KIND_1};
					if (_switchScene.fish_kind[i] == FISH_KIND_31)
					{
						for(int k=0; k<5; k++)
						{
							fish_kind_ex[k] = _switchScene.fish_kindEx[index];

						}
						index++;
					}

					//�������ɵ��㳱·�����ݸ���ǰ��
					std::vector<FPointAngle> fishTraceList;
					std::copy(_fishTrace->scene_kind_3_trace_[i].begin(), _fishTrace->scene_kind_3_trace_[i].end(), std::back_inserter(fishTraceList));
					//����������
					_fishManager->addFish(_switchScene.fish_kind[i], _switchScene.fish_id[i], nullptr, fishTraceList, fish_kind_ex);
				}
			}
			break;
		case SCENE_KIND_4:
			{
				assert(_switchScene.fish_count == arraysize(_fishTrace->scene_kind_4_trace_));
				if (_switchScene.fish_count != arraysize(_fishTrace->scene_kind_4_trace_)) 
					return;

				for (int i = 0; i < _switchScene.fish_count; ++i) 
				{
					FishKind fish_kind_ex[5] = {FISH_KIND_1};
					//�������ɵ��㳱·�����ݸ���ǰ��
					std::vector<FPointAngle> fishTraceList;
					std::copy(_fishTrace->scene_kind_4_trace_[i].begin(), _fishTrace->scene_kind_4_trace_[i].end(), std::back_inserter(fishTraceList));
					//����������
					_fishManager->addFish(_switchScene.fish_kind[i], _switchScene.fish_id[i], nullptr, fishTraceList, fish_kind_ex);
				}
			}
			break;
		case SCENE_KIND_5:
			{
				assert(_switchScene.fish_count == arraysize(_fishTrace->scene_kind_5_trace_));
				if (_switchScene.fish_count != arraysize(_fishTrace->scene_kind_5_trace_)) 
					return;

				int index = 0;
				for (int i = 0; i < _switchScene.fish_count; ++i) 
				{
					FishKind fish_kind_ex[5] = {FISH_KIND_1};
					if (_switchScene.fish_kind[i] == FISH_KIND_31)
					{
						for(int k=0; k<5; k++)
						{
							fish_kind_ex[k] = _switchScene.fish_kindEx[index];
						}
						index++;
					}
					//�������ɵ��㳱·�����ݸ���ǰ��
					std::vector<FPointAngle> fishTraceList;
					std::copy(_fishTrace->scene_kind_5_trace_[i].begin(), _fishTrace->scene_kind_5_trace_[i].end(), std::back_inserter(fishTraceList));
					//����������
					_fishManager->addFish(_switchScene.fish_kind[i], _switchScene.fish_id[i], nullptr, fishTraceList, fish_kind_ex);
				}
			}
			break;
		default:
			break;
		}
	}

	//�����˳�
	void GameTableUI::gameSettlementAndExit()
	{
		//ֹͣ��ʱ�˳�����ʱ
		gameOutTime(false);
		this->stopActionByTag(outTime_tag);

		auto node = CSLoader::createNode(Settlemen_path);
		_tableUi->addChild(node, PLAYER_ZORDER);
		node->setName("SettlementNode");

		auto panel		= dynamic_cast<Layout*>(node->getChildByName("Panel_Settlement"));
		panel->setPosition(_tableUi->getContentSize() / 2);
		//����
		auto settleBG	= dynamic_cast<ImageView*>(panel->getChildByName("Image_SettlementBG"));
		//����ʱ
		auto time = dynamic_cast<TextAtlas*>(settleBG->getChildByName("AtlasLabel_Time"));
		timing(time, 20, [=](){
			node->runAction(Sequence::create(FadeOut::create(0.1f), RemoveSelf::create(true), nullptr));
		});
		//�رհ�ť
		auto btnClose	= dynamic_cast<Button*>(settleBG->getChildByName("Button_Close"));
		btnClose->addClickEventListener([=](Ref* ref){
			time->setVisible(false);
			node->runAction(Sequence::create(FadeOut::create(0.1f), RemoveSelf::create(true), nullptr));
			//��ʼ�´ε���ʱ
			auto seq = Sequence::create(DelayTime::create(60.f), CallFunc::create([this](){
				gameOutTime(true);
			}), nullptr);

			seq->setTag(outTime_tag);
			this->runAction(seq);
		});
		//ȡ����ť
		auto btnCancle	= dynamic_cast<Button*>(settleBG->getChildByName("Button_Cancle"));
		btnCancle->addClickEventListener([=](Ref* ref){
			time->setVisible(false);
			node->runAction(Sequence::create(FadeOut::create(0.1f), RemoveSelf::create(true), nullptr));
			//��ʼ�´ε���ʱ
			auto seq = Sequence::create(DelayTime::create(60.f), CallFunc::create([this](){
				gameOutTime(true);
			}), nullptr);

			seq->setTag(outTime_tag);
			this->runAction(seq);
		});
		//�˳���ť
		auto btnTrue	= dynamic_cast<Button*>(settleBG->getChildByName("Button_True"));
		btnTrue->addClickEventListener([this](Ref* ref){
			returnPlatform();
		});

		char str[32];

		//���ץ���ջ���
		sprintf(str, "%lld", _resultInfo->userGoldMoney);
		auto fishMoney	= dynamic_cast<TextAtlas*>(settleBG->getChildByName("AtlasLabel_FishMoney"));
		fishMoney->setString(str);

		//����ܽ��
		sprintf(str, "%lld", _players[_tableLogic->_MYSEATNO]->getCurrentMoney());
		auto userMoney	= dynamic_cast<TextAtlas*>(settleBG->getChildByName("AtlasLabel_UserMoney"));
		userMoney->setString(str);

		//ÿ����ץ��������
		for (int i = 0; i < 31; i++)
		{
			sprintf(str, "AtlasLabel_Fish%d", i);
			auto fishNum = dynamic_cast<TextAtlas*>(settleBG->getChildByName(str));
			sprintf(str, "%d", _resultInfo->numberOfKindFIsh[i]);
			fishNum->setString(str);
		}
	}

	bool GameTableUI::isValidSeat(BYTE seatNo)
	{
		return (seatNo < PLAY_COUNT && seatNo >= 0);
	}

	/*------------------------------------------------------------------------------*/

	//�ֻ����ؼ��ص�
	void GameTableUI::onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		if (EventKeyboard::KeyCode::KEY_BACK != keyCode) return;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		if (EventKeyboard::KeyCode::KEY_F1 != keyCode) return;
#endif

		auto node = dynamic_cast<Node*>(_tableUi->getChildByName("SettlementNode"));
		auto helpLayer = dynamic_cast<Layout*>(_tableUi->getChildByName("Panel_Help"));

		if (helpLayer->isVisible())
		{
			helpLayer->runAction(Sequence::create(FadeOut::create(0.1f), Hide::create(), nullptr));
			return;
		}
		if (nullptr != node)
		{
			node->runAction(Sequence::create(FadeOut::create(0.1f), RemoveSelf::create(true), nullptr));
			return;
		}
		else
		{
			gameSettlementAndExit();
		}		
	}

	void GameTableUI::onDisconnect()
	{
		_tableLogic->stop();
		stopAllActions();
		unscheduleAllCallbacks();
		auto prompt = GamePromptLayer::create();
		prompt->showPrompt(GBKToUtf8("���������Ѿ��Ͽ��������µ�½��"));
		prompt->setCallBack([this](){			
			RoomLogic()->close();
			PlatformLogic()->close();
			GameMenu::createMenu();
		});
	}

	void GameTableUI::returnPlatform()
	{
		if (PlatformLogic()->isConnect())
		{
			_tableLogic->sendUserUp();
			this->runAction(Sequence::create(DelayTime::create(5.0f), CallFunc::create([=](){
				onDisconnect();
			}), nullptr));
		}
		else
		{
			onDisconnect();
		}
	}

	void GameTableUI::dealLeaveDesk()
	{
		unscheduleAllCallbacks();
		stopAllActions();
		GamePlatform::returnPlatform(LayerType::DESKLIST);
	}

	//��Ϸ��ʼ֪ͨ
	void GameTableUI::dealGameStart()
	{
		
	}

	void GameTableUI::gameOutTime(bool bShow)
	{
		auto tip = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_tableUi, "Image_ExitTip"));
		auto time = (TextAtlas*)tip->getChildByName("AtlasLabel_Time");

		tip->setVisible(bShow);
		time->setVisible(bShow);

		if (bShow)
		{
			//��ʱ����ʱ������ֱ�ӷ���վ���뿪��Ϸ
			timing(time, 10, [this](){
				returnPlatform();
			});
		}
	}

	//����ʱ����,function�ص�Ϊ�������ĵ���ʱ����Ҫ���Ĵ���
	void GameTableUI::timing(Node* node, int dt, std::function<void()> callFunc)
	{
		if (nullptr == node || !node->isVisible()) return;

		char str[12];
		sprintf(str, "%d", dt);
		dt--;
		if (dt < 0) 
		{
			callFunc();
			return;
		}

		TextAtlas* text = (TextAtlas*)node;
		text->setString(str);
		node->runAction(Sequence::create(DelayTime::create(1.0f),
			CallFuncN::create(CC_CALLBACK_1(GameTableUI::timing, this, dt, callFunc)), nullptr));
	}

	bool GameTableUI::onTouchBegan(Touch *touch, Event *unused_event)
	{
		//ֹͣ�ϴε���ʱ
		gameOutTime(false);
		this->stopActionByTag(outTime_tag);

		//�������׼���������״̬����δ�����ɹ�֮ǰ�������
		if (_isReadyLock)
		{
			//��������
			_fishManager->setLockFish(touch->getLocation(), NORMAL);
			return false;
		}

		BYTE me = _tableLogic->_MYSEATNO;

		//�������������ʱ��
		_touchPoint = touch->getLocation();
		if (_isAutoFire) return false;

		schedule(schedule_selector(GameTableUI::setAtuoFire), 0.2f);
		return true;
	}

	void GameTableUI::onTouchMoved(Touch *touch, Event *unused_event)
	{
		_touchPoint = touch->getLocation();
	}

	void GameTableUI::onTouchEnded(Touch *touch, Event *unused_event)
	{
		//�ɿ���ָж�س������������ʱ��
		unschedule(schedule_selector(GameTableUI::setAtuoFire));
		setAtuoFire(0.0f);
	}

	void GameTableUI::setAtuoFire(float dt)
	{
		if (!_isAllowFire) return;
		openFireByTouch(_touchPoint);
	}

	// ������Ļ����
	void GameTableUI::openFireByTouch(const Vec2& touchPoint)
	{
		//�Ѿ������ӵ�����20�ţ�ֹͣ���
		if (_players[_tableLogic->_MYSEATNO]->getBulletNum() > 20)
		{
			return;
		}

		if(!_players[_tableLogic->_MYSEATNO]->isFireEnable())
		{
			showNoticeMessage(GBKToUtf8("����бҲ������޷�����"));
			return ;
		}

		if (!PlatformLogic()->isConnect())
		{
			onDisconnect();
			return;
		}

		Vec2 firePos = Vec2::ZERO;

		//���û���������㣬���ڽǶ�Ϊ��ָ�����ĵط�
		if (_lockFishId == INVALID_FISHID)
		{
			firePos = touchPoint;
		}
		else
		{
			auto fish = dynamic_cast<Fish*>(_fishManager->getFishById(_lockFishId));
			if (nullptr != fish)
			{
				firePos = fish->getPosition();
			}
		}

		Vec2 cononpoint = _players[_tableLogic->_MYSEATNO]->getCannonPoint();
		CMD_C_UserFire userFire;
		userFire.bullet_kind = _players[_tableLogic->_MYSEATNO]->getBullet_Kind();
		userFire.angle = MathAide::CalcAngle(firePos.x , firePos.y , cononpoint.x , cononpoint.y);
		userFire.bullet_mulriple = _players[_tableLogic->_MYSEATNO]->getFirepower();
		userFire.lock_fishid = _lockFishId;
		userFire.bullet_id = ++_iBullet_id;

		if (_players[_tableLogic->_MYSEATNO]->getCurrentMoney() >= userFire.bullet_mulriple)
		{
			_players[_tableLogic->_MYSEATNO]->setUserMoney(_players[_tableLogic->_MYSEATNO]->getCurrentMoney() - userFire.bullet_mulriple);
		}
		else
		{
			Node* root = Director::getInstance()->getRunningScene();
			auto Prompt = dynamic_cast<GamePromptLayer*>(root->getChildByName("Prompt"));
			if (!Prompt)
			{
				Prompt = GamePromptLayer::create();
				Prompt->setName("Prompt");
				Prompt->showPrompt(GBKToUtf8("����бҲ������޷�����"));
			}

			unschedule(schedule_selector(GameTableUI::setAtuoFire));
			if (_isAutoFire)
			{
				_isAutoFire = false;
				_autoBtn->setBright(true);
			}
			return;
		}

		//ֹͣ��ʱ�˳�����ʱ
		gameOutTime(false);
		this->stopActionByTag(outTime_tag);

		//��ʼ�´ε���ʱ
		auto seq = Sequence::create(DelayTime::create(60.f), CallFunc::create([this](){
			gameOutTime(true);
		}), nullptr);

		seq->setTag(outTime_tag);
		this->runAction(seq);

		_tableLogic->sendOpenFire(&userFire, sizeof(userFire));

		//Ϊ�˷�ֹ�����ӳ�����û��������⣬�Լ�����ֱ����ʾ
		CMD_S_UserFire meFire;
		meFire.bullet_kind = userFire.bullet_kind;
		meFire.chair_id = _tableLogic->_MYSEATNO;
		meFire.angle = userFire.angle;
		meFire.bullet_mulriple = userFire.bullet_mulriple;
		meFire.lock_fishid = _lockFishId;
		meFire.bullet_id = userFire.bullet_id;
		dealGameFire(&meFire, true);
	}

	void GameTableUI::sendHitFish(BYTE bSeatNo, INT fishId, INT bulletId, BulletKind bulletKind, INT bulletMuriple)
	{
		_tableLogic->sendHitFish(bSeatNo, fishId, bulletId, bulletKind, bulletMuriple);
	}

	void GameTableUI::showNoticeMessage(const std::string& message)
	{
		_NoticeBG->setVisible(true);
		_notic->postMessage(message);
	}

	//////////////////////////////////////////////////////////////////////////////////
	//����
	//////////////////////////////////////////////////////////////////////////////////
	//��ʾ�������������
	void GameTableUI::showOrHideGameChat(bool bShow)
	{
		if (!bShow)
		{
			Size winSize = Director::getInstance()->getWinSize();
			auto chatLayer = GameChatDlgLayer::create();
			chatLayer->ignoreAnchorPointForPosition(false);
			chatLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
			chatLayer->setPosition(winSize / 2);
			chatLayer->setName("chatLayer");
			chatLayer->setVisible(false);

			chatLayer->onSpeakeCallBack = [this](const std::string& text)
			{
				if (RoomLogic()->getRoomRule() & GRR_FORBID_GAME_TALK)
				{
					auto prompt = GamePromptLayer::create();
					prompt->showPrompt(GBKToUtf8("��Ǹ,�˷����ֹ����!"));
					return;
				}

				_tableLogic->sendChatMsg(text);
			};

			chatLayer->onCloseCallBack = [this](){
				//��ʼ�´ε���ʱ
				auto seq = Sequence::create(DelayTime::create(60.f), CallFunc::create([this](){
					gameOutTime(true);
				}), nullptr);

				seq->setTag(outTime_tag);
				this->runAction(seq);
			};
			addChild(chatLayer, 1000);
		}
		else
		{
			auto chatLayer = dynamic_cast<GameChatDlgLayer*>(this->getChildByName("chatLayer"));
			if (chatLayer)
			{
				//ֹͣ��ʱ�˳�����ʱ
				gameOutTime(false);
				this->stopAllActions();
				chatLayer->showOrHideChatDlg(true);
			}
		}
	}
	void GameTableUI::showUserChatMsg(BYTE seatNo, CHAR msg[])
	{
		if (!isValidSeat(seatNo)) return;

		char str[501] = { 0 };
		std::string record;
		int _RichTag = 0;
		std::string filename("goldenToad/GameChatUi/Res/");//����·��
		std::string filenameSmallImage("goldenToad/GameChatUi/Res/chatSmallRes/");//Сͼ·��
		auto maxFaceCount = 68;
		std::string word;
		char num[3] = { 0, 0, 0 };
		Size lineSize(270, 58);
		int fontSize = 18;
		int imageSize = 24;
		float msgAllLenght = 0;
		int ImageNum = 0;//����������������������ռ�õ�λ��
		Color3B colorArray[] = { colorMagenta, colorBlue, colorRed, colorBlack };

		auto randNumber = INT(std::rand() / float(RAND_MAX + 1) * (sizeof(colorArray) / sizeof(Color3B)));
		Color3B fontColor = colorArray[randNumber];

		auto node = Node::create();
		auto pos = node->getAnchorPoint();
		pos = node->getPosition();
		_tableUi->addChild(node, 1000);

		sprintf(str, "Image_ChatBg%d", seatNo);
		auto chatPos = dynamic_cast<ImageView*>(_tableUi->getChildByName(str));
		auto ptr = chatPos->getPosition();

		sprintf(str, "node%d", seatNo);
		auto chat_Node = (Node*)_tableUi->getChildByName(str);
		if (chat_Node)
		{
			chat_Node->removeFromParent();
		}

		if (seatNo < 3)
		{
			filename.append("chat_bg_text1.png");
		}
		else
		{
			filename.append("chat_bg_text0.png");
		}
		auto tmp = Sprite::create(filename);
		auto tmpSize = tmp->getContentSize();
		Rect fullRect(0, 0, tmpSize.width, tmpSize.height);
		Rect insetRect = Rect(40, 25, 10, 10);//�Զ����ŵ�����

		auto chatBG = ui::Scale9Sprite::create(filename, fullRect, insetRect);
		sprintf(str, "node%d", seatNo);
		node->setName(str);
		node->addChild(chatBG);
		node->setPosition(ptr);

		auto charMsg = RichText::create();
		charMsg->ignoreContentAdaptWithSize(false);
		charMsg->setContentSize(Size(320, lineSize.height));
		node->addChild(charMsg, 1000);

		// �������¼�͵������зֱ���ʾ˵���������
		UserInfoStruct* info = _tableLogic->getUserBySeatNo(seatNo);
		std::string nameSay = StringUtils::format("[%s˵]:", info->nickName);
		record.append(nameSay);
		record.append(msg);

		RichElementText* text = nullptr;

		text = RichElementText::create(_RichTag++, colorBlue, 255, GBKToUtf8(" "), "Arial", fontSize);

		charMsg->pushBackElement(text);

		for (int i = 0; msg[i] != '\0'; i++)
		{
			if (msg[i] == '/' && msg[i + 1] == ':')
			{
				//���ϱ������������ж��Ƿ��ڱ��鷶Χ��
				num[0] = msg[i + 2];
				num[1] = msg[i + 3];
				auto imageNumber = atoi(num);
				// һ���Ǳ���
				if (imageNumber >= 0 && imageNumber < maxFaceCount)
				{
					if (word.length() > 0)
					{
						auto rtext = RichElementText::create(_RichTag++, fontColor, 255, GBKToUtf8(word.c_str()), "Arial", fontSize);
						charMsg->pushBackElement(rtext);
						word.clear();
					}
					ImageNum++;
					auto img = RichElementImage::create(_RichTag++, Color3B(255, 255, 255), 255,
						filenameSmallImage + StringUtils::format("smiley_%d.png", imageNumber));
					charMsg->pushBackElement(img);
					i += 3;
				}
				//���Ǳ���
				else
				{
					msgAllLenght += 0.5f;
					word.push_back(msg[i]);
				}
			}
			else
			{
				msgAllLenght += 0.5f;
				word.push_back(msg[i]);
			}
		}

		//��ʾ�ı�
		if (word.length() > 0)
		{
			auto text = RichElementText::create(_RichTag++, fontColor, 255, GBKToUtf8(word.c_str()), "Arial", fontSize);
			charMsg->pushBackElement(text);
			word.clear();
		}

		float realAllLenght = msgAllLenght * fontSize + ImageNum * imageSize;//�ı�ͼƬ�ܵ���ʵ��С
		float msgRowNum = realAllLenght / 320.0f;
		float bgWidht, bgHeight;
		bgWidht = bgHeight = 0;
		if (msgRowNum < 1)
		{
			//msgRowNumС��1��˵����Ϣֻ��һ�У����������ݵĿ�
			if (msgRowNum <= 0.12f)
			{
				bgWidht = 100;
				bgHeight = lineSize.height;
			}
			else
			{
				bgWidht = realAllLenght + 60;
				bgHeight = lineSize.height;
			}
		}
		else
		{
			//msgRowNum����1�����������ݵĸ�
			bgWidht = lineSize.width + 60;
			bgHeight = (20.0f * (int)msgRowNum) + lineSize.height;
		}
		//�������ݴ�С
		chatBG->setContentSize(Size(bgWidht, bgHeight));
		charMsg->setContentSize(Size(lineSize.width, bgHeight));

		chatBG->setAnchorPoint(Vec2(0, 0.5f));
		charMsg->setAnchorPoint(Vec2(0, 0.5f));
		charMsg->setPosition(Vec2(chatBG->getPositionX() + 35, chatBG->getPositionY() - 15));

		node->runAction(Sequence::create(DelayTime::create(3.0f), RemoveSelf::create(true), nullptr));

		auto chatLayer = dynamic_cast<GameChatDlgLayer*>(this->getChildByName("chatLayer"));
		if (chatLayer)
		{
			chatLayer->addChatRecord(record);
		}
	}

	////////////////////////////////////////////////////////////////////
	//����
	////////////////////////////////////////////////////////////////////
	void GameTableUI::dealSetAdminConfig(bool bAdmin)
	{
		auto btnAdmin = dynamic_cast<Button*>(_tableUi->getChildByName("Button_Admin"));
		btnAdmin->setVisible(bAdmin);
	}

	void GameTableUI::dealStockOperateResult(CMD_S_StockOperateResult* pStock)
	{
		GameAdmin* admin = dynamic_cast<GameAdmin*>(this->getChildByName("adminLayer"));
		if (admin)
		{
			char str[32];
			if (3 == pStock->operate_code)
			{
				sprintf(str, "��ǰ��ˮ��%lld", pStock->stock_score);
			}
			else
			{
				sprintf(str, "��ǰ���أ�%lld", pStock->stock_score);
			}

			admin->showStockOperateResult(str);
		}
	}

	void GameTableUI::dealUpdateOnlinePlayerList(std::vector<OnLineUserInfo> users)
	{
		GameAdmin* admin = dynamic_cast<GameAdmin*>(this->getChildByName("adminLayer"));
		if (admin) admin->createOnlineList(users);
	}

	void GameTableUI::dealUpdateContorlList(std::vector<ControlInfo> controls)
	{
		GameAdmin* admin = dynamic_cast<GameAdmin*>(this->getChildByName("adminLayer"));
		if (admin) admin->createControlList(controls);
	}

	void GameTableUI::dealUpdateSpecialList(std::vector<SpecialUser> specials)
	{
		GameAdmin* admin = dynamic_cast<GameAdmin*>(this->getChildByName("adminLayer"));
		if (admin) admin->createSprcialList(specials);
	}

	void GameTableUI::dealSetSwitchInfo(S_C_SWITCH_INFO* pSwitch)
	{
		GameAdmin* admin = dynamic_cast<GameAdmin*>(this->getChildByName("adminLayer"));
		if (admin) admin->setCheckBoxState(pSwitch);
	}
		
	void GameTableUI::dealUpdateFishName(S_C_FISH_NAME* pName)
	{
		GameAdmin* admin = dynamic_cast<GameAdmin*>(this->getChildByName("adminLayer"));
		if (admin) admin->setSpecialFishName(pName);
	}
}

