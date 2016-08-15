#include "GoldenFlowerGameTableLogic.h"
#include "GoldenFlowerGameTableUI.h"
#include "GoldenFlowerMessageHead.h"
#include "HNNetExport.h"

using namespace HN;

namespace goldenflower
{
	
	GameTableLogic::GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool bAutoCreate):
		_uiCallback(uiCallback), HNGameLogicBase(deskNo, PLAY_COUNT, bAutoCreate, uiCallback) 
	{
		//_tableInfo.byDeskNo = deskNo;
		//_tableInfo.bAutoCreate = bAutoCreate;

		initParams();
	}

	GameTableLogic::~GameTableLogic()
	{
	
	}

	void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		CCAssert(MDM_GM_GAME_NOTIFY == messageHead->bMainID, "mainId error.");

		switch(messageHead->bAssistantID)
		{
		// ��Ϸ��ʼ--
		case S_C_GAME_BEGIN:
			HNLOG("S_C_GAME_BEGIN");
			dealGameBeginResp(object, objectSize);
			break;

		// ��ʼ����--
		case S_C_SEND_CARD:
			HNLOG("S_C_SEND_CARD");
			dealSendCardResp(object, objectSize);
			break;

		// ��ʼ��Ϸ--
		case S_C_GAME_PLAY:
			HNLOG("S_C_GAME_PLAY");
			dealGamePlayResp(object, objectSize);
			break;

		// ֪ͨ��Ҷ���--
		case S_C_CALL_ACTION:
			HNLOG("S_C_CALL_ACTION");
			dealCallActionResp(object, objectSize);
			break;

		// ��Ϸ����--
		case S_C_GAME_END:
			HNLOG("S_C_GAME_END");
			dealGameEndResp(object, objectSize);
			break;

		// ����У��
		case S_C_SUPER_PROOF:
			HNLOG("S_C_SUPER_PROOF");
			dealSuperProofResp(object, objectSize);
			break;

		// �������ý��
		case S_C_SUPER_SET_RESULT:
			HNLOG("S_C_SUPER_SET_RESULT");
			dealSuperSetResultResp(object, objectSize);
			break;

		// ��ҿ��ƽ��--
		case S_C_LOOK_CARD_RESULT:
			HNLOG("S_C_LOOK_CARD_RESULT");
			dealLookCardResultResp(object, objectSize);
			break;

		// ��Ҹ�ע���--
		case S_C_GEN_ZHU_RESULT:
			HNLOG("S_C_GEN_ZHU_RESULT");
			dealGenZhuResultResp(object, objectSize);
			break;

		// ��ע���--
		case S_C_JIA_ZHU_RESULT:
			HNLOG("S_C_JIA_ZHU_RESULT");
			dealJiaZhuResultResp(object, objectSize);
			break;

		// ��ұ��ƽ��--
		case S_C_BI_PAI_RESULT:
			HNLOG("S_C_BI_PAI_RESULT");
			dealBiPaiResultResp(object, objectSize);
			break;
			
		// ������ƽ��--
		case S_C_GIVE_UP_RESULT:
			HNLOG("S_C_GIVE_UP_RESULT");
			dealGiveUpResultResp(object, objectSize);
			break;		
		
		default:
			break;
		}
	}


	void GameTableLogic::dealGameBeginResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(S_C_GameBegin), "size error.");
		S_C_GameBegin* pData = (S_C_GameBegin*)object;

		// ��Ϸ��������
		refreshParams();

		_NTSeatNo   = pData->byNtStation;
		_GuoDi      = pData->iGuoDi;
		_DiZhu      = pData->iDiZhu;
		//_ShangXian  = pData->i64ShangXian;
		_DingZhu    = pData->i64DingZhu;
		_ZongXiaZhu = pData->i64ZongXiaZhu;

		memcpy(_UserState, pData->iUserState, sizeof(_UserState));
		memcpy(_XiaZhuData, pData->i64XiaZhuData, sizeof(_XiaZhuData));

		// ���������Ϣ
		showUserInfo();

		//����׼����־
		_uiCallback->showReadySign(INVALID_SEAT_NO, false);
		_uiCallback->showGiveUpCard(INVALID_SEAT_NO, false);

		// ������Ϣ
		//_uiCallback->showLimitNote(_ShangXian);
		_uiCallback->showBaseNote(_DiZhu);
		_uiCallback->showLimitPerNote(_DingZhu);
		_uiCallback->showGuoDi(_GuoDi);
		_uiCallback->showTotalNote(_ZongXiaZhu);
		_uiCallback->showMyTotalNote(_DiZhu);

		//��¼��ǰ����������ҵ��ǳ�
		for (int i = 0; i < PLAY_COUNT;i++)
		{
			UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
			if (nullptr != pUser)
			{
				_userName[i] = pUser->nickName;
			}			
		}

	}


	void GameTableLogic::dealSendCardResp(void* object, INT objectSize)
	{
		// ���ݴ�С���
		CCAssert(objectSize == sizeof(S_C_SendCard), "size error.");

		// ���ݱ���
		S_C_SendCard* pData = (S_C_SendCard*)object;
		memcpy(_userCard, pData->byCard, sizeof(_userCard));
		memcpy(_userCardCount, pData->byCardCount, sizeof(_userCardCount));

		// ��ʾ����
		std::vector<THandCard> cards;
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			THandCard card;
			memset(card.byCards, 0x00, sizeof(card.byCards));
			card.bySeatNo = logicToViewSeatNo(i);
			_uiCallback->showUserNoteMoney(logicToViewSeatNo(i), 0, 0);

			for(int j = 0; j < _userCardCount[i]; j++)
			{
				card.byCards[j] = _userCard[i][j];
			}
			if (_userCardCount[i]>0)
			{
				cards.push_back(card);
			}
		}

		// ��ʾׯ��
		_uiCallback->showDealer(logicToViewSeatNo(_NTSeatNo));

		_uiCallback->showHandCard(cards);
	}

	void GameTableLogic::dealGamePlayResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(S_C_BeginPlay), "size error.");
		S_C_BeginPlay* pData = (S_C_BeginPlay*)object;

		// ���ݱ���
		_CurrHandleSeatNo = pData->byCurrHandleStation;
		_CanLook          = pData->bLook;
		_CanFollow        = pData->bFollow;
		_CanOpen          = pData->bOpen;
		_CanGiveUp        = pData->bGiveUp;
		memcpy(_CanAdd, pData->bAdd, sizeof(_CanAdd));

		// ������ʾ
		stopAllWait();
		_uiCallback->showWaitTime(logicToViewSeatNo(_CurrHandleSeatNo), _thinkTime, _thinkTime);

		// ��ʾ������ť
		bool isMe = (_CurrHandleSeatNo == _mySeatNo);
		_uiCallback->showDashboard(isMe);
		_uiCallback->setLookVisible(isMe && _CanLook);
		_uiCallback->setFollowVisible(isMe && _CanFollow);
		_uiCallback->setAddVisible(isMe ,_CanAdd);
		_uiCallback->setOpenVisible(isMe && _CanOpen);
		_uiCallback->setGiveUpVisible(isMe && _CanGiveUp);

		//��ǰ������Լ�����ʾʱ��
		if (isMe)
		{
			_uiCallback->IStartTimer(_thinkTime);
		}

		if (0 == _MyZongXiaZhu)
		{
			_MyZongXiaZhu += _DiZhu;
		}
		_uiCallback->showMyTotalNote(_MyZongXiaZhu);
	
	}

	void GameTableLogic::dealCallActionResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(S_C_NoticeAction), "size error.");
		S_C_NoticeAction* pData = (S_C_NoticeAction*)object;

		_CurrHandleSeatNo = pData->byCurrHandleStation;
		_CanLook          = pData->bCanLook;
		_CanFollow        = pData->bCanFollow;
		_CanOpen          = pData->bCanOpen;
		_CanGiveUp        = pData->bCanGiveUp;
		memcpy(_CanAdd, pData->bCanAdd, sizeof(_CanAdd));

		// ��ʾ����ʱ
		stopAllWait();
		_uiCallback->showWaitTime(logicToViewSeatNo(_CurrHandleSeatNo), _thinkTime, _thinkTime);

		// ��ʾ�������
		bool isMe = (_CurrHandleSeatNo == _mySeatNo);
		_uiCallback->showDashboard(isMe);
		_uiCallback->setLookVisible(isMe && _CanLook);
		_uiCallback->setFollowVisible(isMe && _CanFollow);
		_uiCallback->setAddVisible(isMe,_CanAdd);
		_uiCallback->setOpenVisible(isMe && _CanOpen);
		_uiCallback->setGiveUpVisible(isMe && _CanGiveUp);
		//��ǰ������Լ�����ʾʱ��
		if (isMe)
		{
			_uiCallback->IStartTimer(_thinkTime);
		}

	}

	void GameTableLogic::dealGameEndResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(S_C_GameEnd), "size error.");
		S_C_GameEnd* pData = (S_C_GameEnd*)object;

		//��Ҷ�Ӧ����ͼ��λ��
		BYTE viewSeat[PLAY_COUNT] = {INVALID_SEAT_NO};
		// �����Ӯ
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			if(pData->i64ChangeMoney[i] != 0)
			{
				_uiCallback->showUserProfit(logicToViewSeatNo(i), pData->i64ChangeMoney[i]);
				viewSeat[i] = logicToViewSeatNo(i);
			}			
		}
		
		//��ʾ׼����ť
		sendUserReady();
		// ��ʾӮ�����
		_uiCallback->showWin(logicToViewSeatNo(pData->byWiner));
		_uiCallback->showDashboard(false);
		_uiCallback->showWatchCard(INVALID_SEAT_NO, false);
		showUserInfo();	

		//�������Ͳ���ʾ�������
		if (false == _bContestRoom)
		{
			_uiCallback->showEndBox(pData, viewSeat, logicToViewSeatNo(pData->byWiner));
		}
	}


	void GameTableLogic::dealSuperProofResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(S_C_SuperUserProof), "size error.");
		S_C_SuperUserProof* pData = (S_C_SuperUserProof*)object;
	}

	void GameTableLogic::dealSuperSetResultResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(S_C_SuperUserSetResult), "size error.");
		S_C_SuperUserSetResult* pData = (S_C_SuperUserSetResult*)object;
	}

	void GameTableLogic::dealLookCardResultResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(S_C_LookCardResult), "size error.");
		S_C_LookCardResult* pData = (S_C_LookCardResult*)object;

		// ���ݱ���
		_UserState[pData->byDeskStation] = pData->iUserState;
		
		bool isMe = (pData->byDeskStation == _mySeatNo);
		if(isMe)
		{
			_userCardCount[pData->byDeskStation] = pData->byUserCardCount;
			memcpy(_userCard[pData->byDeskStation], pData->byUserCard, sizeof(_userCard[pData->byDeskStation]));

			// ��ʾ�������
			std::vector<BYTE> cards(pData->byUserCardCount);
			for (int i = 0; i < pData->byUserCardCount;i++)
			{
				cards[i] = _userCard[pData->byDeskStation][i];
			}
			//memcpy(&cards.front(), _userCard, sizeof(BYTE) * pData->byUserCardCount);
			_uiCallback->showUserFlipCard(logicToViewSeatNo(_mySeatNo), cards);
			
		}

		// ��ʾ����		
		_uiCallback->showUserLookCard(logicToViewSeatNo(pData->byDeskStation), isMe);	
	}

	void GameTableLogic::dealGenZhuResultResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(S_C_UserGenZhuResult), "size error");
		S_C_UserGenZhuResult* pData = (S_C_UserGenZhuResult*)object;

		// ���ݱ���
		_ZongXiaZhu                       = pData->i64AllZongZhu;
		_UserState[pData->byDeskStation]  = pData->iUserState;		
		_XiaZhuData[pData->byDeskStation] = pData->i64UserZongZhu;

		if (pData->byDeskStation == _mySeatNo)
		{
			_MyZongXiaZhu = pData->i64UserZongZhu;
			// ��ʾ�Լ�����ע
			_uiCallback->showMyTotalNote(pData->i64UserZongZhu);
		}

		// ��ʾ��ס
		_uiCallback->showUserFollow(logicToViewSeatNo(pData->byDeskStation));
	
		_uiCallback->showUserNoteMoney(logicToViewSeatNo(pData->byDeskStation), pData->i64FollowNum,_DiZhu);

		// ��ʾ����ע
		_uiCallback->showTotalNote(_ZongXiaZhu);
	}

	void GameTableLogic::dealJiaZhuResultResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(S_C_JiaZhuResult), "size error");
		S_C_JiaZhuResult* pData = (S_C_JiaZhuResult*)object;

		// ��������
		_ZongXiaZhu                       = pData->i64AllZongZhu;
		_UserState[pData->byDeskStation]  = pData->iUserState;
		_XiaZhuData[pData->byDeskStation] = pData->i64UserZongZhu;

		if (pData->byDeskStation == _mySeatNo)
		{
			_MyZongXiaZhu += pData->i64AddNum;
			// ��ʾ�Լ�����ע
			_uiCallback->showMyTotalNote(_MyZongXiaZhu);
		}

		// ��ʾ��ס
		_uiCallback->showUserNote(logicToViewSeatNo(pData->byDeskStation));
		// ��ʾ��ע
		_uiCallback->showUserNoteMoney(logicToViewSeatNo(pData->byDeskStation), pData->i64AddNum,_DiZhu);

		// ��ʾ����ע
		_uiCallback->showTotalNote(pData->i64AllZongZhu);
	}

	void GameTableLogic::dealBiPaiResultResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(S_C_UserBiPaiResult), "size error");
		S_C_UserBiPaiResult* pData = (S_C_UserBiPaiResult*)object;

		// ���ݱ���
		_ZongXiaZhu = pData->i64AllZongZhu;
		_XiaZhuData[pData->byDeskStation] = pData->i64UserZongZhu;
		memcpy(_UserState, pData->iUserStation, sizeof(_UserState));

		if (pData->byDeskStation == _mySeatNo)
		{
			_MyZongXiaZhu += pData->i64AddNum;
			// ��ʾ�Լ�����ע
			_uiCallback->showMyTotalNote(_MyZongXiaZhu);
		}

		// ��ʾ��ע
		_uiCallback->showUserNoteMoney(logicToViewSeatNo(pData->byDeskStation), pData->i64AddNum, _DiZhu);

		// ��ʾ����ע
		_uiCallback->showTotalNote(_ZongXiaZhu);

		// ��ʾ���ƽ��
		_uiCallback->showCompareResult(logicToViewSeatNo(pData->byWinDesk), logicToViewSeatNo(pData->byLostDesk));
	}

	void GameTableLogic::dealGiveUpResultResp(void* object, INT objectSize)
	{
		CCAssert(objectSize == sizeof(S_C_UserGiveUpResult), "size error");
		S_C_UserGiveUpResult* pData = (S_C_UserGiveUpResult*)object;

		// ��������
		_UserState[pData->byDeskStation] = pData->iUserState;

		// �������
		_uiCallback->showUserGiveUp(logicToViewSeatNo(pData->byDeskStation));
	}

	void GameTableLogic::dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo)
	{
		_gameStation = pGameInfo->bGameStation;

		if(INVALID_DESKNO != _mySeatNo)
		{
			_uiCallback->showNotice(GBKToUtf8(pGameInfo->szMessage));
			//�Ŷӻ�����Ҫ׼����ť
			if (RoomLogic()->getRoomRule() & GRR_QUEUE_GAME || RoomLogic()->getRoomRule() & GRR_CONTEST || RoomLogic()->getRoomRule() & GRR_TIMINGCONTEST)
			{
				_uiCallback->showReady(false);
			}
			else
			{
				_uiCallback->showReady(true);
			}
			
			_uiCallback->showNextGame(false);
			
		}
	}

	void GameTableLogic::dealGameStationResp(void* object, INT objectSize)
	{
		refreshParams();

		switch (_gameStation)
		{
		case GS_WAIT_SETGAME:
		case GS_WAIT_ARGEE:
		case GS_WAIT_NEXT:
			{
				CCAssert(sizeof(S_C_GameStation_WaiteAgree) == objectSize, "size error.");
				dealWaitAgreeResp((S_C_GameStation_WaiteAgree*)object);
			}			
			break;
		case GS_SEND_CARD:
			{
				CCAssert(sizeof(S_C_GameStation_SendCard) == objectSize, "size error.");
				dealSendCardResp((S_C_GameStation_SendCard*)object);
			}
			
			break;
		case GS_PLAY_GAME:
			{
				CCAssert(sizeof(S_C_GameStation_PlayGame) == objectSize, "size error.");
				dealPlayGameResp((S_C_GameStation_PlayGame*)object);
			}
			break;
		default:
			break;
		}

		return;
	}

	void GameTableLogic::dealWaitAgreeResp(S_C_GameStation_WaiteAgree* pData)
	{
		// ���ݻ���
		_thinkTime = pData->byThinkTime;
		_beginTime = pData->byBeginTime;
		_GuoDi     = pData->iGuoDi;
		_DiZhu     = pData->iDiZhu;
		_DingZhu   = pData->i64DingZhu;
		//_ShangXian = pData->i64ShangXian;
		_MyZongXiaZhu = 0;
		memcpy(_agree, pData->bAgree, sizeof(_agree));

		// ������ʾ
		// ��ʾ�Լ�����ע
		_uiCallback->showMyTotalNote(_MyZongXiaZhu);
		_uiCallback->showBaseNote(_DiZhu);
		_uiCallback->showGuoDi(_GuoDi);
		_uiCallback->showLimitPerNote(_DingZhu);
		//_uiCallback->showLimitNote(_ShangXian);
		_uiCallback->showTotalNote(0);
		_uiCallback->showWatchCard(INVALID_SEAT_NO, false);
		_uiCallback->showGiveUpCard(INVALID_SEAT_NO, false);

		//��ʾ׼����־
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			if (_agree[i])
			{
				_uiCallback->showReadySign(logicToViewSeatNo(i), true);
			}
			else
			{
				_uiCallback->showReadySign(logicToViewSeatNo(i), false);
			}
		}

		if(_mySeatNo != INVALID_DESKNO && !_agree[_mySeatNo])
		{
			_uiCallback->showWaitTime(logicToViewSeatNo(_mySeatNo), _thinkTime, _thinkTime);
			//��ǰ������Լ�����ʾʱ��
			_uiCallback->IStartTimer(_thinkTime);
		}

	}

	void GameTableLogic::dealSendCardResp(S_C_GameStation_SendCard* pData)
	{
		// ���ݻ���
		_thinkTime  = pData->byThinkTime;
		_beginTime  = pData->byBeginTime;
		_NTSeatNo   = pData->byNtStation;
		_GuoDi      = pData->iGuoDi;
		_DiZhu      = pData->iDiZhu;
		//_ShangXian  = pData->i64ShangXian;
		_DingZhu    = pData->i64DingZhu;
		_ZongXiaZhu = pData->i64AllZongZhu;

		memcpy(_userCardCount, pData->byUserCardCount, sizeof(_userCardCount));
		memcpy(_UserState, pData->iUserState, sizeof(_UserState));
		memcpy(_XiaZhuData, pData->i64XiaZhuData, sizeof(_XiaZhuData));

		// ������ʾ
		_uiCallback->showBaseNote(_DiZhu);
		_uiCallback->showGuoDi(_GuoDi);
		_uiCallback->showLimitPerNote(_DingZhu);
		//_uiCallback->showLimitNote(_ShangXian);
		_uiCallback->showTotalNote(_ZongXiaZhu);
		_uiCallback->showReady(false);

		//����׼����־
		_uiCallback->showReadySign(INVALID_SEAT_NO, false);
		_uiCallback->showWatchCard(INVALID_SEAT_NO, false);
		_uiCallback->showGiveUpCard(INVALID_SEAT_NO, false);

		// ��ʾ������ϵ���
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			if(_userCardCount[i] > 0)
			{
				std::vector<BYTE> cards(_userCardCount[i]);
				for (int j = 0; j < _userCardCount[i];j++)
				{
					cards[j] = 0;
				}
				_uiCallback->showUserHandCard(logicToViewSeatNo(i), cards);
			}
		}

		// ��ʾ�����ע��Ǯ
		for(int i = 0 ;i < PLAY_COUNT; i++)
		{
			if(_XiaZhuData[i] > 0 )
			{
				_uiCallback->showUserNoteMoney(logicToViewSeatNo(i), _XiaZhuData[i],_DiZhu);
			}

			if (i == _mySeatNo && (STATE_ERR != pData->iUserState[i]))
			{
				_uiCallback->showMyTotalNote(_MyZongXiaZhu);			
			}
			else if (i == _mySeatNo && (STATE_ERR == pData->iUserState[i]))
			{
				_uiCallback->showNextGame(true);
			}

			//��¼��ǰ����������ҵ��ǳ�
			UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
			if (nullptr != pUser)
			{
				_userName[i] = pUser->nickName;
			}
			
		}
	}

	void GameTableLogic::dealPlayGameResp(S_C_GameStation_PlayGame* pData)
	{
		// ���ݱ���
		_thinkTime        = pData->byThinkTime;
		_beginTime        = pData->byBeginTime;
		_NTSeatNo         = pData->byNtStation;
		_CurrHandleSeatNo = pData->byCurrHandleDesk;
		_GuoDi            = pData->iGuoDi;
		
		_DiZhu            = pData->iDiZhu;
		//_ShangXian        = pData->i64ShangXian;
		_DingZhu          = pData->i64DingZhu;
		_ZongXiaZhu       = pData->i64AllZongZhu;
		
		memcpy(_XiaZhuData, pData->i64XiaZhuData, sizeof(_XiaZhuData));
		memcpy(_userCardCount, pData->byUserCardCount, sizeof(_userCardCount));
		memcpy(_userCard, pData->byUserCard, sizeof(_userCard));
		memcpy(_UserState, pData->iUserState, sizeof(_UserState));


		// ������ʾ
		_uiCallback->showBaseNote(_DiZhu);
		_uiCallback->showGuoDi(_GuoDi);
		_uiCallback->showLimitPerNote(_DingZhu);
		//_uiCallback->showLimitNote(_ShangXian);
		_uiCallback->showTotalNote(_ZongXiaZhu);
		_uiCallback->showReady(false);
		//����׼����־
		_uiCallback->showReadySign(INVALID_SEAT_NO, false);

		// ��ʾ����ʱ
		stopAllWait();
		_uiCallback->showWaitTime(logicToViewSeatNo(_CurrHandleSeatNo), pData->iRemainderTime, _thinkTime);

		// ��ʾ������ϵ���
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			if(_userCardCount[i] > 0)
			{
				std::vector<BYTE> cards(_userCardCount[i]);
				_uiCallback->showUserHandCard(logicToViewSeatNo(i), cards);
			}

			if (i == _mySeatNo && (STATE_ERR != pData->iUserState[i]))
			{
				_uiCallback->showMyTotalNote(_MyZongXiaZhu);
				_uiCallback->IStartTimer(pData->iRemainderTime);
			}
			else if (i == _mySeatNo && (STATE_ERR == pData->iUserState[i]))
			{
				_uiCallback->showNextGame(true);
			}

			if (STATE_GIVE_UP == _UserState[i])
			{
				_uiCallback->showGiveUpCard(logicToViewSeatNo(i), true);
			}
			else if (STATE_LOOK == _UserState[i])
			{
				_uiCallback->showWatchCard(logicToViewSeatNo(i), true);
			}

			//��¼��ǰ����������ҵ��ǳ�
			UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
			if (nullptr != pUser)
			{
				_userName[i] = pUser->nickName;
			}
		}

		// ��ʾ������ť
		_CanLook   = pData->bCanLook;
		_CanFollow = pData->bCanFollow;
		_CanOpen   = pData->bCanOpen;
		_CanGiveUp = pData->bCanGiveUp;
		memcpy(_CanAdd, pData->bCanAdd, sizeof(_CanAdd));
		
		_uiCallback->showDashboard(pData->iUserState[_mySeatNo]);
		_uiCallback->setLookVisible(_CanLook && (pData->iUserState[_mySeatNo]));
		_uiCallback->setFollowVisible(_CanFollow && (pData->iUserState[_mySeatNo]));
		_uiCallback->setAddVisible(pData->iUserState[_mySeatNo], _CanAdd);
		_uiCallback->setOpenVisible(_CanOpen && (pData->iUserState[_mySeatNo]));
		_uiCallback->setGiveUpVisible(_CanGiveUp && (pData->iUserState[_mySeatNo]));
	}



	void GameTableLogic::dealUserAgreeResp(MSG_GR_R_UserAgree* agree)
	{
		if(agree->bAgreeGame == 1)
		{
			_uiCallback->showWaitTime(logicToViewSeatNo(agree->bDeskStation), 0, 0);
			_uiCallback->showReadySign(logicToViewSeatNo(agree->bDeskStation), true);
			//��ǰ������Լ�����ʾʱ��
			if (agree->bDeskStation == _mySeatNo)
			{
				_uiCallback->IStartTimer(0);
			}
		}
	}

	void GameTableLogic::dealGameStartResp(BYTE bDeskNO)
	{
		_uiCallback->clearDesk();
	}

	void GameTableLogic::dealGameEndResp(BYTE deskNo)
	{
		_deskUserList->transform([](UserInfoStruct* user, INT index) 
		{
			if (user->bUserState != USER_WATCH_GAME)
			{
				user->bUserState = USER_SITTING;
			}

			if (RoomLogic()->getRoomRule() & GRR_CONTEST)
			{
				user->bDeskNO      = INVALID_DESKNO;
				user->bDeskStation = INVALID_DESKSTATION;
				user->bUserState   = USER_LOOK_STATE;						
			}
		});
	}

	void GameTableLogic::dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		BYTE vSeatNo = logicToViewSeatNo(userSit->bDeskStation);

		if (_existPlayer[userSit->bDeskStation])
		{		
			_uiCallback->showUser(vSeatNo, PlatformLogic()->loginResult.dwUserID == userSit->dwUserID, user->bBoy);
		}
		_uiCallback->showUserName(vSeatNo, user->nickName);
		if (_bContestRoom)
		{
			if (PlatformLogic()->loginResult.dwUserID == userSit->dwUserID)
			{
				//��������
				_uiCallback->updateMyRankNum(user->iRankNum);
				_uiCallback->ShowConstRank(user->iRankNum, user->iRemainPeople);
				//��ʾ�ڼ���
				if (!user->iContestCount)
				{
					user->iContestCount = 1;
				}
				_uiCallback->showContsetJuShu(user->iContestCount);
				//�������
				_uiCallback->showUserMoney(vSeatNo, user->i64ContestScore);

			}
		}
		else
		{
			_uiCallback->showUserMoney(vSeatNo, user->i64Money);
		}
	}

	void GameTableLogic::dealQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& user)
	{

		HNGameLogicBase::dealQueueUserSitMessage(deskNo, user);

		if (_mySeatNo != INVALID_DESKSTATION)
		{
			loadUsers();
		}

		_uiCallback->showQueNotice(GBKToUtf8(""), false);
	}

	void GameTableLogic::dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		HNGameLogicBase::dealUserUpResp(userSit, user);
		//�Ŷӳ��Ͳ�����Լ���ͷ����Ϣ��
	
		_uiCallback->showUserUp(logicToViewSeatNo(userSit->bDeskStation), userSit->bDeskStation == _mySeatNo);
		_uiCallback->showReadySign(logicToViewSeatNo(userSit->bDeskStation), false);
		_uiCallback->showWatchCard(logicToViewSeatNo(userSit->bDeskStation), false);
		_uiCallback->showGiveUpCard(logicToViewSeatNo(userSit->bDeskStation), false);
		if(userSit->dwUserID == PlatformLogic()->loginResult.dwUserID)
		{
			//׼���Ŷ�
			if (_isReadyQueue)
			{
				RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_JOIN_QUEUE);
				_isReadyQueue = false;
				//���ؿ�ʼ��ť
				_uiCallback->showReady(false);
				//���ص���ʱ
				_uiCallback->IStartTimer(0);

				//ȫ�������λ �����Լ�
				for (BYTE i = 0; i < PLAY_COUNT; i++)
				{
					_uiCallback->showUserUp(i, true);
					_uiCallback->showReadySign(i, false);
					_uiCallback->showWatchCard(i, false);
					_uiCallback->showGiveUpCard(i, false);
				}
				_uiCallback->clearInvalidUser();

				//��ʾ����Ϊ��������ʾ
				_uiCallback->showQueNotice(GBKToUtf8("����Ϊ�������������ĵȴ�..."), true);
			}
			else
			{
				_uiCallback->leaveDesk();
			}
		}
		else
		{
			_uiCallback->clearInvalidUser();
		}
	}


	/*----------------------�����ӿ�----------------------------*/
	//������Ϣ�㲥
	void GameTableLogic::dealGameContestNotic(MSG_GR_I_ContestInfo* contestInfo)
	{
		//char message[64] = { 0 };
		//sprintf(message, "������������������%d�ˣ����������", contestInfo->iContestNum);
		//_uiCallback->showNotice(GBKToUtf8(message));
	}
	//�û�������Ϣ
	void GameTableLogic::dealGameUserContset(MSG_GR_ContestChange* contestChange)
	{
		
		//���ؿ�ʼ��ť
		_uiCallback->showReady(false);
		//���ص���ʱ
		_uiCallback->IStartTimer(0);

		//ȫ�������λ �����Լ�
		for (BYTE i = 0; i < PLAY_COUNT; i++)
		{
			_uiCallback->showUserUp(i, true);
			_uiCallback->showReadySign(i, false);
			_uiCallback->showWatchCard(i, false);
			_uiCallback->showGiveUpCard(i, false);
		}
		_uiCallback->clearInvalidUser();

		
		//�����Լ�������
		if ((contestChange->dwUserID == PlatformLogic()->loginResult.dwUserID))
		{
			//��������
			_uiCallback->updateMyRankNum(contestChange->iRankNum);
			//��ʾ�Լ�������
			_uiCallback->ShowConstRank(contestChange->iRankNum, contestChange->iRemainPeople);
		}

		//��ʾ����Ϊ��������ʾ
		_uiCallback->showQueNotice(GBKToUtf8("����ϵͳ��������,�����ĵȴ�..."), true);
	}
	//������̭
	void GameTableLogic::dealGameContestKick()
	{
		_uiCallback->showQueNotice(GBKToUtf8(""), false);
		//������̭
		_uiCallback->showGameContestKick();
	}
	//�ȴ���������
	void GameTableLogic::dealGameContestWaitOver()
	{
		_uiCallback->showQueNotice(GBKToUtf8(""), false);
		// �ȴ���������
		_uiCallback->showGameContestWaitOver();
	}
	//��������
	void GameTableLogic::dealGameContestOver(MSG_GR_ContestAward* contestAward)
	{
		//��Ǳ�������
		_bContestEnd = true;
		_uiCallback->showQueNotice(GBKToUtf8(""), false);
		_uiCallback->showGameContestOver(contestAward);
	}


	/************************line*********************************/
	//�����Ŷ�
	void GameTableLogic::sendQueue()
	{
		_isReadyQueue = true;
		//�Ŷӻ�׼�����ȷ���վ���ٿ�ʼ��Ϸ
		RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_USER_UP);
		_uiCallback->clearDesk();
	}

	void GameTableLogic::sendStandUp()
	{
		do 
		{
			if (!RoomLogic()->isConnect())
			{
				_uiCallback->leaveDesk();
				break;
			}

			if (INVALID_DESKNO == _mySeatNo)
			{
				_uiCallback->leaveDesk();
				break;
			}
			//������
			if (_bContestRoom)
			{
				//��Ϸ������ 
				if (_bContestEnd)
				{
					_uiCallback->leaveDesk();
					break;
				}
				else
				{
					_uiCallback->showNotice(GBKToUtf8("�����ڱ����У������˳�����"));
					break;
				}
			}
			
		

			UserInfoStruct* myInfo = _deskUserList->getUserByDeskStation(_mySeatNo);
			if(myInfo != nullptr && myInfo->bUserState == USER_PLAY_GAME)
			{
				_uiCallback->showNotice(GBKToUtf8("��Ϸ�У������뿪"));
				break;
			}
			HNGameLogicBase::sendUserUp();
		} while (0);
	}

	void GameTableLogic::sendUserSit(BYTE byDeskStation)
	{
		BYTE lSeatNo = viewToLogicSeatNo(byDeskStation);
		MSG_GR_S_UserSit UserSit;
		UserSit.bDeskIndex   = _deskNo;
		UserSit.bDeskStation = lSeatNo;
		RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_USER_SIT, &UserSit, sizeof(UserSit));
	}

	//������Ϸ
	void GameTableLogic::enterGame()
	{
		if (RoomLogic()->getRoomRule() & GRR_CONTEST || (RoomLogic()->getRoomRule() & GRR_TIMINGCONTEST))	// ��ʱ��̭������
		{
			_bContestRoom = true;
			_uiCallback->addContestUI();
		}
		else if (RoomLogic()->getRoomRule() & GRR_QUEUE_GAME)		// �Ŷӻ�
		{
			_bContestRoom = false;
		}
		else					// ��ҳ����ۻ���
		{
			_bContestRoom = false;
		}

		if (_mySeatNo == INVALID_DESKSTATION && !_autoCreate)
		{
			for (int i = 0; i < PLAY_COUNT; i++)
			{
				if (!_existPlayer[i])
				{
					sendUserSit(logicToViewSeatNo(i));
					break;
				}
			}
		}
		else
		{
			
			loadUsers();

			if (_mySeatNo != INVALID_DESKSTATION && _autoCreate)
			{
				sendGameInfo();
			}
		}
	}

	void GameTableLogic::loadUsers()
	{
		
		//��ȫ�������λ �����Լ�
		for (BYTE i = 0; i < PLAY_COUNT; i++)
		{
			_uiCallback->showUserUp(i, true);
			_uiCallback->showReadySign(i, false);
			_uiCallback->showWatchCard(i, false);
			_uiCallback->showGiveUpCard(i, false);
		}
		_uiCallback->clearInvalidUser();

		// �������
		BYTE seatNo = INVALID_DESKNO;
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
			if(nullptr != pUser)
			{				
				seatNo = logicToViewSeatNo(i);
				_uiCallback->showUser(seatNo, i == _mySeatNo, pUser->bBoy);

				_uiCallback->showUserName(seatNo,pUser->nickName);

				//�Ǳ�����
				if (_bContestRoom)
				{
					if (i == _mySeatNo)
					{
						//��������
						if (!pUser->iContestCount)
						{
							pUser->iContestCount = 1;
						}
						_uiCallback->showContsetJuShu(pUser->iContestCount);
						//����
						_uiCallback->updateMyRankNum(pUser->iRankNum);
						_uiCallback->ShowConstRank(pUser->iRankNum, pUser->iRemainPeople);
					}
					
					//�������
					_uiCallback->showUserMoney(seatNo, pUser->i64ContestScore);
				}
				else
				{
					_uiCallback->showUserMoney(seatNo, pUser->i64Money);
				}
			}
			else
			{

			}
		}


		//����ڵ�ʱ����ת
		if(_mySeatNo != INVALID_DESKNO)
		{
			//setTableOffsetStation(_mySeatNo);
		}
		else
		{
			_uiCallback->showNotice(GBKToUtf8("����ɹ����ܽ����ƾ֡�"));
		}

		sendGameInfo();
		
		//��ʾ������Ϣ
		_uiCallback->showTableInfo(RoomLogic()->getSelectedRoom()->szGameRoomName);		
	}

	void GameTableLogic::clearDesk()
	{
		if(_mySeatNo != INVALID_DESKNO)
		{
			memset(_CanAdd, 0, sizeof(_CanAdd));
			_uiCallback->showDashboard(false);
			_uiCallback->setLookVisible(false);
			_uiCallback->setFollowVisible(false);
			_uiCallback->setAddVisible(false,_CanAdd);
			_uiCallback->setOpenVisible(false);
			_uiCallback->setGiveUpVisible(false);
		}
	}

	LLONG GameTableLogic::getMinBuy()
	{
		//return _tableInfo.tagDeskCfg.dz.iSubMinMoney;
		return 0;
	}

	LLONG GameTableLogic::getMaxBuy()
	{
		//return _tableInfo.tagDeskCfg.dz.iSubMaxMoney;
		return 0;
	}

	LLONG GameTableLogic::getPlanBuy()
	{
		//return _tableInfo.tagDeskCfg.dz.iSubPlanMoney;
		return 0;
	}

	BYTE  GameTableLogic::getMeStation()
	{
		//return _tableInfo.byMeStation;
		return 0;
	}

	void GameTableLogic::sendNote(LLONG note)
	{
		//tagUserProcess process;
		//process.iVrebType = ACTION_ADD;
		//process.iNote = note;
		//process.bMing = _tableInfo.bMing[_tableInfo.byMeSeatNo];
		//RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, S_C_CALL_ACTION, &process, sizeof(tagUserProcess));
	}

	void GameTableLogic::sendAdd(LLONG note)
	{
		//tagUserProcess process;
		//if(_curNote > 0)
		//{
		//	process.iVrebType = ACTION_ADD;
		//	process.iNote = note;// _curNote;
		//}
		//else
		//{
		//	process.iVrebType = ACTION_ADD;
		//	process.iNote = note;
		//}
		//if(process.iNote > _tableInfo.iLimtePerNote)
		//{
		//	process.iNote = _tableInfo.iLimtePerNote;
		//}
		//process.bMing = _tableInfo.bMing[_tableInfo.byMeSeatNo];
		//RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, S_C_CALL_ACTION, &process, sizeof(tagUserProcess));
	}

	void GameTableLogic::sendAddBet(int multiple)
	{
		C_S_JiaZhu	TJiaZhu;
		TJiaZhu.byDeskStation = _mySeatNo;
		TJiaZhu.iAddType = multiple;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_JIA_ZHU, &TJiaZhu, sizeof(TJiaZhu));
	}

	void GameTableLogic::sendFollow()
	{
		C_S_UserGenZhu TUserGenZhu;
		TUserGenZhu.byDeskStation = _mySeatNo;

		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_GEN_ZHU, &TUserGenZhu, sizeof(TUserGenZhu));
	}

	void GameTableLogic::sendGiveUp()
	{

		C_S_UserGiveUp TUserGiveUp;
		TUserGiveUp.byDeskStation = _mySeatNo;

		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_GIVE_UP, &TUserGiveUp, sizeof(TUserGiveUp));
	}

	void GameTableLogic::sendLook()
	{
		C_S_LookCard TLookCard;
		TLookCard.byDeskStation = _mySeatNo;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_LOOK_CARD, &TLookCard, sizeof(TLookCard));
	}

	void GameTableLogic::sendCompare(BYTE seatNo)
	{

		C_S_UserBiPai	TUserBiPai;
		TUserBiPai.byDeskStation = _mySeatNo;
		TUserBiPai.byBeDeskStation = seatNo;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_BI_PAI, &TUserBiPai, sizeof(TUserBiPai));
	}

	void GameTableLogic::compareCardReq()
	{
		std::vector<bool> seats(PLAY_COUNT);
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			UserInfoStruct* userInfo = _deskUserList->getUserByDeskStation(i);
			if (userInfo != nullptr && _UserState[i] && (_UserState[i] != STATE_GIVE_UP) && playingGame(userInfo->bUserState) && (i != _mySeatNo))
			{
				seats[logicToViewSeatNo(i)] = true;
			}
			else
			{
				seats[logicToViewSeatNo(i)] = false;
			}
		}
		_uiCallback->showCompareOption(seats);
	}

	void GameTableLogic::sendUserReady()
	{
		if (_mySeatNo != INVALID_DESKNO)
		{
			// ������ʾ
			stopAllWait();
			_uiCallback->showWaitTime(logicToViewSeatNo(_mySeatNo), _beginTime, _beginTime);
			_uiCallback->IStartTimer(_beginTime);
		}
		//���Ǳ���������ʾ��ʼ׼����ť
		if (!(RoomLogic()->getRoomRule() & GRR_CONTEST) && !(RoomLogic()->getRoomRule() & GRR_TIMINGCONTEST))	
		{
			_uiCallback->showNotice(GBKToUtf8("�����ʼ������һ�֡�"));
			_uiCallback->showReady(true);
			_uiCallback->showNextGame(false);
		}
		
	}


	bool GameTableLogic::playingGame(int state)
	{
		return (state > GS_WAIT_ARGEE && state <= GS_PLAY_GAME);
	}
	/************************line*********************************/

	void GameTableLogic::initParams()
	{
		//_mySeatNo = INVALID_DESKNO; 
		_waitTime = 0.f;
		memset(_UserState, 0, sizeof(_UserState));
		memset(_userName, 0, sizeof(_userName));
		_MyZongXiaZhu = 0;
		_bContestEnd = false;
	}

	void GameTableLogic::refreshParams()
	{
		//_thinkTime = 0.f;
		//_beginTime = 0.f;
		_GuoDi = 0;
		_DiZhu = 0;
		_DingZhu = 0;
		//_ShangXian = 0;
		_curNote = 0;
		_MyZongXiaZhu = 0;
		memset(_agree, false, sizeof(_agree));
		memset(_UserState, 0, sizeof(_UserState));
		_isReadyQueue = false;
	}

	void GameTableLogic::stopAllWait()
	{
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			BYTE vSeatNo = logicToViewSeatNo(i);
			_uiCallback->showWaitTime(vSeatNo, 0 ,0);
			_uiCallback->IStartTimer(0);
		}
	}

	void GameTableLogic::initGuoDi()
	{
	}

	void GameTableLogic::showUserInfo()
	{
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			if(_existPlayer[i])
			{
				BYTE vSeatNo = logicToViewSeatNo(i);
				UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
				if(pUser != nullptr)
				{
					_uiCallback->showUserName(vSeatNo, pUser->nickName);
					if (_bContestRoom)
					{
						//�������
						_uiCallback->showUserMoney(vSeatNo, pUser->i64ContestScore);
					}
					else
					{
						_uiCallback->showUserMoney(vSeatNo, pUser->i64Money);
					}
					
				}
			}
		}
	}

	// ��ʾ�������
	// min:��С����
	// max:������
	// plan:�ƻ�����
	void GameTableLogic::showBuyMoney(LLONG min, LLONG max, LLONG plan)
	{

	}

	BYTE GameTableLogic::getNextUserStation(BYTE byStation)
	{
		return byStation;
	}

	BYTE GameTableLogic::getLastUserStation(BYTE byStation)
	{

		return byStation;
	}
}


