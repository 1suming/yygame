#include "DZPokerGameTableUI.h"
#include "DZPokerGameTableLogic.h"
#include "DZPokerBuyDialog.h"
#include "DZPokerGameUserMessageBox.h"
#include "DZPokerGameNotice.h"

#include "HNUIExport.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "HNLobbyExport.h"
//#include "GameSetLayer.h"
//#include "GameDesk.h"
//#include "PlatformConfig.h"

namespace DZPoker
{	
	static const int BUTTON_TAG_CHECK	=	100	;			//��1�����ư�ť��tagΪ100ʱ��ʾ����
	static const int BUTTON_TAG_CALL	=	101;			//��1�����ư�ť��tagΪ101ʱ��ʾ��ע
	static const int BUTTON_TAG_ALLIN	=	102	;			//��1�����ư�ť��tagΪ102ʱ��ʾȫ��

	static const int BUTTON_TAG_BET		=	201	;			//��2�����ư�ť��tagΪ201ʱ��ʾ��ע
	static const int BUTTON_TAG_RAISE	=	202	;			//��2�����ư�ť��tagΪ202ʱ��ʾ��ע

	static const int BUTTON_TAG_FLOD	=	301	;			//��3�����ư�ť��tagΪ301ʱ��ʾ����
	//#define MAX_ZORDER              100 //���㼶
	static const int MEMBAN_ZORDER		=	300;
	static const int DIALOG_ZORDER		=	300;
	static const int Dealer_Zorder      =   100;
	static const int Player_Zorder      =   110;

	static const char* GAME_BIPAI_FALSE			=		"dzpoker/gameMusic/bipai_shu.mp3";
	static const char* GAME_BIPAI_WIN			=		"dzpoker/gameMusic/bipai_yin.mp3";
	static const char* GAME_CLICK				=		"dzpoker/gameMusic/click.mp3";
	
	static const char* GAME_ADD					=		"dzpoker/gameMusic/jiazhu.mp3";

	static const char* GAME_TEJIANGZHONG		=		"dzpoker/gameMusic/tejiangzhong.mp3";
	static const char* GAME_XIAZHU				=		"dzpoker/gameMusic/xiazhu.mp3";
	static const char* GAME_START				=		"dzpoker/gameMusic/start.mp3";

	static const char* TABLE_BG					=		"dzpoker/table/table_bg.png";

	static const char* DZPOKER_CSB				=		"dzpoker/DZPoker_1.csb";

	static const char* DASHBOARD_CSB			=		"dzpoker/Dashboard.csb";

	static const char* PULLDOWNMENU_CSB			=		"dzpoker/pulldownmenu.csb";

	static const char* JSON_NODE_POSITION		=		"Point8";	//8����(��ȡjson�����ļ�ʱ�Ͷ�ȡ8�������ýڵ�"Point8")
	static const char* JSON_NODE_DEALER			=		"Dealer";
	static const char* JSON_NODE_PUBLICCARD		=		"PublicCard";


	HNGameUIBase* GameTableUI::create(BYTE bDeskIndex, bool bAutoCreate)
	{
		GameTableUI* tableUI = new GameTableUI();
		if(tableUI->init(bDeskIndex,bAutoCreate))
		{
			tableUI->autorelease();
			return tableUI;
		}
		else
		{
			CC_SAFE_DELETE(tableUI);
			return nullptr;
		}
	}

	bool GameTableUI::init(BYTE bDeskIndex, bool bAutoCreate)
	{
		if(!HNLayer::init())
		{
			return false;
		}
		//Size size = Director::getInstance()->getVisibleSize();
		//this->ignoreAnchorPointForPosition(false);
		//this->setPosition(size.width / 2, size.height / 2);

		Size winSize = Director::getInstance()->getWinSize();
		this->ignoreAnchorPointForPosition(false);
		this->setPosition(winSize.width / 2, winSize.height / 2);

		auto dispatcher = Director::getInstance()->getEventDispatcher();
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = CC_CALLBACK_2(GameTableUI::onTouchBegan,this);
		dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
		_tableView = new TableView();

		inflateLayout();

		initPosition();

		initGameData();

		//�ɹٴ�����
		_dealer = GameDelearUI::create();
		_dealer->setPosition(Vec2::ZERO);
		this->addChild(_dealer, Dealer_Zorder);
	
		std::vector<Vec2> seatPos(PLAY_COUNT);
		std::vector<Vec2> tablePos(PLAY_COUNT);
		std::vector<Vec2> card(5);
		Vec2 offset;
		Size designResolutionSize  = PlatformConfig::getInstance()->getGameDesignSize();
		offset.x = (winSize.width - designResolutionSize.width) / 2;
		offset.y = (winSize.height - designResolutionSize.height) / 2;
		auto posssss = _tableView->table->getPosition();
		auto recccct = _tableView->table->getContentSize();
		auto sizeeee = _tableView->table->getBoundingBox();
		for(int i = 0; i < PLAY_COUNT; i++)
		{
 			seatPos[i]  = _tableView->table->convertToWorldSpace(_positionOfPlayer[i]);
 			tablePos[i] = _tableView->table->convertToWorldSpace(_positionOfJetton[i]);
		}
		_dealer->setTablePoint(tablePos);
		_dealer->setSeatPoint(seatPos);
		for(int i = 0; i < 5; i++)
		{
			//card[i] = _positionOfPublicCard[i] + offset;
			card[i] = _tableView->table->convertToWorldSpace(_positionOfPublicCard[i]);
		}
		_dealer->setCommunityPoint(card);

		//Vec2 dealerPos = _positionOfDealer + offset;
		Vec2 dealerPos = _tableView->table->convertToWorldSpace(_positionOfDealer);
		_dealer->setDealerPoint(dealerPos);
	
		//�����߼���
		_tableLogic = new GameTableLogic(this, bDeskIndex, bAutoCreate);
		_tableLogic->clearDesk();

		_tableLogic->enterGame();

		return true;
	}

	void GameTableUI::onExit()
	{
		SpriteFrameCache* cache = SpriteFrameCache::getInstance();
		cache->removeSpriteFramesFromFile("dzpoker/table/cards.plist");
		cache->removeSpriteFramesFromFile("dzpoker/table/cardtype.plist");

		HNGameUIBase::onExit();
	}

	bool GameTableUI::onTouchBegan(Touch *touch, Event *unused_event)
	{
		Vec2 touchPos = touch->getLocation();
		auto size = Director::getInstance()->getWinSize();
		if (_tableView->pulldownMenu->getPosition().y < size.height)
		{
			auto des_pos = Vec2(_tableView->pulldownMenu->getPositionX() , size.height + 5);
			_tableView->pulldownMenu->stopAllActions();
			auto mveTo = MoveTo::create(0.2f,des_pos);
			_tableView->pulldownMenu->runAction(mveTo);

			return false;
		}

		return true;
	}

