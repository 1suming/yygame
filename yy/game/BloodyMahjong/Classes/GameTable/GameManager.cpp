#include "GameManager.h"
#include "SimpleAudioEngine.h"

namespace XZMJ
{
	GameManager* GameManager::_instance = nullptr;

	GameManager::GameManager(void)
	{
		_instance = this;
	}

	GameManager::~GameManager(void)
	{
		ResourceLoader::clearAll();																						// �����Դ
		HNAudioEngine::getInstance()->stopBackgroundMusic();											// ֹͣ����
		CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();							// ֹͣ��Ч
	}

	bool GameManager::init()
	{
		if (!Node::init())
		{
			return false;
		}
		// ��Դ��ʼ��
		setGlobalScale();
		this->setScale(_globalScale);
		loadResource();
		return true;
	}

	
	GameManager* GameManager::getInstance()
	{
		if (_instance == nullptr)
		{
			_instance = GameManager::create();
		}
		return _instance;
	}

	void GameManager::loadResource()
	{
		initData();
		addLoader();
		setText();
		setButton();
		addUI();
	}

	void GameManager::initData()
	{
		_isPlayGame = false;																		// ��Ϸ״̬�Ƿ�����4�˴���
		_isTrusteeShep = false;																	// ��Ϸ״̬�Ƿ��й�
		_isSouthPlayOper = false;																// �Ƿ��ֵ���Ҳ���
		_queMen = 4;
		_bigShowCard = nullptr;
		_isSouthPlayOper = false;
		_clockID = 0;
	}

	void GameManager::addLoader()
	{
		// ������Դ������
		_cLoader = cocosResourceLoader::create(COCOS_PATH + "MainScene.csb");
		this->addChild(_cLoader);

		_sLoader = spriteResourceLoader::create(SPRITE_PATH+"path.xml");
		this->addChild(_sLoader);

		_pLoader = plistResourceLoader::create(PLIST_PATH + "resource.plist");
		this->addChild(_pLoader);
	}

