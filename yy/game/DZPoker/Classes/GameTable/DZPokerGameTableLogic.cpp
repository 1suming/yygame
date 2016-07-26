#include "DZPokerGameTableLogic.h"
#include "DZPokerGameTableUI.h"
#include "DZPokerUpgradeMessage.h"

namespace DZPoker
{

	/************************�ָ���*********************************/
	GameTableLogic::GameTableLogic(GameTableUICallback* uiCallback, BYTE bDeskIndex, bool bAutoCreate):
		_uiCallback(uiCallback), HNGameLogicBase(bDeskIndex, PLAY_COUNT, bAutoCreate, uiCallback)
		, _myInfo(nullptr)
	{
		_tableInfo.bDeskIndex = bDeskIndex;
		_tableInfo.bAutoCreate = bAutoCreate;

		_tableInfo.byMeStation = INVALID_DESKNO;
		_tableInfo.iOffsetStation = 0;
		memset(_tableInfo.bHaveUser, 0, sizeof(_tableInfo.bHaveUser));
		_myInfo = UserInfoModule()->findUser(PlatformLogic()->loginResult.dwUserID);
		initData();
	}

	GameTableLogic::~GameTableLogic()
	{

	}

	/************************�ָ���*********************************/
	void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		switch(messageHead->bAssistantID)//֪ͨ��Ϣ
		{
		case ASS_UG_USER_SET:
			HNLOG("dealUserSetResp");
			dealUserSetResp(object,objectSize);
			break;
		case ASS_UG_USER_SET_CHANGE:
			HNLOG("dealUserSetChangeResp");
			dealUserSetChangeResp(object,objectSize);
			break;
		case ASS_GAME_BEGIN://��Ϸ��ʼ
			HNLOG("dealGameBeginResp");
			dealGameBeginResp(object,objectSize);
			break;
		case ASS_SYS_DESK_CFG://�յ�ϵͳ�������ӵ���Ϣ����������
			HNLOG("dealSysDeskCfgResp");
			dealSysDeskCfgResp(object,objectSize);
			break;
		case ASS_CFG_DESK://���õ�ע
			HNLOG("dealCfgDeskResp");
			dealCfgDeskResp(object,objectSize);
			break;
		case ASS_DESK_CFG://�յ�ע����Ϣ����������
			HNLOG("dealDeskCfgResp");
			dealDeskCfgResp(object,objectSize);
			break;
		case ASS_AGREE_DESK_CFG:
			HNLOG("dealAgreeDeskCfgResp");
			dealAgreeDeskCfgResp(object,objectSize);
			break;
		case ASS_BET:
			HNLOG("dealBetResp");
			dealBetResp(object,objectSize);
			break;
		case ASS_SEND_A_CARD://�յ���������һ���ư���������
			HNLOG("dealHandCardResp");
			dealHandCardResp(object,objectSize);
			break;
		case ASS_SEND_3_CARD://�յ���������һ���ư���������
			HNLOG("dealSend3CardResp");
			dealSend3CardResp(object,objectSize);
			break;
		case ASS_SEND_4_5_CARD://�յ���������һ���ư���������
			HNLOG("dealSend4_5CardResp");
			dealSend4_5CardResp(object,objectSize);
			break;
		case ASS_SEND_CARD_FINISH://�յ�����������������ҷ�����ɵ���Ϣ��������
			HNLOG("dealSendCardFinishResp");
			dealSendCardFinishResp(object,objectSize);
			break;
		case ASS_BETPOOL_UP://�յ��������߳ظ��°���������
			HNLOG("dealBetPoolUpResp");
			dealBetPoolUpResp(object,objectSize);
			break;
		case ASS_BET_REQ:
			HNLOG("dealBetReqResp");
			dealBetReqResp(object,objectSize);
			break;
		case ASS_BET_INFO://�յ������ע����Ϣ
			HNLOG("dealBetInfoResp");
			dealBetInfoResp(object,objectSize);
			break;
		case ASS_NEW_USER:
			HNLOG("dealNewUserResp");
			dealNewUserResp(object,objectSize);
			break;
		case ASS_PASS_USER:
			HNLOG("dealPassUserResp");
			dealPassUserResp(object,objectSize);
			break;
		case ASS_SUB_MONEY:
			HNLOG("dealSubMoneyResp");
			dealSubMoneyResp(object,objectSize);
			break;
		case ASS_COMPARE_CARD://������Ϣ
			HNLOG("dealCompareCardResp");
			dealCompareCardResp(object,objectSize);
			break;
		case ASS_CALLMANDATE:
			HNLOG("dealCallMandateResp");
			dealCallMandateResp(object,objectSize);
			break;
		case ASS_CHECKMANDATE:
			HNLOG("dealCheckMandateResp");
			dealCheckMandateResp(object,objectSize);
			break;
		case ASS_CALLANYMANDATE:
			HNLOG("dealCallAnyMandateResp");
			dealCallAnyMandateResp(object,objectSize);
			break;
		case ASS_PASSABANDONMANDATE:
			HNLOG("dealPassAbandonMandateResp");
			dealPassAbandonMandateResp(object,objectSize);
			break;
		case ASS_USER_LEFT_DESK:
			HNLOG("dealUserLeftDeskResp");
			dealUserLeftDeskResp(object,objectSize);
			break;
		case ASS_SORT_OUT://ϵͳ�Զ�����Ƭ
			HNLOG("dealSortOutResp");
			dealSortOutResp(object,objectSize);
			break;
		case ASS_TOKEN://�յ�������Ϣ�������û�
			HNLOG("dealTokenResp");
			dealTokenResp(object,objectSize);
			break;
		case ASS_RESULT://�յ�������Ϣ����������
			HNLOG("dealResultResp");
			dealResultResp(object,objectSize);
			break;
		case ASS_SHOW_IN_MONEY://�յ���������Ϣ����������
			HNLOG("dealBuyMoneyResp");
			dealBuyMoneyResp(object,objectSize);
			break;
		case ASS_LOOK_CARD://��ҿ���
			HNLOG("dealLookCardResp");
			dealLookCardResp(object,objectSize);
			break;
		case ASS_CAN_SHOW_CARD://��չʾ�� 
			HNLOG("dealCanShowCardResp");
			dealCanShowCardResp(object,objectSize);
			break;
		case ASS_SHOW_CARD://չʾ�� 
			HNLOG("dealShowCardResp");
			dealShowCardResp(object,objectSize);
			break;
		case ASS_NO_PLAYER://û����ҽ�����Ϸ���˳�����
			HNLOG("dealNoPlayerResp");
			dealNoPlayerResp(object,objectSize);
			break;
		case ASS_AGREE://����Ƿ�ͬ�����Ϸ�ģ�����
			HNLOG("dealAgreeResp");
			dealAgreeResp(object,objectSize);
			break;
		case ASS_CFG_DESK_TIMEOUT://���õ�ע��ʱ 
			HNLOG("dealCfgDeskTimeOutResp");
			dealCfgDeskTimeOutResp(object,objectSize);
			break;
		case ASS_NOT_ENOUGH_MONEY://������ϵ�Ǯ���㹻ʱ������
			HNLOG("dealNotEnoughMoneyResp");
			dealNotEnoughMoneyResp(object,objectSize);
			break;
		case ASS_MONEY_LACK:
			HNLOG("dealMoneyLackResp");
			dealMoneyLackResp(object,objectSize);
			break;
		case ASS_NEXT_ROUND_REQ:
			HNLOG("dealNextRoundReqResp");
			dealNextRoundReqResp(object,objectSize);
			break;
		case ASS_NEW_ROUND_BEGIN://�յ��µ�һ�غϿ�ʼ��Ϣ����������
			HNLOG("dealNewRoundBeginResp");
			dealNewRoundBeginResp(object,objectSize);
			break;
		case ASS_FINISH_ROUND_REQ:
			HNLOG("dealFinishRoundReqResp");
			dealFinishRoundReqResp(object,objectSize);
			break;
		default:
			break;
		}
	}

	/************************�ָ���*********************************/

	/************************************************************************/
	/* ��Ϣ������                                                         */
	/************************************************************************/
	void GameTableLogic::dealUserSetResp(void* object, INT objectSize)
	{

	}

	void GameTableLogic::dealCfgDeskResp(void* object, INT objectSize)
	{
		TCfgDesk* data = (TCfgDesk*)object;

		if (objectSize != sizeof(TCfgDesk))
		{
			return;
		}
		//������Ϸ��״̬
		_tableInfo.bGameStatus = GS_CONFIG_NOTE;
	}

	void GameTableLogic::dealDeskCfgResp(void* object, INT objectSize)
	{
		TDeskCfg* data = (TDeskCfg*)object;

		_tableInfo.bGameStatus = GS_CONFIG_NOTE;
		::memcpy(&_tableInfo.tagDeskCfg,data,sizeof(TDeskCfg));
	}

	void GameTableLogic::dealAgreeResp(void* object, INT objectSize)
	{
		TAgree* data = (TAgree*)object;
	}

	void GameTableLogic::dealCfgDeskTimeOutResp(void* object, INT objectSize)
	{

	}

	void GameTableLogic::dealNotEnoughMoneyResp(void* object, INT objectSize)
	{
		TMoneyLack * pMoneyLack = (TMoneyLack *)object;

		if (_tableInfo.byMeStation == pMoneyLack->nbyUser)
		{
			_uiCallback->showUserLackMoney(logicToViewStation(pMoneyLack->nbyUser));
		}
	}

	void GameTableLogic::dealBetResp(void* object, INT objectSize)
	{
		TBet* data = (TBet*)object;
	}

	void GameTableLogic::dealSubMoneyResp(void* object, INT objectSize)
	{
		TSubMoney* data = (TSubMoney*)object;

	}

	void GameTableLogic::dealCompareCardResp(void* object, INT objectSize)
	{
		showAutoAction(false);

		showHelp(true);

		//����ɢ�ƣ����ӣ����ԣ���������С˳�ӣ�˳�ӣ�ͬ������«����������Сͬ��˳��ͬ��˳���ʼ�ͬ��˳
		int cardType[12] = 
		{
			0/*ɢ��*/ ,1/*һ��*/, 2/*����*/, 3/*����*/, 
			4/*��С˳��*/, 4/*˳��*/, 5/*ͬ��*/, 6/*��«*/, 
			7/*����*/, 8/*��Сͬ��˳*/, 8/*ͬ��˳*/, 9/*�ʼ�ͬ��˳*/
		};

		TCompareCard* pCmd = (TCompareCard*)object;
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			UserInfoStruct* User = _deskUserList->getUserByDeskStation(i);
			if (!User || pCmd->nHandCardCount[i] != 2)
			{
				continue ;
			}

			//��ʾ������ϵ���
			std::vector<BYTE> byHandCards;
			for(int j = 0; j < pCmd->nHandCardCount[i]; j++)
			{
				byHandCards.push_back(pCmd->bHandCards[i][j]);
			}
			if(i != _tableInfo.byMeStation)
			{
				_uiCallback->showUserHandCard(logicToViewStation(i),byHandCards);
			}

			//�������е���
			BYTE bCard[2] = {0,0};
			if (i == _tableInfo.byMeStation)
			{
				// �Լ�����2�ſ��õ��Ƶ���
				for (int j = 0; j < 5; j++)
				{
					if (pCmd->bHandCards[i][0] == pCmd->bCards[i][j])
					{
						bCard[0] = pCmd->bCards[i][j];
					}

					if (pCmd->bHandCards[i][1] == pCmd->bCards[i][j])
					{
						bCard[1] = pCmd->bCards[i][j];
					}

					if (bCard[0] && bCard[1])
					{
						break;
					}
				}
			}

			for (int j = 0; j < 2; j++)
			{
				if (bCard[j] != 0x00)
				{
					_uiCallback->upUserHandCard(_tableInfo.byMeStation,j);
				}
			}

			//������ʾ

			//���𹫹���
			if (i == _tableInfo.byMeStation)
			{
				BYTE bCard[5] = {0,0,0,0,0};
				BYTE bCardCount = 0;

				for (int j = 0; j < 5; j++)
				{
					for (int k = 0; k < 5; k++)
					{
						if (_tableInfo.byCommunityCard[k] == pCmd->bCards[i][j])
						{
							bCard[k] = pCmd->bCards[i][j];
							break;
						}
					}
				}

				for (int j = 0; j < 5; j++)
				{
					if (bCard[j] != 0x00)
					{
						_uiCallback->upCommunityCard(j);
					}
				}
			}

			//��ʾ����
			if(pCmd->nHandCardCount[i] > 0)
			{
				_uiCallback->showUserCardType(logicToViewStation(i), cardType[pCmd->nCardKind[i]-1]);
			}
		}
	}

	void GameTableLogic::dealHandCardResp(void* object, INT objectSize)
	{
		if(_tableInfo.byNTUser == INVALID_DESKNO)
		{
			return;
		}

		TCards* data = (TCards*)object;
		_tableInfo.nTypeCard = data->nTypeCard;

		::memcpy(_tableInfo.byHandCard, data->byCards, MAX_HANDCARDS_NUM);
		_tableInfo.iHandCardCounts = data->iCardsNum;

		//��ׯ��λ�ÿ�ʼ������
		std::vector<THandCard> bDeskStations;
		BYTE byNextUser = _tableInfo.byNTUser;
		do 
		{
			THandCard handCard;
			if(byNextUser == _tableInfo.byMeStation && data->iCardsNum == 2)
			{
				memcpy(handCard.byCards, data->byCards[byNextUser], sizeof(handCard.byCards));
			}
			handCard.byUser = logicToViewStation(byNextUser);

			bDeskStations.push_back(handCard);
			byNextUser = getNextUserStation(byNextUser);
		} while (byNextUser != _tableInfo.byNTUser);

		_uiCallback->showHandCard(bDeskStations);
	}

	void GameTableLogic::dealSend3CardResp(void* object, INT objectSize)
	{
		TCards* data = (TCards*)object;

		memcpy(_tableInfo.byCommunityCard, data->byCards, sizeof(BYTE) * (data->iCardsNum));
		_tableInfo.iCommunityCardCounts += data->iCardsNum;

		// ��ʾ3�Ź�����
		if(data->iCardsNum == 3)
		{
			std::vector<BYTE> byCards;
			byCards.push_back(data->byPubCards[0]);
			byCards.push_back(data->byPubCards[1]);
			byCards.push_back(data->byPubCards[2]);
			_uiCallback->showFlopCard(byCards);
		}
	}

	void GameTableLogic::dealSend4_5CardResp(void* object, INT objectSize)
	{
		TCards* data = (TCards*)object;
		memcpy(_tableInfo.byCommunityCard, data->byCards, sizeof(BYTE) * (data->iCardsNum));
		_tableInfo.iCommunityCardCounts = data->iCardsNum;

		// ��ʾ4 - 5 �����ƣ�����һ����Ϣ
		if(data->iCardsNum == 4)
		{
			_uiCallback->showTurnCard(data->byPubCards[3]);
		}

		if(data->iCardsNum == 5)
		{
			_uiCallback->showRiverCard(data->byPubCards[4]);
		}
	}

	void GameTableLogic::dealSendCardFinishResp(void* object, INT objectSize)
	{

	}

	void GameTableLogic::dealUserLeftDeskResp(void* object, INT objectSize)
	{
		// 	OnShowUserLogo();
		// 
		// 	UserLeftDeskStruct *pLeftData = (UserLeftDeskStruct *)buffer ; 
		// 
		// 	if(pLeftData->bClearLogo)
		// 	{
		// 		BYTE byView = m_pGameFrame->DeskStation2View(pLeftData->bDeskStation);
		// 
		// 		SAFE_CTRL_OPERATE(IImage,DZPK_READY_MAP + byView,SetControlVisible(false));
		// 		SAFE_CTRL_OPERATE(IImage,DZPK_USER_MAP + byView,SetControlVisible(false));
		// 		SAFE_CTRL_OPERATE(IText,DZPK_USER_NAME + byView,SetControlVisible(false));
		// 		SAFE_CTRL_OPERATE(IImage,DZPK_FOLD_LOGO + byView,SetControlVisible(false));
		// 		SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_USER_CHAIR + byView,SetControlVisible(true));	// ��ʾ�������
		// 
		// 		GameUserLeft(pLeftData->bDeskStation);
		// 
	}

	void GameTableLogic::dealSortOutResp(void* object, INT objectSize)
	{

	}

	void GameTableLogic::dealTokenResp(void* object, INT objectSize)
	{
		TToken* pToken = (TToken*)object;
		if(pToken->byUser == INVALID_DESKNO)
		{
			return;
		}
		_tableInfo.byTokenUser = pToken->byUser;

		//�ֵ���¼��Ҳ���
		BYTE byMeStation = _tableInfo.byMeStation;
		_uiCallback->changeToken(byMeStation == pToken->byUser, logicToViewStation(pToken->byUser));
		if(byMeStation == pToken->byUser)
		{
			showAutoAction(false);

			//�Ƿ�������йܲ���
			if(pToken->bNewTurn)
			{
				// һ���е�һ��������, ������в�����־
				memset(_tableInfo.emLastOperation, ET_UNKNOWN, sizeof(_tableInfo.emLastOperation));

				// ��ղ����˼�¼ջ
				while (!_tableInfo.stackLastStation.empty())
				{
					_tableInfo.stackLastStation.pop();   // �Ƴ�ջ������
				}
			}
			else
			{
				// ���������йܣ������йܲ���
				if (doUserAutoAction(object))
				{
					// �Զ��йܳɹ�
					return ;
				}
			}

			showAction(true, object);
			_tableInfo.nCurrentMoney = 0;

		}
		else
		{
			showAction(false, object);

			//�йܲ�����ʾ�����û�����ƺ�ȫ�£������ڵ�ǰ��ҵ�ǰ����λ��
			bool bWatch = _tableInfo.bFold[byMeStation] || _tableInfo.bAllIn[byMeStation];
			BYTE byUser1 = getNextUserStation(_tableInfo.byTokenUser);
			BYTE byUser2 = getNextUserStation(byUser1);
			showAutoAction(!bWatch && (byMeStation == byUser1 || byMeStation == byUser2));
		}

		//��ʾʣ�����ʱ��
		BYTE byUser = logicToViewStation(pToken->byUser);
		_uiCallback->showUserLeftTime(byUser,
			_tableInfo.tagDeskCfg.Time.byOperate - pToken->iPassedTime, 
			_tableInfo.tagDeskCfg.Time.byOperate, 
			pToken->byUser == _tableInfo.byMeStation);
	}

	void GameTableLogic::dealBetPoolUpResp(void* object, INT objectSize)
	{
		TBetPool* data = (TBetPool*)object;
		memcpy(&_tableInfo.tagBetPool, data, sizeof(_tableInfo.tagBetPool));

		if (nullptr == _uiCallback) return;

		for (int i = 0; i < 8; i++)
		{
			if (data->iBetPools[i] > 0)
			{
				_uiCallback->showPot(i);
			}
		}
	}

	void GameTableLogic::dealBetInfoResp(void* object, INT objectSize)
	{
		TBet* pBet = (TBet*)object;

		BYTE vSeatNo = logicToViewStation(pBet->byUser);

		if (pBet->nType == ET_AUTO || pBet->nType == ET_BET || pBet->nType == ET_RAISE || pBet->nType == ET_ALLIN || pBet->nType == ET_CALL)
		{
			_tableInfo.nBetMoney[pBet->byUser] += pBet->nMoney; 
			_tableInfo.iMoneys[pBet->byUser] -= pBet->nMoney;

			_uiCallback->showUserMoney(vSeatNo,_tableInfo.iMoneys[pBet->byUser]);
		}

		// ��¼ȫ����������
		if (pBet->nType == ET_ALLIN)
		{
			_tableInfo.bAllIn[pBet->byUser] = true;
		}
		else if (pBet->nType == ET_FOLD)
		{
			_tableInfo.bFold[pBet->byUser] = true;		
		}


		// ��¼�����Ҳ�������
		_tableInfo.emLastOperation[pBet->byUser] = pBet->nType;

		// ��¼�������
		_tableInfo.stackLastStation.push(pBet->byUser);

		//�������Ͳ���
		switch (pBet->nType)
		{
		case ET_BET:// ��ע
			_uiCallback->showUserBet(vSeatNo);
			break;
		case ET_CALL:// ��ע
			_uiCallback->showUserCall(vSeatNo);
			break;
		case ET_RAISE:// ��ע
			_uiCallback->showUserAdd(vSeatNo);
			break;
		case ET_CHECK:// ����
			_uiCallback->showUserCheck(vSeatNo);
			break;
		case ET_FOLD:// ����
			_uiCallback->showUserFold(vSeatNo);
			break;
		case ET_ALLIN:// ȫ��
			_uiCallback->showUserAllIn(vSeatNo);
			break;
		default:
			break;
		}

		if(pBet->nMoney > 0)
		{
			_uiCallback->showUserBetMoney(vSeatNo, _tableInfo.nBetMoney[pBet->byUser]);
		}
		_uiCallback->showUserLeftTime(vSeatNo, 0, 0, pBet->byUser == _tableInfo.byMeStation);
	}

	void GameTableLogic::dealResultResp(void* object, INT objectSize)
	{
		showAction(false, nullptr);
		showAutoAction(false);

		if (_tableInfo.bGameStatus != GS_PLAY_GAME)
		{
			return;
		}

		TResult* pResult = (TResult*)object;

		//��¼��Ϸ����
		::memcpy(&_tableInfo.tagResult,pResult,sizeof(TResult));

		//������Ϸ��״̬
		_tableInfo.bGameStatus = GS_WAIT_NEXT_ROUND;
		memcpy(_tableInfo.iMoneys, pResult->nSubMoney, sizeof(_tableInfo.iMoneys));

		showUserMoney(nullptr);

		//�ɽ�
		std::vector<std::vector<LLONG>> winPool;
		winPool.resize(PLAY_COUNT);
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			int seatNo = logicToViewStation(i);
			winPool[seatNo].resize(PLAY_COUNT);
			for(int j = 0; j < PLAY_COUNT; j++)
			{
				winPool.at(seatNo).at(j) = pResult->nWinPoolsMoney[i][j];
			}
		}
		_uiCallback->showWinPool(winPool);

		//ֹͣ����
		if(_tableInfo.byTokenUser != INVALID_DESKNO)
		{
			_uiCallback->showUserLeftTime(logicToViewStation(_tableInfo.byTokenUser),
				0,
				_tableInfo.tagDeskCfg.Time.byOperate,
				_tableInfo.byTokenUser == _tableInfo.byMeStation);
		}
		_uiCallback->isWaitTime(true);
	}

	void GameTableLogic::dealBuyMoneyResp(void* object, INT objectSize)
	{
		TSubMoney* pData = (TSubMoney*)object;	

		if(_tableInfo.bHaveUser[pData->bDeskStation])
		{
			_tableInfo.iMoneys[pData->bDeskStation] = pData->nMoney[pData->bDeskStation];
		}

		//_uiCallback->showMyMoney(HNPlatformLogic::getInstance()->loginResult.i64Money);
		_uiCallback->showUserMoney(logicToViewStation(pData->bDeskStation), _tableInfo.iMoneys[pData->bDeskStation]);
	
		if (pData->bDeskStation == _mySeatNo)
		{
			_uiCallback->isWaitTime(false);
		}
	
	}

	void GameTableLogic::dealLookCardResp(void* object, INT objectSize)
	{
		// 	if(sizeof(UserLookCardStruct) != nLen)
		// 	{
		// 		return;
		// 	}
		// 
		// 	UserLookCardStruct *pLookCard = (UserLookCardStruct*)buffer;
		// 
		// 	BYTE byViewStation = m_pGameFrame->DeskStation2View(pLookCard->bDeskStation);	//��ͼλ��
		// 
		// 	//�Լ�����ͼλ�ò����ſ��ƶ���
		// 	if(byViewStation == m_pGameFrame->DeskStation2View(m_pGameFrame->GetMyDeskStation()))
		// 	{
		// 		return;
		// 	}
		// 
		// 	SAFE_CTRL_OPERATE(IImage,DZPK_SHOW_SMALL_CARD+byViewStation,SetControlVisible(false));
		// 	SAFE_CTRL_OPERATE(IImage,DZPK_V1_PAI+byViewStation,SetControlVisible(false));
		// 	SAFE_CTRL_OPERATE(IImage,DZPK_V2_PAI+byViewStation,SetControlVisible(false));
		// 
		// 
		// 	///�ؼ����ƶ���
		// 	IExAnimate *pAnima = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(DZPK_LOOK_CARD_ANIMA_0 + byViewStation));
		// 	if(nullptr != pAnima)
		// 	{
		// 		pAnima->SetIsLoop(false);				//��ѭ��
		// 		pAnima->SetPlayTotalTime(400);			//���ò���ʱ��
		// 		pAnima->SetPlayState(true);				//���ò���
		// 		pAnima->SetIsSymmetrical(false);		//�����Ƿ�Գ�
		// 		pAnima->SetControlVisible(true);
		// 
	}

	void GameTableLogic::dealNextRoundReqResp(void* object, INT objectSize)
	{
		TNextRoundReq* data = (TNextRoundReq*)object;
	}

	void GameTableLogic::dealNewRoundBeginResp(void* object, INT objectSize)
	{
		TNextRoundBeginInfo* data = (TNextRoundBeginInfo*)object;

		//��¼ׯ�ҵ�λ��
		_tableInfo.byNTUser = data->byNTUser;

		//��¼��Сäע
		_tableInfo.bySmallBlind = data->bSmallBlind;
		_tableInfo.byBigBlind = data->bBigBlind;

		//�й�״̬
		_tableInfo.bTuoGuan = false;

		//������գ���������
		::memset(_tableInfo.byBackCard,0,MAX_BACKCARDS_NUM);
		_tableInfo.iBackCardNums = 0;

		_uiCallback->showDealer(logicToViewStation(_tableInfo.byNTUser));
		_uiCallback->showSmallBlind(logicToViewStation(_tableInfo.bySmallBlind));
		_uiCallback->showBigBlind(logicToViewStation(_tableInfo.byBigBlind));
	}

	void GameTableLogic::dealShowCardResp(void* object, INT objectSize)
	{
		// 	ShowCardStruct* pShowCard = (ShowCardStruct*)buffer;
		// 
		// 	//��ȫ�ж�
		// 	if(nLen != sizeof(ShowCardStruct) || 255 == pShowCard->byDeskStation)
		// 	{
		// 		return false;
		// 	}
		// 
		// 	//��ʾ չʾ����ҵ� ����
		// 	INoMoveCard* pCard = dynamic_cast<INoMoveCard*>(m_pUI->GetIControlbyID(DZPK_SHOW_CARD+m_pGameFrame->DeskStation2View(pShowCard->byDeskStation)));
		// 	if(nullptr != pCard)
		// 	{
		// 		pCard->SetCardList(pShowCard->byCardList,pShowCard->iCardCount);
		// 		pCard->SetControlVisible(true);
		// 
	}

	void GameTableLogic::dealNoPlayerResp(void* object, INT objectSize)
	{
		TNoPlayer* data = (TNoPlayer*)object;
	}

	void GameTableLogic::dealMoneyLackResp(void* object, INT objectSize)
	{
		TMoneyLack* data = (TMoneyLack*)object;
	}

	void GameTableLogic::dealGameBeginResp(void* object, INT objectSize)
	{
		_tableInfo.bGameStatus = GS_PLAY_GAME;
		//_tableInfo.bGameStatus = TYPE_PLAY_GAME;

		// 	if(_tableInfo.bWatch)
		// 	{
		// 		initData();
		// 		return;
		// 	}

		_tableInfo.iUserStation = TYPE_PLAY_GAME;
		showUserInfo();
		initData();
	}

	void GameTableLogic::dealSysDeskCfgResp(void* object, INT objectSize)
	{
		TDeskCfg* data = (TDeskCfg*)object;

		::memcpy(&_tableInfo.tagDeskCfg,data,sizeof(TDeskCfg));

		//ϵͳ�����������ݺ��UI������Ҫ��ʾ��Ϸ����
	}

	void GameTableLogic::dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo)
	{
		HNLOG("onGameInfoMessage");
		_tableInfo.bGameStatus = pGameInfo->bGameStation;
		_tableInfo.bEableWatch = pGameInfo->bWatchOther;
		if(_tableInfo.bAutoCreate)
		{
			_uiCallback->showTableReady(false);
		}
		else
		{
			_uiCallback->noticeMessage(GBKToUtf8(pGameInfo->szMessage));
		}
	}

	void GameTableLogic::dealGameStationResp(void* object, INT objectSize)
	{
		UserInfoStruct* myInfo = _deskUserList->getUserByUserID(PlatformLogic()->loginResult.dwUserID);
		_tableInfo.byMeStation = (myInfo) ? myInfo->bDeskStation : INVALID_DESKNO;

		TGSBase* data = (TGSBase*)object;

		//����������Ϣ
		saveTableInfo(object);

		//��ʾ�����Ϣ
		showUserInfo();

		_tableInfo.bGameStatus = data->bGameStatus;

		if (_tableInfo.bGameStatus == GS_WAIT_ARGEE || GS_WAIT_SETGAME)
		{
			_uiCallback->isWaitTime(true);
		}

		//��������״̬��Ϣ�����½���
		switch (_tableInfo.bGameStatus)
		{
		case GS_WAIT_SETGAME://�ȴ���������״̬
		case GS_WAIT_ARGEE://�ȴ�ͬ������
		case GS_WAIT_NEXT_ROUND://�ȴ���һ�غϽ���״̬
			{
				TGSWaitAgree* pGSWaitAgree = (TGSWaitAgree*)data;
				TSubMoney cmd;
				cmd.bDeskStation = -1;
				memcpy(cmd.nMoney,pGSWaitAgree->nSubMoney,sizeof(cmd.nMoney));
				memcpy(cmd.bReady,pGSWaitAgree->bReady,sizeof(cmd.bReady));

				//���´�������Ϣ
				showUserMoney(&cmd);

				//�����������Ҫ�õ��������Ϣ
				showBuyMoney(pGSWaitAgree->tagDeskCfg.dz.iSubMinMoney,
					pGSWaitAgree->tagDeskCfg.dz.iSubMaxMoney,
					pGSWaitAgree->tagDeskCfg.dz.iSubPlanMoney);
			}
			break;
		case GS_PLAY_GAME://��Ϸ����״̬
			{
				_tableInfo.bIsOffline = true;

				TGSPlaying * pGSPlaying = (TGSPlaying *)object;
				// ��õõ����Ƶ����
				_tableInfo.byTokenUser = pGSPlaying->byTokenUser;

				// ׯ�Ҵ�С��
				_tableInfo.byNTUser = pGSPlaying->byNTUser;
				_tableInfo.bySmallBlind = pGSPlaying->bySmallBlind;
				_tableInfo.byBigBlind = pGSPlaying->byBigBlind;
				_tableInfo.iUserStation = pGSPlaying->iUserStation; 

				// ȫ�����
				memcpy(_tableInfo.bAllIn, pGSPlaying->bNoBet, sizeof(pGSPlaying->bNoBet));

				// �������
				memcpy(_tableInfo.bFold, pGSPlaying->bGiveUp, sizeof(pGSPlaying->bGiveUp));

				// ��ȡ������
				memcpy(_tableInfo.iMoneys, pGSPlaying->nSubMoney, sizeof(pGSPlaying->nSubMoney));

				if (_tableInfo.byNTUser != INVALID_DESKNO)
				{
					//��ʾׯ��
					_uiCallback->showDealer(logicToViewStation(_tableInfo.byNTUser));
				}
				else
				{	
					//û��ׯ��
					HNLOG("û��ׯ��");
				}

				//�µ�Сäע�ʹ�äע������������ע�����Ҫ��������
				if (_tableInfo.bySmallBlind != INVALID_DESKNO)
				{
					// ��ʾСä
					BYTE byStation = logicToViewStation(_tableInfo.bySmallBlind);
					_uiCallback->showSmallBlind(byStation);
				}
				else
				{
					//û��Сäע
					HNLOG("û��Сä");
				}

				if (_tableInfo.byBigBlind != INVALID_DESKNO)
				{
					// ��ʾ��äע
					BYTE byStation = logicToViewStation(_tableInfo.byBigBlind);
					_uiCallback->showBigBlind(byStation);
				}
				else
				{
					// û�д�äע
					HNLOG("û�д�ä");
				}


				// ������ƴ��������ֻ���ص�¼��ҵ����ƣ�
				std::vector<BYTE> byCards;
				_tableInfo.iHandCardCounts = pGSPlaying->nHandCardNums;
				//memcpy(_tableInfo.byHandCard, pGSPlaying->byHandCard, sizeof(BYTE) * _tableInfo.iHandCardCounts);
				memcpy(_tableInfo.byHandCard, pGSPlaying->byHandCard, PLAY_COUNT*MAX_DEAL_CARDS);

				// �����ƴ���
				std::vector<BYTE> flopCards,leftCards;
				_tableInfo.iCommunityCardCounts = pGSPlaying->nCardsCount;
				memcpy(_tableInfo.byCommunityCard, pGSPlaying->byCards, sizeof(BYTE) *  pGSPlaying->nCardsCount);
				for(int i = 0; i < _tableInfo.iCommunityCardCounts; i++)
				{
					if(i < 3)
					{
						flopCards.push_back(_tableInfo.byCommunityCard[i]);
					}
					else
					{
						leftCards.push_back(_tableInfo.byCommunityCard[i]);
					}
				}
				if(_tableInfo.byMeStation != INVALID_DESKNO)
				{
					if(flopCards.size() > 0)
					{
						_uiCallback->showFlopCard(flopCards);
					}
					if(leftCards.size() > 1)
					{
						_uiCallback->showTurnCard(leftCards[0]);
						_uiCallback->showRiverCard(leftCards[1]);
					}
					else if(leftCards.size() > 0)
					{
						_uiCallback->showTurnCard(leftCards[0]);
					}
				}

				// ��ע���
				for (int i = 0; i < PLAY_COUNT; i++)
				{
					if (!pGSPlaying->bHaveUser[i])
					{
						continue ;
					}

					BYTE vSeatNo = logicToViewStation(i);

					// ��ʾ������
					_uiCallback->showUserMoney(vSeatNo, _tableInfo.iMoneys[i]);

					//��ʾ�ǳ�
					UserInfoStruct* userInfo = _deskUserList->getUserByDeskStation(i);
					if(userInfo != nullptr)
					{
						_uiCallback->showUserNickName(vSeatNo, GBKToUtf8(userInfo->nickName));
					}

					_tableInfo.nBetMoney[i] = 0;
					for (int j = 0; j < PLAY_COUNT; j++)
					{
						_tableInfo.nBetMoney[i] += pGSPlaying->nBetMoney[i][j];
					}

					if (_tableInfo.nBetMoney[i] > 0 && _tableInfo.bHaveUser[i])
					{
						// ��ʾ��ע����
						_uiCallback->showUserBetMoney(vSeatNo, _tableInfo.nBetMoney[i]);
					}
					else
					{
						// ������ע����	
						HNLOG("���������û�г���");
					}

					//��������
					if(userInfo != nullptr && userInfo->bUserState == USER_PLAY_GAME)
					{
						if (_tableInfo.bFold[i])
						{
							_uiCallback->showUserFold(vSeatNo);
						}

						if (_tableInfo.bAllIn[i])
						{
							_uiCallback->showUserAllIn(vSeatNo);
						}

						std::vector<BYTE> handCards;
						handCards.push_back(_tableInfo.byHandCard[i][0]);
						handCards.push_back(_tableInfo.byHandCard[i][1]);
						_uiCallback->showUserHandCard(vSeatNo, handCards);
					}
				}

				// ��ǰ�������
				TToken toKen;
				toKen.bNewTurn	 = pGSPlaying->bNewTurn;
				toKen.bNewTurns	 = pGSPlaying->bNewTurns;
				toKen.byUser	 = pGSPlaying->byTokenUser;
				toKen.byVerbFlag = pGSPlaying->byVerbFlag;
				toKen.nCallMoney = pGSPlaying->nCallMoney;
				toKen.iPassedTime = pGSPlaying->iEvPassTime;
				dealTokenResp(&toKen,sizeof(&toKen));
			}
			break;
		default:
			break;
		}

		//test send ready.
		sendUserReady();
	}


	void GameTableLogic::dealUserAgreeResp(MSG_GR_R_UserAgree* agree)
	{

	}

	void GameTableLogic::dealGameStartResp(BYTE bDeskNO)
	{
		if(bDeskNO == _tableInfo.bDeskIndex)
		{
			_uiCallback->clearDesk();
		}
	}

	void GameTableLogic::dealGameEndResp(BYTE bDeskNO)
	{
		HNLOG("dealGameEndResp");
		if(bDeskNO == _tableInfo.bDeskIndex)
		{
			// �����û�״̬
// 			UserInfoModule()->transform(bDeskNO, [](UserInfoStruct* user, INT index) 
// 			{
// 				if (user->bUserState != USER_WATCH_GAME)
// 				{
// 					user->bUserState = USER_SITTING;
// 				}
// 				//����������һ�ֺ��û���Ҫ���������������������Ӻź���λ��
// 				if (RoomLogic()->getSelectedRoom()->dwRoomRule & GRR_CONTEST)
// 				{
// 					user->bDeskNO = INVALID_DESKNO;
// 					user->bDeskStation = INVALID_DESKSTATION;
// 					user->bUserState = USER_LOOK_STATE;						
// 				}
// 			});

			_deskUserList->transform([](UserInfoStruct* user, INT index) 
			{
				if (user->bUserState != USER_WATCH_GAME)
				{
					user->bUserState = USER_SITTING;
				}
				//����������һ�ֺ��û���Ҫ���������������������Ӻź���λ��
				if (RoomLogic()->getSelectedRoom()->dwRoomRule & GRR_CONTEST)
				{
					user->bDeskNO = INVALID_DESKNO;
					user->bDeskStation = INVALID_DESKSTATION;
					user->bUserState = USER_LOOK_STATE;						
				}
			});

			//_uiCallback->clearDesk();

			if(_tableInfo.byMeStation != INVALID_DESKNO && _tableInfo.bHaveUser[_tableInfo.byMeStation])
			{
				sendUserReady();
			}
		}
	}

	void GameTableLogic::dealGamePointResp(void* object, INT objectSize)
	{
		CCAssert(sizeof (MSG_GR_R_UserPoint) == objectSize, "MSG_GR_R_UserPoint is error.");
		MSG_GR_R_UserPoint * pUserPoint = (MSG_GR_R_UserPoint*)object;

		if(_myInfo != nullptr)
		{
			_uiCallback->showMyMoney(_myInfo->i64Money);
		}
	}

	void GameTableLogic::dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		HNLOG("onUserSitMessage");
		if(_tableInfo.bDeskIndex == userSit->bDeskIndex)
		{
			BYTE seatNo = logicToViewStation(userSit->bDeskStation);
			if(!_tableInfo.bHaveUser[userSit->bDeskStation])
			{
				_uiCallback->showUser(seatNo, userSit->dwUserID == PlatformLogic()->loginResult.dwUserID, user->bBoy);
				_tableInfo.bHaveUser[userSit->bDeskStation] = true;

				UserInfoStruct* userInfo = _deskUserList->getUserByUserID(userSit->dwUserID);
				if(userInfo != nullptr)
				{
					_uiCallback->showUserNickName(seatNo, GBKToUtf8(userInfo->nickName));
					_uiCallback->showUserMoney(seatNo, 0);
				}

				if (userSit->dwUserID == PlatformLogic()->loginResult.dwUserID)
				{
					setTableOffsetStation(userSit->bDeskStation);
					_tableInfo.byMeStation = userSit->bDeskStation;

					MSG_GM_S_ClientInfo ClientInfo;
					RoomLogic()->sendData(MDM_GM_GAME_FRAME, ASS_GM_GAME_INFO, (CHAR*)&ClientInfo, sizeof(ClientInfo));
				}
			}
			else
			{

			}

		}
	}

	void GameTableLogic::dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		HNLOG("onUserUpMessage");
		if(_tableInfo.bDeskIndex == userSit->bDeskIndex)
		{
			_uiCallback->showUserUp(logicToViewStation(userSit->bDeskStation), userSit->bDeskStation == _tableInfo.byMeStation);
			_tableInfo.bHaveUser[userSit->bDeskStation] = false;

			if(userSit->dwUserID == PlatformLogic()->loginResult.dwUserID)
			{
				_tableInfo.byMeStation = INVALID_DESKNO;
				_uiCallback->leaveDesk();
			}
		}
	}

	/************************�ָ���*********************************/

	void GameTableLogic::sendUserRaise(LLONG money)
	{
		LLONG betMoney = money;
		if(betMoney > _tableInfo.iMoneys[_tableInfo.byMeStation])
		{
			betMoney = _tableInfo.iMoneys[_tableInfo.byMeStation];
		}
		sendUserBet(ET_RAISE,betMoney);
	}

	void GameTableLogic::sendUserBet(LLONG money)
	{
		LLONG betMoney = money;
		if(betMoney > _tableInfo.iMoneys[_tableInfo.byMeStation])
		{
			betMoney = _tableInfo.iMoneys[_tableInfo.byMeStation];
		}
		sendUserBet(ET_RAISE,betMoney);
	}

	void GameTableLogic::sendUserAllIn()
	{
		sendUserBet(ET_ALLIN,0);
	}

	void GameTableLogic::sendUserFold()
	{
		sendUserBet(ET_FOLD,0);
	}

	void GameTableLogic::sendUserCall()
	{
		sendUserBet(ET_CALL,_tableInfo.nCallMoney);
	}

	void GameTableLogic::sendUserCheck()
	{
		sendUserBet(ET_CHECK,0);
	}

	void GameTableLogic::sendUserUp()
	{
		do 
		{
			if (!RoomLogic()->isConnect())
			{
				_uiCallback->leaveDesk();
				break;
			}

			if (INVALID_DESKNO == _tableInfo.byMeStation)
			{
				_uiCallback->leaveDesk();
				break;
			}

			UserInfoStruct* myInfo = _deskUserList->getUserByDeskStation(_tableInfo.byMeStation);
			if(myInfo != nullptr && myInfo->bUserState == USER_PLAY_GAME)
			{
				_uiCallback->noticeMessage(GBKToUtf8("��Ϸ�У������뿪��"));
				break;
			}

			HNGameLogicBase::sendUserUp();

		} while (0);
	}

	void GameTableLogic::loadDeskUsersUI()
	{		
		//�Զ�����������
		if(_tableInfo.bAutoCreate)
		{
			_uiCallback->showTableReady(true);
		}

		//��ʾ�������
		BYTE seatNo = INVALID_DESKNO;
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			UserInfoStruct* userInfo = _deskUserList->getUserByDeskStation(i);
			if(nullptr != userInfo)
			{
				if(userInfo->dwUserID == RoomLogic()->loginResult.pUserInfoStruct.dwUserID)
				{
					_tableInfo.byMeStation = i;
				}
				seatNo = logicToViewStation(i);
				_uiCallback->showUser(seatNo, i == _tableInfo.byMeStation, userInfo->bBoy);
				_tableInfo.bHaveUser[i] = true;

				_uiCallback->showUserNickName(seatNo, GBKToUtf8(userInfo->nickName));
				_uiCallback->showUserMoney(seatNo,0);
			}
		}

		// ����ڵ�ʱ����ת
		if(_tableInfo.byMeStation != INVALID_DESKNO)
		{
			setTableOffsetStation(_tableInfo.byMeStation);
		}
		else
		{
			_uiCallback->noticeMessage(GBKToUtf8("����ɹ����ܽ����ƾ֡�"));
		}