	void GameTableUI::showCardTypeCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		if (Widget::TouchEventType::ENDED == touchtype)
		{
			_cardTypeLayer->setVisible(true);
			auto des_pos = Vec2(0.0f, 0.0f);
			_cardTypeLayer->stopAllActions();
			auto mveTo = MoveTo::create(0.2f, des_pos);
			_cardTypeLayer->runAction(mveTo);
		}    
	}

	void GameTableUI::deskClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		if (Widget::TouchEventType::ENDED != touchtype)
		{
			return ;
		}
		BYTE seatNo = INVALID_DESKNO;
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			if(pSender == _tableView->btnPosition[i])
			{
				seatNo = i;
				break;
			}
		}
		if(seatNo != INVALID_DESKNO)
		{
			_tableLogic->sendUserSit(seatNo);
		}
	}

	void GameTableUI::jettonClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		auto btn = (Button*)pSender;
	}

	void GameTableUI::controllBtnClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		if (touchtype != Widget::TouchEventType::ENDED)
		{
			return ;
		}
		auto btn = (Button*)pSender;
		if (btn == _tableView->btnControll[0])					//��1�����ư�ť
		{
			int data = _tableView->btnControllTag[0];
			switch (data)
			{
			case BUTTON_TAG_CHECK:
				{
					_tableLogic->sendUserCheck();
				}
				break;
			case BUTTON_TAG_CALL:
				{
					_tableLogic->sendUserCall();
				}
				break;
			case BUTTON_TAG_ALLIN:
				{
					_tableLogic->sendUserAllIn();
				}
				break;
			default:
				break;
			}
		}
		else if (btn == _tableView->btnControll[1])			//��2�����ư�ť
		{
			int data = _tableView->btnControllTag[1];	
			switch (data)
			{
			case BUTTON_TAG_BET:
				{
					HNAudioEngine::getInstance()->playEffect(GAME_XIAZHU);
					std::string str = _tableView->addSliderMoney->getString();
					int num = atoi(str.c_str());
					_tableLogic->sendUserBet(num);
					_tableView->addSliderbg->setVisible(false);
				}
				break;
			case BUTTON_TAG_RAISE:
				{
					HNAudioEngine::getInstance()->playEffect(GAME_ADD);
					std::string str = _tableView->addSliderMoney->getString();
					int num = atoi(str.c_str());
					_tableLogic->sendUserRaise(num);
					_tableView->addSliderbg->setVisible(false);
				}
				break;
			default:
				break;
			}
		}
		else if (btn == _tableView->btnControll[2])			//��3�����ư�ť
		{
			_tableLogic->sendUserFold();											//����
		}

		//ֻҪ�����κο��ư�ť�����ã����ÿ����������
		auto size = Director::getInstance()->getWinSize();
		auto mveTo = MoveTo::create(0.2f,Vec2(size.width*0.5,-70.0f));
		_tableView->dashboard->runAction(EaseSineIn::create(mveTo));
	}

	void GameTableUI::sliderCallback(cocos2d::Ref* pSender,ui::Slider::EventType type)
	{
		auto slider = (Slider*)pSender;
		float dt = slider->getPercent()/100.0f;
		float money = dt*(_tableView->silderMaxMoney - _tableView->sliderMinMoney) + _tableView->sliderMinMoney;
		char str[20];
		sprintf(str,"%.0f",money);
		_tableView->addSliderMoney->setString(str);
	}

	void GameTableUI::pullDownCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		if (Widget::TouchEventType::ENDED != touchtype)
		{
			return ;
		}

		auto size = Director::getInstance()->getWinSize();
		Size rt = _tableView->pulldownMenu->getContentSize();
		auto des_pos = Vec2( _tableView->pulldownMenu->getPositionX() , size.height - rt.height - 5);
		_tableView->pulldownMenu->stopAllActions();
		auto mveTo = MoveTo::create(0.2f, des_pos);
		_tableView->pulldownMenu->runAction(mveTo);
	}

	void GameTableUI::pullDownItemCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
	{
		if (Widget::TouchEventType::ENDED != touchtype)
		{
			return ;
		}
		Size winSize = Director::getInstance()->getWinSize();
		auto btn = (Button*) pSender;
	// 	if (btn == _tableView->pullDownItemMume[0])					//�˳��Ӱ�ť
	// 	{
	// 		//_tableLogic->sendForceQuit();
	// 		//leaveDesk();
	// 	}
		if (btn == _tableView->pullDownItemMume[0])			//վ���Ӱ�ť      
		{
			_tableLogic->sendUserUp();
		}
		else if (btn == _tableView->pullDownItemMume[1])			//�����Ӱ�ť
		{
			auto setLayer = GameSetLayer::create();
			setLayer->showSet(this, 105);
		}
	// 	else if (btn == _tableView->pullDownItemMume[2])			//�̳��Ӱ�ť
	// 	{
	// 		btn->setEnabled(false);
	// 		auto gameStore = GameStoreLayer::createGameStore(this, 8);
	// 		gameStore->setPosition(Vec2(0, winSize.height));
	// 		addChild(gameStore, 105);
	// 		this->runAction(MoveBy::create(0.2f, Vec2(0, -winSize.height / 2)));
	// 		gameStore->runAction(Sequence::create(MoveBy::create(0.2f, Vec2(0, -winSize.height / 2)),
	// 			CallFunc::create(CC_CALLBACK_0(GameTableUI::menuTouchReset, this, btn)), nullptr));
	// 	}
		else if (btn == _tableView->pullDownItemMume[2])			//�������Ӱ�ť
		{
			_cardTypeLayer->setVisible(true);
			auto des_pos = Vec2(0.0f,0.0f);
			_cardTypeLayer->stopAllActions();
			auto mveTo = MoveTo::create(0.2f,des_pos);
			_cardTypeLayer->runAction(mveTo);
		}

		auto des_pos = Vec2( 2 , winSize.height );
		_tableView->pulldownMenu->stopAllActions();
		auto mveTo = MoveTo::create(0.2f,des_pos);
		_tableView->pulldownMenu->runAction(mveTo);
	}

	//��ť������ܸ�λ
	void GameTableUI::menuTouchReset(Button* btn)
	{
		btn->setEnabled(true);
	}

	void GameTableUI::initGameData()
	{
		int dir4[4][2] = { {0 , -1} , {-1 , 0} , {0 , 1} , {1 , 0}};
		int dir8[8][2] = { {0 , -1} , {-1 , -1} , {-1 , 0} , {-1 , 1} , {0 , 1} , {1 , 1} , {1 , 0} , {1 , -1}};
		for (BYTE i = 0 ; i < 4 ; i++ )
		{
			_directionOf4[i].x = dir4[i][0];
			_directionOf4[i].y = dir4[i][1];
		}
		for (BYTE i = 0 ; i < 8 ; i++)
		{
			_directionOf8[i].x = dir8[i][0];
			_directionOf8[i].y = dir8[i][1];
		}

		_cardTypeLayer = GameCardType::create();
		auto size = _cardTypeLayer->getContentSize();
		//_cardTypeLayer->setScale(1.3);
		_cardTypeLayer->setVisible(true);
		_cardTypeLayer->setPosition(Vec2(-size.width ,0));
		this->addChild(_cardTypeLayer, 101);

		memset(_players, 0 , sizeof(_players));

		for (BYTE i = 0 ; i < NUM_PUBLIC_CARD ; ++i)
		{
			_publicCard[i]=PokerCard::create();
			_publicCard[i]->setVisible(false);
			_publicCard[i]->setScale(0.8f);
			this->addChild(_publicCard[i],2);
		}

		//����ʱ
		//auto timerSP = Sprite::create("table/green.png");  //maxcard_tip.png  verify_selected.png
		//_progressTimer = ProgressTimer::create(timerSP); 
		//_progressTimer->setVisible(false);
		//_progressTimer->setType(ProgressTimer::Type::RADIAL); 
		//_progressTimer->setReverseDirection(true);
		//_progressTimer->setPercentage(100);
		//this->addChild(_progressTimer,20);

		_tableView->btnControllTag[0] = BUTTON_TAG_CALL;
		_tableView->btnControllTag[1] = BUTTON_TAG_RAISE;
		_tableView->btnControllTag[2] = BUTTON_TAG_FLOD;
		_tableView->sitposition = -1;

		Rect rt = _tableView->bg->getBoundingBox();

		//��ʼ�� texas_chip05.png

		money_dichi_jetton[0]= Sprite::create("dzpoker/table/texas_chip05.png");
		money_dichi_jetton[1]= Sprite::create("dzpoker/table/texas_chip05.png");
		money_dichi_jetton[2]= Sprite::create("dzpoker/table/texas_chip05.png");
		money_dichi_jetton[3]= Sprite::create("dzpoker/table/texas_chip05.png");
		money_dichi_jetton[4]= Sprite::create("dzpoker/table/texas_chip05.png");
		for (int i = 0; i < 5; i++)
		{
			money_dichi_jetton[i]->setPosition(Vec2(635,320+i*2));
			money_dichi_jetton[i]->setScale(0.5f);
			money_dichi_jetton[i]->setVisible(false);
			this->addChild(money_dichi_jetton[i],10);
		}
		_iContestNum = -1;
		//_gameanimation = new GameAnimation();
		//this->addChild(_gameanimation,10);
	}

	void GameTableUI::initPosition()
	{
		auto layout_dz_Poker = dynamic_cast<Layout*>(_tableView->table->getChildByName("dz_Poker"));
		for (int i = 0;  i < PLAY_COUNT ; ++i)
		{
			string jetton_name = StringUtils::format("Image_jetton_%d", i);
			auto image_jetton = dynamic_cast<ImageView*>(layout_dz_Poker->getChildByName(jetton_name.c_str()));
			image_jetton->setVisible(false);
			_positionOfJetton[i] = image_jetton->getPosition();
		}

		for (int i = 0;  i < PUBLIC_CARD_COUNT ; ++i)
		{
			string public_card_name = StringUtils::format("Image_card_%d", i);
			auto image_public_card = dynamic_cast<ImageView*>(layout_dz_Poker->getChildByName(public_card_name.c_str()));
			image_public_card->setVisible(false);
			_positionOfPublicCard[i] = image_public_card->getPosition();
		}

		auto image_dealer_all = dynamic_cast<ImageView*>(layout_dz_Poker->getChildByName("Image_dealer_all"));
		image_dealer_all->setVisible(false);
		_positionOfDealer = image_dealer_all->getPosition();
		/*
		Size winSize = Director::getInstance()->getVisibleSize();

		std::string filename("dzpoker/position.json");
		if (!FileUtils::getInstance()->isFileExist(filename))
		{
			return;
		}
		std::string json = FileUtils::getInstance()->getStringFromFile(filename);
		rapidjson::Document doc;
		doc.Parse<rapidjson::kParseDefaultFlags>(json.c_str());
		if (doc.HasParseError())
		{
			return;
		}
		if (doc.IsObject() && doc.HasMember("Position"))
		{
			const rapidjson::Value &val = doc["Position"];
			if(val.IsArray())
			{
				for (rapidjson::SizeType i=0 ; i < val.Size() ; ++i)
				{
					const rapidjson::Value &node = val[i];

					
					//��ȡ�����ע�����λ��
					if (node.HasMember(JSON_NODE_POSITION))
					{
						const rapidjson::Value &nd = node[JSON_NODE_POSITION];
						if (nd.IsArray())
						{
							for (rapidjson::SizeType j=0 ; j < nd.Size() ; ++j)
							{
								const rapidjson::Value &vitem =nd[j]; 
								const rapidjson::Value & vx = vitem["x"];
								const rapidjson::Value & vy = vitem["y"];
								_positionOfJetton[j].x = vx.GetDouble();// * (winSize.width / designResolutionSize.width);
								_positionOfJetton[j].y = vy.GetDouble();// * (winSize.height / designResolutionSize.height);
							}
						}
						continue;
					}
					
					
					if (node.HasMember(JSON_NODE_PUBLICCARD))
					{
						const rapidjson::Value &nd = node[JSON_NODE_PUBLICCARD];
						if (nd.IsArray())
						{
							for (rapidjson::SizeType j =0 ; j < nd.Size() ; ++j)
							{
								const rapidjson::Value &_item = nd[j];
								const rapidjson::Value &valx = _item["x"];
								const rapidjson::Value &valy = _item["y"];
								_positionOfPublicCard[j].x = valx.GetDouble();// * (winSize.width / designResolutionSize.width);
								_positionOfPublicCard[j].y = valy.GetDouble();// * (winSize.height / designResolutionSize.height);
							}
						}
						continue;
					}
					
					//��ȡ�ɹ�λ��
					if ( node.HasMember(JSON_NODE_DEALER))
					{
						const rapidjson::Value &nd = node[JSON_NODE_DEALER];
						const rapidjson::Value &ndx = nd["x"];
						const rapidjson::Value &ndy = nd["y"];
						_positionOfDealer.x = ndx.GetDouble();// * (winSize.width / designResolutionSize.width);
						_positionOfDealer.y = ndy.GetDouble();// * (winSize.height / designResolutionSize.height);
					}
				}
			}
		}
		*/
	}


	void GameTableUI::initNewturn()
	{
		for (BYTE i = 0 ; i < PLAY_COUNT; ++i )
		{
			if (nullptr != _players[i])
			{
				_players[i]->hideHandCard();
				_players[i]->stopWait();
				_players[i]->stopPlayWin();
			}
		}
		for (BYTE i = 0 ; i < NUM_PUBLIC_CARD ; ++i)
		{
			_publicCard[i]->setVisible(false);
		}
		
		_dealer->clear();
		_tableView->btnControllTag[0] = BUTTON_TAG_CALL;
		_tableView->btnControllTag[1] = BUTTON_TAG_RAISE;
		_tableView->btnControllTag[2] = BUTTON_TAG_FLOD;
	}

	int GameTableUI::getSizeOfPlayerList()
	{
		int count = 0;
		for (BYTE i = 0 ; i < PLAY_COUNT; ++i )
		{
			if (nullptr == _players[i])
			{
				continue;
			}
			count++;
		}
		return count;
	}


	GameTableUI::GameTableUI():_firstOnEnter(true)
		, _lPlayerMoney(nullptr)
		, _lTableName(nullptr)
		, _lConstJuShu(nullptr)
		, _lConstRank(nullptr)
		, _waitTime(30)
	{
		memset(_players, 0 , sizeof(_players));
	}

	GameTableUI::~GameTableUI()
	{
		HN_SAFE_DELETE(_tableLogic);
	}

	PlayerUI* GameTableUI::getPlayer(int seatNo)
	{
		return (seatNoIsOk(seatNo)? _players[seatNo]: nullptr) ;
	}

	/************************�ָ���*********************************/
	void GameTableUI::showUserBet(BYTE byDeskStation)
	{
		if(seatNoIsOk(byDeskStation) && _players[byDeskStation] != nullptr)
		{
			HNAudioEngine::getInstance()->playEffect(GAME_XIAZHU);
			_players[byDeskStation]->setStatus(GBKToUtf8("��ע"), BUTTON_TAG_BET);
		}
	}

	void GameTableUI::showUserCall(BYTE byDeskStation)
	{
		if(seatNoIsOk(byDeskStation) && _players[byDeskStation] != nullptr)
		{
			HNAudioEngine::getInstance()->playEffect(GAME_XIAZHU);
			_players[byDeskStation]->setStatus(GBKToUtf8("��ע"),BUTTON_TAG_CALL);
		}
	}

	void GameTableUI::showUserAdd(BYTE byDeskStation)
	{
		if(seatNoIsOk(byDeskStation) && _players[byDeskStation] != nullptr)
		{
			HNAudioEngine::getInstance()->playEffect(GAME_XIAZHU);
			_players[byDeskStation]->setStatus(GBKToUtf8("��ע"),BUTTON_TAG_RAISE);
		}
	}

	void GameTableUI::showUserCheck(BYTE byDeskStation)
	{
		if(seatNoIsOk(byDeskStation) && _players[byDeskStation] != nullptr)
		{
			_players[byDeskStation]->setStatus(GBKToUtf8("����"),BUTTON_TAG_CHECK);
		}
	}

	void GameTableUI::showUserFold(BYTE byDeskStation)
	{
		if(seatNoIsOk(byDeskStation) && _players[byDeskStation] != nullptr)
		{
			_players[byDeskStation]->setStatus(GBKToUtf8("����"),BUTTON_TAG_FLOD);
			_players[byDeskStation]->hideHandCard();
			_dealer->dealFold(byDeskStation);
		}
	}

	void GameTableUI::showUserAllIn(BYTE byDeskStation)
	{
		if(seatNoIsOk(byDeskStation) && _players[byDeskStation] != nullptr)
		{
			HNAudioEngine::getInstance()->playEffect(GAME_XIAZHU);
			_players[byDeskStation]->setStatus(GBKToUtf8("ȫ��"),BUTTON_TAG_ALLIN);
		}
	}

	//��Ҳ�����ť����
	void GameTableUI::showAllIn(bool bVisible, bool bEnable)
	{
		if (bVisible)
		{
			_tableView->btnControllTag[0] = BUTTON_TAG_ALLIN;
			_tableView->btnControll[0]->setTitleText(GBKToUtf8("ȫ ��"));
		}
		_tableView->btnControll[0]->setVisible(bVisible);
	}

	void GameTableUI::showCall(bool bVisible, bool bEnable)
	{
		if (bVisible)
		{
			_tableView->btnControllTag[0] = BUTTON_TAG_CALL;
			_tableView->btnControll[0]->setTitleText(GBKToUtf8("�� ע"));
		}
		_tableView->btnControll[0]->setVisible(bVisible);
	}

	void GameTableUI::showCheck(bool bVisible, bool bEnable)
	{
		if (bVisible)
		{
			_tableView->btnControllTag[0] = BUTTON_TAG_CHECK;
			_tableView->btnControll[0]->setTitleText(GBKToUtf8("�� ��"));
		}
		_tableView->btnControll[0]->setVisible(bVisible);
	}

	void GameTableUI::showBet(bool bVisible, bool bEnable, LLONG benMoney)
	{
		if(bVisible)
		{
			_tableView->btnControllTag[1] = BUTTON_TAG_BET;
			_tableView->btnControll[1]->setTitleText(GBKToUtf8("�� ע"));
		}
		_tableView->btnControll[1]->setVisible(bVisible);
	}

	void GameTableUI::showRaise(bool bVisible, bool bEnable)
	{
		if (bVisible)
		{
			_tableView->btnControllTag[1] = BUTTON_TAG_RAISE;
			_tableView->btnControll[1]->setTitleText(GBKToUtf8("�� ע"));
		}
		_tableView->btnControll[1]->setVisible(bVisible);
	}

	void GameTableUI::showFold(bool bVisible, bool bEnable)
	{
		_tableView->btnControll[2]->setVisible(bVisible);
	}

	void GameTableUI::showAdd(bool bVisible, bool bEnable)
	{

	}

	void GameTableUI::showSub(bool bVisible, bool bEnable)
	{

	}

	void GameTableUI::showSlider(bool bVisible, bool bEnable, LLONG max, LLONG min, LLONG current)
	{
		_tableView->addSliderbg->setVisible(bVisible);
		if (bVisible)
		{
			char str[20];
			sprintf(str,"%lld",min);
			_tableView->addSliderMoney->setString(str);
			_tableView->addSlider->setPercent(0);
			_tableView->silderMaxMoney = max;
			_tableView->sliderMinMoney = min;
			_tableView->addSlider->addEventListener(CC_CALLBACK_2(GameTableUI::sliderCallback,this));
		}
	}

	void GameTableUI::setSliderValue(LLONG value)
	{

	}

	LLONG GameTableUI::getSliderValue()
	{
		return -1;
	}

	void GameTableUI::showBetMoney(bool bVisible, LLONG money)
	{

	}

	void GameTableUI::showCallMoney(bool bVisible, LLONG money)
	{

	}

	void GameTableUI::showRaiseMoney(bool bVisible, LLONG money)
	{

	}



	void GameTableUI::showPot(BYTE index)
	{

	}

	/**************************************************************/

	void GameTableUI::updatePlayerByDeskStation(BYTE bDeskStation)
	{
		std::vector<BYTE> deskStations;
		deskStations.push_back(bDeskStation);
		updatePlayerByDeskStation(deskStations);
	}

	void GameTableUI::updatePlayerByDeskStation(std::vector<BYTE>& byDeskStations)
	{
	}

	/**************************************************************/

	//������
	void GameTableUI::showHandCard(std::vector<THandCard>& handCards)
	{
		_dealer->dealHandCard(handCards);
		_dealer->setSendCardFinish([=](){
			sendData(MDM_GM_GAME_NOTIFY, ASS_SEND_CARD_FINISH, NULL, NULL);
		});
	// 	HNAudioEngine::getInstance()->playEffect(GAME_START); 
		//this->schedule(schedule_selector(GameTableUI::showHandCardCallback),INTERVAL_FAPAI,2*getSizeOfPlayerList()-1,0);
	}


	void GameTableUI::showFlopCard(const std::vector<BYTE>& byCards)
	{
		_dealer->dealFlopCard(byCards);
	}

	//ת��
	void GameTableUI::showTurnCard(BYTE card)
	{
		_dealer->dealTurnCard(card);
	}

	//����
	void GameTableUI::showRiverCard(BYTE card)
	{
		_dealer->dealRiverCard(card);
	}

	/************************************************************************/
	// deskStation:��λ��
	// nickName:�û���                                                      
	// money:������� 
	void GameTableUI::showUser(BYTE byDeskStation, bool bMe, bool sex)
	{
		if(!seatNoIsOk(byDeskStation))
		{
			return;
		}
		
		PlayerUI* player = PlayerUI::create(_tableLogic->getUserId(_tableLogic->viewToLogicSeatNo(byDeskStation)));
		player->setAnchorPoint(Vec2(0.5f,0.5f));
		player->setPosition(_positionOfPlayer[byDeskStation]);
		player->setSex(sex);

		_tableView->table->addChild(player, Player_Zorder);
	
		_players[byDeskStation] = player;
	
		_tableView->btnPosition[byDeskStation]->setVisible(false);
		if (bMe)
		{
			_tableView->sitposition = byDeskStation;
			for (BYTE i = 0; i < PLAY_COUNT ; i++)
			{
				_tableView->btnPosition[i]->setEnabled(false);
			}
		}
	}

	void GameTableUI::showUserUp(BYTE byDeskStation, bool bMe)
	{
		if (seatNoIsOk(byDeskStation) && nullptr != _players[byDeskStation])
		{
			_players[byDeskStation]->removeFromParentAndCleanup(true);
			_players[byDeskStation] = nullptr;
		}
		_tableView->btnPosition[byDeskStation]->setVisible(true);

		// ������  ��ͷ������
		_tableView->btnPosition[byDeskStation]->setEnabled(true);

		if (bMe)
		{
			_tableView->sitposition = -1;
			for (BYTE i = 0; i < PLAY_COUNT ; i++)
			{
				// ������  ��ͷ������
				if (nullptr == _players[i])
				{
					_tableView->btnPosition[i]->setEnabled(true);
				}

			}
		}
	}

	void GameTableUI::showUserNickName(BYTE byDeskStation, const std::string& nickName)
	{
		if(seatNoIsOk(byDeskStation) && _players[byDeskStation] != nullptr)
		{
			_players[byDeskStation]->setName(nickName);
		}
	}

	void GameTableUI::showUserMoney(BYTE byDeskStation, LLONG MONEY)
	{
		if(seatNoIsOk(byDeskStation) && _players[byDeskStation] != nullptr)
		{
			_players[byDeskStation]->setChip(MONEY);
		}
	}

	void GameTableUI::showUserCardType(BYTE seatNo, INT type)
	{
		if(seatNoIsOk(seatNo) && _players[seatNo] != nullptr)
		{
			_players[seatNo]->playWin(type);
		}
	
		//_playerlist[deskNo]->initCardTypeSprite(type);
		//_gameanimation->spriteMoveByPoint(_playerlist[deskNo]->cardType,Vec2(0,100));
	}

	void GameTableUI::showDealer(BYTE byDeskStation)
	{
		_dealer->dealButton(byDeskStation);
	}

	//��ʾСäע
	void GameTableUI::showSmallBlind(BYTE byDeskStation)
	{
		//showUserBetMoney�Ѿ�����
	// 	if (getSizeOfPlayerList() > 0 && nullptr != _playerlist[byDeskStation])
	// 	{
	// 		auto _currentPT = _playerlist[byDeskStation];
	// 		_currentPT->_money_label->setString("50");
	// 	}
	}

	//��ʾ��äע
	void GameTableUI::showBigBlind(BYTE byDeskStation)
	{
		//showUserBetMoney�Ѿ�����
	// 	if (getSizeOfPlayerList() > 0 && nullptr != _playerlist[byDeskStation])
	// 	{
	// 		auto _currentPT = _playerlist[byDeskStation];
	// 		_currentPT->_money_label->setString("100");
	// 	}
	}

	//��ʾ��ע���루�����ϵģ� 
	void GameTableUI::showUserBetMoney(BYTE byDeskStation,LLONG money)
	{
		//if (getSizeOfPlayerList() > 0 && nullptr != _playerlist[byDeskStation])
		//{
		//	auto _currentPT = _playerlist[byDeskStation];
		//	_gameanimation->betAnimation(_currentPT);	//������ע����
		//	char str[20];
		//	sprintf(str,"%lld",money);
		//	_currentPT->_money_label->setString(str);
		//}

		_dealer->betMoney(byDeskStation, money);
	}

	//�ɷ�
	void GameTableUI::showWinPool(const std::vector<std::vector<LLONG>> &winPool)
	{
		_dealer->dealWinPool(winPool);
	}

	//��ʾ�������
	void GameTableUI::showUserHandCard(BYTE byDeskStaion, const std::vector<BYTE>& byCards)
	{
		if(seatNoIsOk(byDeskStaion) && _players[byDeskStaion] != nullptr)
		{
			_players[byDeskStaion]->setHandCard(0, byCards[0]);
			_players[byDeskStaion]->setHandCard(1, byCards[1]);
		}
	}

	//��߹�����
	void GameTableUI::upCommunityCard(BYTE index)
	{
		Vec2 pos = _publicCard[index]->getPosition();
		_publicCard[index]->setPosition(Vec2(pos.x , pos.y + 20));
	}

	//����������
	void GameTableUI::upUserHandCard(BYTE byDeskStation, BYTE index)
	{
		/*if (getSizeOfPlayerList() > 0 && nullptr != _playerlist[byDeskStation])
		{
			auto _currentPT = _playerlist[byDeskStation];
			Vec2 pos = _currentPT->_card[index]->getPosition();
			Vec2 position = Vec2(pos.x,pos.y+20);
			_currentPT->_card[index]->setPosition(position);
		}*/
	}

	//��ʾ���ʣ�����ʱ��
	void GameTableUI::showUserLeftTime(BYTE byDeskStation,BYTE byTime, BYTE total, bool isMe)
	{
		if(seatNoIsOk(byDeskStation) && _players[byDeskStation] != nullptr)
		{
			if(byTime > 0)
			{
				_players[byDeskStation]->startWait(byTime, total);
			}
			else
			{
				_players[byDeskStation]->stopWait();
			}
		}
	}

	//�Զ�������ť
	void GameTableUI::showAutoCall(bool bVisible, bool bSelected)
	{

	}

	void GameTableUI::showAutoCallAny(bool bVisible, bool bSelected)
	{

	}

	void GameTableUI::showAutoCheck(bool bVisible, bool bSelected)
	{

	}

	void GameTableUI::showAutoCheckFold(bool bVisible, bool bSelected)
	{

	}

	//��ʾ���ֽ�ѧ
	void GameTableUI::showHelp(bool bVisible)
	{

	}

	//�������Ǯ����
	void GameTableUI::showUserLackMoney(BYTE byDeskStation)
	{

	}

	void GameTableUI::clearDesk()
	{
		initNewturn();
	}

	//�����������
	void GameTableUI::clearDeskCard()
	{
		for (BYTE i = 0; i < NUM_PUBLIC_CARD; ++i)
		{
			_publicCard[i]->setVisible(false);
		}
	}


	//�뿪����
	void GameTableUI::leaveDesk()
	{
		//�����׳�||������
		if ((RoomLogic()->getRoomRule() & GRR_QUEUE_GAME) || RoomLogic()->getRoomRule() & GRR_CONTEST || (RoomLogic()->getRoomRule() & GRR_TIMINGCONTEST))
		{
			RoomLogic()->close();
			GamePlatform::returnPlatform(LayerType::ROOMLIST);
		}
		else
		{
			GamePlatform::returnPlatform(LayerType::DESKLIST);
		}
	}

	//��ʾ����׼��
	void GameTableUI::showTableReady(bool bVisible)
	{
		if (bVisible)
		{
			_loadSprite = Sprite::create("dzpoker/table/loading_view.png");
			_loadSprite->setVisible(true);
			Size winSize = Director::getInstance()->getWinSize();
			_loadSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
			_loadSprite->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
			auto spSize = _loadSprite->getContentSize();
			float scX = winSize.width / spSize.width;
			float scY = winSize.height / spSize.height;
			_loadSprite->setScaleX(scX);
			_loadSprite->setScaleY(scY);
			this->addChild(_loadSprite,MEMBAN_ZORDER);
		}
		else
		{
			auto fadeOut = FadeOut::create(2.0f);
			_loadSprite->runAction(fadeOut);
		}
	}

	//ת����λ
	void GameTableUI::rotateStation(int offset)
	{
		//��ת���������Һ�����
		PlayerUI* oldPlayers[PLAY_COUNT] = {0};
		PlayerUI* newPlayers[PLAY_COUNT] = {0};
		ui::Button* oldSeat[PLAY_COUNT] = {0};
		ui::Button* newSeat[PLAY_COUNT] = {0};

		memcpy(oldPlayers, _players, sizeof(_players));

		memcpy(oldSeat, _tableView->btnPosition, sizeof(_tableView->btnPosition));

		//�ȸ���λ�ã���ֹ��Ϣ�����λ�ó���
		for(unsigned int i = 0; i < PLAY_COUNT; i++)
		{
			int index = (i + offset + PLAY_COUNT) % PLAY_COUNT;
			if(_players[i] != nullptr)
			{
				newPlayers[index] = _players[i];
			}
			newSeat[index] = _tableView->btnPosition[i];
		}
		memcpy(_players, newPlayers, sizeof(_players));
		memcpy(_tableView->btnPosition, newSeat, sizeof(_tableView->btnPosition));

		//���ϵ�λ��������
		int rotateTimes = abs(offset);
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			Vector<FiniteTimeAction*> actionArray;
			if(offset > 0)
			{
				//��ʱ��ת
				for(int j = 0; j <= rotateTimes; j++)
				{
					int newPos = (i + j + PLAY_COUNT) % PLAY_COUNT;
					actionArray.pushBack(MoveTo::create(0.3f, _positionOfPlayer[newPos]));
				}
			}
			else
			{
				//˳ʱ��ת
				for(int j = 0; j <= rotateTimes; j++)
				{
					int newPos = (i - j + PLAY_COUNT) % PLAY_COUNT;
					actionArray.pushBack(MoveTo::create(0.2f, _positionOfPlayer[newPos]));
				}
			}
			auto sequence = Sequence::create(actionArray);
			if(oldPlayers[i] != nullptr)
			{
				oldPlayers[i]->runAction(sequence->clone());
			}
			oldSeat[i]->runAction(sequence);
		}
	}

	//��ʾ����Ի���
	void GameTableUI::showBuyMoneyDialog(BYTE deskStation, LLONG min, LLONG max, LLONG plan, LLONG total)
	{
		BuyDialog* buy = BuyDialog::create(this,max,min,plan, total, deskStation);
		this->reorderChild(buy, DIALOG_ZORDER);
		buy->show();
	}

	//֪ͨ��ʾ
	void GameTableUI::noticeMessage(const std::string &message)
	{
		GameNotice* notice = GameNotice::create(this, message);
		this->reorderChild(notice, MAX_ZORDER);
		notice->show();
	}

	//��������
	void GameTableUI::changeToken(bool isMe, BYTE who)
	{
		if (who > 7 || nullptr == _players[who])
		{
			return;
		}
		auto size = Director::getInstance()->getWinSize();
		std::string name = _players[who]->getName();
		_players[who]->setName(name.c_str());
		if (isMe)
		{
			auto mveTo = MoveTo::create(0.2f, Vec2(size.width*0.5, 0.0f));
			_tableView->dashboard->stopAllActions();
			_tableView->dashboard->runAction(mveTo);
		}
		else
		{
			auto mveTo = MoveTo::create(0.2f,Vec2(size.width*0.5, -70.0f));
			_tableView->dashboard->stopAllActions();
			_tableView->dashboard->runAction(mveTo);
		}
	}

	void GameTableUI::showTableInfo(const std::string& tableName)
	{
		char str[100] = {0};
		sprintf(str, "����:%s", tableName.c_str());
		_lTableName->setString(GBKToUtf8(str));	
	}

	void GameTableUI::showMyMoney(LLONG money)
	{
		//���Ǳ���������ʾ
		if (!_tableLogic->isContestGame())
		{
			char str[100] = { 0 };
			sprintf(str, "�ܶ�:%lld", money);
			_lPlayerMoney->setString(GBKToUtf8(str));
		}
	}

	//��������̭
	void GameTableUI::showGameContestKick()
	{
		auto size = Director::getInstance()->getWinSize();
		auto kickImg = Sprite::create("landlord/game/contest/ContestKick.png");
		kickImg->setPosition(Vec2(size.width / 2, size.height / 2));
		kickImg->setName("kickPtr");
		kickImg->setScale(0.0f);
		addChild(kickImg, 3);

		kickImg->runAction(Sequence::create(DelayTime::create(3.7f),
			Spawn::create(ScaleTo::create(1.0f, 1.0f),
			RotateBy::create(1.0f, 360.0f), nullptr), DelayTime::create(3.0f),
			CallFunc::create([=](){leaveDesk(); }), nullptr));
	}

	//�ȴ���������

	void GameTableUI::showGameContestWaitOver()
	{
		auto size = Director::getInstance()->getWinSize();
		auto waitPtr = Sprite::create("landlord/game/contest/ContestWait.png");
		waitPtr->setPosition(Vec2(size.width / 2, size.height / 2));
		waitPtr->setName("waitPtr");
		addChild(waitPtr, 25);
	}


	void GameTableUI::showGameContestOver(MSG_GR_ContestAward* contestAward) //��������
	{
		
		//��ȫ�����һ��
		clearDesk();
		for (BYTE i = 0; i < PLAY_COUNT; i++)
		{
			showUserUp(i, false);
		}
		//�Ƴ��ȴ�����
		auto waitPtr = dynamic_cast<Sprite*>(this->getChildByName("waitPtr"));
		if (waitPtr != nullptr)
		{
			waitPtr->removeFromParent();
		}

		//û�н��� �Ǿ��Ǳ���̭��
		if (!contestAward->iAward)
		{
			showGameContestKick();
			return;
		}
		Sprite* awardDlg = nullptr;
		if (contestAward->iAwardType)
		{
			awardDlg = Sprite::create("landlord/game/contest/ContestAwardsX.png");
		}
		else
		{
			awardDlg = Sprite::create("landlord/game/contest/ContestAwards.png");
		}
		auto size = Director::getInstance()->getWinSize();
		awardDlg->setPosition(Vec2(size.width / 2, size.height / 2));
		addChild(awardDlg, 3);

		awardDlg->setName("awardDlg");
		ParticleSystem *meteor = ParticleSystemQuad::create("landlord/game/contest/huoyan00.plist");
		meteor->setPosition(Vec2(awardDlg->getContentSize().width / 2, 10));
		meteor->setAutoRemoveOnFinish(true);
		awardDlg->addChild(meteor);

		awardDlg->setScale(0.0f);
		awardDlg->runAction(Sequence::create(DelayTime::create(3.7f), ScaleTo::create(0.2f, 1.0f), nullptr));

		char str[64] = { 0 };
		sprintf(str, "%d", _iContestNum);
		auto rankText = TextAtlas::create(str, "landlord/game/contest/js_win_num.png", 23, 28, "0");
		rankText->setPosition(Vec2(awardDlg->getContentSize().width * 0.73, awardDlg->getContentSize().height * 0.555));
		awardDlg->addChild(rankText);

		sprintf(str, "%d", contestAward->iAward);
		auto awardText = TextAtlas::create(str, "landlord/game/contest/js_win_num.png", 23, 28, "0");
		awardText->setPosition(Vec2(awardDlg->getContentSize().width * 0.45, awardDlg->getContentSize().height * 0.345));
		awardText->setAnchorPoint(Vec2(0, 0.5));
		awardDlg->addChild(awardText);

		auto MyListener = EventListenerTouchOneByOne::create();
		MyListener->setSwallowTouches(true);
		MyListener->onTouchBegan = [&](Touch* touch, Event* event)
		{
			auto awardDlg = (Sprite*)getChildByName("awardDlg");
			auto dlgSize = awardDlg->getBoundingBox();
			Size s = this->getContentSize();
			Rect rect = Rect(0, 0, s.width, s.height);
			if (rect.containsPoint(touch->getLocation()))
			{
				if (dlgSize.containsPoint(touch->getLocation())) return true;
				awardDlg->runAction(Sequence::create(ScaleTo::create(0.2f, 0.0f), CallFunc::create([&](){leaveDesk(); }), RemoveSelf::create(true), nullptr));
				return true;
			}
			else
				return false;
		};

		_eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, awardDlg);
	}

	//��������
	void GameTableUI::showConstJuShu(int Index)
	{
		//����������ʾ
		if (_tableLogic->isContestGame())
		{
			char str[100] = { 0 };
			sprintf(str, "��%d��", Index);
			_lConstJuShu->setString(GBKToUtf8(str));
		}
	}

	//��ʾ����
	void GameTableUI::ShowConstRank(int iRankNum, int iRemainPeople)					
	{
		//����������ʾ
		if (_tableLogic->isContestGame())
		{
			char str[100] = { 0 };
			sprintf(str, "������%d/%d", iRankNum, iRemainPeople);
			_lConstRank->setString(GBKToUtf8(str));
		}
	}
	//�����Լ�������	
	void GameTableUI::updateMyRankNum(int iValue) 						
	{
		_iContestNum = iValue;
	}

	/************************�ָ���*********************************/
	void GameTableUI::inflateLayout()
	{
		setBackGroundImage(TABLE_BG);
		Size winSize = Director::getInstance()->getWinSize();
		Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
		//auto layer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(DZPOKER_JSON);
		//layer->setScale(1.2f);

		auto root = CSLoader::createNode(DZPOKER_CSB);
		root->setAnchorPoint(Vec2(0.5f, 0.5f));
		root->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
		this->addChild(root, 2);

		_tableView->table = root;
		auto layout_dz_Poker = dynamic_cast<Layout*>(root->getChildByName("dz_Poker"));
		_tableView->bg = dynamic_cast<ImageView*>(layout_dz_Poker->getChildByName("table"));

		for (BYTE i = 0 ; i < PLAY_COUNT ; ++i)
		{
			_tableView->btnPosition[i] = dynamic_cast<Button*>(layout_dz_Poker->getChildByTag(36 + i));
			_tableView->btnPosition[i]->addTouchEventListener(CC_CALLBACK_2(GameTableUI::deskClickCallback, this));
			_positionOfPlayer[i] = _tableView->btnPosition[i]->getPosition();
		}

		for (BYTE i = 0 ; i < 4 ; ++i)
		{
			_tableView->btnJetton[i] = dynamic_cast<Button*>(layout_dz_Poker->getChildByTag(134 + i));
			_tableView->btnJetton[i]->addTouchEventListener(CC_CALLBACK_2(GameTableUI::jettonClickCallback, this));
		}

		// ��������ʾ
		_tableView->btnCardType = dynamic_cast<Button*>(layout_dz_Poker->getChildByName("btn_cardtype"));
		_tableView->btnCardType->setAnchorPoint(Vec2(0, 0));
		_tableView->btnCardType->setPosition(layout_dz_Poker->convertToNodeSpace(Vec2(0, winSize.height*0.2)));
		_tableView->btnCardType->addTouchEventListener(CC_CALLBACK_2(GameTableUI::showCardTypeCallback, this));

		_tableView->btnMenu = dynamic_cast<Button*>(layout_dz_Poker->getChildByName("btn_menu"));
		_tableView->btnMenu->setAnchorPoint(Vec2(0,1));
		_tableView->btnMenu->setPosition(layout_dz_Poker->convertToNodeSpace(Vec2(0, winSize.height)));
		_tableView->btnMenu->addTouchEventListener(CC_CALLBACK_2(GameTableUI::pullDownCallback, this));

		_lTableName = createLabel("", 18);
		_lTableName->setAnchorPoint(Vec2(1, 0.5f));
		_lTableName->setPosition(winSize.width - 10, winSize.height - 10 - 9);
		this->addChild(_lTableName, 3);
		
		_lPlayerMoney = createLabel("", 18);
		_lPlayerMoney->setAnchorPoint(Vec2(1, 0.5f));
		_lPlayerMoney->setPosition(winSize.width - 10, winSize.height - 10 - 9 - 18);
		this->addChild(_lPlayerMoney, 3);

		//��������
		_lConstJuShu = createLabel("", 18);
		_lConstJuShu->setAnchorPoint(Vec2(1, 0.5f));
		_lConstJuShu->setPosition(winSize.width - 20, winSize.height - 10 - 9 - 18);
		this->addChild(_lConstJuShu, 3);

		//��������
		_lConstRank = createLabel("", 18);
		_lConstRank->setAnchorPoint(Vec2(1, 0.5f));
		_lConstRank->setPosition(winSize.width - 10, winSize.height - 10 - 9 - 18-18);
		this->addChild(_lConstRank, 3);
		
		
		
		auto root_dashboard = CSLoader::createNode(DASHBOARD_CSB);
		root_dashboard->setAnchorPoint(Vec2(0.5, 0));
		root_dashboard->setPosition(Vec2(winSize.width*0.5,0));
		this->addChild(root_dashboard, 20);

		_tableView->dashboard = root_dashboard;
		auto layout_dashboard = root_dashboard->getChildByTag(265);
		
		_tableView->idashbroad = dynamic_cast<ImageView*>(layout_dashboard->getChildByTag(269));
		_tableView->idashbroad->setAnchorPoint(Vec2(0.0f, 0.0f));
		//auto size = _tableView->dashbroad->getContentSize();
		_tableView->idashbroad->setPosition(Vec2(0.0f, 0.0f));
		_tableView->btnControll[0] = dynamic_cast<Button*>(layout_dashboard->getChildByTag(276));
		_tableView->btnControll[1] = dynamic_cast<Button*>(layout_dashboard->getChildByTag(278));
		_tableView->btnControll[2] = dynamic_cast<Button*>(layout_dashboard->getChildByTag(277));
	
		for (BYTE i = 0 ; i < 3 ; ++i)
		{
			_tableView->btnControll[i]->addTouchEventListener(CC_CALLBACK_2(GameTableUI::controllBtnClickCallback,this));
		}

		// ������
		_tableView->addSliderbg = dynamic_cast<ImageView*>(layout_dashboard->getChildByName("Slider_bg"));
		_tableView->addSlider = dynamic_cast<Slider*>(_tableView->addSliderbg->getChildByName("slider"));
		_tableView->addSliderMoney = dynamic_cast<TextAtlas*>(_tableView->addSliderbg->getChildByName("much_money"));

		// �����˵�
		_tableView->pulldownMenu = CSLoader::createNode(PULLDOWNMENU_CSB);
		_tableView->pulldownMenu->setAnchorPoint(Vec2(0, 0));
		_tableView->pulldownMenu->setPosition(Vec2(2, winSize.height + 10));
		this->addChild(_tableView->pulldownMenu, 102);

		auto layout_pulldownMenu = _tableView->pulldownMenu->getChildByTag(310);
		for (BYTE i = 0 ; i < 3; i++)
		{
			_tableView->pullDownItemMume[i] = dynamic_cast<Button*>(layout_pulldownMenu->getChildByTag(i + 349));
			_tableView->pullDownItemMume[i]->addTouchEventListener(CC_CALLBACK_2(GameTableUI::pullDownItemCallback,this));
		}

		SpriteFrameCache* cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile("dzpoker/table/cards.plist");
		cache->addSpriteFramesWithFile("dzpoker/table/cardtype.plist");
	}

	bool GameTableUI::seatNoIsOk(BYTE seatNo)
	{
		return (seatNo < PLAY_COUNT && seatNo >= 0);
	}

	void GameTableUI::isWaitTime(bool isWait)
	{
		if (isWait)
		{
			_waitTime = 30;
			schedule(schedule_selector(GameTableUI::updateTime), 1);
		}
		else
		{
			unschedule(schedule_selector(GameTableUI::updateTime));
		}
	}

	void GameTableUI::updateTime(float dt)
	{
		_waitTime--;
		if (_waitTime == 0)
		{
			unschedule(schedule_selector(GameTableUI::updateTime));
			_tableLogic->sendUserUp();
		}
	}
}