	void GameManager::setText()
	{
		// ���������Ϣtext �ؼ�����
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			auto textLabel = COCOS_NODE(Text, StringUtils::format("name%d", i+1));
			textLabel->setColor(colorGold);
			textLabel->setString("");
			textLabel->ignoreAnchorPointForPosition(false);
			textLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
			textLabel = COCOS_NODE(Text, StringUtils::format("money%d", i+1));
			textLabel->setColor(colorGold);
			textLabel->ignoreAnchorPointForPosition(false);
			textLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
			textLabel->setString("");
		}
	}

	void GameManager::startButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				GTLogic()->sendAgreeGame();
				break;
			}
		default:
			break;
		}
	}

	void GameManager::trusteeshepButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				if (_isPlayGame)
				{
					Usertouguan msg;
					memset(&msg, 0, sizeof(msg));
					msg.desk = RoomLogic()->loginResult.pUserInfoStruct.bDeskStation;
					msg.touguang = true;
					RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_AUTOCARD, &msg, sizeof(msg));
				}
				break;
			}
		default:
			break;
		}
	}

	void GameManager::quxiaotuoguangButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				if (_isPlayGame)
				{
					Usertouguan msg;
					memset(&msg, 0, sizeof(msg));
					msg.desk = RoomLogic()->loginResult.pUserInfoStruct.bDeskStation;
					msg.touguang = false;
					RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_AUTOCARD, &msg, sizeof(msg));
				}
				break;
			}
		default:
			break;
		}
	}

	void GameManager::returnButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				GTLogic()->safeQuit();
				break;
			}
		default:
			break;
		}
	}

	void GameManager::queWanButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				endQueMen(0);
				break;
			}
		default:
			break;
		}
	}

	void GameManager::queTongButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				endQueMen(2);
				break;
			}
		default:
			break;
		}
	}

	void GameManager::queTiaoButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				endQueMen(1);
				break;
			}
		default:
			break;
		}
	}

	void GameManager::chiButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				COCOS_NODE(Sprite, "dikuang")->setVisible(false);
				COCOS_NODE(Button, "chi")->setEnabled(false);
				COCOS_NODE(Button, "chi")->setBright(false);
				endAllTimer();
				break;
			}
		default:
			break;
		}
	}

	void GameManager::pengButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				COCOS_NODE(Sprite, "dikuang")->setVisible(false);
				COCOS_NODE(Button, "peng")->setEnabled(false);
				COCOS_NODE(Button, "peng")->setBright(false);
				endAllTimer();
				tagPengPaiEx msg;
				auto card = getLastestOutCard();
				msg.byPs = INT(card->getCardColor()) * 10 + card->getCardNumber();
				msg.byUser = RoomLogic()->loginResult.pUserInfoStruct.bDeskStation;
				RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, THING_PENG_PAI, &msg, sizeof(msg));
				break;
			}
		default:
			break;
		}
	}

	void GameManager::gangButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				COCOS_NODE(Sprite, "dikuang")->setVisible(false);
				COCOS_NODE(Button, "gang")->setEnabled(false);
				COCOS_NODE(Button, "gang")->setBright(false);
				endAllTimer();
				tagGangPaiEx msg;


				msg.byUser = RoomLogic()->loginResult.pUserInfoStruct.bDeskStation;
				msg.byType = _kongType;

				if (msg.byType == ACTION_MING_GANG) // ����
				{
					auto card = getLastestOutCard();
					msg.byPs = INT(card->getCardColor()) * 10 + card->getCardNumber();
				}
				else
				{
					msg.byPs = _concealedNumber;  // ���ܻ��߲��ܵ���
				}
				RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, THING_GANG_PAI, &msg, sizeof(msg));
				break;
			}
		default:
			break;
		}
	}

	void GameManager::tingButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				COCOS_NODE(Sprite, "dikuang")->setVisible(false);
				COCOS_NODE(Button, "ting")->setEnabled(false);
				COCOS_NODE(Button, "ting")->setBright(false);
				endAllTimer();
				break;
			}
		default:
			break;
		}
	}

	void GameManager::huButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				COCOS_NODE(Sprite, "dikuang")->setVisible(false);
				COCOS_NODE(Button, "hu")->setEnabled(false);
				COCOS_NODE(Button, "hu")->setBright(false);
				endAllTimer();
				RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, THING_HU_PAI, &_huMsg, sizeof(_huMsg));

				break;
			}
		default:
			break;
		}
	}

	void GameManager::qiButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				COCOS_NODE(Sprite, "dikuang")->setVisible(false);
				endAllTimer();
				RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_PASS_CHECK);

				auto pool = _vecUser.at(0)->getOwnPool();
				if (pool->getCanHu())
				{
					pool->setNotCheckHu();
				}
				if (pool->getCanPeng())
				{
					pool->setNotCheckPeng();
				}

				break;
			}
		default:
			break;
		}
	}

	void GameManager::huQueDingButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				COCOS_NODE(Sprite, "hubg")->setVisible(false);
				COCOS_NODE(Sprite, "hu_fangpao1")->setVisible(false);
				COCOS_NODE(Sprite, "hu_hupai1")->setVisible(false);
				COCOS_NODE(Sprite, "hu_zimo1")->setVisible(false);
				COCOS_NODE(Sprite, "hu_fangpao2")->setVisible(false);
				COCOS_NODE(Sprite, "hu_hupai2")->setVisible(false);
				COCOS_NODE(Sprite, "hu_zimo2")->setVisible(false);
				COCOS_NODE(Text, "hu_name1")->setString("");
				COCOS_NODE(Text, "hu_name2")->setString("");
				break;
			}
		default:
			break;
		}
	}

	void GameManager::confirmButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				COCOS_NODE(Sprite, "gfxy")->setVisible(false);
				break;
			}
		default:
			break;
		}
	}

	void GameManager::cancelButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				COCOS_NODE(Sprite, "gfxy")->setVisible(false);
				break;
			}
		default:
			break;
		}
	}

	void GameManager::continueButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				COCOS_NODE(Sprite, "finish")->setVisible(false);
				COCOS_NODE(Sprite, "gfxy")->setVisible(false);
				COCOS_NODE(Sprite, "hubg")->setVisible(false);
				COCOS_NODE(Sprite, "finish")->stopAllActions();
				restartGame();
				GTLogic()->sendAgreeGame();
				break;
			}
		default:
			break;
		}
	}

	void GameManager::leaveButtonClickCallBack(Ref* ref,Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				COCOS_NODE(Sprite, "finish")->setVisible(false);
				COCOS_NODE(Sprite, "gfxy")->setVisible(false);
				COCOS_NODE(Sprite, "hubg")->setVisible(false);
				COCOS_NODE(Sprite, "finish")->stopAllActions();
				restartGame();
				GTLogic()->sendUserUp();
				break;
			}
		default:
			break;
		}
	}

	void GameManager::endQueMen(INT queColor, bool isRand)
	{
		COCOS_NODE(Sprite, "queMen")->setVisible(false);
		// ���Ͷ�ȱ�¼�
		tagDingQueEx que;
		_queMen = queColor;
		// ȡ�����ٵĻ�ɫ������ȡ��ߵĻ�ɫ
		if (isRand)
		{
			int countWan = 0;
			int countTiao = 0;
			int countTong = 0;
			for (auto v : _vecUser.at(0)->getOwnPool()->getListCard())
			{
				if (v->getCardColor() == mahjongColor::WAN)
				{
					++countWan;
				}
				else if  (v->getCardColor() == mahjongColor::TIAO)
				{
					++countTiao;
				}
				else  if  (v->getCardColor() == mahjongColor::TONG)
				{
					++countTong;
				}
			}
			
			auto min = (countWan <= countTiao ? countWan : countTiao);
			min = (min <= countTong ? min : countTong);
			if (countWan == min)
			{
				_queMen = 0;
			}
			else if (countTiao == min)
			{
				_queMen = 1;
			}
			else if (countTong == min)
			{
				_queMen = 2;
			}

		}
		que.Clear();
		que.byQue = _queMen;
		que.byUser = RoomLogic()->loginResult.pUserInfoStruct.bDeskStation;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, THING_DING_QUE, &que, sizeof(tagDingQueEx));
		endAllTimer();
	}

	void GameManager::endAllTimer()
	{
		this->unschedule(schedule_selector(GameManager::waitTimer));
		this->unschedule(schedule_selector(GameManager::queMenTimer));
		this->unschedule(schedule_selector(GameManager::waitOutTimer));
		this->unschedule(schedule_selector(GameManager::waitThinkTimer));
		
		_pAtlasTimerNumber->setVisible(false);
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			COCOS_NODE(Sprite, StringUtils::format("timerPoint%d", i+1))->setVisible(false);
		}
	}

	int GameManager::getRd_A_N(int a, int n)
	{
		srand(time(nullptr));
		auto x = int(rand() % (n+1));
		if (x < a)
		{
			x = a;
		}
		return x;
	}

	void GameManager::startTimer(const sitDir& timerPointDir, timerType timerType)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(_clockID);
		// ��ֹ������ʱ��
		endAllTimer();

		// ����ָ��
		COCOS_NODE(Sprite, StringUtils::format("timerPoint%d", INT(timerPointDir)))->setVisible(true);

		// ����ʣ��ʱ��
		switch (timerType)
		{
		case WAIT_START:
			{
				setRemainTime(GTLogic()->getGameStation().byBeginTime + 0.1f);
				break;
			}
		case DING_QUE:
			{
				setRemainTime(GTLogic()->getGameStation().ucDingQueTime + 0.1f);
				break;
			}
		case WAIT_OUT:
			{
				setRemainTime(GTLogic()->getGameStation().byOutTime + 0.1f);
				break;
			}
		case WAIT_BLOCK:
			{
				setRemainTime(GTLogic()->getGameStation().byThinkTime + 0.1f);
				break;
			}
		default:
			break;
		}

		_isSouthPlayOper = (timerPointDir == sitDir::SOUTH_DIR);								// �Ƿ��˲���

		// ��ʾָ��
		_pAtlasTimerNumber->setString(StringUtils::format("%d", INT(_remainTime)));
		_pAtlasTimerNumber->setVisible(true);

		// log
		log("type = %d", INT(timerType));

		// ������ʱ��
		switch (timerType)
		{
		case WAIT_START:
			{
				HNAudioEngine::getInstance()->playBackgroundMusic((MUSIC_PATH + "waiting.mp3").c_str());
				this->schedule(schedule_selector(GameManager::waitTimer), 1.0f);
				break;
			}
		case DING_QUE:
			{
				COCOS_NODE(Sprite, "queMen")->setVisible(true);  // ��ȱ
				this->schedule(schedule_selector(GameManager::queMenTimer), 1.0f);
				break;
			}
		case WAIT_OUT:
			{
				_clockID = HNAudioEngine::getInstance()->playEffect((MUSIC_PATH + "Clock.mp3").c_str());
				if (_isSouthPlayOper)
				{
					_vecUser.at(0)->getOwnPool()->setCanOper();
				}
				this->schedule(schedule_selector(GameManager::waitOutTimer), 1.0f);
				break;
			}
		case WAIT_BLOCK:
			{
				this->schedule(schedule_selector(GameManager::waitThinkTimer), 1.0f);
				break;
			}
		default:
			break;
		}
	}

	void GameManager::waitTimer(float dt)
	{
		if (INT(_remainTime <= 1))
		{
			GTLogic()->sendAgreeGame();
			HNAudioEngine::getInstance()->stopBackgroundMusic();
			endAllTimer();
		}
		else
		{
			_remainTime -= dt;
			_pAtlasTimerNumber->setString(StringUtils::format("%d", INT(_remainTime)));
		}
	}

	void GameManager::queMenTimer(float dt)
	{
		if (INT(_remainTime <= 1))
		{
			endQueMen(0, true);
			endAllTimer();
		}
		else
		{
			_remainTime -= dt;
			_pAtlasTimerNumber->setString(StringUtils::format("%d", INT(_remainTime)));
		}
	}

	void GameManager::waitOutTimer(float dt)
	{
		if (INT(_remainTime <= 1))
		{
			// �Զ�����
			if (_isSouthPlayOper)
			{
				autoSendCard(sitDir::SOUTH_DIR);
			}
			endAllTimer();
		}
		else
		{
			
			_pAtlasTimerNumber->setString(StringUtils::format("%d", INT(_remainTime)));
			_remainTime -= dt;
			if (_isTrusteeShep)  // �й�
			{
				// �Զ�����
				if (_isSouthPlayOper)
				{
					autoSendCard(sitDir::SOUTH_DIR);
					endAllTimer();
				}
			}
		}
	}

	void GameManager::waitThinkTimer(float dt)
	{
		if (INT(_remainTime <= 1))
		{
			COCOS_NODE(Sprite, "dikuang")->setVisible(false);
			RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_PASS_CHECK);
			endAllTimer();
		}
		else
		{
			_remainTime -= dt;
			_pAtlasTimerNumber->setString(StringUtils::format("%d", INT(_remainTime)));
		}
	}

	void GameManager::playBgTimer(float dt)
	{
		if (_isPlayGame && !CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		{
			auto i = getRd_A_N(1, 4);
			HNAudioEngine::getInstance()->playBackgroundMusic((MUSIC_PATH + StringUtils::format("background/bg%d.mp3", i)).c_str());
		}
	}

	void GameManager::autoReturnDeskWhenLag()
	{
		_isConnectSuccess = false;
		this->scheduleOnce(schedule_selector(GameManager::checkConnectSuccess), 1.0f);
		this->scheduleOnce(schedule_selector(GameManager::willReturnTimer), 2.0f);
	}

	void GameManager::checkConnectSuccess(float dt)
	{
		if (!_isConnectSuccess)
		{
			auto promt = GamePromptLayer::create();
			promt->showPrompt(GBKToUtf8("��Ϸ������ʧЧ����������..."));
			promt->removeFromParentAndCleanup(false);
			this->addChild(promt);
		}
	}

	void GameManager::willReturnTimer(float dt)
	{
		if (!_isConnectSuccess)
		{
			returnDesk();
		}
	}

	void GameManager::returnDesk(bool safeLeave)
	{
		if (RoomLogic()->isConnect())
		{
			if (safeLeave)
			{
				GTLogic()->sendUserUp();
			}
			else
			{
				trusteeshepSendMsg(true);
				GTLogic()->sendForceQuit();    // ����ǿ���˳���Ϣ	
			}
			GamePlatform::returnPlatform(LayerType::DESKLIST);
		}
		HNAudioEngine::getInstance()->stopBackgroundMusic();
		GamePlatform::returnPlatform(LayerType::DESKLIST);
	}

	void GameManager::trusteeshepSendMsg(bool trusteeshep)
	{
		Usertouguan msg;
		memset(&msg, 0, sizeof(msg));
		msg.desk = RoomLogic()->loginResult.pUserInfoStruct.bDeskStation;
		msg.touguang = trusteeshep;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_AUTOCARD, &msg, sizeof(msg));
	}

	void GameManager::setButton()
	{
		// ���ÿ��ӿ���
		resetDiKuang();

		// ���ûص�����
		COCOS_NODE(Button, "start")->addTouchEventListener(CC_CALLBACK_2(GameManager::startButtonClickCallBack, this));
		COCOS_NODE(Button, "trusteeshep")->addTouchEventListener(CC_CALLBACK_2(GameManager::trusteeshepButtonClickCallBack, this));
		COCOS_NODE(Button, "quxiaotuoguang")->addTouchEventListener(CC_CALLBACK_2(GameManager::quxiaotuoguangButtonClickCallBack, this));
		COCOS_NODE(Button, "return")->addTouchEventListener(CC_CALLBACK_2(GameManager::returnButtonClickCallBack, this));
		COCOS_NODE(Button, "queWan")->addTouchEventListener(CC_CALLBACK_2(GameManager::queWanButtonClickCallBack, this));
		COCOS_NODE(Button, "queTong")->addTouchEventListener(CC_CALLBACK_2(GameManager::queTongButtonClickCallBack, this));
		COCOS_NODE(Button, "queTiao")->addTouchEventListener(CC_CALLBACK_2(GameManager::queTiaoButtonClickCallBack, this));
		COCOS_NODE(Button, "chi")->addTouchEventListener(CC_CALLBACK_2(GameManager::chiButtonClickCallBack, this));
		COCOS_NODE(Button, "peng")->addTouchEventListener(CC_CALLBACK_2(GameManager::pengButtonClickCallBack, this));
		COCOS_NODE(Button, "gang")->addTouchEventListener(CC_CALLBACK_2(GameManager::gangButtonClickCallBack, this));
		COCOS_NODE(Button, "ting")->addTouchEventListener(CC_CALLBACK_2(GameManager::tingButtonClickCallBack, this));
		COCOS_NODE(Button, "hu")->addTouchEventListener(CC_CALLBACK_2(GameManager::huButtonClickCallBack, this));
		COCOS_NODE(Button, "qi")->addTouchEventListener(CC_CALLBACK_2(GameManager::qiButtonClickCallBack, this));
		COCOS_NODE(Button, "hu_queding")->addTouchEventListener(CC_CALLBACK_2(GameManager::huQueDingButtonClickCallBack, this));
		COCOS_NODE(Button, "confirm")->addTouchEventListener(CC_CALLBACK_2(GameManager::confirmButtonClickCallBack, this));
		COCOS_NODE(Button, "cancel")->addTouchEventListener(CC_CALLBACK_2(GameManager::cancelButtonClickCallBack, this));
		COCOS_NODE(Button, "continue")->addTouchEventListener(CC_CALLBACK_2(GameManager::continueButtonClickCallBack, this));
		COCOS_NODE(Button, "continue_0")->addTouchEventListener(CC_CALLBACK_2(GameManager::leaveButtonClickCallBack, this));
	}

	void GameManager::addUI()
	{
		// �齫
		this->setScale(_globalScale);
		auto origin = _newOrigin;
		_vecHasOutCardCount.assign(4, 0);
		_lineCount = 10;
		COCOS_NODE(Layout, "RootPanel")->setPosition(_newOrigin);
		// ���ӽ����Ƶ�λ��
		auto pos = COCOS_NODE(Layout, "handPanel1")->getPosition() + origin;
		auto size = COCOS_NODE(Layout, "handPanel1")->getContentSize();
		Vec2 catchPos1, catchPos2, catchPos3, catchPos4;
		auto spN = Sprite::createWithSpriteFrameName("liNorthxiao.png");
		auto spS = Sprite::createWithSpriteFrameName("ZhengDaTong1.png");
		auto spEW = Sprite::createWithSpriteFrameName("liYou.png");
		catchPos1 = pos + Vec2(size.width - spS->getContentSize().width / 4.0f, size.height/2);
		_vecCatchEndPos.push_back(catchPos1);

		
		pos = COCOS_NODE(Layout, "handPanel2")->getPosition() + origin;
		size = COCOS_NODE(Layout, "handPanel2")->getContentSize();
		catchPos2 = pos + Vec2(size.width/2, spEW->getContentSize().height / 1.5f);
		_vecCatchEndPos.push_back(catchPos2);

		pos = COCOS_NODE(Layout, "handPanel3")->getPosition() + origin;
		size = COCOS_NODE(Layout, "handPanel3")->getContentSize();
		catchPos3 = pos + Vec2(spN->getContentSize().width/3.0, size.height/2);
		_vecCatchEndPos.push_back(catchPos3);

		pos = COCOS_NODE(Layout, "handPanel4")->getPosition() + origin;
		size = COCOS_NODE(Layout, "handPanel4")->getContentSize();
		catchPos4 = pos + Vec2(COCOS_NODE(Layout, "handPanel2")->getContentSize().width, size.height - spEW->getContentSize().height/1.5);
		_vecCatchEndPos.push_back(catchPos4);

		_vecCatchPos.push_back(catchPos1);
		_vecCatchPos.push_back(catchPos2);
		_vecCatchPos.push_back(catchPos3);
		_vecCatchPos.push_back(catchPos4);

	   // �����齫���Ƶİڷŷ�ʽ
		northSouthCount = 28;
		westEastCount = 26;

		// ���ӿ�Ƭ   // ˳ʱ�����  ����ʼ
		Card* card = nullptr;                     //  ����
		Vec2 initPos = Vec2::ZERO;           //  ������ʼ��
		Layout* layout = nullptr;              // ����
		Size panelSZ;                               // �����ߴ�
		Size cardSZ;								  //  ��Ƭ�ߴ�
		Vec2 startAddInPos;					  //  ��ʼ���ӿ���λ��
		Vec2 panelPos;                           // ����λ��
		float height = PLIST_NODE("DaoBeiEW")->getContentSize().height/4.0;                    // �������ƴ�������ܸ߲�
		INT zorder = CARD_TOTAL_NUMBER + 1;
		for (auto i = 0; i < CARD_TOTAL_NUMBER; i++)                  // �ڵ���
		{
			if (i < northSouthCount)                      // �Ϸ����ҿ�ʼ��
			{
				card = MahjongCard::create(mahjongCreateType::DI_BEI_SHU, sitDir::MID_DIR);
				card->setCardZorder(--zorder);
				layout= COCOS_NODE(Layout, "bottomPanel1");
				panelSZ = layout->getContentSize();
				cardSZ = card->getCardSize();
				panelPos = layout->getPosition() + origin;
				startAddInPos = Vec2(panelSZ.width - cardSZ.width/2, cardSZ.height/2) + panelPos;
				if (i % 2 == 1)  // ��
				{
					initPos = startAddInPos;
				}
				else
				{
					initPos = startAddInPos + Vec2(0, panelSZ.height - cardSZ.height);
				}
				initPos.x -= i/2 * cardSZ.width;
			}
			else if (i < northSouthCount + westEastCount)     // �������¿�ʼ��
			{
				card = MahjongCard::create(mahjongCreateType::DI_BEI_HENG, sitDir::MID_DIR);
				card->setCardZorder(--zorder);
				layout= COCOS_NODE(Layout, "bottomPanel2");
				panelPos = layout->getPosition() + origin;
				panelSZ = layout->getContentSize();
				cardSZ = card->getCardSize();
				startAddInPos = Vec2(panelSZ.width/2, cardSZ.height/2) + panelPos;
				if (i % 2 == 1)  // ��
				{
					initPos = startAddInPos;
				}
				else
				{
					initPos = startAddInPos + Vec2(0, height);
				}
				initPos.y += (i- northSouthCount)/2* (cardSZ.height-height);
			}
			else if (i < northSouthCount * 2 + westEastCount)                // ��������ʼ��
			{
				card = MahjongCard::create(mahjongCreateType::DI_BEI_SHU, sitDir::MID_DIR);
				card->setCardZorder(--zorder);
				layout= COCOS_NODE(Layout, "bottomPanel3");
				panelSZ = layout->getContentSize();
				panelPos = layout->getPosition() + origin;
				cardSZ = card->getCardSize();
				startAddInPos = Vec2(cardSZ.width/2, cardSZ.height/2) + panelPos;
				if (i % 2 == 1)  // ��
				{
					initPos = startAddInPos;
				}
				else
				{
					initPos = startAddInPos + Vec2(0, panelSZ.height - cardSZ.height);
				}
				initPos.x += (i -northSouthCount-westEastCount)/2  * cardSZ.width;
			}
			else if (i < northSouthCount * 2 + westEastCount * 2)                      // �������Ͽ�ʼ��
			{
				if (i ==  northSouthCount * 2 + westEastCount)
				{
					zorder -= 26;
				}
				card = MahjongCard::create(mahjongCreateType::DI_BEI_HENG, sitDir::MID_DIR);
				layout= COCOS_NODE(Layout, "bottomPanel4");
				panelPos = layout->getPosition() + origin;
				panelSZ = layout->getContentSize();
				cardSZ = card->getCardSize();
				startAddInPos = Vec2(cardSZ.width/2, panelSZ.height - cardSZ.height/2) + panelPos;
				if (i % 2 == 1)  // ��
				{
					initPos = startAddInPos + Vec2(0, -height);
					card->setCardZorder(zorder-1);
				}
				else
				{
					initPos = startAddInPos;
					zorder += 2;
					card->setCardZorder(zorder);
				}
				initPos.y -= (i -2 *northSouthCount-westEastCount)/2  * (cardSZ.height-height);
			}

			layout->addChild(card);
			card->setCardPos(initPos);
			_vecBottomCard.push_back(card);
		}

		// ��������������
		for (auto i = 0; i < 3; i++)
		{
			std::vector<std::vector<Card *>> vVec;
			for (auto j = 1; j <= 9; j++)
			{
				std::vector<Card *> vec;
				for (auto k = 0; k < 4; k++)
				{
					auto card = MahjongCard::create(mahjongCreateType::ZHENG_LI, sitDir::MID_DIR, i * 10 + j);
					this->addChild(card);
					card->setCardTouchEvent();
					card->setCardVisible(false);
					card->setCardZorder(340);
					vec.push_back(card);
				}
				vVec.push_back(vec);
				vec.clear();
			}
			_vvvZhengLiCard.push_back(vVec);
			vVec.clear();
		}

		// ������������

		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			std::vector<Card *> v;
			for (auto j = 0; j < 14; j++)
			{
				Card* card = nullptr;
				if (i == 0)  // ��
				{
					card = MahjongCard::create(mahjongCreateType::HU_BEI_DAO, sitDir::SOUTH_DIR);
				} 
				else if (i == 1)   // ��
				{
					card = MahjongCard::create(mahjongCreateType::DI_BEI_HENG, sitDir::WEST_DIR);
					card->setCardZorder(j+1);
				}
				else if (i == 2)  // ��
				{
					card = MahjongCard::create(mahjongCreateType::DI_BEI_SHU, sitDir::NORTH_DIR);
				}
				else if (i == 3)  // ��
				{
					card = MahjongCard::create(mahjongCreateType::DI_BEI_HENG, sitDir::EAST_DIR);
					card->setCardZorder(15-j);
				}
				
				this->addChild(card);
				card->setCardVisible(false);

				v.push_back(card);
			}
			_vvecHuCard.push_back(v);
			v.clear();
		}
		

		// ��������������
		for (auto i = 0; i < 3; i++)
		{
			std::vector<std::vector<Card *>> vVec;
			for (auto j = 1; j <= 9; j++)
			{
				std::vector<Card *> vec;
				for (auto k = 0; k < 4; k++)
				{
					auto card = MahjongCard::create(mahjongCreateType::ZHENG_DAO, sitDir::MID_DIR, i * 10 + j);
					this->addChild(card);
					card->setCardVisible(false);
					card->setCardZorder(340);
					vec.push_back(card);
				}
				vVec.push_back(vec);
				vec.clear();
			}
			_vvvZhengDaoCard.push_back(vVec);
			vVec.clear();
		}

		// ��������������
		for (auto i = 0; i < 30; i++)
		{
			auto card = MahjongCard::create(mahjongCreateType::QIAN_LI, sitDir::MID_DIR);
			this->addChild(card);
			card->setCardVisible(false);
			_vecBeiLiCard.push_back(card);
		}

		// ��������������
		for (auto i = 0; i < 30; i++)
		{
			auto card = MahjongCard::create(mahjongCreateType::ZUO_LI, sitDir::MID_DIR);
			this->addChild(card);
			card->setCardVisible(false);
			_vecZuoLiCard.push_back(card);
		}

		// ��������������
		for (auto i = 0; i < 30; i++)
		{
			auto card = MahjongCard::create(mahjongCreateType::YOU_LI, sitDir::MID_DIR);
			this->addChild(card);
			card->setCardVisible(false);
			_vecYouLiCard.push_back(card);
		}

		// ������������
		for (auto m = 0; m < 4; m++)
		{
			mahjongCreateType type;
			if (m == 0)
			{
				type = mahjongCreateType::DI_ZHENG_SOUTH;
			}
			else if (m == 1)
			{
				type = mahjongCreateType::DI_ZHENG_WEST;
			}
			else if (m == 2)
			{
				type = mahjongCreateType::DI_ZHENG_NORTH;
			}
			else if (m == 3)
			{
				type = mahjongCreateType::DI_ZHENG_EAST;
			}
			std::vector<std::vector<std::vector<Card *>>>vvVec;
			for (auto i = 0; i < 3; i++)
			{
				std::vector<std::vector<Card *>> vVec;
				for (auto j = 1; j <= 9; j++)
				{
					std::vector<Card *> vec;
					for (auto k = 0; k < 4; k++)
					{
						auto card = MahjongCard::create(type, sitDir::MID_DIR, i * 10 + j);
						this->addChild(card);
						card->setCardVisible(false);
						card->setCardZorder(350);
						vec.push_back(card);
					}
					vVec.push_back(vec);
					vec.clear();
				}
				vvVec.push_back(vVec);
				vVec.clear();
			}
			_vvvvXiaoDaoCard.push_back(vvVec);
			vvVec.clear();
		}

		// ������������


		this->schedule(schedule_selector(GameManager::playBgTimer), 5.0f);
		
		reParent("dikuang");
		reParent("queMen");
		reParent("gfxy");
		reParent("finish");
		reParent("hubg");

		pos = COCOS_NODE(Button, "start")->getPosition();
		COCOS_NODE(Button, "start")->removeFromParent();
		this->addChild(COCOS_NODE(Button, "start"));
		COCOS_NODE(Button, "start")->setLocalZOrder(99998);
		COCOS_NODE(Button, "start")->setPosition(pos + _newOrigin);

		// ����timer
		auto spSZ = Sprite::create(FNT_PATH + "timeNumber.png")->getContentSize();
		_pAtlasTimerNumber = LabelAtlas::create("0123456789", FNT_PATH + "timeNumber.png", spSZ.width/10.0f, spSZ.height, '0');
		this->addChild(_pAtlasTimerNumber);
		_pAtlasTimerNumber->setScale(_globalScale);
		_pAtlasTimerNumber->setPosition(this->convertToNodeSpace(COCOS_NODE(Text, "timer")->convertToWorldSpace(Vec2(COCOS_NODE(Text, "timer")->getContentSize().width/2, COCOS_NODE(Text, "timer")->getContentSize().height/2))));
		_pAtlasTimerNumber->setString("0");
		_pAtlasTimerNumber->setVisible(false);
		_pAtlasTimerNumber->ignoreAnchorPointForPosition(false);
		_pAtlasTimerNumber->setAnchorPoint(Vec2(0.5f, 0.5f));

		// ����ʹ����
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			auto user = MahjongCardPoolUser::create();
			this->addChild(user);
			_vecUser.push_back(user);
		}


		//  1. ����ʹ����
		//  2. Ϊʹ������ӿ�Ƭ��
		//  3. ��ӿ�Ƭ�أ����ڷ�������
		auto southPool = Factory::createSouthPool(1);
		_vecUser.at(0)->setPool(southPool);

		auto westPool = Factory::createWestPool(1);
		_vecUser.at(1)->setPool(westPool);

		auto northPool = Factory::createNorthPool(1);
		_vecUser.at(2)->setPool(northPool);

		auto eastPool = Factory::createEastPool(1);
		_vecUser.at(3)->setPool(eastPool);

		// ���4��������Ƴظ�����
		addCardPool(southPool);
		addCardPool(westPool);
		addCardPool(northPool);
		addCardPool(eastPool);

		// ��Ӵ��ƵĶ���
		_touchCard = Factory::createTouchCardAction();
		this->addChild(_touchCard);
		_meldedKong = Factory::createMeldedKongAction();
		this->addChild(_meldedKong);
		_concealedKong = Factory::createConcealedKongAction();
		this->addChild(_concealedKong);
		_touchKong = Factory::createTouchKongAction();
		this->addChild(_touchKong);
		_huCard = Factory::createHuCardAction();
		this->addChild(_huCard);

		_touchCard->setPosition(_touchCard->getPosition() + _newOrigin);
		_meldedKong->setPosition(_meldedKong->getPosition() + _newOrigin);
		_concealedKong->setPosition(_concealedKong->getPosition() + _newOrigin);
		_touchKong->setPosition(_touchKong->getPosition() + _newOrigin);
		_huCard->setPosition(_huCard->getPosition() + _newOrigin);
	}

	void GameManager::reParent(std::string name)
	{
		auto pos = COCOS_NODE(Sprite, name)->getPosition();
		COCOS_NODE(Sprite, name)->removeFromParent();
		this->addChild(COCOS_NODE(Sprite, name));
		COCOS_NODE(Sprite, name)->setLocalZOrder(99999);
		COCOS_NODE(Sprite, name)->setPosition(pos + _newOrigin);
	}

	void GameManager::resetDiKuang()
	{
		COCOS_NODE(Sprite, "dikuang")->setVisible(false);
		COCOS_NODE(Button, "chi")->setBright(false);
		COCOS_NODE(Button, "chi")->setEnabled(false);
		COCOS_NODE(Button, "peng")->setBright(false);
		COCOS_NODE(Button, "peng")->setEnabled(false);
		COCOS_NODE(Button, "gang")->setBright(false);
		COCOS_NODE(Button, "gang")->setEnabled(false);
		COCOS_NODE(Button, "ting")->setBright(false);
		COCOS_NODE(Button, "ting")->setEnabled(false);
		COCOS_NODE(Button, "hu")->setBright(false);
		COCOS_NODE(Button, "hu")->setEnabled(false);
	}

	void GameManager::addUser(const sitDir& dir, const UserInfoStruct& user)
	{
		_vecUser.at(INT(dir) - 1)->setUserInfo(user);     // ���������Ϣ

		COCOS_NODE(Text, StringUtils::format("name%d", dir))->setString(GBKToUtf8(user.nickName));
		COCOS_NODE(Text, StringUtils::format("money%d", dir))->setString(StringUtils::format("%lld", user.i64Money) );
		if (user.bUserState == USER_ARGEE)
		{
			COCOS_NODE(Sprite, StringUtils::format("ready%d", dir))->setVisible(true);
		}
	}

	void GameManager::userLeave(const sitDir& dir)
	{
		UserInfoStruct user;
		user.nickName[0] = '\0';
		if (INT(_vecUser.size()) >= INT(dir))
		{
		_vecUser.at(INT(dir) - 1)->setUserInfo(user);     // ���������Ϣ
		}
		COCOS_NODE(Text, StringUtils::format("name%d", dir))->setString("");
		COCOS_NODE(Text, StringUtils::format("money%d", dir))->setString("");
		COCOS_NODE(Sprite, StringUtils::format("ready%d", dir))->setVisible(false);
	}

	void GameManager::userTrusteeshep(const sitDir& dir, bool isTrust)
	{
		if (dir == sitDir::SOUTH_DIR)
		{
			_isTrusteeShep = isTrust;
			COCOS_NODE(Button, "trusteeshep")->setVisible(!isTrust);
			COCOS_NODE(Button, "quxiaotuoguang")->setVisible(isTrust);
		}
		COCOS_NODE(Sprite, StringUtils::format("tuoguang%d", dir))->setVisible(isTrust);
	}

	void GameManager::userAgree(const sitDir& dir)
	{
		COCOS_NODE(Sprite, StringUtils::format("ready%d", dir))->setVisible(true);
		if (dir == sitDir::SOUTH_DIR)
		{
			HNAudioEngine::getInstance()->stopBackgroundMusic();

			COCOS_NODE(Button, "start")->setVisible(false);
			COCOS_NODE(Sprite, "waiting")->setVisible(true);
			_pAtlasTimerNumber->setVisible(false);
			COCOS_NODE(Sprite, "queMen")->setVisible(false);

			endAllTimer();
			HNAudioEngine::getInstance()->playEffect((MUSIC_PATH + "Ready.mp3").c_str());
			
			for (auto i = 1; i <= PLAY_COUNT; i++)
			{
				COCOS_NODE(Sprite, StringUtils::format("timerPoint%d", i))->setVisible(false);
			}
		}
	}

	void GameManager::startGame(const bool& autoBegin)
	{
		HNAudioEngine::getInstance()->stopBackgroundMusic();
		endAllTimer();
		_autoBegin = autoBegin;
		COCOS_NODE(Sprite, "waiting")->setVisible(false);
		COCOS_NODE(Button, "start")->setVisible(false);
		COCOS_NODE(Button, "return")->setVisible(false);
		
		for (auto i = 1; i <= PLAY_COUNT; i++)
		{
			COCOS_NODE(Sprite, StringUtils::format("ready%d", i))->setVisible(false);
		}
	}

	void GameManager::startSendBottomCard(const sitDir& seziDir, INT sezi0, INT sezi1, std::vector<std::vector<INT>> vvSouthCard)
	{
		initAllHandCard(seziDir, sezi0, sezi1, vvSouthCard);
		_startDir = seziDir;
	}

	const vector<CardPoolUser *>& GameManager::getVecUser() 
	{ 
		return _vecUser; 
	}

	void GameManager::catchCard(const sitDir& dir, const INT& number, const bool& head)
	{
		if (!_isPlayGame)
		{
			return;
		}
		log("------------------- catch %d, dir = %d", number, INT(dir));
		resetDiKuang();
		_vecUser.at(INT(dir)-INT(sitDir::SOUTH_DIR))->getOwnPool()->setCanCheckAction(); // ���ÿ�������
		Card * card = nullptr;
		if (head)
		{
			card = commonCatchCard(dir, number);
			if (card == nullptr)
			{
				return;
			}
		}
		else
		{
			card = tailCatchCard(dir, number);
		}

	}

	void GameManager::outCard(const sitDir& dir, const INT& number)
	{
		resetDiKuang();
		log("------------------- out %d, dir = %d", number, INT(dir));
		// ֹͣ��ʱ��
		for (auto i = 1; i <= PLAY_COUNT; i++)
		{
			COCOS_NODE(Sprite, StringUtils::format("timerPoint%d", i))->setVisible(false);
		}
		_pAtlasTimerNumber->setVisible(false);


		// 1. �����ʾ��ͼ��
		addBigShowSp(dir, number); 

		// 2. �����������λ��
		Vec2 outPos = _vecUser.at(INT(dir)-INT(sitDir::SOUTH_DIR))->getOwnPool()->getOutToDeskPos();;

		// 3. ȡ�����ڳ��������е�λ��
		Vec2 setPos = getOutCardDeskPos(dir);

		// 4. ȡ�ý�������
		auto card =  getXiaoDaoCard(dir, number);
		auto zorder = card->getCardZorder();
		card->setCardPos(outPos);
		card->setCardZorder(1000);

		// 5. �ƶ�����
		card->getCardSprite()->runAction(Sequence::createWithTwoActions(EaseSineOut::create(MoveTo::create(0.56f, setPos)), 
			CallFunc::create(CC_CALLBACK_0(Card::setCardZorder, card, zorder))));

		// 6. ��������
		afterOutCard(dir);

		// 7. ��⶯��. �Ƴ�����
		auto pool = dynamic_cast<SouthMahjongCardPool *>(_vecUser.at(0)->getOwnPool());
		assert(pool != nullptr);
		if (dir != sitDir::SOUTH_DIR)
		{
			pool->afterOutCheckSomething(card);
		}
	}

	void GameManager::showMoney()
	{
		this->runAction(
			Sequence::createWithTwoActions(
			DelayTime::create(1.0f),
			CallFunc::create(CC_CALLBACK_0(GameManager::showMoneyCallBack, this))
			));
	}

	void GameManager::showMoneyCallBack()
	{
		auto userInfo = RoomLogic()->loginResult.pUserInfoStruct;
		// ��ʾ���
		vector<UserInfoStruct *> vec;
		UserInfoModule()->findDeskUsers(userInfo.bDeskNO, vec);
		for (auto &v : vec)
		{
			addUser(GTLogic()->getUserDir(v->bDeskStation), *v);
		}
	}

	void GameManager::reconnected(
			std::vector<std::vector<INT>>& vvHandCard, 
			std::vector<std::vector<INT>>& vvOutCard, 
			std::vector<std::vector<std::vector<INT>>>& vvvActionCard,
			std::vector<std::vector<INT>>& vvMenPai,
			std::vector<bool>& vTrusteeshep,
			std::vector<INT>& vQueMen,
			sitDir& ntDir)
	{
		
		// ��������
		auto info = GTLogic()->getGameStation();
		_startDir = GTLogic()->getUserDir(info.byNtStation);
		_autoBegin = (info.bAutoBegin == 1);    // �Զ���ʼ
		auto userInfo = RoomLogic()->loginResult.pUserInfoStruct;
		_queMen = info.m_byDingQue[userInfo.bDeskStation];   // ȱ��
		_isPlayGame = true;  // ������
		_isTrusteeShep = false;   // �Ƿ��й�
		_isSouthPlayOper = (info.m_byNowOutStation == userInfo.bDeskStation);  // ���ϲ���
		auto nowDir = GTLogic()->getUserDir(info.m_byNowOutStation);

		COCOS_NODE(Button, "return")->setVisible(false);	// ���ش�������
		COCOS_NODE(Button, "start")->setVisible(false);	// ��ʼ����

		// ��ʾ������Ϣ
		for (auto i = 1; i <= PLAY_COUNT; i++)
		{
			COCOS_NODE(Sprite, StringUtils::format("timerPoint%d", i))->setVisible(false);
		}
		_pAtlasTimerNumber->setVisible(false);


		// ��ʾ�����Ϣ
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			auto nickName = GTLogic()->getUserBySeatNo(i)->nickName;
			auto money = GTLogic()->getUserBySeatNo(i)->i64Money;
			auto index = GTLogic()->getUserVecIndex(i);
			COCOS_NODE(Text, StringUtils::format("name%d", index + 1))->setString(GBKToUtf8(nickName));
			COCOS_NODE(Text, StringUtils::format("money%d", index + 1))->setString(StringUtils::format("%lld", money));
		}

		// ��ʾׯ��/�й�
		for (auto i = 1; i <= PLAY_COUNT; i++)
		{
			COCOS_NODE(Sprite, StringUtils::format("zhuang%d", i))->setVisible(i == INT(ntDir));          // ׯ��
			COCOS_NODE(Sprite, StringUtils::format("tuoguang%d", i))->setVisible(vTrusteeshep.at(i - 1));   // �й�
			if (vQueMen.at(i-1) == 0)  // ȱ��
			{
				COCOS_NODE(Sprite, StringUtils::format("que_wanSP%d", i))->setVisible(true);
				COCOS_NODE(Sprite, StringUtils::format("que_tiaoSP%d", i))->setVisible(false);
				COCOS_NODE(Sprite, StringUtils::format("que_tongSP%d", i))->setVisible(false);
			}
			else if (vQueMen.at(i-1) == 1) // ȱ��
			{
				COCOS_NODE(Sprite, StringUtils::format("que_wanSP%d", i))->setVisible(false);
				COCOS_NODE(Sprite, StringUtils::format("que_tiaoSP%d", i))->setVisible(true);
				COCOS_NODE(Sprite, StringUtils::format("que_tongSP%d", i))->setVisible(false);
			}
			else if (vQueMen.at(i-1) == 2)  // ȱͲ
			{
				COCOS_NODE(Sprite, StringUtils::format("que_wanSP%d", i))->setVisible(false);
				COCOS_NODE(Sprite, StringUtils::format("que_tiaoSP%d", i))->setVisible(false);
				COCOS_NODE(Sprite, StringUtils::format("que_tongSP%d", i))->setVisible(true);
			}
			
		}

		showPatternBottomCard(vvMenPai);   // ��ʾ����
		showActionCard(vvvActionCard);   // ��ʾ������
		showHandCard(vvHandCard);   // ��ʾ����
		showOutCard(vvOutCard);  // ��ʾ����

		if (_isSouthPlayOper)   // ���ÿɲ���
		{
			_vecUser.at(0)->getOwnPool()->setCanOper(true);
		}
	}

	
	void GameManager::hideAllBottomCard(bool hide)
	{
		for (auto &v : _vecBottomCard)
		{
			v->setCardVisible(!hide);
		}
	}

	void GameManager::showPatternBottomCard(std::vector<std::vector<INT>>& vvec)
	{
		hideAllBottomCard();  // ��������
		int count = 0;
		for (auto &v : vvec)
		{
			for (auto &var : v)
			{
				_vecBottomCard.at(count++)->setCardVisible(var == 1);
			}
		}

		// ���������յ�
		auto bottomSize = _vecBottomCard.size();
		if ((*_vecBottomCard.rbegin())->isCardVisible())
		{
			count = bottomSize;
			for (auto btCardIter = _vecBottomCard.rbegin(); btCardIter != _vecBottomCard.rend(); btCardIter++)
			{
				auto card = *btCardIter;
				if (!card->isCardVisible())
				{
					_currCardIndex = count;
					break;
				}
				--count;
			}
			count = 0;
			for (auto btCardIter = _vecBottomCard.begin(); btCardIter != _vecBottomCard.end(); btCardIter++)
			{
				auto card = *btCardIter;
				if (!card->isCardVisible())
				{
					_lastCardIndex = (bottomSize + count - 1) % bottomSize;
					if (_lastCardIndex % 2 == 1 && _vecBottomCard.at(_lastCardIndex - 1)->isCardVisible())
					{
						--_lastCardIndex;
					}
					break;
				}
				count++;
			}
		}
		else
		{
			count = bottomSize;
			for (auto btCardIter = _vecBottomCard.rbegin(); btCardIter != _vecBottomCard.rend(); btCardIter++)
			{
				auto card = *btCardIter;
				if (card->isCardVisible())
				{
					_lastCardIndex = count;
					if (_lastCardIndex % 2 == 1 && _vecBottomCard.at(_lastCardIndex - 1)->isCardVisible())
					{
						--_lastCardIndex;
					}
					break;
				}
				--count;
			}
			count = 0;
			for (auto btCardIter = _vecBottomCard.begin(); btCardIter != _vecBottomCard.end(); btCardIter++)
			{
				auto card = *btCardIter;
				if (card->isCardVisible())
				{
					_currCardIndex = count;
					break;
				}
				count++;
			}
		}
	}

	void GameManager::showHandCard(std::vector<std::vector<INT>>& vvec)    // ��ʾ����
	{
		auto msg = GTLogic()->getGameStation();
		vector<bool> vHu(PLAY_COUNT, false);
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			auto index = GTLogic()->getUserVecIndex(i);
			if (msg.m_bIsHu[i])
			{
				vHu.at(index) = true;
			}
		}
		for (size_t i = 0; i < vvec.size(); i++)
		{
			for (auto &v : vvec.at(i))
			{
				auto card = getCard(_vecCardPool.at(i)->getSitDir(), v);
				_vecCardPool.at(i)->addCard(card);
				_vecCardPool.at(i)->sortCard();
				_vecCardPool.at(i)->setHandCardPos();
			}
			if (vHu.at(i))
			{
				_vecCardPool.at(i)->huCard(true, false);
			}
		}
	}

	void GameManager::showActionCard(std::vector<std::vector<std::vector<INT>>>& vvvec)  // ��ʾ������
	{
		for (size_t i = 0; i < vvvec.size(); i++)
		{
			for (size_t j = 0; j < vvvec.at(i).size(); j++)
			{
				// ��
				if ( (vvvec.at(i).at(j).size() == 3) && (vvvec.at(i).at(j).at(0) == vvvec.at(i).at(j).at(1)) && (vvvec.at(i).at(j).at(1) == vvvec.at(i).at(j).at(2)))
				{
					_vecCardPool.at(i)->addSomeOutCards(3, vvvec.at(i).at(j).at(0), false);
				}
				else if (vvvec.at(i).at(j).size() == 4)  // ��
				{
					_vecCardPool.at(i)->addSomeOutCards(4, vvvec.at(i).at(j).at(0), false);
				}
			}
		}
	}

	void GameManager::showOutCard(std::vector<std::vector<INT>>& vvec)
	{
		for (size_t i = 0; i < vvec.size(); i++)
		{
			for (size_t j = 0; j < vvec.at(i).size() ; j++)
			{
				auto pos = getOutCardDeskPos(sitDir(i+1));
				auto card = getXiaoDaoCard(sitDir(i+1), vvec.at(i).at(j));
				card->setCardPos(pos);
			}
		}
	}

	void GameManager::finishGame(const std::vector<std::vector<int>>& vvNum)
	{
		_vecCardPool.at(0)->setCanOper(false);  // ���ɲ���
		endAllTimer();
		for (auto i = 0; i < PLAY_COUNT; i++)
		{
			auto listC = _vecCardPool.at(i)->getListCard();
			auto iter = listC.begin();
			std::vector<Card *> vTmp;
			for (size_t j = 0; j < listC.size(); j++)
			{
				Card* card = nullptr;
				if (i == 0)
				{
					card = getZhengDaoCard(vvNum.at(i).at(j));
				}
				else
				{
					card = getXiaoDaoCard(sitDir(i+1), vvNum.at(i).at(j));
				}
				card->setCardPos((*iter)->getCardPos());
				(*iter)->setCardVisible(false);
				vTmp.push_back(card);
				++iter;
			}
			_vvLastShow.push_back(vTmp);
		}
	}

	void GameManager::restartGame()
	{
		for (auto &v : _vecCardPool)
		{
			v->finishGame();
		}

		// ��պ�������
		for (auto &v : _vecTmpHuCard)
		{
			if (v->getCardSprite() != nullptr)
			{
				_pLoader->removeNode(v->getCardSprite());
			}
			if (v != nullptr)
			{
				v->removeFromParent();
			}
		}
		_vecTmpHuCard.clear();


		// �����ʾ����������
		for (auto &vv : _vvLastShow)
		{
			for (auto &v : vv)
			{
				v->setCardVisible(false);
			}
		}
		_vvLastShow.clear();

		///////////// ��������ֵ
		// ��������
		for (auto &v : _vecBottomCard)
		{
			v->setCardVisible(true);
		}
		for (auto &vvv : _vvvZhengLiCard)
		{
			for (auto &vv : vvv)
			{
				for (auto &v : vv)
				{
					v->setCardVisible(false);
				}
			}
		}
		for (auto &vvv : _vvvZhengDaoCard)
		{
			for (auto &vv : vvv)
			{
				for (auto &v : vv)
				{
					v->setCardVisible(false);
				}
			}
		}
		for (auto &v : _vecBeiLiCard)
		{
			v->setCardVisible(false);
		}
		for (auto &v : _vecZuoLiCard)
		{
			v->setCardVisible(false);
		}
		for (auto &v : _vecYouLiCard)
		{
			v->setCardVisible(false);
		}
		for (auto &vvv : _vvvvXiaoDaoCard)
		{
			for (auto &vv : vvv)
			{
				for (auto &v : vv)
				{
					for (auto &var : v)
					{
						var->setCardVisible(false);
					}
				}
			}
		}
		for (auto &vv : _vvecHuCard)
		{
			for (auto &v : vv)
			{
				v->setCardVisible(false);
			}
		}

		_lastestOutCard = nullptr;
		_sendCardCount = 0;
		_vvStartHandCard.clear();
		_vecHasOutCardCount.assign(PLAY_COUNT, 0);

		for (auto i = 1; i <= PLAY_COUNT; i++)
		{
			COCOS_NODE(Sprite, StringUtils::format("que_tongSP%d", i))->setVisible(false);
			COCOS_NODE(Sprite, StringUtils::format("que_wanSP%d", i))->setVisible(false);
			COCOS_NODE(Sprite, StringUtils::format("que_tiaoSP%d", i))->setVisible(false);
			COCOS_NODE(Sprite, StringUtils::format("tuoguang%d", i))->setVisible(false);
		}
		COCOS_NODE(Sprite, "timerPoint1")->setVisible(true);
		COCOS_NODE(Button, "return")->setVisible(true);
		COCOS_NODE(Button, "start")->setVisible(true);
		COCOS_NODE(Button, "trusteeshep")->setVisible(true);
		COCOS_NODE(Button, "quxiaotuoguang")->setVisible(false);
		auto remainTime = GTLogic()->getGameStation().byBeginTime;
		remainTime = std::min<int> (remainTime, 30);
		setRemainTime(remainTime);

		for (auto i = 1; i <= PLAY_COUNT; i++)
		{
			COCOS_NODE(Sprite, StringUtils::format("timerPoint%d", i))->setVisible(false);
		}
		///COCOS_NODE(Sprite, StringUtils::format("timerPoint%d", sitDir::SOUTH_DIR))->setVisible(true);
		HNAudioEngine::getInstance()->stopBackgroundMusic();
		HNAudioEngine::getInstance()->playBackgroundMusic((MUSIC_PATH + "waiting.mp3").c_str());

		startTimer(sitDir::SOUTH_DIR, timerType::WAIT_START);
		
		if (_bigShowCard != nullptr)
		{
			getPR()->removeNode(_bigShowCard->getCardSprite());
			_bigShowCard->removeFromParent();
			_bigShowCard = nullptr;
		}
		
		_queMen = 4; 
		_isPlayGame = false;
		_isTrusteeShep = false;
		_isSouthPlayOper = false;  // ���ϲ���

		_startDir = sitDir::MID_DIR;    // ׯ��
		_huMsg.Clear();
	}

	void GameManager::initAllHandCard(const sitDir& seziDir, INT sezi0, INT sezi1, std::vector<std::vector<INT>> vvSouthCard)
	{
		// ����ׯ��
		_startSendDir = seziDir;
		COCOS_NODE(Sprite, StringUtils::format("zhuang%d", seziDir))->setVisible(true);
		// vv.at 0 ��ʾ south . 1 ��ʾwest. 2 nortH  3.east
		_vvStartHandCard = vvSouthCard;
		// ���ƶ���
		_vecCurrSendIndex.assign(4, 0);
		// ��ʼ����
		sendCard(_startSendDir, sezi0 + sezi1 - 1);

		_zorderWest = _zorderEast = _zorderSouth = _zorderNorth = 200;
	}

	Card* GameManager::commonCatchCard(const sitDir& dir, const INT& mahjongNumber)
	{
		if (_lastCardIndex % 2 == 0)
		{
			if (_currCardIndex % CARD_TOTAL_NUMBER == (_lastCardIndex+2)%CARD_TOTAL_NUMBER)
			{
				log("No card!!!!!!!!!");
				return nullptr;
			}
		}
		else
		{
			++_currCardIndex;
			if (_currCardIndex % CARD_TOTAL_NUMBER == (_lastCardIndex+1)%CARD_TOTAL_NUMBER)
			{
				log("No card!!!!!!!!!");
				return nullptr;
			}
		}

		// ��������
		Card* giveCard = getCard(dir, mahjongNumber);

		// ȡ����
		if (!_vecBottomCard.at(_currCardIndex)->getCardSprite()->isVisible())  
		{
			_currCardIndex = _lastCardIndex;
		}

		auto card = _vecBottomCard.at(_currCardIndex)->getCardSprite();
		auto posStart = card->getPosition();

		card->runAction(
			Sequence::create(
			EaseSineOut::create(MoveTo::create(0.5f, _vecCatchEndPos.at(INT(dir)-INT(sitDir::SOUTH_DIR))))
			,CallFunc::create(CC_CALLBACK_0(Node::setVisible, card, false))
			,CallFunc::create(CC_CALLBACK_0(Card::setCardPos, _vecBottomCard.at(_currCardIndex), posStart))
			,CallFunc::create(CC_CALLBACK_0(CardPool::addCard, _vecCardPool.at(INT(dir)-INT(sitDir::SOUTH_DIR)), giveCard))
			,CallFunc::create(CC_CALLBACK_0(GameManager::checkHandCardData, this))
			,CallFunc::create(CC_CALLBACK_0(CardPool::afterCatchCheckSomething, _vecCardPool.at(INT(dir)-INT(sitDir::SOUTH_DIR)), giveCard))
			,nullptr
			));
		_currCardIndex = ++_currCardIndex % CARD_TOTAL_NUMBER;
		return giveCard;
	}

	Card* GameManager::tailCatchCard(const sitDir& dir, const INT& mahjongNumber)
	{
		if (_lastCardIndex % 2 == 0)
		{
			if (_currCardIndex % CARD_TOTAL_NUMBER == (_lastCardIndex+2)%CARD_TOTAL_NUMBER)
			{
				log("No card!!!!!!!!!");
				return nullptr;
			}
		}
		else
		{
			if (_currCardIndex % CARD_TOTAL_NUMBER == (_lastCardIndex+1)%CARD_TOTAL_NUMBER)
			{
				log("No card!!!!!!!!!");
				return nullptr;
			}
		}
		Card* giveCard = getCard(dir, mahjongNumber);
		auto card = _vecBottomCard.at(_lastCardIndex)->getCardSprite();
		auto posStart = card->getPosition();
		card->runAction(
			Sequence::create(
			EaseSineOut::create(MoveTo::create(0.5f, _vecCatchEndPos.at(INT(dir)-1)))
			,CallFunc::create(CC_CALLBACK_0(Node::setVisible, card, false))
			,CallFunc::create(CC_CALLBACK_0(Card::setCardPos, _vecBottomCard.at(_lastCardIndex), posStart))
			,CallFunc::create(CC_CALLBACK_0(CardPool::addCard, _vecCardPool.at(INT(dir)-1), giveCard))
			,CallFunc::create(CC_CALLBACK_0(CardPool::afterCatchCheckSomething, _vecCardPool.at(INT(dir)-INT(sitDir::SOUTH_DIR)), giveCard))
			,nullptr
			));
		if (_lastCardIndex % 2 == 0)
		{
			_lastCardIndex = ++_lastCardIndex % CARD_TOTAL_NUMBER;
		}
		else
		{
			_lastCardIndex = (_lastCardIndex - 3 + CARD_TOTAL_NUMBER) % CARD_TOTAL_NUMBER;
		}
		return giveCard;
	}

	void GameManager::addCardPool(CardPool * pool)
	{
		_vecCardPool.push_back(pool);
		pool->setCatchPos(_vecCatchEndPos.at(_vecCardPool.size()-1));
	}

	std::vector<CardPool *>& GameManager::getUserCardPool()
	{ 
		return _vecCardPool; 
	}

	
	void GameManager::hasHu(const sitDir& dir, bool isZiMo, bool isQiangGang)
	{
		int index = INT(dir)-INT(sitDir::SOUTH_DIR);
		if (isZiMo)
		{
			playEffect(0, 4, "ZiMo_", index);
		}
		else if (isQiangGang)
		{
			playEffect(0, 3, "GangPao_", index);
		}
		else
		{
			playEffect(0, 2, "DianPao_", index);
		}
		// ���ú��ƺ������״̬
		auto &listCard = _vecCardPool.at(index)->getListCard();
		auto &huList = _vecCardPool.at(index)->getHuListCard();
		int count = 0;

		for (auto &v: listCard)       // �������Ʋ��ɼ�
		{
			v->setCardVisible(false);
			_vvecHuCard.at(index).at(count)->setCardVisible(true);
			_vvecHuCard.at(index).at(count)->setCardPos(v->getCardPos());
			huList.push_back(_vvecHuCard.at(index).at(count));
			++count;
		}

		{
			auto number = _vecCardPool.at(index)->getLastActionCardNumber();
			Card* card = nullptr;
			if (dir == sitDir::SOUTH_DIR)
			{
				card = getZhengDaoCard(number);
				card->setCardVisible(true);
				card->setCardPos(_vecCatchEndPos.at(index));
				huList.push_back(card);
			}
			else if (dir == sitDir::EAST_DIR)
			{
				card = MahjongCard::create(mahjongCreateType::DI_ZHENG_EAST, sitDir::EAST_DIR, number);
				card->setCardVisible(true);
				card->setCardPos(_vecCatchEndPos.at(index));
				this->addChild(card);
				_vecTmpHuCard.push_back(card);
			}
			else if (dir == sitDir::WEST_DIR)
			{
				card = MahjongCard::create(mahjongCreateType::DI_ZHENG_WEST, sitDir::WEST_DIR, number);
				card->setCardVisible(true);
				card->setCardPos(_vecCatchEndPos.at(index));
				this->addChild(card);
				_vecTmpHuCard.push_back(card);
			}
			else if (dir == sitDir::NORTH_DIR)
			{
				card = MahjongCard::create(mahjongCreateType::DI_ZHENG_NORTH, sitDir::NORTH_DIR, number);
				card->setCardVisible(true);
				card->setCardPos(_vecCatchEndPos.at(index));
				this->addChild(card);
				_vecTmpHuCard.push_back(card);
			}
		}

	}

	void GameManager::setLastActionCard(const sitDir& dir, const INT& number)
	{
		auto index = INT(dir) - INT(sitDir::SOUTH_DIR);
		_vecCardPool.at(index)->setLastActionCardNumber(number);
	}

	void GameManager::setHasHu(const sitDir& dir, const bool& bHasHu)
	{
		auto index = INT(dir) - INT(sitDir::SOUTH_DIR);
		_vecCardPool.at(index)->setHasHu(bHasHu);
	}

	Card* GameManager::getCard(const sitDir& dir, const INT& mahjongNumber)
	{
		Card* giveCard = nullptr;
		auto pool = _vecCardPool.at(INT(dir)-INT(sitDir::SOUTH_DIR));
		switch (dir)
		{
		case sitDir::SOUTH_DIR:
			{
				giveCard = getZhengLiCard(mahjongNumber);
				giveCard->setCardOwner(sitDir::SOUTH_DIR);
				break;
			}
		case sitDir::WEST_DIR:
			{
				giveCard = getZuoLiCard();
				giveCard->setCardOwner(sitDir::WEST_DIR);
				break;
			}
		case sitDir::NORTH_DIR:
			{
				giveCard = getBeiLiCard();
				giveCard->setCardOwner(sitDir::NORTH_DIR);
				break;
			}
		case sitDir::EAST_DIR:
			{
				giveCard = getYouLiCard();
				giveCard->setCardOwner(sitDir::EAST_DIR);
				break;
			}
		default:
			break;
		}
		if (pool->getListCard().size() > 0)
		{
			giveCard->setCardZorder((*(pool->getListCard().rbegin()))->getCardZorder() + 1);
		}
		giveCard->setCardPos(Vec2(-1000, -1000));
		giveCard->setCardVisible(true);
		return giveCard;
	}

	Card* GameManager::getZhengDaoCard(const INT& mahjongNumber)
	{
		//' 1. ȡ��ɫ
		INT color = mahjongNumber / 10;
		INT number = mahjongNumber % 10 - 1;
		for (auto v : _vvvZhengDaoCard.at(color).at(number))
		{
			if (!v->isCardVisible())
			{
				v->setCardVisible(true);
				v->setCardOwner(sitDir::SOUTH_DIR);
				return v;
			}
		}
		assert("Card" == "nullptr");
		return nullptr;
	}

	Card* GameManager::getXiaoDaoCard(const sitDir& dir, const INT& mahjongNumber)
	{
		// 1. ȡ��λ
		INT sitNumber = INT(INT(dir)-1);
		//' 1. ȡ��ɫ
		INT color = mahjongNumber / 10;
		INT number = mahjongNumber % 10 - 1;
		for (auto v : _vvvvXiaoDaoCard.at(sitNumber).at(color).at(number))
		{
			if (!v->isCardVisible())
			{
				v->setCardVisible(true);
				if (dir == sitDir::WEST_DIR)
				{
					v->setCardZorder(++_zorderWest);
				}
				else if (dir == sitDir::SOUTH_DIR)
				{
					v->setCardZorder(--_zorderSouth);
				}
				else if (dir == sitDir::NORTH_DIR)
				{
					v->setCardZorder(++_zorderNorth);
				}
				else if (dir == sitDir::EAST_DIR)
				{
					v->setCardZorder(--_zorderEast);
				}
				_lastestOutCard = v;
				v->setCardOwner(dir);
				return v;
			}
		}
		assert("Card" == "nullptr");
		return nullptr;
	}

	

	
	Card* GameManager::getZhengLiCard(const INT& mahjongNumber)
	{
		//' 1. ȡ��ɫ
		INT color = mahjongNumber / 10;
		INT number = mahjongNumber % 10 - 1;
		for (auto v : _vvvZhengLiCard.at(color).at(number))
		{
			if (!v->isCardVisible())
			{
				v->setCardVisible(true);
				return v;
			}
		}
		assert("Card" == "nullptr");
		return nullptr;
	}

	Card* GameManager::getBeiLiCard()
	{
		for (auto v : _vecBeiLiCard)
		{
			if (!v->isCardVisible())
			{
				v->setCardVisible(true);
				return v;
			}
		}
		assert("Card" == "nullptr");
		return nullptr;
	}
	
	Card* GameManager::getZuoLiCard()
	{
		for (auto v : _vecZuoLiCard)
		{
			if (!v->isCardVisible())
			{
				v->setCardVisible(true);
				return v;
			}
		}
		assert("Card" == "nullptr");
		return nullptr;
	}

	Card* GameManager::getYouLiCard()
	{
		for (auto v : _vecYouLiCard)
		{
			if (!v->isCardVisible())
			{
				v->setCardVisible(true);
				return v;
			}
		}
		assert("Card" == "nullptr");
		return nullptr;
	}

	
	void GameManager::autoSendCard(const sitDir& dir)
	{
		if (dir != sitDir::SOUTH_DIR)
		{
			return;
		}
		auto card = _vecCardPool.at(0)->getAutoSendCard();
		_vecCardPool.at(0)->removeCardMsg(card);
	}

	Vec2 GameManager::getOutCardDeskPos(const sitDir& dir)
	{
		auto startPos = Vec2::ZERO;
		Size outRect;
		auto index = INT(dir) - INT(sitDir::SOUTH_DIR);
		auto count = _vecHasOutCardCount.at(index);
		++_vecHasOutCardCount.at(index);
		Vec2 returnPos = startPos;
		auto origin =  _newOrigin;
		auto panelSz= COCOS_NODE(Layout, "outCardPanel")->getContentSize();
		auto panelOrigin = COCOS_NODE(Layout, "outCardPanel")->getPosition() + origin;
		Size mahjongSzEW = PLIST_NODE("ChuWestWan1")->getContentSize();
		Size mahjongSzNS = PLIST_NODE("ChuSouthWan1")->getContentSize();
		INT lineCount = _lineCount;
		switch (dir)
		{
		case sitDir::SOUTH_DIR:
			{
				outRect = mahjongSzNS;
				startPos = panelOrigin  + Vec2(outRect.width/2 + panelSz.width / 6.0f, outRect.height/2);
				returnPos = startPos + Vec2((count / lineCount)*outRect.width, 0) + Vec2((count % lineCount) * outRect.width, (count / lineCount) * outRect.height*0.76);

				break;
			}
		case sitDir::WEST_DIR:
			{
				outRect = mahjongSzEW;
				startPos = panelOrigin+Vec2(outRect.width/2, panelSz.height * 5.0f / 6.0f - outRect.height/2);
				returnPos = startPos + Vec2(0, -(count / lineCount)*outRect.height*0.7)  + Vec2((count / lineCount) * outRect.width,  -(count % lineCount) * (outRect.height*0.70));

				break;
			}
		case sitDir::NORTH_DIR:
			{
				outRect = mahjongSzNS;
				startPos = panelOrigin + Vec2(panelSz.width * 5.0f/6.0f - outRect.width/2, panelSz.height - outRect.height/2);
				returnPos = startPos + Vec2(-(count / lineCount)*outRect.width, 0)  + Vec2(-(count % lineCount) * outRect.width,  -(count / lineCount) * outRect.height*0.76);

				break;
			}
		case sitDir::EAST_DIR:
			{
				outRect = mahjongSzEW;
				startPos = panelOrigin+Vec2(Vec2(panelSz.width - outRect.width/2, outRect.height/2 + panelSz.height / 6.0f));
				returnPos = startPos + Vec2(0, (count / lineCount)*outRect.height*0.7) + Vec2(-(count / lineCount) * outRect.width,  +(count % lineCount) * outRect.height*0.70);

				break;
			}
		default:
			break;
		}
		return returnPos;
	}

	void GameManager::afterOutCard(const sitDir& dir)
	{
		auto poolIndex = INT(dir)-INT(sitDir::SOUTH_DIR);
		_vecCardPool.at(poolIndex)->removeCard();
		_vecCardPool.at(poolIndex)->setOutCardCount(_vecCardPool.at(poolIndex)->getOutCardCount()+1);
		_vecCardPool.at(poolIndex)->sortCard();
		_vecCardPool.at(poolIndex)->setHandCardPos();
	}

	void GameManager::startOutCard(const sitDir& dir)
	{
		auto index = INT(dir)-INT(sitDir::SOUTH_DIR);
		auto pool = _vecCardPool.at(index);
		auto card = pool->getAutoSendCard();
		pool->sortCard();
		pool->setHandCardPos(card);
		card->setCardPos(_vecCatchEndPos.at(index));
		if (dir == sitDir::SOUTH_DIR)   // ���ܼ��
		{
			pool->setCanOper(true);
			if ((dynamic_cast<SouthMahjongCardPool *> (_vecCardPool.at(0)))->checkConcealedKong())
			{
				startTimer(dir, timerType::WAIT_BLOCK);
			}
		}
	}

	void GameManager::setMinusOutCount(const sitDir& dir)
	{
		INT index = INT(dir) - INT(sitDir::SOUTH_DIR);
		_vecHasOutCardCount.at(index) = _vecHasOutCardCount.at(index) - 1;
	}

	Card* GameManager::getLastestOutCard() 
	{ 
		return _lastestOutCard; 
	}

	bool GameManager::isNoCardCanCatch()
	{
		return (_currCardIndex % CARD_TOTAL_NUMBER == (_lastCardIndex+2)%CARD_TOTAL_NUMBER);
	}

	void GameManager::checkHandCardData()
	{
		auto & listCard = _vecCardPool.at(0)->getListCard();
		auto & numCard = _vvNum.at(0);
		auto hasHu = _vecCardPool.at(0)->getHasHu();
		if (hasHu)
		{
			return;
		}
		else
		{
			auto iter = listCard.begin();
			for (size_t i = 0; i < listCard.size(); i++)
			{
				auto card = *iter;
				if (card->getCardSumNumber() != numCard.at(i))
				{
					for (auto &v : listCard)
					{
						v->setCardVisible(false);
					}
					listCard.clear();
					for (auto &v : numCard)
					{
						auto newCard = getCard(sitDir::SOUTH_DIR, v);
						_vecCardPool.at(0)->addCard(newCard);
					}
					_vecCardPool.at(0)->sortCard();
					_vecCardPool.at(0)->setHandCardPos();
					_vecCardPool.at(0)->setCanOper(true);
					return;
				}
				++iter;
			}
		}
	}

	void GameManager::setHandCardFormServer(const std::vector<std::vector<int>>& vvNum) 
	{ 
		_vvNum = vvNum; 
	}

	
	void GameManager::sendCard(const sitDir& dir, const INT& heapCount)
	{
		switch (dir)
		{
		case sitDir::SOUTH_DIR:
			{
				_currCardIndex = heapCount * 2;
				break;
			}
		case sitDir::WEST_DIR:
			{
				_currCardIndex = northSouthCount  +  heapCount * 2;
				break;
			}
		case sitDir::NORTH_DIR:
			{
				_currCardIndex = (northSouthCount + westEastCount)  +  heapCount * 2;
				break;
			}
		case sitDir::EAST_DIR:
			{
				_currCardIndex = (2 * northSouthCount + westEastCount)  +  heapCount * 2;
				break;
			}
		default:
			assert("non sitDir" == "");
			break;
		}
		_lastCardIndex = (_currCardIndex - 2 + CARD_TOTAL_NUMBER) % CARD_TOTAL_NUMBER;
		_sendCardCount = 0;
	
		this->schedule(schedule_selector(GameManager::sendCardTimer), 0.08f);
	}

	



	void GameManager::sendCardToPool(const INT& cardCount)
	{
		HNAudioEngine::getInstance()->playEffect((MUSIC_PATH + "zhuapai.mp3").c_str());
		std::vector<Card *> vecTmpCard;     // ��Ҫ������
		auto startIndex = _vecCurrSendIndex.at(INT(_startSendDir)-1);     // ��Ҫ��������ʼλ��
		auto endIndex = startIndex + cardCount - 1;                                 // ��Ҫ�����ƽ���λ��
		for (auto i = startIndex; i <= endIndex; i++)                                  // �����Ҫ������
		{
			auto num = _vvStartHandCard.at(INT(_startSendDir)-1).at(i);
			Card * cardSend = getCard(_startSendDir, num);
			vecTmpCard.push_back(cardSend);   // ���Ҫ������
		}
		_vecCurrSendIndex.at(INT(_startSendDir)-1) = endIndex+1;   // �����µ���ʼ��

		// ���÷�����
		switch (_startSendDir)
		{
		case XZMJ::sitDir::SOUTH_DIR:
			_vecCardPool.at(0)->sendSomeCard(vecTmpCard);
			break;
		case XZMJ::sitDir::EAST_DIR:
			_vecCardPool.at(3)->sendSomeCard(vecTmpCard);
			break;
		case XZMJ::sitDir::NORTH_DIR:
			_vecCardPool.at(2)->sendSomeCard(vecTmpCard);
			break;
		case XZMJ::sitDir::WEST_DIR:
			_vecCardPool.at(1)->sendSomeCard(vecTmpCard);
			break;
		case XZMJ::sitDir::MID_DIR:
			break;
		default:
			break;
		}
		_startSendDir = sitDir((INT(_startSendDir))%4+1);    // ����һ������
	}

	void GameManager::sendCardTimer(float dt)
	{
		if (_sendCardCount < 12)  // С��12��, ÿ�η�4��
		{
			for (auto i = 0; i < 4; i++)   // һ��4��
			{
				_vecBottomCard.at(_currCardIndex)->setCardVisible(false);
				_currCardIndex = (++_currCardIndex) % CARD_TOTAL_NUMBER;
			}
			sendCardToPool(4);
		}
		else if (_sendCardCount == 12)  // ��ʮ���Σ���2�Ÿ�ׯ��
		{
			_vecBottomCard.at(_currCardIndex)->setCardVisible(false);   
			_currCardIndex = (_currCardIndex+4) % CARD_TOTAL_NUMBER;
			_vecBottomCard.at(_currCardIndex)->setCardVisible(false);
			_currCardIndex = (_currCardIndex-3+CARD_TOTAL_NUMBER) % CARD_TOTAL_NUMBER;
			sendCardToPool(2);
		}
		else if (_sendCardCount < 16)   // ��һ��
		{
			_vecBottomCard.at(_currCardIndex)->setCardVisible(false);
			_currCardIndex = (++_currCardIndex) % CARD_TOTAL_NUMBER;
			if (_sendCardCount == 15)
			{
				_currCardIndex = (++_currCardIndex) % CARD_TOTAL_NUMBER;    // ��λץ����ʼ��
			}
			sendCardToPool(1);
			
		}
		else
		{
			this->unschedule(schedule_selector(GameManager::sendCardTimer));
			_sendCardCount = 0;
			if (_startSendDir == sitDir::SOUTH_DIR)
			{
				_vecCardPool.at(0)->setCanOper(false);
			}
			return;
		}
		++_sendCardCount;
	}

	void GameManager::setQueMen(INT queMen) 
	{ 
		_queMen = queMen; 
	};

	INT& GameManager::getQueMen()
	{
		return _queMen;
	}

	void GameManager::setPlayGame(bool bs)
	{
		_isPlayGame = bs;
	}

	const bool& GameManager::isGamePlaying()
	{ 
		return _isPlayGame; 
	} 

	void GameManager::setKongType(INT type)
	{
		_kongType = type;
	}

	void GameManager::setConcealedNumber(INT number)
	{
		_concealedNumber = number;
	}

	tagHuPaiEx& GameManager::getHuMsg()
	{
		return _huMsg;
	}

	sitDir& GameManager::getStartDir()
	{
		return _startDir;
	}

	void GameManager::setGlobalScale()
	{
		// ����
		auto visibleSZ = Director::getInstance()->getVisibleSize();
		auto visibleOrigin = Director::getInstance()->getVisibleOrigin();

		// ������
		auto bg = Sprite::create(PIC_PATH + "bg.png");
		this->addChild(bg);
		bg->setAnchorPoint(Vec2::ZERO);
		bg->setPosition(visibleOrigin);
		bg->setLocalZOrder(-111);

		auto newOrigin = visibleOrigin;
		auto uiSZ = Size(1280, 720);
		float scale = 1.0f;

		if (visibleSZ.width >= uiSZ.width && visibleSZ.height >= uiSZ.height)		// 1 . ȫ����
		{
			newOrigin += Vec2((visibleSZ.width - uiSZ.width)/2.0f, (visibleSZ.height - uiSZ.height)/2.0f);
		}
		else if (visibleSZ.width >= uiSZ.width && visibleSZ.height < uiSZ.height)
		{
			scale = (visibleSZ.height / uiSZ.height);
			newOrigin.x += (visibleSZ.width - uiSZ.width * scale)/2.0f;
		}
		else if (visibleSZ.width < uiSZ.width && visibleSZ.height >= uiSZ.height)
		{
			scale = (visibleSZ.width / uiSZ.width);
			newOrigin.y += (visibleSZ.height - uiSZ.height * scale) / 2.0f;
		}
		else if (visibleSZ.width < uiSZ.width && visibleSZ.height < uiSZ.height)
		{
			auto scaleA = visibleSZ.width/uiSZ.width;
			auto scaleB = visibleSZ.height/uiSZ.height;
			scale = scaleA < scaleB ? scaleA : scaleB;
			newOrigin.x += (visibleSZ.width - uiSZ.width * scale) / 2.0f;
			newOrigin.y += (visibleSZ.height - uiSZ.height * scale) / 2.0f;
		}
		_globalScale = scale;
		_newOrigin = newOrigin;
		
	}

	const float& GameManager::getGlobalScale()
	{
		return _globalScale;
	}
	
	const Vec2& GameManager::getNewOrigin()
	{
		return _newOrigin;
	}

	void GameManager::playEffect(int a, int n, std::string head, int index)
	{
		auto i = getRd_A_N(a, n);
		auto man = isMan(index);
		string floder = "";
		if (man)
		{
			floder = "putong/Man/";
		}
		else
		{
			floder = "putong/Wom/";
		}
		auto soundPath = (MUSIC_PATH + floder +head+ StringUtils::format("%d.mp3", i));
		log("%s", soundPath.c_str());
		HNAudioEngine::getInstance()->playEffect(soundPath.c_str());
	}

	bool GameManager::isMan(int index)
	{
		return _vecUser.at(index)->getUserInfo().bBoy;
	}

	Card* & GameManager::getBigShowSp()
	{
		return _bigShowCard;
	}

	void GameManager::addBigShowSp(const sitDir& dir, const INT& number)
	{
		if (_bigShowCard != nullptr)
		{
			getPR()->removeNode(_bigShowCard->getCardSprite());
			_bigShowCard->removeFromParent();
		}
		_bigShowCard = MahjongCard::create(mahjongCreateType::ZHENG_LI, dir, number);
		_bigShowCard->setCardZorder(500);
		this->addChild(_bigShowCard);
		_bigShowCard->setCardPos(COCOS_NODE(Text, StringUtils::format("outBigShow%d", INT(dir)))->getPosition()+_newOrigin);
	}
}