//		if(_tableInfo.bAutoCreate)
//		{
        sendGameInfo();
//		}

		// ��ʾ������Ϣ
		_uiCallback->showTableInfo(RoomLogic()->getSelectedRoom()->szGameRoomName);
		if(nullptr != _myInfo)
		{
			_uiCallback->showMyMoney(_myInfo->i64Money);
		}
		//HNGameLogicBase::loadDeskUsers();
	}

	void GameTableLogic::clearDesk()
	{
		showAction(false, nullptr);
		showAutoAction(false);
	}

	LLONG GameTableLogic::getMinBuy() const
	{
		return _tableInfo.tagDeskCfg.dz.iSubMinMoney;
	}

	LLONG GameTableLogic::getMaxBuy() const
	{
		return _tableInfo.tagDeskCfg.dz.iSubMaxMoney;
	}

	LLONG GameTableLogic::getPlanBuy() const
	{
		return _tableInfo.tagDeskCfg.dz.iSubPlanMoney;
	}

	BYTE  GameTableLogic::getMeStation() const
	{
		return _tableInfo.byMeStation;
	}

	void GameTableLogic::sendUserReady()
	{
		//Ĭ�ϴ���ƻ���������
		BYTE byMeStation = _tableInfo.byMeStation;
		do 
		{
			if (_tableInfo.byMeStation == INVALID_DESKNO) break;

			if(_tableInfo.bGameStatus == GS_PLAY_GAME)
			{
				_uiCallback->noticeMessage(GBKToUtf8("�ƾֽ����У���һ�ֲ��ܹ���"));
				break;
			}

			UserInfoStruct* myInfo = _deskUserList->getUserByDeskStation(byMeStation);
			if(nullptr != myInfo && (myInfo->i64Money < _tableInfo.tagDeskCfg.dz.iSubMinMoney))
			{
				_uiCallback->noticeMessage(GBKToUtf8("���벻�����룬���ֵ���ٲμӡ�"));
				break;
			}

			LLONG min = _tableInfo.tagDeskCfg.dz.iSubMinMoney;
			LLONG max = _tableInfo.tagDeskCfg.dz.iSubMaxMoney;
			LLONG plan = _tableInfo.tagDeskCfg.dz.iSubMinMoney;
			/*if(_tableInfo.iMoneys[byMeStation] > min && _tableInfo.iMoneys[byMeStation] < max)
			{
				plan = _tableInfo.iMoneys[byMeStation];
			}*/
			_uiCallback->showBuyMoneyDialog(_tableInfo.byMeStation, min, max, plan, myInfo->i64Money);
		} while (0);
	}

	void GameTableLogic::sendUserBet(emType type, LLONG money)
	{
		TBet tagBet;
		tagBet.nMoney = money;
		tagBet.nType = type;
		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_BET, &tagBet,sizeof(tagBet));
	}

	//˳ʱ��+����ʱ��-
	void GameTableLogic::setTableOffsetStation(BYTE bDeskStation)
	{
		int oldOffset = _tableInfo.iOffsetStation;
		_tableInfo.iOffsetStation = -bDeskStation;
		int rotate = _tableInfo.iOffsetStation - oldOffset;
		if(rotate > 3)
		{
			rotate = PLAY_COUNT - rotate;
		}
		else if(rotate < -3)
		{
			rotate = PLAY_COUNT + rotate;
		}

		_uiCallback->rotateStation(rotate);
	}
	/************************�ָ���*********************************/
	//���±���ÿһ�ֿ�ʼ��������
	void GameTableLogic::initData()
	{
		_tableInfo.bIsOffline = false;

		// ��¼��ע���
		//m_iBetCounts = 0;

		// ��ʼ���ۼӽ��
		_tableInfo.nCurrentMoney = 0;
		//m_nCurrentMoney = 0;

		// ��ע���
		_tableInfo.nCallMoney = 0;
		//m_nCallMoney = 0;

		//ׯ��λ��
		_tableInfo.byNTUser = INVALID_DESKNO;
		//m_byNTUser = 255;

		//Сäעλ��
		_tableInfo.bySmallBlind = INVALID_DESKNO;
		//m_bySmallBlind = 255;

		//��äעλ��
		_tableInfo.byBigBlind = INVALID_DESKNO;
		//m_byBigBlind = 255;

		//ÿ���û����ý��
		//memset(m_nMoney, 0, sizeof(m_nMoney));

		// ȫ�����
		memset(_tableInfo.bAllIn, 0, sizeof(_tableInfo.bAllIn));

		// �������
		memset(_tableInfo.bFold, 0, sizeof(_tableInfo.bFold));

		// �йܱ�־
		memset(_tableInfo.bTrusteeship, 0, sizeof(_tableInfo.bTrusteeship));

		// ��һ�β�������
		memset(_tableInfo.emLastOperation, ET_UNKNOWN,sizeof(_tableInfo.emLastOperation));

		//������
		_tableInfo.iHandCardCounts = 0;

		//��������
		memset(_tableInfo.byHandCard, 0, sizeof(_tableInfo.byHandCard));

		//������
		_tableInfo.iCommunityCardCounts = 0;
		memset(_tableInfo.byCommunityCard, 0, sizeof(_tableInfo.byCommunityCard));

		// ÿ�������ע���
		memset(_tableInfo.nBetMoney, 0, sizeof(_tableInfo.nBetMoney));

		// ���ջ
		while (!_tableInfo.stackLastStation.empty())
		{
			_tableInfo.stackLastStation.pop();   // �Ƴ�ջ������
		}
	}

	void GameTableLogic::saveTableInfo(void* object)
	{
		TGSBase* pGSBase = (TGSBase*)object;

		memcpy(&_tableInfo.tagDeskCfg,&pGSBase->tagDeskCfg,sizeof(TDeskCfg));
		memcpy(&_tableInfo.bHaveUser,pGSBase->bHaveUser,sizeof(_tableInfo.bHaveUser));
	}


	void GameTableLogic::showUserInfo()
	{
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			if(_tableInfo.bHaveUser[i])
			{
				BYTE seatNo = logicToViewStation(i);

				//��ʾ�����Ϣ
				UserInfoStruct* user = _deskUserList->getUserByDeskStation(i);
				if(user != nullptr)
				{
					_uiCallback->showUserNickName(seatNo, GBKToUtf8(user->nickName));
				}
				//if(i == _tableInfo.byMeStation && _tableInfo.iMoneys[_tableInfo.byMeStation] <= 0)
				//{
				//	_uiCallback->showUserMoney(seatNo,user->i64Money);
				//}
			}
		}
	}

	// ��ʾ��ҳ���
	// pSubMoney��������������ҵĳ���
	void GameTableLogic::showUserMoney(const TSubMoney* pSubMoney)
	{
		if(pSubMoney != nullptr)
		{
			memcpy(&_tableInfo.iMoneys, pSubMoney->nMoney, sizeof(_tableInfo.iMoneys));	
		}

		UserInfoStruct* userInfo = nullptr;
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			userInfo = _deskUserList->getUserByDeskStation(i);
			if(nullptr != userInfo && _tableInfo.bHaveUser[i])
			{
				BYTE byUser = logicToViewStation(i);
				_uiCallback->showUserMoney(byUser,_tableInfo.iMoneys[i]);
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
		int nCount = 0;
		BYTE nextSeatNo = byStation;
		UserInfoStruct* userInfo = nullptr;

		// �ƹ���Ч������ѷ��������
		do 
		{
			nextSeatNo = (nextSeatNo + 1 + PLAY_COUNT) % PLAY_COUNT;
			userInfo = _deskUserList->getUserByDeskStation(nextSeatNo);
			nCount++;
			if(nCount >= PLAY_COUNT)
			{
				return byStation;
			}
		} while (userInfo == nullptr || _tableInfo.bFold[nextSeatNo] || _tableInfo.iUserStation != TYPE_PLAY_GAME);

		return nextSeatNo;
	}

	BYTE GameTableLogic::getLastUserStation(BYTE byStation)
	{
		int nCount = 0;
		BYTE lastSeatNo = byStation;
		UserInfoStruct* userInfo = nullptr;

		// �ƹ���Ч������ѷ��������
		do 
		{
			lastSeatNo = (lastSeatNo - 1 + PLAY_COUNT) % PLAY_COUNT;
			userInfo = _deskUserList->getUserByDeskStation(lastSeatNo);
			nCount++;
			if(nCount >= PLAY_COUNT)
			{
				return byStation;
			}
		} while (userInfo == nullptr || _tableInfo.bFold[lastSeatNo] || _tableInfo.iUserStation != TYPE_PLAY_GAME);

		return lastSeatNo;
	}

	BYTE GameTableLogic::viewToLogicStation(BYTE byStation)
	{
		return (byStation - _tableInfo.iOffsetStation + PLAY_COUNT) % PLAY_COUNT;
	}

	BYTE GameTableLogic::logicToViewStation(BYTE byStation)
	{
		return (byStation + _tableInfo.iOffsetStation + PLAY_COUNT) % PLAY_COUNT;
	}

	/**��ʾ������ť**/
	void GameTableLogic::showAction(bool bVisible, void* object)
	{
		if(bVisible)
		{
			TToken* pToken = (TToken*)object;
			BYTE byVerbFlag = pToken->byVerbFlag;
			BYTE byMeStation = _tableInfo.byMeStation;



			LLONG nMoney = _tableInfo.nBetMoney[getLastUserStation(byMeStation)] - _tableInfo.nBetMoney[byMeStation];
			bool bRaiseable = _tableInfo.iMoneys[byMeStation] > nMoney && _tableInfo.iMoneys[byMeStation] > pToken->nCallMoney;

			BYTE tmp = getLastUserStation(byMeStation);
			CCLOG("*****************************lastNo:%d",tmp);
			CCLOG("*****************************lastBetMoney:%lld",_tableInfo.nBetMoney[tmp]);
			CCLOG("*****************************MeNo:%d",_tableInfo.byMeStation);
			CCLOG("*****************************MeBetMoney:%lld",_tableInfo.nBetMoney[byMeStation]);
			CCLOG("*****************************MeMoney:%lld",_tableInfo.iMoneys[byMeStation]);
			CCLOG("*****************************call:%lld",pToken->nCallMoney);

			//��һ����ť
			if(pToken->nCallMoney > 0)
			{
				if(!bRaiseable)
				{
					_uiCallback->showAllIn(true,true);
				}
				else
				{
					_uiCallback->showCall(true,true);
				}
			}
			else
			{
				_uiCallback->showCheck(true,true);
			}

			//�ڶ�����ť
			LLONG max = _tableInfo.iMoneys[byMeStation];
			LLONG min = pToken->nCallMoney;
			if(bRaiseable)
			{
				LLONG tmp = _tableInfo.tagDeskCfg.dz.iBigBlindNote + pToken->nCallMoney;
				if(tmp < _tableInfo.iMoneys[byMeStation])
				{
					min = tmp;
				}
				else
				{
					min = _tableInfo.iMoneys[byMeStation];
				}

				if(pToken->bNewTurn)
				{
					_uiCallback->showBet(true,true,0);
					_uiCallback->showSlider(true,true,max,min,min);
				}
				else if(bRaiseable)
				{
					_uiCallback->showRaise(true,true);
					_uiCallback->showSlider(true,true,max,min,min);
				}
			}
			else
			{
				_uiCallback->showBet(false,false,0);
				_uiCallback->showRaise(false,false);
				_uiCallback->showSlider(false,false,0,0,0);
			}


			//��������ť
			_uiCallback->showFold(true,true);
		}
		else
		{
			_uiCallback->showBet(false,false,0);
			_uiCallback->showCall(false,false);
			_uiCallback->showRaise(false,false);
			_uiCallback->showCheck(false,false);
			_uiCallback->showFold(false,false);
			_uiCallback->showAllIn(false,false);
			_uiCallback->showSlider(false,false,0,0,0);

			_uiCallback->showBetMoney(false,0);
			_uiCallback->showCallMoney(false,0);
			_uiCallback->showRaiseMoney(false,0);
		}
	}

	//��ʾ��ѧ
	void GameTableLogic::showHelp(bool bVisible)
	{
		_uiCallback->showHelp(bVisible);
	}

	//��ʾ�й�
	void GameTableLogic::showAutoAction(bool bVisible)
	{
		_uiCallback->showAutoCall(bVisible,_tableInfo.bTrusteeship[0]);
		_uiCallback->showAutoCallAny(bVisible,_tableInfo.bTrusteeship[1]);
		_uiCallback->showAutoCheck(bVisible,_tableInfo.bTrusteeship[2]);
		_uiCallback->showAutoCheckFold(bVisible,_tableInfo.bTrusteeship[3]);
	}

	void GameTableLogic::setSliderValue()
	{
		LLONG nPercentMoney = _tableInfo.iMoneys[_tableInfo.byMeStation];
		LLONG number =  _uiCallback->getSliderValue();
		_tableInfo.nCurrentMoney = number;

		// �������ؼ��е㲻��׼, ���ܻᳬ��һ����, �����ڴ��жϽ��
		if (_tableInfo.nCurrentMoney > nPercentMoney)
		{
			_tableInfo.nCurrentMoney = nPercentMoney;
		}
		else if (_tableInfo.nCurrentMoney < _tableInfo.tagDeskCfg.dz.iMinusMoney)
		{
			_tableInfo.nCurrentMoney = _tableInfo.tagDeskCfg.dz.iMinusMoney;
			_uiCallback->setSliderValue(_tableInfo.nCurrentMoney);
		}

		if (nPercentMoney < _tableInfo.tagDeskCfg.dz.iMinusMoney)
		{
			_tableInfo.nCurrentMoney = nPercentMoney;
		}

		_uiCallback->showRaiseMoney(true,_tableInfo.nCurrentMoney);
		_uiCallback->showBetMoney(true,_tableInfo.nCurrentMoney);
	}

	//�йܴ���
	bool GameTableLogic::doUserAutoAction(void* object)
	{
		TToken* pToken = (TToken*)object;

		// 	if(_tableInfo.bWatch)
		// 	{
		// 		return false;
		// 	}

		if (!_tableInfo.stackLastStation.empty())
		{
			// ������Ը�ע�Ľ��, �����ø�ע��ť
			_tableInfo.nCallMoney = 0;

			// ����ϼҲ����Ƿ���, ��������ϼҵĲ�����ע���
			stack<BYTE> skTemp(_tableInfo.stackLastStation);
			if (!skTemp.empty())
			{
				BYTE bLastStation = skTemp.top();
				skTemp.pop();

				// �����ϼ���ע����ע��ȫ�µ����
				while (bLastStation != INVALID_DESKSTATION && _tableInfo.emLastOperation[bLastStation] == ET_FOLD)
				{
					if (!skTemp.empty())
					{
						// ��ջ��ȡ��һ��Ԫ��
						bLastStation = skTemp.top();
						skTemp.pop();
					}
					else
					{
						// ջΪ��
						bLastStation = -1;
						break;
					}
				}

				if (bLastStation != INVALID_DESKSTATION)
				{
					// ����Ҫ��ע�Ľ��
					_tableInfo.nCallMoney = _tableInfo.nBetMoney[bLastStation] - _tableInfo.nBetMoney[_tableInfo.byMeStation];
				}
				else
				{
					log("Client: AutoTrusteeship ��ע����, ����ջû���ҵ��ϼҲ�����");
				}
			}
			else
			{
				log("Client: AutoTrusteeship ��ע����, ����ջΪ��");
			}

			stack<BYTE> skTemp2(_tableInfo.stackLastStation);
			BYTE bLastStation = skTemp2.top();
			skTemp2.pop();

			if (bLastStation != INVALID_DESKNO)
			{
				if (pToken->byVerbFlag & UD_VF_BET)	// ������ע�����
				{
					log("Client: ������ע�����");
					// �ڴ˽����Զ��йܹ������Զ��й������ж�
					if (_tableInfo.emLastOperation[bLastStation] == ET_CHECK)
					{
						// �Զ�����, �ϼ���������²�
						if (_tableInfo.bTrusteeship[2] || _tableInfo.bTrusteeship[3])
						{
							// �����Զ����Ʋ���
							_tableInfo.bTrusteeship[2] = false;  // (ֻ��һ��)

							log("Client: ִ���Զ����Ʋ���");

							sendUserCheck();
							return true;
						}
					}
					else if (_tableInfo.emLastOperation[bLastStation] == ET_FOLD)
					{
						// �Զ�����
						if (_tableInfo.bTrusteeship[3])
						{
							// �����Զ����Ʋ���
							log("Client: ִ���Զ����Ʋ���");

							sendUserFold();
							return true;
						} 
					}
				}
				else
				{
					//�ڴ˽����Զ��йܹ������Զ��й������ж�
					if (_tableInfo.emLastOperation[bLastStation] == ET_CHECK)
					{
						// �Զ�����, �ϼ���������²�
						if (_tableInfo.bTrusteeship[2] || _tableInfo.bTrusteeship[3])
						{
							// �����Զ����Ʋ���
							_tableInfo.bTrusteeship[2] = false;  // (ֻ��һ��)

							log("Client: ִ���Զ����Ʋ���");

							sendUserCheck();
							return true;
						}
					}
					// �Զ���ע(ֻ��һ��) �� �Զ����κ�ע(ÿ�ζ���)
					else if (_tableInfo.bTrusteeship[0] || _tableInfo.bTrusteeship[1])
					{
						// �ڴ˽����Զ���ע����
						log("Client: ִ���Զ���ע����");
						_tableInfo.bTrusteeship[0] = false;

						sendUserCall();
						return  true;
					}
					else if (_tableInfo.bTrusteeship[3])
					{
						// �Զ�����/����
						if (_tableInfo.emLastOperation[bLastStation] == ET_FOLD)
						{
							// �����Զ����Ʋ���
							log("Client: ִ���Զ����Ʋ���2");

							sendUserFold();
							return true;
						}
					}
				}
			}
			else
			{
				log("Client: Error: �ǵ�һ������, �������˵�ջΪ��");
			}
		}

		return false;
	}


}
