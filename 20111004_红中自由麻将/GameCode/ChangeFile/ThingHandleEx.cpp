#include "stdafx.h"
#include "ThingHandleEx.h"
#include "../server/ServerManage.h"
#include "../server/DebugPrintf.h"

//ȫ�ֶ���
//ThingHandleEx g_ThingHandle;
	
ThingHandleEx::ThingHandleEx(void)
{
}
ThingHandleEx::~ThingHandleEx(void)
{
}
/*-------------------------------------------------------------------------------------------------------------*/
//���������¼�
bool ThingHandleEx::GetGameStaton(BYTE bDeskStation, UINT uSocketID, bool bWatchUser,BYTE GameStation)
{
	//GameStationDataEx GameStationData;
	
	switch (GameStation)
	{
	case GS_WAIT_SETGAME:	//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:		//�ȴ���ҿ�ʼ״̬
	case GS_WAIT_NEXT:		//�ȴ���һ����Ϸ��ʼ
		{
			GameStation_Wait_Agree TGameStation;

			TGameStation.iVersion		= MAIN_VERSION;
			TGameStation.iVersion2		= ASS_VERSION;

			TGameStation.byGameStation	= GameStation;

			TGameStation.byBeginTime	= pDesk->sGameData.m_mjRule.byBeginTime;	//��ʼ�ȴ�ʱ��
			TGameStation.byOutTime		= pDesk->sGameData.m_mjRule.byOutTime;		//����ʱ��
			TGameStation.byBlockTime	= pDesk->sGameData.m_mjRule.byBlockTime;	//˼��ʱ��
			TGameStation.bForceTuoGuan	= pDesk->sGameData.m_mjRule.bForceTuoGuan;	//ǿ���Ƿ��й�

			for(vector<long int>::iterator it = pDesk->m_vlSuperUserID.begin(); it != pDesk->m_vlSuperUserID.end(); it++)
			{
				if(pDesk->m_pUserInfo[bDeskStation]->m_UserData.dwUserID == *it)
				{
					TGameStation.bIsSuperUser = true;
					break;
				}
			}

			for (int i=0; i<PLAY_COUNT; i++)
			{
				if(NULL != pDesk->m_pUserInfo[i])
				{
					if(pDesk->m_pUserInfo[i]->m_UserData.bUserState == USER_ARGEE)  //�����Ϊͬ��״̬
					{
						TGameStation.bUserAgree[i] = true;
					}
				}
			}

			//��������
			pDesk->SendGameStationEx(bDeskStation,  bWatchUser, &TGameStation, sizeof(TGameStation));
			return true;
		}
	
	case GS_SEND_CARD:		//����״̬(��δ����)
		{
			//��������
			//pDesk->SendGameStationEx(bDeskStation,  bWatchUser, &GameStationData, sizeof(GameStationData));
			GameStationData TGameStation;
			TGameStation.iVersion		= MAIN_VERSION;
			TGameStation.iVersion2		= ASS_VERSION;
			TGameStation.byGameStation	= GameStation;

			TGameStation.byBeginTime	= pDesk->sGameData.m_mjRule.byBeginTime;	//��ʼ�ȴ�ʱ��
			TGameStation.byOutTime		= pDesk->sGameData.m_mjRule.byOutTime;		//����ʱ��
			TGameStation.byBlockTime	= pDesk->sGameData.m_mjRule.byBlockTime;	//˼��ʱ��
			TGameStation.bForceTuoGuan	= pDesk->sGameData.m_mjRule.bForceTuoGuan;	//ǿ���Ƿ��й�

			for(vector<long int>::iterator it = pDesk->m_vlSuperUserID.begin(); it != pDesk->m_vlSuperUserID.end(); it++)
			{
				if(pDesk->m_pUserInfo[bDeskStation]->m_UserData.bLogoID == *it)
				{
					TGameStation.bIsSuperUser = true;
					break;
				}
			}
			TGameStation.byNtStation = pDesk->sGameData.m_byNtStation;
			//TGameStation.m_byNowOutStation = pDesk->sUserData.m_byNowOutStation;
			//pDesk->sUserData.m_bTuoGuan[bDeskStation] = false;
			memcpy(TGameStation.m_bistuoguan,pDesk->sUserData.m_bTuoGuan,sizeof(TGameStation.m_bistuoguan));
			memcpy(TGameStation.m_byArHandPai,pDesk->sUserData.m_byArHandPai,sizeof(TGameStation.m_byArHandPai));
			memcpy(TGameStation.m_iArMenPai,pDesk->sUserData.m_iArMenPai,sizeof(TGameStation.m_iArMenPai));

			//��������
			pDesk->SendGameStationEx(bDeskStation,  bWatchUser, &TGameStation, sizeof(TGameStation));
			return true;
		}
	case GS_PLAY_GAME:	//��Ϸ��״̬
		{
			//if(pDesk->sUserData.m_bIsHu[bDeskStation])//����Ѿ�������
			//{
			//	
			//}
			//for(int i=0;i<4;++i)//����Ƿ��Ѿ�����
			//{
			//	if(pDesk->sUserData.m_bIsHu[i])//����Ѿ�������
			//	{
			//		GameStationData.m_bIsHu[i] = true;
			//		for(int j=0;j<17;j++)
			//		{
			//			GameStationData.m_byArHandPai[i][j] = pDesk->sUserData.m_byArHandPai[i][j];
			//		}
			//	}
			//}

			GameStationDataEx TGameStation;
			TGameStation.iVersion		= MAIN_VERSION;
			TGameStation.iVersion2		= ASS_VERSION;
			TGameStation.byGameStation	= GameStation;

			TGameStation.byBeginTime	= pDesk->sGameData.m_mjRule.byBeginTime;	//��ʼ�ȴ�ʱ��
			TGameStation.byOutTime		= pDesk->sGameData.m_mjRule.byOutTime;		//����ʱ��
			TGameStation.byBlockTime	= pDesk->sGameData.m_mjRule.byBlockTime;	//˼��ʱ��
			TGameStation.bForceTuoGuan	= pDesk->sGameData.m_mjRule.bForceTuoGuan;	//ǿ���Ƿ��й�

			for(vector<long int>::iterator it = pDesk->m_vlSuperUserID.begin(); it != pDesk->m_vlSuperUserID.end(); it++)
			{
				if(pDesk->m_pUserInfo[bDeskStation]->m_UserData.bLogoID == *it)
				{
					TGameStation.bIsSuperUser = true;
					break;
				}
			}
			TGameStation.byNtStation = pDesk->sGameData.m_byNtStation;
			TGameStation.m_byNowOutStation = pDesk->sUserData.m_byNowOutStation;
			//pDesk->sUserData.m_bTuoGuan[bDeskStation] = false;
			memcpy(TGameStation.m_bistuoguan,pDesk->sUserData.m_bTuoGuan,sizeof(TGameStation.m_bistuoguan));
			memcpy(TGameStation.m_byArHandPai,pDesk->sUserData.m_byArHandPai,sizeof(TGameStation.m_byArHandPai));
			memcpy(TGameStation.m_iArMenPai,pDesk->sUserData.m_iArMenPai,sizeof(TGameStation.m_iArMenPai));
			memcpy(TGameStation.m_byArOutPai,pDesk->sUserData.m_byArOutPai,sizeof(TGameStation.m_byArOutPai));
			memcpy(TGameStation.m_UserGCPData,pDesk->sUserData.m_UserGCPData,sizeof(TGameStation.m_UserGCPData));
			//memcpy(TGameStation.m_byHandPaiCount,pDesk->sUserData.m_byArHandPaiCount,sizeof(TGameStation.m_byHandPaiCount));
			//memcpy(TGameStation.m_byOutPaiCount,pDesk->sUserData.m_byArOutPaiCount,sizeof(TGameStation.m_byOutPaiCount));

			/*for(int i = 0; i < PLAY_COUNT; i++)
			{
			pDesk->sUserData.CopyHandPai(TGameStation.m_byArHandPai,i,true);
			pDesk->sUserData.CopyMenPai(i,TGameStation.m_iArMenPai[i]);
			}*/
			//��������
			//pDesk->SendGameStationEx(bDeskStation,  bWatchUser, &GameStationData, sizeof(GameStationData));
			//if(!pDesk->sUserData.m_bIsHu[bDeskStation] && pDesk->sGameData.T_CPGNotify[bDeskStation].m_byMaxThing > ACTION_NO && pDesk->sGameData.T_CPGNotify[bDeskStation].m_byMaxThing <= ACTION_HU)//����г����ܾͽ��д���
			//{
			//	pDesk->SendGameDataEx(bDeskStation, &pDesk->sGameData.T_CPGNotify[bDeskStation], sizeof(pDesk->sGameData.T_CPGNotify[bDeskStation]), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
			//}
			//��������
			pDesk->SendGameStationEx(bDeskStation,  bWatchUser, &TGameStation, sizeof(TGameStation));
			return true;
		}
	
	}
    return true;
}
/*---------------------------------------------------------------------------------------------------------*/
///���ղ�������Ϣ
int ThingHandleEx::HandleNotifyMessage(int bDeskStation, UINT code, BYTE* pData, int uSize, bool bWatchUser)
{
	pDesk->KillTimer(TIME_CHECK_GAME_MSG);
	pDesk->SetTimer(TIME_CHECK_GAME_MSG,TIME_DEFAULT_HANDLE);//�������ó�ʱ��ʱ��

	if(bWatchUser)
	{
		return 0;
	}

    switch (code)
    {
	case ASS_GM_AGREE_GAME:		//	�յ��û�ͬ����Ϸ��Ϣ	DWJ
		{
			return	ReceiveUserAgreeGame(bDeskStation, pData, uSize,	bWatchUser);
			break;
		}
	case ASS_AUTOCARD:		//�й�		DWJ
		{	
			return ReceiveUserTuoGuan(bDeskStation, pData, uSize, bWatchUser);
			break;
		}
	case ASS_CHANGE_PAI:	//������һ��� �ֻ����Կ���
		{
			return	ReceiveUserChangeCard(bDeskStation, pData, uSize, bWatchUser);
			break;
		}
	case THING_OUT_PAI:		//	�û�����	DWJ
		{
			return	ReceiveUserOutCard(bDeskStation, pData, uSize, bWatchUser);
			break;
		}
	case THING_GUO_PAI:		//	��������	DWJ
		{
			return	ReceiveUserPassCheck(bDeskStation, pData, uSize,	bWatchUser);
			break;
		}
	case THING_CHI_PAI:		//	����������   DWJ
		{
			return	ReceiveUserChiCard(bDeskStation, pData, uSize, bWatchUser);
			break;
		}
	case THING_PENG_PAI:	//	�û�����	DWJ
		{
			return	ReceiveUserPengCard(bDeskStation, pData, uSize, bWatchUser);
			break;
		}
	case THING_GANG_PAI:	//	�û�����	DWJ
		{
			return	ReceiveUserGangCard(bDeskStation, pData, uSize, bWatchUser);
			break;
		}
	case THING_TING_PAI:	// �û�����		DWJ
		{
			return	ReceiveUserTingCard(bDeskStation, pData, uSize, bWatchUser);
			break;
		}
	case THING_HU_PAI:		// �û�����		DWJ
		{
			return	ReceiveUserHuCard(bDeskStation, pData, uSize, bWatchUser);
			break;
		}
#ifdef MJ_CAN_SETPAI//�����ͻ���ץ�ƣ�����
    case ASS_SUPER://�����ͻ���,����������
        {
			ReceiveSuperChangePai(bDeskStation,pData, uSize, bWatchUser);
        }
        break;
#endif // MJ_CAN_SETPAI
    }
    return 0;
}





/*---------------------------------------------------------------------------------------------------------*/
/*--------------------���������Ϣ-------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/
//�յ��û�ͬ����Ϸ��Ϣ
int		ThingHandleEx::ReceiveUserAgreeGame(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	return ThingHandle::ReceiveUserAgreeGame(bDeskStation, pData, uSize, bWatchUser);
}
//�Ƿ񳬶��û�
bool	ThingHandleEx::IsSuperUser(BYTE bDeskStation)
{
	if (bDeskStation < 0 || bDeskStation >= PLAY_COUNT)
	{
		return false;
	}
	if (NULL == pDesk->m_pUserInfo[bDeskStation])
	{
		return false;
	}
	for(int i=0; i<pDesk->m_vlSuperUserID.size(); i++)
	{
		if (pDesk->m_pUserInfo[bDeskStation]->m_UserData.dwUserID == pDesk->m_vlSuperUserID.at(i))
		{
			return true;
		}
	}
	return false;
}
//��һ���
int		ThingHandleEx::ReceiveUserChangeCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	if(uSize != sizeof(tagSuperChangeCard))
	{
		return 0;
	}

	tagSuperChangeCard * pChangeCard = (tagSuperChangeCard *)pData;

	if(pChangeCard == NULL || bWatchUser || pDesk->m_byGameStation != GS_PLAY_GAME)
	{
		return 0;
	}

	if (pChangeCard->byChangeCard == 0 || pChangeCard->byChangeCard == 255 || pChangeCard->byNewCard == 0 || pChangeCard->byNewCard == 255)
	{
		return 0;
	}
	//�Ƿ񳬶����
	if (!IsSuperUser(bDeskStation) && 0)
	{
		return 0;
	}
	tagSuperChangeCard TSuperChangeCard;
	//��������������&&��ǽ���������Ʋ��ܻ���
	if (pDesk->sUserData.IsHaveAHandPai(bDeskStation,pChangeCard->byChangeCard) && pDesk->sUserData.IsHaveAMenPai(pChangeCard->byNewCard))
	{

		if (pDesk->sUserData.ChangeAHandPai(bDeskStation,pChangeCard->byChangeCard,pChangeCard->byNewCard) && pDesk->sUserData.ChangeAMenPai(pChangeCard->byNewCard,pChangeCard->byChangeCard))
		{

			TSuperChangeCard.bSuccese = true;
			TSuperChangeCard.byArHandPaiCount = pDesk->sUserData.CopyOneUserHandPai(TSuperChangeCard.byArHandPai,bDeskStation);
		}
	}

	pDesk->SendGameDataEx(bDeskStation, &TSuperChangeCard, sizeof(TSuperChangeCard), MDM_GM_GAME_NOTIFY, ASS_CHANGE_PAI_RESULT, 0);
	pDesk->SendWatchDataEx(bDeskStation, &TSuperChangeCard, sizeof(TSuperChangeCard), MDM_GM_GAME_NOTIFY, ASS_CHANGE_PAI_RESULT, 0);

	return 0;

}
/*---------------------------------------------------------------------------------------------------------*/
// �յ��й���Ϣ
int		ThingHandleEx::ReceiveUserTuoGuan(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	if(uSize != sizeof(UserTouGuan))
	{
		return 0;
	}
	if(bWatchUser || pDesk->m_byGameStation != GS_PLAY_GAME)//������Ϸ�в����й�
	{
		return 0;
	}

	UserTouGuan *pUserTouGuan = (UserTouGuan *)pData;
	if(pUserTouGuan == NULL)
	{
		return 0;
	}

	pDesk->sUserData.m_bTuoGuan[bDeskStation] = pUserTouGuan->bIsTuoGuan;

	pDesk->m_byAutoTime[bDeskStation] = 0;

	// ��������й�״̬
	UserTouGuan	TUserTouGuan;
	TUserTouGuan.byStation	= pUserTouGuan->byStation;
	TUserTouGuan.bIsTuoGuan	= pUserTouGuan->bIsTuoGuan;
	for(int i=0;i<PLAY_COUNT;++i)
	{
		TUserTouGuan.bTuoGuan[i] = pDesk->sUserData.m_bTuoGuan[i];		
	}
	for(int i=0;i<PLAY_COUNT;i++)
	{
		pDesk->SendGameDataEx(i,&TUserTouGuan,sizeof(TUserTouGuan),MDM_GM_GAME_NOTIFY,ASS_AUTOCARD,0);
		pDesk->SendWatchDataEx(i,&TUserTouGuan,sizeof(TUserTouGuan),MDM_GM_GAME_NOTIFY,ASS_AUTOCARD,0);
	}

	//�Ƿ����ڵȴ��Լ����¼� && ���й��¼�
	if(pDesk->sGameData.m_MjWait.byCurChair == pUserTouGuan->byStation && pUserTouGuan->bIsTuoGuan && pDesk->sGameData.m_MjWait.byTime > 1)
	{
		//1����������
		pDesk->sGameData.m_MjWait.byTime = 1;
	}
	return 0;
}
/*---------------------------------------------------------------------------------------------------------*/
// �û�����
int		ThingHandleEx::ReceiveUserOutCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	if(uSize != sizeof(tagOutPaiEx))
	{
		return 0;
	}
	tagOutPaiEx * pOutCard = (tagOutPaiEx *)pData;

	if(pOutCard == NULL || bWatchUser || pDesk->m_byGameStation != GS_PLAY_GAME)
	{
		DebugPrintf("�û� %d ����ʧ�ܣ�����Ϸ״̬", bDeskStation);
		return 0;
	}


	if(pOutCard->byPs == 255 || pDesk->sUserData.GetAHandPaiCount(pOutCard->byUser,pOutCard->byPs)==0 || !pDesk->sUserData.IsOutPaiPeople(pOutCard->byUser))
	{
		DebugPrintf("�û� %d ����ʧ�ܣ�û�д���", bDeskStation);
		return 0;
	}

	//��¼����ִ�е��¼�
	pDesk->sGameData.m_byThingDoing = THING_OUT_PAI;
//	pDesk->sGameData.T_HuPai.Clear();//������Ƽ�¼ 2012-08-15

	//��¼����״̬��ӵ�ڸܿ�
	for(int i=0;i<PLAY_COUNT;i++)
	{
		//if(i != bDeskStation)
		{
			pDesk->sGameData.m_bGangState[i] = false;
		}
	}


	//��¼�Ѿ���������(��������غ����)
	pDesk->sUserData.m_bHaveOut[bDeskStation] = true;

	//�������������
	pDesk->sGameData.T_OutPai.Clear();

	pDesk->sGameData.T_OutPai = * pOutCard;
	

	//ɾ��һ������
	pDesk->sUserData.DelleteAHandPai(pOutCard->byUser,pOutCard->byPs);
	//���һ�ų���
	pDesk->sUserData.AddToOutPai(pOutCard->byUser,pOutCard->byPs);
	//������ʱ����
	ClearTempData();

	pDesk->sUserData.m_byBuGangPai = 255;//���ܵ÷ֵ��ƣ����ڲ��ܣ�ץ���ܲ��ܵ��ƺ����ϲ��ܲ��з֣������޷֣�

	//��ҵ��������
	if(pDesk->sGameData.T_TingPai.bHitTing[pOutCard->byUser] && !pDesk->sUserData.m_bTing[pOutCard->byUser])//��ҵ��������,
	{
		if(CLogicBase::IsHaveInArr(pDesk->sGameData.T_CPGNotify[pOutCard->byUser].m_byTingCanOut,pOutCard->byPs,HAND_CARD_NUM))
		{
			pDesk->sUserData.m_bTing[pOutCard->byUser] = true;
			pDesk->sGameData.T_OutPai.bTing = true;
		}
		else
		{
			pDesk->sUserData.m_bTing[pOutCard->byUser] = false;
		}
		pDesk->sGameData.T_TingPai.bHitTing[pOutCard->byUser] = false;
	}

	//�����ƺ� �������ҵ�����
	pDesk->sUserData.SortHandPai(pOutCard->byUser,false);

	
	for(int i=0;i<PLAY_COUNT;i++)
	{
		//��������
		pDesk->sGameData.T_OutPai.m_byArOutPaiCount[i] = pDesk->sUserData.CopyOutPai(i,pDesk->sGameData.T_OutPai.m_byArOutPai[i]);
		//��ҵ���������
		pDesk->sGameData.T_OutPai.m_byArHandPaiCount[i] = pDesk->sUserData.GetHandPaiCount(i);
		//��ȡ����״̬
		pDesk->sGameData.T_OutPai.bUserTing[i] = pDesk->sUserData.m_bTing[i];
	}

	//���ͳ�������
	for (int i=0;i<PLAY_COUNT;i++)
	{	
		memset(pDesk->sGameData.T_OutPai.m_byArHandPai,255,sizeof(pDesk->sGameData.T_OutPai.m_byArHandPai));
		pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_OutPai.m_byArHandPai,pOutCard->byUser,true);
		//��������
		//pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_OutPai.m_byArHandPai,i,pDesk->sUserData.m_bTing[i]);

		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_OutPai, sizeof(pDesk->sGameData.T_OutPai), MDM_GM_GAME_NOTIFY, THING_OUT_PAI, 0);
		pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_OutPai, sizeof(pDesk->sGameData.T_OutPai), MDM_GM_GAME_NOTIFY, THING_OUT_PAI, 0);
	}

	//������⣺û��������Ӧ��ֱ��ץ��	
	if(!CheckAction(pOutCard->byUser,pOutCard->byPs,2))
	{
		//û�ж�����ֱ����һ�����ץ��
		pDesk->sGameData.ApplyThingID = THING_ZHUA_PAI;
		ApplyAction(THING_ZHUA_PAI,100);	//������һ���¼�
	}
	else
	{
		//���������Ƹ��ͻ������ 
		if(HaveUserCanDoAction(bDeskStation))
		{

		}
		else
		{
			//ô�з��������Ƹ��ͻ������ ������ץ���¼�
			//û�ж�����ֱ����һ�����ץ��
			pDesk->sGameData.ApplyThingID = THING_ZHUA_PAI;
			ApplyAction(THING_ZHUA_PAI,100);	//������һ���¼�
		}
	}
	return 0;
}
/*---------------------------------------------------------------------------------------------------------*/
// �յ��û�pass ��Ϣ
int		ThingHandleEx::ReceiveUserPassCheck(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser,bool bAuto)
{
	if(pDesk->sGameData.ApplyThingID != 255 || bWatchUser)//�������¼��Ͳ���Ӧpass��Ϣ
	{
		return 0;
	}
	//����Ѿ���Ӧ�˴˶��� 
	if (pDesk->sGameData.T_CPGNotify[bDeskStation].bHaveReturn || !pDesk->sGameData.T_CPGNotify[bDeskStation].bCanAction)
	{
		return 0;
	}

	//��ǰ���ǵȴ������
	if(pDesk->sGameData.m_MjWait.byCurChair != bDeskStation)
	{
		return 0;
	}

/*	if(bDeskStation != pDesk->sUserData.m_byNowOutStation && pDesk->sGameData.T_CPGNotify[bDeskStation].bHu)//����ܺ�û�к���,��¼����
	{
		pDesk->sUserData.m_bQiHu[bDeskStation] = true;//����Ƿ�����״̬
	}
*/

	pDesk->sGameData.T_CPGNotify[bDeskStation].bHaveReturn = true;//��¼��һ�Ӧ
	pDesk->sGameData.T_CPGNotify[bDeskStation].Clear();

	//���ع��ƽ����Ϣ���ͻ��� THING_GUO_PAI
	pDesk->SendGameDataEx(bDeskStation, MDM_GM_GAME_NOTIFY, THING_GUO_PAI, 0);
	pDesk->SendWatchDataEx(bDeskStation, MDM_GM_GAME_NOTIFY, THING_GUO_PAI, 0);

	//�������Ƿ��п������Ķ���
	if(HaveUserCanDoAction(bDeskStation))
	{
		OutputDebugString("dwjsdmj::�����˻��ж���");
	}
	else
	{
		//�Ƿ����˺�����
		bool	bHaveHuPai = false;
		for(int i=0; i<PLAY_COUNT; i++)
		{
			if(pDesk->sGameData.T_HuPai.bHaveHu[i])
			{
				bHaveHuPai = true;
				break;
			}
		}

		//���˺����� �ͽ���
		if(bHaveHuPai)
		{
			//��Ϸ����
			pDesk->sGameData.ApplyThingID	= THING_COUNT_FEN;
			ApplyAction(pDesk->sGameData.ApplyThingID,200);
		}
		else
		{
			//�Ƿ���ҳ���
			bool bIsOutPai = false;	
			for(int i=0;i<PLAY_COUNT;++i)
			{
				if(pDesk->sUserData.IsOutPaiPeople(i))//�������
				{
					bIsOutPai = true;
					break;
				}
			}

			//����ǳ��� ��ô�ͳ���
			if(bIsOutPai)
			{
				//�����ϵͳ���� ��ôֱ�Ӿͳ�����
				if(bAuto)
				{
					tagOutPaiEx outpai;
					outpai.Clear();
					outpai.byUser = bDeskStation;
					outpai.byPs = pDesk->sUserData.GetLastHandPai(bDeskStation);
					ReceiveUserOutCard(bDeskStation,&outpai,sizeof(outpai),false);
				}
				else
				{
					OutputDebugString("dwjsdmj::�������ȡ�����ƺ�Ҫ����--ʱ��Ϊ��ǰ��ʣ��ʱ��");
					//�������ȡ�����ƺ� ���������������Ƽ�ʱ�� ��Ϊǰ���������������� �������˼�ʱ������ �����Զ����Ƶ�
					if(pDesk->sGameData.m_MjWait.byTime > 1)
					{
						//�����Զ������������
						SetDoingThing(bDeskStation,THING_OUT_PAI,pDesk->sGameData.m_MjWait.byTime-1);
					}
					else
					{
						//�����Զ������������
						SetDoingThing(bDeskStation,THING_OUT_PAI,1);
					}
				}
			}
			else	//���ǳ��� ����ץ��
			{
				//��һ�����ץ��
				pDesk->sGameData.ApplyThingID = THING_ZHUA_PAI;
				ApplyAction(THING_ZHUA_PAI,100);	//������һ���¼�

			}
		}
	}
	
	return 0;
}
/*---------------------------------------------------------------------------------------------------------*/
// �û�������Ϣ
int		ThingHandleEx::ReceiveUserChiCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	if(bWatchUser || pDesk->m_byGameStation != GS_PLAY_GAME)
	{
		return 0;
	}
	if(uSize != sizeof(tagChiPaiEx))
	{
		return 0;
	}
	tagChiPaiEx * pChiCard = (tagChiPaiEx *)pData;
	if(pChiCard == NULL )
	{
		return 0;
	}
	if(!pDesk->sGameData.T_CPGNotify[bDeskStation].bChi)
	{
		return 0;
	}

	if(pChiCard->byPs == 255 )
	{
		return 0;
	}

	if(!pDesk->sGameData.m_mjAction.bChi)
	{
		return 0;
	}

	//�ж��Ƿ���������ҿ��������ܡ��� ʲô��
	bool bCanChi = true;
	for(int i=0; i<PLAY_COUNT;i++)
	{
		if(i == bDeskStation)
		{
			continue;
		}
		if(pDesk->sGameData.T_CPGNotify[i].bGang || pDesk->sGameData.T_CPGNotify[i].bPeng || pDesk->sGameData.T_CPGNotify[i].bHu)
		{
			bCanChi = false;
			break;
		}
	}

	if(!bCanChi)
	{
		return 0;
	}

	
	//��ʼ����������
	pDesk->sGameData.T_ChiPai.Clear();
	memcpy(&pDesk->sGameData.T_ChiPai, pChiCard, sizeof(pDesk->sGameData.T_ChiPai));

	BYTE NextID=255,StarTime=255;
	HandleChiPai(NextID,StarTime);
	return 0;
}
/*---------------------------------------------------------------------------------------------------------*/
// �û�������Ϣ
int		ThingHandleEx::ReceiveUserPengCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	if(uSize != sizeof(tagPengPaiEx))
	{
		return 0;
	}

	//�ж��Ƿ���������ҿ��Ժ� ʲô��
	bool bCanPeng = true;
	for(int i=0; i<PLAY_COUNT;i++)
	{
		if(i == bDeskStation)
		{
			continue;
		}
		if(pDesk->sGameData.T_CPGNotify[i].bHu)
		{
			bCanPeng = false;
			break;
		}
	}

	if(!bCanPeng)
	{
		return 0;
	}


	tagPengPaiEx * pPengPai = (tagPengPaiEx *)pData;
	if(pPengPai == NULL || bWatchUser ||  pDesk->m_byGameStation != GS_PLAY_GAME)
	{
		return 0;
	}

	if(!pDesk->sGameData.T_CPGNotify[bDeskStation].bPeng || pPengPai->byPs == 255 || !pDesk->sGameData.m_mjAction.bPeng)
	{
		return 0;
	}

	//��ʼ����������
	pDesk->sGameData.T_PengPai.Clear();
	memcpy(&pDesk->sGameData.T_PengPai, pPengPai, sizeof(pDesk->sGameData.T_PengPai));

	BYTE NextID=255,StarTime=255;
	HandlePengPai(NextID,StarTime);

	return 0;
}
/*---------------------------------------------------------------------------------------------------------*/
// �û�������Ϣ
int		ThingHandleEx::ReceiveUserGangCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	if(uSize != sizeof(tagGangPaiEx))
	{
		return 0;
	}

	//�ж��Ƿ���������ҿ��Ժ� ʲô��
	bool bCanGang = true;
	for(int i=0; i<PLAY_COUNT;i++)
	{
		if(i == bDeskStation)
		{
			continue;
		}
		if(pDesk->sGameData.T_CPGNotify[i].bHu)
		{
			bCanGang = false;
			break;
		}
	}

	if(!bCanGang)
	{
		return 0;
	}

	tagGangPaiEx * pGangPai = (tagGangPaiEx *)pData;
	if(pGangPai == NULL || bWatchUser ||  pDesk->m_byGameStation != GS_PLAY_GAME)
	{
		return 0;
	}

	if(!pDesk->sGameData.T_CPGNotify[bDeskStation].bGang || pGangPai->byPs == 255 || !pDesk->sGameData.m_mjAction.bGang)
	{
		return 0;
	}

	//��ʼ����������
	pDesk->sGameData.T_GangPai.Clear();
	memcpy(&pDesk->sGameData.T_GangPai, pGangPai, sizeof(pDesk->sGameData.T_GangPai));

	BYTE NextID=255,StarTime=255;
	HandleGangPai(NextID,StarTime);

	return 0;
}
/*---------------------------------------------------------------------------------------------------------*/
// �û�������Ϣ
int		ThingHandleEx::ReceiveUserTingCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	return 0;//û������
	if(uSize != sizeof(tagTingPaiEx))
	{
		return 0;
	}

	tagTingPaiEx * pTingCard = (tagTingPaiEx *)pData;
	if(pTingCard == NULL || bWatchUser ||  pDesk->m_byGameStation != GS_PLAY_GAME)
	{
		return 0;
	}
	
	if(!pDesk->sGameData.T_CPGNotify[bDeskStation].bTing || !pDesk->sGameData.m_mjAction.bTing)
	{
		return 0;
	}
	//����Ѿ������� 
	if(pDesk->sUserData.m_bTing[pTingCard->byUser])
	{
		return 0;
	}

	//�������ҵ�������������
//	pDesk->sGameData.T_CPGNotify[pTingCard->byUser].Clear();
	//��¼����Ѿ����������
	pDesk->sGameData.T_TingPai.bHitTing[pTingCard->byUser] = pTingCard->bTing;
	//֪ͨ�������
	for (int i=0;i<PLAY_COUNT;i++)
	{	
		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_TingPai, sizeof(pDesk->sGameData.T_TingPai), MDM_GM_GAME_NOTIFY, THING_TING_PAI, 0);
		pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_TingPai, sizeof(pDesk->sGameData.T_TingPai), MDM_GM_GAME_NOTIFY, THING_TING_PAI, 0);
	}
	return 0;
}
/*---------------------------------------------------------------------------------------------------------*/
// �û�������Ϣ
int		ThingHandleEx::ReceiveUserHuCard(BYTE bDeskStation, void * pData, UINT uSize, bool bWatchUser)
{
	if(uSize != sizeof(tagHuPaiEx))
	{
		return 0;
	}

	tagHuPaiEx * pHuCard = (tagHuPaiEx *)pData;
	if(pHuCard == NULL || bWatchUser || pDesk->m_byGameStation != GS_PLAY_GAME)
	{
		return 0;
	}

	if(!pDesk->sGameData.T_CPGNotify[bDeskStation].bHu)
	{
		return 0;
	}

	//���˵���
	if(pDesk->sGameData.m_byThingDoing == THING_OUT_PAI && pDesk->sGameData.T_OutPai.byUser != 255)
	{
		pDesk->sGameData.T_HuPai.byDianPao	= pDesk->sGameData.T_OutPai.byUser;
		pDesk->sGameData.T_HuPai.byPs		= pDesk->sGameData.T_OutPai.byPs;
	}
	else if(pDesk->sGameData.m_byThingDoing == THING_GANG_PAI && pDesk->sGameData.T_GangPai.byUser != 255)	//���ܺ�
	{
		pDesk->sGameData.T_HuPai.bQiangGang	= true;
		pDesk->sGameData.T_HuPai.byDianPao	= pDesk->sGameData.T_GangPai.byUser;
		pDesk->sGameData.T_HuPai.byPs		= pDesk->sGameData.T_GangPai.byPs;
	}
	else if((pDesk->sGameData.m_byThingDoing == THING_ZHUA_PAI) || (pDesk->sGameData.m_byThingDoing == THING_ONE_BUHUA) || (pDesk->sGameData.m_byThingDoing == THING_BEGIN_OUT))	//����
	{
		pDesk->sGameData.T_HuPai.bZimo = true;
		pDesk->sGameData.T_HuPai.byDianPao = 255;
		pDesk->sGameData.T_HuPai.byPs = 255;
		//�Ƿ�ܿ�
		if(pDesk->sGameData.m_bGangState[pHuCard->byUser])
		{
			pDesk->sGameData.T_HuPai.bGangKai = true;
			//�ܿ��� �Ÿ���ҵ���
			pDesk->sGameData.T_HuPai.byDianPao = pDesk->sGameData.T_OutPai.byUser;
		}
	}
	//������ҵ�λ��
	pDesk->sGameData.T_HuPai.byUser = pHuCard->byUser;

	//��ʶ����Һ�����
	pDesk->sGameData.T_HuPai.bHaveHu[pHuCard->byUser] = true;
	pDesk->sGameData.T_HuPai.byUserNum++;
	//��ȡ��������
	for(int i=0;i<MAX_HUPAI_TYPE;i++)
	{
		pDesk->sGameData.T_HuPai.byHuType[bDeskStation][i] = pDesk->sGameData.T_CPGNotify[bDeskStation].byHuType[i];
	}
	//������ҵ���������
	for(int j = 0; j < PLAY_COUNT; j++)
	{
		for(int i=0;i<HAND_CARD_NUM;i++)
		{
			pDesk->sGameData.T_HuPai.m_byArHandPai[j][i] = pDesk->sUserData.m_byArHandPai[j][i];
		}
		//������ҵ���������
		pDesk->sGameData.T_HuPai.m_byArHandPaiCount[j] = pDesk->sUserData.GetHandPaiCount(j);
	}

	//�������ҵ�������������
	pDesk->sGameData.T_CPGNotify[pHuCard->byUser].Clear();
	//��¼��һ�Ӧ
	pDesk->sGameData.T_CPGNotify[pHuCard->byUser].bHaveReturn = true;

	//֪ͨ��Һ�����
	for (int i=0;i<PLAY_COUNT;i++)
	{	
		/*memset(pDesk->sGameData.T_HuPai.m_byArHandPai,255,sizeof(pDesk->sGameData.T_HuPai.m_byArHandPai));
		pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_HuPai.m_byArHandPai,i,false);*/

		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_HuPai, sizeof(pDesk->sGameData.T_HuPai), MDM_GM_GAME_NOTIFY, THING_HU_PAI, 0);
		pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_HuPai, sizeof(pDesk->sGameData.T_HuPai), MDM_GM_GAME_NOTIFY, THING_HU_PAI, 0);
	}
	//���˺����� �Ͳ�������
	pDesk->sGameData.T_HuPai.bIsLiuJu = false;

	bool bHaveFourZhong(false);
	for(int i = 0; i < MAX_HUPAI_TYPE; i++)
	{
		if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == 255)
			break;
		else
		{
			if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == HUPAI_TYPE_FOUR_ZHONG)
			{
				bHaveFourZhong = true;
				break;
			}
		}
	}

	//��������� ֻ��һ���˺��� 
	if(pDesk->sGameData.T_HuPai.bZimo && !bHaveFourZhong)
	{
		ApplyAction(THING_MAI_MA,1*1000);
		
		//������һ���¼�
		//ApplyAction(THING_COUNT_FEN,2*100);
		//ApplyAction(THING_ZHONG_NIAO,1*1000);
	}
	else
	{
		bool bHaveSend = false;	//�Ƿ��Ѿ���������Ϣ
		BYTE byCanActionDesk = 255;	//���������������
		//����Ƿ���������ҿ��Ժ���
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(i == bDeskStation)
			{
				continue;
			}

			if(pDesk->sGameData.T_CPGNotify[i].bHu)
			{
				/*pDesk->SendGameDataEx(i, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
				pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);*/

				HaveAction action;
				action.byDeskStation = pDesk->sUserData.m_byNowOutStation;
				memcpy(action.T_CPGNotify,pDesk->sGameData.T_CPGNotify,sizeof(action.T_CPGNotify));
				for(int i = 0; i < PLAY_COUNT; i++)
				{
					pDesk->SendGameDataEx(i, &action, sizeof(HaveAction), MDM_GM_GAME_NOTIFY, THING_HAVE_ACTION, 0);
					pDesk->SendWatchDataEx(i, &action, sizeof(HaveAction), MDM_GM_GAME_NOTIFY, THING_HAVE_ACTION, 0);
				}

				bHaveSend = true;		
				byCanActionDesk = i;
				break;
			}
		}

		//���˻��к��� //û�˿��Ժ����� ��������¼�
		if(bHaveSend && byCanActionDesk != 255)
		{
			SetDoingThing(byCanActionDesk,THING_HU_PAI);
		}
		else	
		{
			ApplyAction(THING_COUNT_FEN,2*100);
		}
	}

	return 0;
}












































///���ƣ�CheckNextAction
///�����������һ������
void ThingHandleEx::CheckNextAction()
{
    bool bNotify = false;
    switch(pDesk->sGameData.m_byNextAction)//ȷ������һ������Ӧ�¼����ԣ������ܣ���
    {
    case ACTION_HU:					//������
        {
			//���������Ҫ��������е���ɾ��
			if(pDesk->sGameData.T_CPGNotify[pDesk->sGameData.T_HuPai.byUser].bQiangGang) //���ܺ�
			{
				pDesk->sUserData.DelleteAHandPai(pDesk->sGameData.T_CPGNotify[pDesk->sGameData.T_HuPai.byUser].m_byDianPao,pDesk->sGameData.T_CPGNotify[pDesk->sGameData.T_HuPai.byUser].m_iHuPs);
			}
           

 //           HandleHuPai(NextID,StarTime);
			///��ȡ������ҵķ�������
			BYTE st = pDesk->sGameData.T_HuPai.byUser;
            ///֪ͨ��Һ�����




         

          

        }
        break;
    }
}

BYTE ThingHandleEx::GetCanHuUser(BYTE outst,bool hu[],BYTE hust)//��ȡ�������ҽ������
{

	return ThingHandle::GetCanHuUser(outst,hu, hust);
}

///��ȡ�¼�λ��
BYTE ThingHandleEx::GetNextStation(BYTE station,bool shun)
{
    BYTE re = 255;
    if(shun)//˳ʱ��
    {
        int nLoop = 8;
        re = (station + 1)%PLAY_COUNT;
        while (nLoop-->0)
        {            
            if (!pDesk->sUserData.m_bIsHu[ re ])
            {
                break;
            }
            re = (re + 1)%PLAY_COUNT;
        }
    }
    else
    {
        int nLoop = 8;
        re = (station + 3)%PLAY_COUNT;
        while (nLoop-->0)
        {            
            if (!pDesk->sUserData.m_bIsHu[ re ])
            {
                break;
            }
            re = (re+3)%PLAY_COUNT;
        }
    }
    return re;
}

///������ʱ����
void ThingHandleEx::ClearTempData()
{
	ThingHandle::ClearTempData();
}


///////////�����¼�����///////////////////////////////////////////////////////////////////////////////////////
/*------------------------------------------------------------------------------------------------*/
//�����¼�,typeҪ������¼�	DWJ
void ThingHandleEx::ApplyAction(int type,int time)
{
	if(type >= THING_GAME_BEGIN && type <= THING_ENG_UN_NORMAL)
	{
		pDesk->sGameData.ApplyThingID = type;
	}

    if(pDesk->sGameData.ApplyThingID != 255)
    {
        pDesk->SetTimer(10,time);
    }
}
/*------------------------------------------------------------------------------*/
///���ƣ�GameCPU
///��������Ϸ�¼��������ģ�
void	ThingHandleEx::GameCPU(int type)
{
    BYTE nextID = 255,time = 255;    
    switch(type)
    {
    case THING_GAME_BEGIN:	//��Ϸ��ʼ�¼�	DWJ
        {
            ExecuteGameBegin(nextID,time);
			break;
        }
    case THING_DING_NT:		//�׾��Զ�Ϊׯ�¼�	DWJ
        {
            ExecuteDongNt(nextID,time);
			break;
        }
	case THING_2SEZI_NT:  //�׾�������ɫ�Ӷ�ׯ���¼�
		{
			ExecuteTwoSeziNt(nextID,time);
			break;
		}
	case THING_2SEZI_DIR:	//��2��ɫ���¼������Ʒ����¼�	DWJ
		{
			ExecuteTwoSeziDir(nextID,time);
			break;
		}
	case THING_2SEZI_GP: ////��2��ɫ���¼������Ƶ��¼�
		{
			ExecuteTwoSeziGp(nextID,time);
			break;
		}
	case THING_2SEZI_DIR_GP:	//��2��ɫ���¼������Ʒ�������Ƶ��¼�	DWJ
		{
			ExecuteTwoSeziDirGp(nextID,time);
			break;
		}
	case THING_SEND_PAI:	//�����¼�	DWJ
		{
			ExecuteSendPai(nextID,time);
			break;
		}
	case THING_ALL_BUHUA:	//Ϊ������Ҳ����¼�	DWJ
		{
			ExecuteAllBuHua(nextID,time);
			break;
		}
	case THING_BEGIN_OUT:	//ׯ�ҿ�ʼ����֪ͨ�¼�	DWJ
		{
			pDesk->SetParamaterStation(GS_PLAY_GAME);
			pDesk->m_byGameStation = GS_PLAY_GAME;
			ExecuteBeginOut(nextID,time);
			break;
		}
	case THING_ZHUA_PAI:	//���ץ��	DWJ
		{
			ExecuteZhuaPai(nextID,time);
			break;
		}
	case THING_ONE_BUHUA:	//�������ץ�����ƺ󲹻��¼�
		{
			ExecuteOneBuHua(nextID,time);
			break;
		}
	case THING_MAI_MA:	//����
		{
			ExecuteMaiMa(nextID,time);
			break;
		}
	case THING_ZHONG_NIAO:  //�����¼�
		{
			ExecuteZhongNiao(nextID,time);
			break;
		}
	case THING_COUNT_FEN:	//����¼�
		{
			ExecuteCountFen(nextID,time);
			break;
		}
	case THING_ENG_HANDLE://���������¼�
		{
			ExecuteNormalEnd(nextID,time);
			break;
		}
    case THING_ENG_UN_NORMAL://�쳣���������¼�
        {
            ExecuteUnNormalEnd(nextID,time);
			break;
        }
    }

    pDesk->sGameData.ApplyThingID = nextID;
    ApplyAction(nextID,time*100);//������һ���¼�

}
void ThingHandleEx::ExecuteMaiMa(BYTE &nextID,BYTE &time)
{
	pDesk->sGameData.m_byThingDoing = THING_MAI_MA;
	pDesk->sGameData.m_TMaiMa.clear();
	//���������������
	pDesk->sGameData.m_TMaiMa.byUser = pDesk->sGameData.T_HuPai.byUser;
	pDesk->sGameData.m_TMaiMa.byCount = pDesk->m_iMaiMaCount;
	
	//ֱ����ȷ��������
	BYTE byPaiData[300];
	memset(&byPaiData,255,sizeof(byPaiData));
	DisPatchCardForMaiMa(byPaiData);

	for(int i = 0; i < pDesk->sGameData.m_TMaiMa.byCount; i++)
	{
		BYTE byCard = 255;
		do 
		{
			int iRand = rand()%24;
			byCard = byPaiData[iRand];
			byPaiData[iRand] = 255;
		} while (255 == byCard);
		
		pDesk->sGameData.m_TMaiMa.byCard[i] = byCard;
		pDesk->sGameData.m_ZhongNiao.byCard[i] = byCard;
	}
	//����ǳ������ ����ȫ�к���
	if (IsSuperUser(pDesk->sGameData.m_TMaiMa.byUser))
	{
		for(int i = 0; i < pDesk->sGameData.m_TMaiMa.byCount; i++)
		{
			pDesk->sGameData.m_TMaiMa.byCard[i] = 35;
			pDesk->sGameData.m_ZhongNiao.byCard[i] = 35;
		}
	}

	//֪ͨ�ͻ���
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		pDesk->SendGameData(i,&pDesk->sGameData.m_TMaiMa,sizeof(pDesk->sGameData.m_TMaiMa),MDM_GM_GAME_NOTIFY,THING_MAI_MA,0);
	}
	//�趨����ʱ��
	SetDoingThing(pDesk->sGameData.m_TMaiMa.byUser,THING_MAI_MA,pDesk->sGameData.m_mjRule.byOutTime);

}
/*-------------------------------------------------------------------------------------------------*/
void ThingHandleEx::ExecuteZhongNiao(BYTE &nextID,BYTE &time)
{
	pDesk->sGameData.m_byThingDoing = THING_ZHONG_NIAO;

	nextID = THING_COUNT_FEN;
	time = 100;

	for(int i = 0; i < 4; i++)
	{

		BYTE card = pDesk->sGameData.m_ZhongNiao.byCard[i];

		if(card == 255)
		{
			break;
		}
		//2  3 ���в�����
		if((card % 10)== 2)
		{
			pDesk->sGameData.T_CountFen.iZhongFan[pDesk->sGameData.T_HuPai.byUser] += 6;
			for(int j = 0; j < PLAY_COUNT; j++)
			{
				if(j == pDesk->sGameData.T_HuPai.byUser)
				{
					continue;
				}

				pDesk->sGameData.T_CountFen.iZhongFan[j] -= 2;
			}
		}
		else if((card % 10)== 3)
		{
			pDesk->sGameData.T_CountFen.iZhongFan[pDesk->sGameData.T_HuPai.byUser] += 9;
			for(int j = 0; j < PLAY_COUNT; j++)
			{
				if(j == pDesk->sGameData.T_HuPai.byUser)
				{
					continue;
				}

				pDesk->sGameData.T_CountFen.iZhongFan[j] -= 3;
			}
		}
		else if((card)== 35)
		{
			pDesk->sGameData.T_CountFen.iZhongFan[pDesk->sGameData.T_HuPai.byUser] += 15;
			for(int j = 0; j < PLAY_COUNT; j++)
			{
				if(j == pDesk->sGameData.T_HuPai.byUser)
				{
					continue;
				}

				pDesk->sGameData.T_CountFen.iZhongFan[j] -= 5;
			}
		}
	}
	
	memcpy(pDesk->sGameData.m_ZhongNiao.byMenPai,pDesk->sUserData.m_iArMenPai,sizeof(pDesk->sGameData.m_ZhongNiao.byMenPai));
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		pDesk->SendGameData(i,&pDesk->sGameData.m_ZhongNiao,sizeof(pDesk->sGameData.m_ZhongNiao),MDM_GM_GAME_NOTIFY,THING_ZHONG_NIAO,0);
	}
}
///��ʼ�¼�����	DWJ
void	ThingHandleEx::ExecuteGameBegin(BYTE &nextID,BYTE &time)
{   
	//��ʼ����Ϸ���ݺ��������
    pDesk->sGameData.InitData();

    pDesk->sUserData.InitData();       

	memset(pDesk->m_byAutoTime,0,sizeof(pDesk->m_byAutoTime));

	pDesk->KillTimer(TIME_WAIT_MSG);
	pDesk->SetTimer(TIME_WAIT_MSG,1000);//�������ó�ʱ��ʱ��

	pDesk->sGameData.m_byThingDoing = THING_GAME_BEGIN;

	//ׯ�Ҳ����� ��ִ�����һ��ׯ���¼�
	if(pDesk->sGameData.m_byNtStation == 255)
	{
		pDesk->sGameData.m_iHuangZhuangCount = 0;
		//���һ�������ׯ
		srand(GetTickCount());
		//pDesk->sGameData.m_byNtStation = rand()%PLAY_COUNT;
		nextID = THING_2SEZI_NT;//THING_DING_NT;
	}
	else 
	{
		nextID = THING_2SEZI_DIR;//THING_2SEZI_DIR_GP;//��ɫ�ӷ���
	}
	time	= 1;

	// ׯ��λ��
    pDesk->sGameData.T_Begin.byNt = pDesk->sGameData.m_byNtStation;	
	pDesk->sGameData.T_Begin.iHuangZhuangCount = pDesk->sGameData.m_iHuangZhuangCount;	

	//��������
    memset(pDesk->sGameData.T_Begin.byMenPai,0,sizeof(pDesk->sGameData.T_Begin.byMenPai));

	//���Ϳ�ʼ��Ϣ
	for (int i=0;i<PLAY_COUNT;i++)
	{
		pDesk->SendGameDataEx(i,&pDesk->sGameData.T_Begin,sizeof(pDesk->sGameData.T_Begin),MDM_GM_GAME_NOTIFY,THING_GAME_BEGIN,0);
		pDesk->SendWatchDataEx(i,&pDesk->sGameData.T_Begin,sizeof(pDesk->sGameData.T_Begin),MDM_GM_GAME_NOTIFY,THING_GAME_BEGIN,0);
	}
//    pDesk->sUserData.m_MenPai.byGetPaiDir = pDesk->sGameDatam_byNtStation;
    ///��ǰץ�Ʒ���true ˳ʱ�룬false ��ʱ��
//    pDesk->sUserData.m_bZhuaPaiDir = true;

//    pDesk->sUserData.m_MenPai.byGetPaiDir = pDesk->sGameData.m_byNtStation;//���Ʒ���(ÿ�ֳ�ʼ��Ϊׯ����ǰ����)
}
/*-------------------------------------------------------------------------------------------------*/
///�Ե�һ�����Ϊׯ�ң��Զ�Ϊׯ�¼���	DWJ
void	ThingHandleEx::ExecuteDongNt(BYTE &nextID,BYTE &time)
{	
	pDesk->sGameData.m_byThingDoing = THING_DING_NT;

	//������һ���¼�Ϊ ��ɸ��ȷ�����Ʒ�λ�¼�
	nextID = THING_2SEZI_DIR;
	time = 2;	//200������

	//���������
	pDesk->sGameData.T_DongNt.Clear();
	// ׯ��λ��	
	pDesk->sGameData.T_DongNt.byNt = pDesk->sGameData.m_byNtStation;	
	pDesk->sGameData.T_DongNt.iHuangZhuangCount = pDesk->sGameData.m_iHuangZhuangCount;

	for (int i=0;i<PLAY_COUNT;i++)
	{
		pDesk->SendGameDataEx(i,&pDesk->sGameData.T_DongNt,sizeof(pDesk->sGameData.T_DongNt),MDM_GM_GAME_NOTIFY,THING_DING_NT,0);
		pDesk->SendWatchDataEx(i,&pDesk->sGameData.T_DongNt,sizeof(pDesk->sGameData.T_DongNt),MDM_GM_GAME_NOTIFY,THING_DING_NT,0);
	}
}
/*-------------------------------------------------------------------------------------------------*/
///��2��ɫ��ȷ�����Ʒ��򣨵�����ȷ������	DWJ
void	ThingHandleEx::ExecuteTwoSeziDir(BYTE &nextID,BYTE &time)	
{
	pDesk->sGameData.m_byThingDoing = THING_2SEZI_DIR;
	///��2��ɫ��ȷ������λ���¼�
	nextID	= THING_2SEZI_GP;//THING_2SEZI_DIR_GP;//׼��ִ�����ڶ���ɫ��ȷ�����ƶ����¼�
	time	= 30; //3000ms = 3S

	//���������
	pDesk->sGameData.T_TwoSeziDir.Clear();
	//��ɸ����ҵ�λ�� = ׯ�ҵ�λ��
	pDesk->sGameData.T_TwoSeziDir.byUser = pDesk->sGameData.m_byNtStation;
	pDesk->sGameData.T_TwoSeziDir.byNt = pDesk->sGameData.m_byNtStation;
	CLogicBase::GetSeziData(&pDesk->sGameData.T_TwoSeziDir.bySezi0,&pDesk->sGameData.T_TwoSeziDir.bySezi1);

	//��¼���Ƶķ���(1Ϊ�Լ� ��ʱ����)
	pDesk->sUserData.m_MenPai.byGetPaiDir = (pDesk->sGameData.T_TwoSeziDir.bySezi1 + pDesk->sGameData.T_TwoSeziDir.bySezi0 +  pDesk->sGameData.T_TwoSeziDir.byUser - 1)%4;
	//���Ʒ���
	pDesk->sGameData.T_TwoSeziDir.byDirection = pDesk->sUserData.m_MenPai.byGetPaiDir;

	//�����������ͻ���
	for (int i=0;i<PLAY_COUNT;i++)
	{
		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_TwoSeziDir, sizeof(pDesk->sGameData.T_TwoSeziDir), MDM_GM_GAME_NOTIFY, THING_2SEZI_DIR, 0);
	}
	pDesk->SendWatchDataEx(PLAY_COUNT, &pDesk->sGameData.T_TwoSeziDir, sizeof(pDesk->sGameData.T_TwoSeziDir), MDM_GM_GAME_NOTIFY, THING_2SEZI_DIR, 0);
	
}
/*-------------------------------------------------------------------------------------------------*/
///��2��ɫ��ȷ�����Ʒ�������ƶ����������ͷ���С�������DWJ
void	ThingHandleEx::ExecuteTwoSeziDirGp(BYTE &nextID,BYTE &time)	
{    
	pDesk->sGameData.m_byThingDoing = THING_2SEZI_DIR_GP;

	//���������
	pDesk->sGameData.T_TwoSeziDirAndGetPai.Clear();
	///��2��ɫ��ȷ������λ�ã������ͣ������Ƶ㣨��С�㣩�¼�	
	pDesk->sGameData.T_TwoSeziDirAndGetPai.byUser = pDesk->sGameData.m_byNtStation;
	//ҡ����
	CLogicBase::GetSeziData(&pDesk->sGameData.T_TwoSeziDirAndGetPai.bySezi0,&pDesk->sGameData.T_TwoSeziDirAndGetPai.bySezi1);

	//���Ƶ��� = (��һ��ɸ�ӵ��ܺ�+�������ɸ�ӵ�����)	DWJTEST (�ĸ�ɸ�ӵ���>18��ô��)
	pDesk->sUserData.m_MenPai.byGetPai = (pDesk->sGameData.T_TwoSeziDir.bySezi1 + pDesk->sGameData.T_TwoSeziDir.bySezi0 + pDesk->sGameData.T_TwoSeziDirAndGetPai.bySezi0 + pDesk->sGameData.T_TwoSeziDirAndGetPai.bySezi1)%18;
	
	//֪ͨ�ͻ���
	for (int i=0;i<PLAY_COUNT;i++)
	{
		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_TwoSeziDirAndGetPai, sizeof(pDesk->sGameData.T_TwoSeziDirAndGetPai), MDM_GM_GAME_NOTIFY, THING_2SEZI_DIR_GP, 0);
	}
	pDesk->SendWatchDataEx(PLAY_COUNT, &pDesk->sGameData.T_TwoSeziDirAndGetPai, sizeof(pDesk->sGameData.T_TwoSeziDirAndGetPai), MDM_GM_GAME_NOTIFY, THING_2SEZI_DIR_GP, 0);

	//��һ�������¼�
	nextID = THING_SEND_PAI;
	time = 30;

}
/*-------------------------------------------------------------------------------------------------*/
///����	
void	ThingHandleEx::ExecuteSendPai(BYTE &nextID,BYTE &time)	
{
	pDesk->sGameData.m_byThingDoing = THING_SEND_PAI;//��¼���ڷ������¼�

	///��ǰץ�Ʒ���true ˳ʱ�룬false ��ʱ��
	pDesk->sUserData.m_bZhuaPaiDir = true;    
	///�����¼�
	pDesk->sGameData.T_SendPai.byDo = 1;	 
	//������
	DisPatchCard();

	//�·�������ҵ�13����
	for(int i=0;i<PLAY_COUNT;i++)
	{
		for(int j=0;j<MAX_HAND_PAI-1;++j)
		{
			GetPai(i,true,1);
		}
	}

	//ׯ���ٶ�ץһ����
	BYTE card = 255;
	card=GetPai(pDesk->sGameData.m_byNtStation,true,1);

	//��������
	ChangeMenPai();
	pDesk->sGameData.T_SendPai.Clear();
	for(int i=0;i<PLAY_COUNT;++i)
	{
		pDesk->sUserData.CopyMenPai(i,pDesk->sGameData.T_SendPai.byMenPai[i]);//��������

		//���������
		if(i == pDesk->sGameData.m_byNtStation)
		{
			pDesk->sGameData.T_SendPai.m_byArHandPaiCount[i] = 14;
		}
		else
		{
			pDesk->sGameData.T_SendPai.m_byArHandPaiCount[i] = 13;
		}
	}

	//dwjtesttest
	/*pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][0] = 35;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][1] = 35;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][2] = 35;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][3] = 35;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][4] = 15;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][5] = 6;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][6] = 11;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][7] = 12;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][8] = 13;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][9] = 14;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][10] = 15;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][11] = 16;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][12] = 27;
	pDesk->sUserData.m_byArHandPai[pDesk->sGameData.m_byNtStation][13] = 28;

	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][0] = 1;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][1] = 2;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][2] = 3;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][3] = 4;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][4] = 15;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][5] = 6;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][6] = 11;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][7] = 12;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][8] = 13;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][9] = 14;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][10] = 15;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][11] = 16;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+1)%PLAY_COUNT][12] = 28;

	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][0] = 1;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][1] = 2;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][2] = 3;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][3] = 4;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][4] = 15;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][5] = 6;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][6] = 11;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][7] = 12;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][8] = 13;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][9] = 14;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][10] = 15;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][11] = 16;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+2)%PLAY_COUNT][12] = 28;

	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][0] = 1;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][1] = 2;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][2] = 3;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][3] = 4;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][4] = 15;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][5] = 6;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][6] = 11;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][7] = 12;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][8] = 13;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][9] = 14;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][10] = 15;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][11] = 16;
	pDesk->sUserData.m_byArHandPai[(pDesk->sGameData.m_byNtStation+3)%PLAY_COUNT][12] = 28;*/

	

	//���ĸ���ҷַ����е���
	for(int i=0;i<PLAY_COUNT;i++)
	{	
		//��������
		pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_SendPai.m_byArHandPai,i,false);
		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_SendPai, sizeof(pDesk->sGameData.T_SendPai), MDM_GM_GAME_NOTIFY, THING_SEND_PAI, 0);
		pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_SendPai, sizeof(pDesk->sGameData.T_SendPai), MDM_GM_GAME_NOTIFY, THING_SEND_PAI, 0);
	}


	//�����������Ƿ��л��� 
	bool bNeedBuHua = false;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(pDesk->sUserData.IsHuaPaiInHand(i))
		{
			bNeedBuHua = true;
			break;
		}
	}
	nextID = THING_BEGIN_OUT;//THING_ALL_BUHUA;
	time = 50;
}
/*-------------------------------------------------------------------------------------------------*/
///��ȡ������	DWJ
void	ThingHandleEx::DisPatchCard()
{
	//����
	BYTE index=0;
	memset(pDesk->sUserData.m_MenPai.byMenPai,255,sizeof(pDesk->sUserData.m_MenPai.byMenPai));//
	

	for(int i=0;i<4;i++)
	{
		if(pDesk->sGameData.m_mjRule.bHaveWan)//����
		{
			for(int j=0;j<9;j++)
			{
				pDesk->sUserData.m_MenPai.byMenPai[index] = stcArr_A_Mj_W[j];
				index++;
			}
		}
		if(pDesk->sGameData.m_mjRule.bHaveBing)//�б�
		{
			for(int j=0;j<9;j++)
			{
				pDesk->sUserData.m_MenPai.byMenPai[index] = stcArr_A_Mj_B[j];
				index++;
			}
		}
		if(pDesk->sGameData.m_mjRule.bHaveTiao)//����
		{
			for(int j=0;j<9;j++)
			{
				pDesk->sUserData.m_MenPai.byMenPai[index] = stcArr_A_Mj_T[j];
				index++;
			}
		}
		if(pDesk->sGameData.m_mjRule.bHaveBaiBan)//�аװ�
		{
			pDesk->sUserData.m_MenPai.byMenPai[index] = stcArr_A_Mj_ZI[2];
			index++;
		}
		if(pDesk->sGameData.m_mjRule.bHaveFaCai)//�з���
		{
			pDesk->sUserData.m_MenPai.byMenPai[index] = stcArr_A_Mj_ZI[1];
			index++;
		}
		if(pDesk->sGameData.m_mjRule.bHaveHongZhong)//�к���
		{
			pDesk->sUserData.m_MenPai.byMenPai[index] = stcArr_A_Mj_ZI[0];
			index++;
		}
		if(pDesk->sGameData.m_mjRule.bHaveFengPai)//�ж�������
		{
			for(int j=0;j<4;j++)
			{
				pDesk->sUserData.m_MenPai.byMenPai[index] = stcArr_A_Mj_FENG[j];
				index++;
			}
		}
	}

	if(pDesk->sGameData.m_mjRule.bHaveFlower)//�л���
	{
		for(int i=0;i<8;i++)
		{
			pDesk->sUserData.m_MenPai.byMenPai[index] = stcArr_A_Mj_Other[i];
			index++;
		}
	}


	///������
	srand(GetTickCount());
	BYTE temp=255,data=0;
	for(int i=0;i<index;i++)
	{
		temp = rand()%(index-i)+i;
		data = pDesk->sUserData.m_MenPai.byMenPai[temp];
		pDesk->sUserData.m_MenPai.byMenPai[temp] = pDesk->sUserData.m_MenPai.byMenPai[i];
		pDesk->sUserData.m_MenPai.byMenPai[i] = data;
	}


	///////////////	DWJ???
//	pDesk->sUserData.m_MenPai.byGetPai = pDesk->sUserData.m_MenPai.byGetPai%13;
	//pDesk->sUserData.m_MenPai.byGetPaiDir = pDesk->sUserData.m_MenPai.byGetPaiDir%4;
	//if(pDesk->sUserData.m_MenPai.byGetPaiDir == 255)
	//{
	//	pDesk->sUserData.m_MenPai.byGetPaiDir = pDesk->sGameData.m_byNtStation;
	//}
	//pDesk->sUserData.m_MenPai.byGetPaiDir = pDesk->sUserData.m_MenPai.byGetPaiDir%4;

	int dun = 0,num=0;
	dun = index/8;
	num = (dun*pDesk->sUserData.m_MenPai.byGetPaiDir + pDesk->sUserData.m_MenPai.byGetPai)*2;
	if(num == 0)
	{
		num=2;
	}
	pDesk->sUserData.m_MenPai.byStartIndex = num;//������ʼ����
	pDesk->sUserData.m_MenPai.byEndIndex = num-1;	//���ƽ�������
	pDesk->sUserData.m_MenPai.byAllPaiNum = index;//�����Ƶ�����
}
/*----------------------------------------------------------------------------------------------------*/
//Ϊ����������ҵ���
void	ThingHandleEx::DisPatchCardForMaiMa(BYTE byPai[300])
{
	//����
	BYTE	index=0;
	BYTE    byMenPai[300];
	memset(byMenPai,255,sizeof(byMenPai));//

	for(int i=0;i<4;i++)
	{
		if(pDesk->sGameData.m_mjRule.bHaveWan)//����
		{
			for(int j=0;j<9;j++)
			{
				byMenPai[index] = stcArr_A_Mj_W[j];
				index++;
			}
		}
		if(pDesk->sGameData.m_mjRule.bHaveBing)//�б�
		{
			for(int j=0;j<9;j++)
			{
				byMenPai[index] = stcArr_A_Mj_B[j];
				index++;
			}
		}
		if(pDesk->sGameData.m_mjRule.bHaveTiao)//����
		{
			for(int j=0;j<9;j++)
			{
				byMenPai[index] = stcArr_A_Mj_T[j];
				index++;
			}
		}
		if(pDesk->sGameData.m_mjRule.bHaveBaiBan)//�аװ�
		{
			byMenPai[index] = stcArr_A_Mj_ZI[2];
			index++;
		}
		if(pDesk->sGameData.m_mjRule.bHaveFaCai)//�з���
		{
			byMenPai[index] = stcArr_A_Mj_ZI[1];
			index++;
		}
		if(pDesk->sGameData.m_mjRule.bHaveHongZhong)//�к���
		{
			byMenPai[index] = stcArr_A_Mj_ZI[0];
			index++;
		}
		if(pDesk->sGameData.m_mjRule.bHaveFengPai)//�ж�������
		{
			for(int j=0;j<4;j++)
			{
				byMenPai[index] = stcArr_A_Mj_FENG[j];
				index++;
			}
		}
	}

	if(pDesk->sGameData.m_mjRule.bHaveFlower)//�л���
	{
		for(int i=0;i<8;i++)
		{
			byMenPai[index] = stcArr_A_Mj_Other[i];
			index++;
		}
	}


	///������
	srand(GetTickCount());
	BYTE temp=255,data=0;
	for(int i=0;i<index;i++)
	{
		temp = rand()%(index-i)+i;
		data = byMenPai[temp];
		byMenPai[temp] = byMenPai[i];
		byMenPai[i] = data;

		byPai[i] = byMenPai[i];
	}

	//���ٳ��ֺ�2��3�����еĸ���
	int iTmp23zhongCount = 0;
FIND23ZHONG:
	iTmp23zhongCount = 0;
	for(int i=0; i<24; i++)
	{
		if ((byPai[i] % 10 == 2) || (byPai[i] % 10 == 3) || (byPai[i] == 35))
		{
			iTmp23zhongCount++;
		}
	}

	if (iTmp23zhongCount < pDesk->m_i23ZhongCount)
	{
		//��ȥ����һ��
		for(int i=0; i<24; i++)
		{
			if ((byPai[i] % 10 != 2) && (byPai[i] % 10 != 3) && (byPai[i] != 35))
			{
				byPai[i] = 255;
				break;
			}
		}
		//��������� 2��3�����е����滻
		for(int i=0; i<24; i++)
		{
			if (255 == byPai[i])
			{
				for(int j=24; j<300; j++)
				{
					if ( 255 == byPai[j] )
					{
						continue;
					}
					if ((byPai[j] % 10 == 2) || (byPai[j] % 10 == 3) || (byPai[j] == 35))
					{
						byPai[i] = byPai[j];
						byPai[j] = 255;
						break;
					}
				}
			}
		}
		goto FIND23ZHONG;
	}

	
}
/*----------------------------------------------------------------------------------------------------*/
///������ץһ����
///@param station ץ����ҵ�λ�� , head  �Ƿ��ǰ��ץ,type ץ������ 0 ����ץ�ƣ�1��ʼǰ���ƣ�2��ǰ����ץ�ƣ�3����
BYTE ThingHandleEx::GetPai(int station,bool head,BYTE type,BYTE *pIndex)
{
	return ThingHandle::GetPai(station, head, type,pIndex);
}
/*----------------------------------------------------------------------------------------------------*/
///������ת����������,�������������е�����ת����ÿ����ҵ�����������
void ThingHandleEx::ChangeMenPai()
{
	for(int i=0;i<MEN_CARD_NUM;++i)//0λ�õ�����
	{
		if(pDesk->sUserData.m_MenPai.byMenPai[i] != 255)
		{
			pDesk->sUserData.m_iArMenPai[0][i] = 0;
		}
		else
		{
			pDesk->sUserData.m_iArMenPai[0][i] = 255;
		}
	}
	for(int i=MEN_CARD_NUM;i<(2*MEN_CARD_NUM);++i)//1λ�õ�����
	{
		if(pDesk->sUserData.m_MenPai.byMenPai[i] != 255)
		{
			pDesk->sUserData.m_iArMenPai[1][i-MEN_CARD_NUM] = 0;
		}
		else
		{
			pDesk->sUserData.m_iArMenPai[1][i-MEN_CARD_NUM] = 255;
		}
	}
	for(int i=(2*MEN_CARD_NUM);i<(3*MEN_CARD_NUM);++i)//2λ�õ�����
	{
		if(pDesk->sUserData.m_MenPai.byMenPai[i] != 255)
		{
			pDesk->sUserData.m_iArMenPai[2][i-(2*MEN_CARD_NUM)] = 0;
		}
		else
		{
			pDesk->sUserData.m_iArMenPai[2][i-(2*MEN_CARD_NUM)] = 255;
		}
	}
	for(int i=(3*MEN_CARD_NUM);i<(4*MEN_CARD_NUM);++i)//3λ�õ�����
	{
		if(pDesk->sUserData.m_MenPai.byMenPai[i] != 255)
		{
			pDesk->sUserData.m_iArMenPai[3][i-(3*MEN_CARD_NUM)] = 0;
		}
		else
		{
			pDesk->sUserData.m_iArMenPai[3][i-(3*MEN_CARD_NUM)] = 255;
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/
///������Ҳ����¼�	DWJ
void	ThingHandleEx::ExecuteAllBuHua(BYTE &nextID,BYTE &time)	
{
	pDesk->sGameData.m_byThingDoing = THING_ALL_BUHUA;

	//����ղ�������
	pDesk->sGameData.T_AllBuHua.Clear();
	
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(!pDesk->sUserData.IsHuaPaiInHand(i))
		{
			continue;
		}
		while(pDesk->sUserData.IsHuaPaiInHand(i))
		{
			int num = pDesk->sUserData.MoveHuaPaiFormHand(i);
			for(int j=0;j<num;j++)
			{
				GetPai(i,false,1);//��ʼǰ����ץ��
			}
		}

		if(i == pDesk->sGameData.m_byNtStation)
		{
			pDesk->sGameData.T_AllBuHua.m_byArHandPaiCount[i] = 14;
		}
		else
		{
			pDesk->sGameData.T_AllBuHua.m_byArHandPaiCount[i] = 13;
		}
	}

	//ȡ���ƺ� ��ʱת������
	ChangeMenPai();
	//��������
	for(int i=0;i<PLAY_COUNT;i++)
	{
		pDesk->sUserData.CopyHuaPai(i,pDesk->sGameData.T_AllBuHua.m_byArHuaPai[i]);
	}


	///����������
	for (int i=0;i<PLAY_COUNT;i++)
	{		
		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_AllBuHua, sizeof(pDesk->sGameData.T_AllBuHua), MDM_GM_GAME_NOTIFY, THING_ALL_BUHUA, 0);
		pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_AllBuHua, sizeof(pDesk->sGameData.T_AllBuHua), MDM_GM_GAME_NOTIFY, THING_ALL_BUHUA, 0);
	}
	///��һ��ʱ����� ��ʼ�����¼�
	nextID = THING_BEGIN_OUT;
	time = 10;
	//ApplyAction(nextID,1500);
}
/*----------------------------------------------------------------------------------------------------*/
///��ʼ����֪ͨ�¼�	DWJ
void	ThingHandleEx::ExecuteBeginOut(BYTE &nextID,BYTE &time)
{    
	pDesk->sGameData.m_byThingDoing = THING_BEGIN_OUT;//��¼���ڷ������¼�


	///��ʼ����֪ͨ�¼� 
	pDesk->sGameData.T_BeginOutPai.byNt		= pDesk->sGameData.m_byNtStation;//ׯ��λ��
	pDesk->sGameData.T_BeginOutPai.byUser	= pDesk->sGameData.m_byNtStation;//ׯ��λ��
	pDesk->sUserData.m_byNowOutStation		= pDesk->sGameData.m_byNtStation;//����λ��

	//ת������
	ChangeMenPai();
	//��������
	for(int i=0;i<PLAY_COUNT;i++)
	{
		pDesk->sUserData.CopyMenPai(i,pDesk->sGameData.T_BeginOutPai.byMenPai[i]);//��������
		if(i == pDesk->sGameData.m_byNtStation)
		{
			pDesk->sGameData.T_BeginOutPai.m_byArHandPaiCount[i] = 14;
		}
		else
		{
			pDesk->sGameData.T_BeginOutPai.m_byArHandPaiCount[i] = 13;
		}
		//����������ҵ�����
		pDesk->sUserData.SortHandPai(i,false);
	}

	
	//����֪ͨ
	for (int i=0;i<PLAY_COUNT;i++)
	{	
		memset(pDesk->sGameData.T_BeginOutPai.m_byArHandPai,255,sizeof(pDesk->sGameData.T_BeginOutPai.m_byArHandPai));
		pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_BeginOutPai.m_byArHandPai,i,false);
		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_BeginOutPai, sizeof(pDesk->sGameData.T_BeginOutPai), MDM_GM_GAME_NOTIFY, THING_BEGIN_OUT, 0);
	}
	pDesk->SendWatchDataEx(PLAY_COUNT, &pDesk->sGameData.T_BeginOutPai, sizeof(pDesk->sGameData.T_BeginOutPai), MDM_GM_GAME_NOTIFY, THING_BEGIN_OUT, 0);


	//��⶯��
	if(CheckAction(pDesk->sGameData.m_byNtStation,pDesk->sUserData.GetLastHandPai(pDesk->sUserData.m_byNowOutStation),0))
	{
		//�����ץ�ƺ��ж���(�ͻ���������ʾ����ʱʱ���)
		pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].bZhuaPai	 = true;
		//pDesk->SendGameDataEx(pDesk->sGameData.m_byNtStation, &pDesk->sGameData.T_CPGNotify ,sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
		//pDesk->SendWatchDataEx(pDesk->sGameData.m_byNtStation, &pDesk->sGameData.T_CPGNotify ,sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);

		HaveAction action;
		action.byDeskStation = pDesk->sUserData.m_byNowOutStation;
		memcpy(action.T_CPGNotify,pDesk->sGameData.T_CPGNotify,sizeof(action.T_CPGNotify));
		for(int i = 0; i < PLAY_COUNT; i++)
		{
			pDesk->SendGameDataEx(i, &action, sizeof(HaveAction), MDM_GM_GAME_NOTIFY, THING_HAVE_ACTION, 0);
			pDesk->SendWatchDataEx(i, &action, sizeof(HaveAction), MDM_GM_GAME_NOTIFY, THING_HAVE_ACTION, 0);
		}
		//����к��ƶ�����ô�����������Զ������ʱ��
		if(pDesk->sGameData.T_CPGNotify[pDesk->sGameData.m_byNtStation].bHu)
		{
			//û�ж��� ����ִ���¼� �Զ�������ʱ�����ʱ��
			SetDoingThing(pDesk->sGameData.m_byNtStation,THING_HU_PAI,pDesk->sGameData.m_mjRule.byOutTime);
		}
		else
		{
			//û�ж��� ����ִ���¼� �Զ�������ʱ�����ʱ��
			SetDoingThing(pDesk->sGameData.m_byNtStation,THING_BEGIN_OUT);
		}
	}
	else
	{
		//û�ж��� ����ִ���¼� �Զ�������ʱ�����ʱ��
		SetDoingThing(pDesk->sGameData.m_byNtStation,THING_BEGIN_OUT);
	}
}

/*--------------------------------------------------------------------------------------------------*/
///������station��ǰ������Щ����	DWJ
///������station ���λ��,pai �����Ҵ�����ƣ�type ���� ���� 0 ץ�ƣ�1�ܿ���2 ���ڣ�3���� 4���ƺ���
bool ThingHandleEx::CheckAction(int station,BYTE pai,BYTE type)
{
	for(int i=0;i<PLAY_COUNT;++i)
	{
		pDesk->sGameData.T_CPGNotify[i].Clear();//����
	}

	bool action = false;
	switch(type)
	{
	case 0://����
	case 1://�ܿ�
		{
			//��
			if(pDesk->sCheckHuPai.CanHu(station,pai,pDesk->sGameData.T_CPGNotify[station].byHuType,true))
			{
				pDesk->sGameData.T_CPGNotify[station].bHu = true;
				pDesk->sGameData.T_CPGNotify[station].m_iHuPs = pDesk->sUserData.GetLastHandPai(station);
				pDesk->sGameData.T_CPGNotify[station].m_byDianPao = station;
			}
			//��(�����˾Ͳ��ܸ���)
			if(pai != 35)
			{
				if(!pDesk->sUserData.m_bTing[station] && pDesk->sCPGAction.CanGang(station,pai,pDesk->sGameData.T_CPGNotify[station].m_iGangData ,true))
				{
					pDesk->sGameData.T_CPGNotify[station].bGang = true;
				}
			}
			////��
			//if(!pDesk->sUserData.m_bTing[station] && CanTingDWJ(station))
			//{
			//	pDesk->sGameData.T_CPGNotify[station].bTing = true;
			//}
			if(pDesk->sGameData.T_CPGNotify[station].bHu || pDesk->sGameData.T_CPGNotify[station].bGang || pDesk->sGameData.T_CPGNotify[station].bTing)
			{
				action=true;
				pDesk->sGameData.T_CPGNotify[station].bCanAction = true;
			}
		}
		break;
	case 2://����
		{
			for(int i=0;i<PLAY_COUNT;++i)
			{
				if (pDesk->sUserData.m_bIsHu[i] || pDesk->sUserData.m_bFoceLeave[i])        // �Ѻ������,���߶���״̬
				{
					continue;
				}

				if(i==station)
				{
					continue;
				}
				//����¼��Ƿ��ܳ�
				//��
				/*if(!pDesk->sUserData.m_bTing[i] && GetNextStation(station)==i)
				{
				if(pDesk->sCPGAction.CanChi(i,pai,pDesk->sGameData.T_CPGNotify[i].m_iChi))
				{					
				pDesk->sGameData.T_CPGNotify[i].bChi = true;
				pDesk->sGameData.T_CPGNotify[i].m_byMaxThing = ACTION_CHI;
				}
				}*/
				//��
				if(!pDesk->sUserData.m_bTing[i] && pDesk->sCPGAction.CanPeng(i,pai))
				{	
					pDesk->sGameData.T_CPGNotify[i].bPeng = true;
					pDesk->sGameData.T_CPGNotify[i].m_byMaxThing = ACTION_PENG;
				}
				//��
				if(!pDesk->sUserData.m_bTing[i] && pDesk->sCPGAction.CanGang(i,pai,pDesk->sGameData.T_CPGNotify[i].m_iGangData,false))
				{
					pDesk->sGameData.T_CPGNotify[i].bGang = true;
					pDesk->sGameData.T_CPGNotify[i].m_byMaxThing = ACTION_MING_GANG;

				}
				//��
				//������� ���ܺ�   ��������� ֻ������
				/*if(!pDesk->sUserData.m_bQiHu[i] && !pDesk->sUserData.m_bTing[i] &&pDesk->sCheckHuPai.CanHu(i,pai,pDesk->sGameData.T_CPGNotify[i].byHuType,false))
				{
					pDesk->sGameData.T_CPGNotify[i].bHu = true;
					pDesk->sGameData.T_CPGNotify[i].m_byMaxThing = ACTION_HU;
					pDesk->sGameData.T_CPGNotify[i].m_iHuPs = pai;
					pDesk->sGameData.T_CPGNotify[i].m_byDianPao = station;
				}*/
				if(pDesk->sGameData.T_CPGNotify[i].bChi || pDesk->sGameData.T_CPGNotify[i].bHu || pDesk->sGameData.T_CPGNotify[i].bPeng || pDesk->sGameData.T_CPGNotify[i].bGang || pDesk->sGameData.T_CPGNotify[i].bTing)
				{
					action=true;
					pDesk->sGameData.T_CPGNotify[i].bCanAction = true;
				}
			}
		}
		break;
	case 3://����
		{
			for(int i=0;i<PLAY_COUNT;++i)
			{
				if (pDesk->sUserData.m_bIsHu[i]|| pDesk->sUserData.m_bFoceLeave[i])        // �Ѻ������,���߶���״̬
				{
					continue;
				}
				if(i==station)
				{
					continue;
				}
				if(pDesk->sUserData.m_bQiHu[i] )
				{
					OutputDebugString("���ܼ�⣺�������������ܺ�");
				}
				//�� ������ �������ܺ�
				if(!pDesk->sUserData.m_bQiHu[i] && !pDesk->sUserData.m_bTing[i] && pDesk->sCheckHuPai.CanHu(i,pai,pDesk->sGameData.T_CPGNotify[i].byHuType,false))
				{
					//������������
					pDesk->sCheckHuPai.SetAHupaiType(HUPAI_TYPE_QIANG_GANG, pDesk->sGameData.T_CPGNotify[i].byHuType);
		
					pDesk->sGameData.T_CPGNotify[i].bHu = true;
					pDesk->sGameData.T_CPGNotify[i].m_iHuPs = pai;//��¼������
					pDesk->sGameData.T_CPGNotify[i].m_byDianPao = station;		
					pDesk->sGameData.T_CPGNotify[i].bQiangGang = true; //���ܺ�

					pDesk->sGameData.T_CPGNotify[i].m_byMaxThing = ACTION_HU;
					action=true;
					pDesk->sGameData.T_CPGNotify[i].bCanAction = true;
				}
			}
		}
		break;
	case 4:	//���ƺ���(���ƺ�ֻ�����͸� ���ܺ�)
		{
			//��(�����˾Ͳ��ܸ���)
			if(!pDesk->sUserData.m_bTing[station] && pDesk->sCPGAction.CanGang(station,pai,pDesk->sGameData.T_CPGNotify[station].m_iGangData ,true))
			{
				pDesk->sGameData.T_CPGNotify[station].bGang = true;
			}
			if(!pDesk->sUserData.m_bTing[station])
			{
				if(CanTingDWJ(station))
				{
					pDesk->sGameData.T_CPGNotify[station].bTing = true;
				}
			}
			
			if(pDesk->sGameData.T_CPGNotify[station].bGang || pDesk->sGameData.T_CPGNotify[station].bTing)
			{
				action=true;
				pDesk->sGameData.T_CPGNotify[station].bCanAction = true;
			}
			break;
		}
	}
	if(action)
	{
		return true;
	}
	return false;
}
/*--------------------------------------------------------------------------------------------------*/
//����ܷ�����	DWJ
bool	ThingHandleEx::CanTingDWJ(BYTE byDeskStation)
{
	if(byDeskStation < 0 || byDeskStation >= PLAY_COUNT)
	{
		return false;
	}
	//�ȿ�����������
	BYTE	byUserHandCards[HAND_CARD_NUM] = {0};
	memcpy(byUserHandCards, &pDesk->sUserData.m_byArHandPai[byDeskStation],sizeof(byUserHandCards));

	int iCardCount = pDesk->sUserData.GetHandPaiCount(byDeskStation);

	BYTE	byTingCanOut[HAND_CARD_NUM];//�����ܴ����
	memset(byTingCanOut,255,sizeof(byTingCanOut));

	BYTE	byDelValue = 255;
	for(int i = 0; i< iCardCount; i++)
	{
		memcpy(&pDesk->sUserData.m_byArHandPai[byDeskStation], byUserHandCards, sizeof(pDesk->sUserData.m_byArHandPai[byDeskStation]));
		//��ȥ��һ���� �ж��ܷ����� 
		byDelValue	= pDesk->sUserData.m_byArHandPai[byDeskStation][i];
		pDesk->sUserData.m_byArHandPai[byDeskStation][i] = 255;
		//����ҵ�������
		pDesk->sUserData.SortHandPai(i,false);

		//�����ж��ܷ�����
		if(pDesk->sGameData.m_mjAction.bTing && !pDesk->sUserData.m_bTing[byDeskStation])//���û������
		{
			//pDesk->sGameData.T_CPGNotify[byDeskStation].bTing = pDesk->sCheckHuPai.CanTing(byDeskStation,pDesk->sGameData.T_CPGNotify[byDeskStation].m_byTingCanOut);
			if(pDesk->sCheckHuPai.CanTing(byDeskStation,pDesk->sGameData.T_CPGNotify[byDeskStation].m_byTingCanOut))
			{
				for(int x=0; x<HAND_CARD_NUM;x++)
				{
					if(byTingCanOut[x] == 255)
					{
						byTingCanOut[x] = byDelValue;
						break;
					}
				}
				//break;
			}
			else
			{

			}
		}
	}

	//���������ܴ���Ƶ�����
	memcpy(pDesk->sGameData.T_CPGNotify[byDeskStation].m_byTingCanOut, byTingCanOut, sizeof(pDesk->sGameData.T_CPGNotify[byDeskStation].m_byTingCanOut));
	//���ԭ��ҵ�ԭʼ������
	memcpy(&pDesk->sUserData.m_byArHandPai[byDeskStation], byUserHandCards, sizeof(pDesk->sUserData.m_byArHandPai[byDeskStation]));

	//�ж��ܷ�����
	for(int i=0; i<HAND_CARD_NUM;i++)
	{
		if(pDesk->sGameData.T_CPGNotify[byDeskStation].m_byTingCanOut[i] != 255)
		{
			pDesk->sGameData.T_CPGNotify[byDeskStation].bCanAction = true;
			pDesk->sGameData.T_CPGNotify[byDeskStation].bTing = true;
			return true;
		}
	}
	return false;
}
/*--------------------------------------------------------------------------------------------------*/
///����������Ϣ�����	DWJ
bool	ThingHandleEx::HaveUserCanDoAction(BYTE byIgnoreStation)
{
	bool bHaveSend = false;	//�Ƿ��Ѿ���������Ϣ
	BYTE byCanActionDesk = 255;	//���������������
	if(!bHaveSend)
	{
		//�������ȼ��������ȷ�
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(i == byIgnoreStation || pDesk->sUserData.m_bFoceLeave[i])
			{
				continue;
			}

			//���ȸ��ܺ��Ƶ���ҷ��Ͷ�����Ϣ 
			if(pDesk->sGameData.T_CPGNotify[i].bHu)
			{
				/*pDesk->SendGameDataEx(i, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
				pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);*/
				HaveAction action;
				action.byDeskStation = byIgnoreStation;
				memcpy(action.T_CPGNotify,pDesk->sGameData.T_CPGNotify,sizeof(action.T_CPGNotify));
				for(int j = 0; j < PLAY_COUNT; j++)
				{
					pDesk->SendGameDataEx(j,&action,sizeof(HaveAction),MDM_GM_GAME_NOTIFY,THING_HAVE_ACTION,0);
					pDesk->SendWatchDataEx(j,&action,sizeof(HaveAction),MDM_GM_GAME_NOTIFY,THING_HAVE_ACTION,0);
				}
				byCanActionDesk = i;
				bHaveSend = true;
				break;
			}
		}
		if(bHaveSend)
		{
			SetDoingThing(byCanActionDesk,THING_HU_PAI);//����ִ���¼�
		}
		
	}
	//û�з��ͺ�����Ϣ ��ô�����ȷ��͸� ����������Ϣ
	if(!bHaveSend)
	{
		//�������ȼ��������ȷ�
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(i == byIgnoreStation || pDesk->sUserData.m_bFoceLeave[i])
			{
				continue;
			}
			//�����ȸ��ܸܡ����Ƶ���ҷ��Ͷ�����Ϣ 
			if(pDesk->sGameData.T_CPGNotify[i].bGang || pDesk->sGameData.T_CPGNotify[i].bPeng)
			{
				/*pDesk->SendGameDataEx(i, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
				pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);*/
				//֪ͨ���������ĳ����ж���
				HaveAction action;
				action.byDeskStation = byIgnoreStation;
				memcpy(action.T_CPGNotify,pDesk->sGameData.T_CPGNotify,sizeof(action.T_CPGNotify));
				for(int j = 0; j < PLAY_COUNT; j++)
				{
					pDesk->SendGameDataEx(j,&action,sizeof(HaveAction),MDM_GM_GAME_NOTIFY,THING_HAVE_ACTION,0);
					pDesk->SendWatchDataEx(j,&action,sizeof(HaveAction),MDM_GM_GAME_NOTIFY,THING_HAVE_ACTION,0);
				}
				byCanActionDesk = i;
				bHaveSend = true;
				break;
			}
		}
		if(bHaveSend)
		{
			SetDoingThing(byCanActionDesk,THING_GANG_PAI);//����ִ���¼�
		}
	}

	//û�з��ͺ����ܡ�����Ϣ �ټ�鷢�ͳ�����Ϣ
	if(!bHaveSend)
	{
		//�������ȼ��������ȷ�
		for(int i=0;i<PLAY_COUNT;i++)
		{
			if(i == byIgnoreStation || pDesk->sUserData.m_bFoceLeave[i])
			{
				continue;
			}
			//�����ȸ��ܸܡ����Ƶ���ҷ��Ͷ�����Ϣ 
			if(pDesk->sGameData.T_CPGNotify[i].bChi)
			{
				pDesk->SendGameDataEx(i, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
				pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
				byCanActionDesk = i;
				bHaveSend = true;
				break;
			}
		}
		if(bHaveSend)
		{
			SetDoingThing(byCanActionDesk,THING_CHI_PAI);//����ִ���¼�
		}
	}

	return bHaveSend;
}
/*--------------------------------------------------------------------------------------------------*/
///ץ���¼�
void	ThingHandleEx::ExecuteZhuaPai(BYTE &nextID,BYTE &time)	
{
	///ץ�����¼�
	nextID = 255;
	BYTE card = 255;
	if(pDesk->sUserData.m_MenPai.GetMenPaiNum()<=pDesk->sGameData.m_mjRule.byGamePassNum)
	{
		OutputDebugString("dwjsdmj::------�� ץ���� ��Ϸ����");
		//������պ�����Ϣ
		pDesk->sGameData.T_HuPai.Clear();
		//ץ������ ��ֽ�����Ϸ
		nextID = THING_COUNT_FEN;
		time = 2;
	}
	else
	{
		//�����¼� �ͻ�ȡ������ҵ���һ�����ץ��
		if( pDesk->sGameData.m_byThingDoing == THING_OUT_PAI)
		{
			pDesk->sUserData.m_byNowOutStation = GetNextStation(pDesk->sGameData.T_OutPai.byUser);//ץ��λ��
			//��ǰ��ץ
			pDesk->sUserData.m_bZhuaPaiDir = true;
		}
		else if(pDesk->sGameData.m_byThingDoing == THING_GANG_PAI)	//����Ǹ����¼�  ��ô���Ǹ������ץ��	
		{
			//ץ��λ��
			pDesk->sUserData.m_byNowOutStation = pDesk->sGameData.T_GangPai.byUser;
			//�Ӻ���ץ
			pDesk->sUserData.m_bZhuaPaiDir = false;
		}

//		pDesk->sGameData.T_HuPai.Clear();//������Ƽ�¼

		pDesk->sGameData.m_byThingDoing = THING_ZHUA_PAI;//��¼���ڷ������¼�

		//����ץ������
		pDesk->sGameData.T_ZhuaPai.Clear();

		pDesk->sGameData.T_ZhuaPai.byUser = pDesk->sUserData.m_byNowOutStation;
		card = GetPai(pDesk->sUserData.m_byNowOutStation,pDesk->sUserData.m_bZhuaPaiDir,0,&pDesk->sGameData.T_ZhuaPai.byGetPaiIndex);//����ץ��
		pDesk->sGameData.T_ZhuaPai.byPs = card;		//ץ�Ƶ�ֵ

//		pDesk->sUserData.m_bQiHu[pDesk->sUserData.m_byNowOutStation] = false;//�����������״̬


		//ץ������ǿ�ƽ���
		if(card==255 && (pDesk->sUserData.m_MenPai.GetMenPaiNum())<=pDesk->sGameData.m_mjRule.byGamePassNum)
		{
			//������պ�����Ϣ
			pDesk->sGameData.T_HuPai.Clear();

			//ץ������ ��ֽ�����Ϸ
			nextID = THING_COUNT_FEN;
			time = 2;
			
			return;
		}


		ChangeMenPai();//ת������

		for(int i=0;i<PLAY_COUNT;i++)
		{
			//��������
			pDesk->sUserData.CopyMenPai(i,pDesk->sGameData.T_ZhuaPai.byMenPai[i]);
			//�����������
			pDesk->sGameData.T_ZhuaPai.m_byArHandPaiCount[i] = pDesk->sUserData.GetHandPaiCount(i);
		}

		//����ץ������
		for (int i=0;i<PLAY_COUNT;i++)
		{			
			//��������
			memset(pDesk->sGameData.T_ZhuaPai.m_byArHandPai,255,sizeof(pDesk->sGameData.T_ZhuaPai.m_byArHandPai));
			pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_ZhuaPai.m_byArHandPai,i,true);
			
			pDesk->SendGameDataEx(i, &pDesk->sGameData.T_ZhuaPai, sizeof(pDesk->sGameData.T_ZhuaPai), MDM_GM_GAME_NOTIFY, THING_ZHUA_PAI, 0);
			pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_ZhuaPai, sizeof(pDesk->sGameData.T_ZhuaPai), MDM_GM_GAME_NOTIFY, THING_ZHUA_PAI, 0);
		}


		//����ǻ��� ��ô��������
		if(pDesk->sUserData.CheckIsHuaPai(card))
		{
			nextID	= THING_ONE_BUHUA;
			time	= 5;
		}
		else
		{
			//�������
			if(!CheckAction(pDesk->sUserData.m_byNowOutStation,card,0))
			{
				//û�ж��� ��ô���������Ƽ�ʱ��
				SetDoingThing(pDesk->sUserData.m_byNowOutStation,THING_OUT_PAI);
			}
			else
			{
				//�����ץ�ƺ��ж���(�ͻ���������ʾ����ʱʱ���)
				pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].bZhuaPai	 = true;
				/*pDesk->SendGameDataEx(pDesk->sUserData.m_byNowOutStation, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
				pDesk->SendWatchDataEx(pDesk->sUserData.m_byNowOutStation, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);*/

				HaveAction action;
				action.byDeskStation = pDesk->sUserData.m_byNowOutStation;
				memcpy(action.T_CPGNotify,pDesk->sGameData.T_CPGNotify,sizeof(action.T_CPGNotify));
				for(int i = 0; i < PLAY_COUNT; i++)
				{
					pDesk->SendGameDataEx(i, &action, sizeof(HaveAction), MDM_GM_GAME_NOTIFY, THING_HAVE_ACTION, 0);
				}

				//����Ǻ��� ��ô���� �Զ�������Ӧ��ʱ��
				if(pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].bHu)
				{
					SetDoingThing(pDesk->sUserData.m_byNowOutStation,THING_HU_PAI,pDesk->sGameData.m_mjRule.byOutTime);
				}
				else //�������� ������ �Զ����Ƽ�ʱ��
				{
					SetDoingThing(pDesk->sUserData.m_byNowOutStation,THING_OUT_PAI,pDesk->sGameData.m_mjRule.byOutTime);
				}
			}
		}	


	}

	//20131218
//	����ץ�ƺͲ������ж�������ʱ�����--�ͻ�����ʾ����ͷ���˴������

	//��¼����״̬��ӵ�ڸܿ��͸ܺ���
	for(int i=0;i<PLAY_COUNT;++i)
	{
		if(i != pDesk->sUserData.m_byNowOutStation)
		{
			pDesk->sGameData.m_bGangState[i] = false;
		}
	}
}
/*--------------------------------------------------------------------------------------------------*/
///������Ҳ����¼�
void	ThingHandleEx::ExecuteOneBuHua(BYTE &nextID,BYTE &time)
{	
	nextID = 255;
	pDesk->sGameData.m_byThingDoing = THING_ONE_BUHUA;
	
	//��ղ�������
	pDesk->sGameData.T_OneBuHua.Clear();
	///������Ҳ�����λ�ü�ץ����ҵ�λ��
	pDesk->sGameData.T_OneBuHua.byUser = pDesk->sGameData.T_ZhuaPai.byUser;

	pDesk->sUserData.m_byNowOutStation = pDesk->sGameData.T_OneBuHua.byUser;

	//��ʼ����
	if(pDesk->sUserData.IsHuaPaiInHand(pDesk->sUserData.m_byNowOutStation))
	{
		int num = pDesk->sUserData.MoveHuaPaiFormHand(pDesk->sUserData.m_byNowOutStation);
		for(int i=0;i<num;i++)
		{
			pDesk->sGameData.T_OneBuHua.byPs = GetPai(pDesk->sUserData.m_byNowOutStation,false,3);//����ץ��
		}
	}

	//������������˿��� ����Ϸ����
	if(pDesk->sGameData.T_OneBuHua.byPs == 255 || (pDesk->sUserData.m_MenPai.GetMenPaiNum())<=pDesk->sGameData.m_mjRule.byGamePassNum)
	{
		//������պ�����Ϣ
		pDesk->sGameData.T_HuPai.Clear();
		//ץ������ ��ֽ�����Ϸ
		nextID = THING_COUNT_FEN;
		time = 2;
		return;
	}

	//ȡ���ƺ� ��ʱת������
	ChangeMenPai();
	for(int i=0;i<PLAY_COUNT;i++)
	{
		//��������
		pDesk->sUserData.CopyHuaPai(i,pDesk->sGameData.T_OneBuHua.m_byArHuaPai[i]);
		//��������
		pDesk->sUserData.CopyMenPai(i,pDesk->sGameData.T_OneBuHua.byMenPai[i]);

		//��������
		pDesk->sGameData.T_OneBuHua.m_byArHandPaiCount[i] = pDesk->sUserData.GetHandPaiCount(i);
		
	}
	///����������
	for (int i=0;i<PLAY_COUNT;i++)
	{			
		memset(pDesk->sGameData.T_OneBuHua.m_byArHandPai,255,sizeof(pDesk->sGameData.T_OneBuHua.m_byArHandPai));
		pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_OneBuHua.m_byArHandPai,i,true);
		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_OneBuHua, sizeof(pDesk->sGameData.T_OneBuHua), MDM_GM_GAME_NOTIFY, THING_ONE_BUHUA, 0);
		pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_OneBuHua, sizeof(pDesk->sGameData.T_OneBuHua), MDM_GM_GAME_NOTIFY, THING_ONE_BUHUA, 0);
	}

	//����ǻ��� ��ô�Ͳ���
	if(pDesk->sUserData.CheckIsHuaPai(pDesk->sGameData.T_OneBuHua.byPs))
	{
		nextID	= THING_ONE_BUHUA;
		time	= 5;
	}
	else
	{
		//�������
		if(!CheckAction(pDesk->sUserData.m_byNowOutStation,pDesk->sGameData.T_OneBuHua.byPs,0))
		{
			//û�ж��� ��ô���������Ƽ�ʱ��
			SetDoingThing(pDesk->sUserData.m_byNowOutStation,THING_OUT_PAI);
		}
		else
		{
			//��Ϊ��ץ���ж��� ��������˼��ʱ���ǳ���ʱ��
			//pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].byBlockTime = pDesk->sGameData.m_mjRule.byOutTime;
			//�����ץ�ƺ��ж���(�ͻ���������ʾ����ʱʱ���)
			pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].bZhuaPai	 = true;
			pDesk->SendGameDataEx(pDesk->sUserData.m_byNowOutStation, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
			pDesk->SendWatchDataEx(pDesk->sUserData.m_byNowOutStation, &pDesk->sGameData.T_CPGNotify, sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);


			//����Ǻ��� ��ô���� �Զ�������Ӧ��ʱ��
			if(pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].bHu)
			{
				SetDoingThing(pDesk->sUserData.m_byNowOutStation,THING_HU_PAI,pDesk->sGameData.m_mjRule.byOutTime);
			}
			else //�������� ������ �Զ����Ƽ�ʱ��
			{
				SetDoingThing(pDesk->sUserData.m_byNowOutStation,THING_OUT_PAI,pDesk->sGameData.m_mjRule.byOutTime);
			}
		}
	}	
}
/*--------------------------------------------------------------------------------------------------*/
///����¼�
void	ThingHandleEx::ExecuteCountFen(BYTE &nextID,BYTE &time)
{	
	OutputDebugString("20160615::ExecuteCountFen-0");
	//��¼����ִ�е��¼�
	pDesk->sGameData.m_byThingDoing = THING_COUNT_FEN;

	pDesk->sGameData.T_CountFen.bIsLiuJu	= pDesk->sGameData.T_HuPai.bIsLiuJu;		//�Ƿ�����
	pDesk->sGameData.T_CountFen.bZiMo		= pDesk->sGameData.T_HuPai.bZimo;			//�Ƿ�����
	pDesk->sGameData.T_CountFen.byUser		= pDesk->sGameData.T_HuPai.byUser;			//�������λ��
	pDesk->sGameData.T_CountFen.byUserNum	= pDesk->sGameData.T_HuPai.byUserNum;		//������ҵĸ���	
	pDesk->sGameData.T_CountFen.byDianPao	= pDesk->sGameData.T_HuPai.byDianPao;		//������ҵ�λ��
	pDesk->sGameData.T_CountFen.bQiangGang	= pDesk->sGameData.T_HuPai.bQiangGang;		//�Ƿ�����
	pDesk->sGameData.T_CountFen.byHuUserNum	= pDesk->sGameData.T_HuPai.byUserNum;		//������ҵĸ���
	pDesk->sGameData.T_CountFen.bGangKai	= pDesk->sGameData.T_HuPai.bGangKai;		//�Ǹܿ�
	for(int i=0;i<PLAY_COUNT;i++)
	{
		//��¥���
		pDesk->sGameData.T_CountFen.bMingPai[i] = pDesk->sUserData.m_bTing[i];
		//���Ƶ����
		pDesk->sGameData.T_CountFen.m_bIsHu[i] = pDesk->sGameData.T_HuPai.bHaveHu[i];
		//��������
		for(int j=0;j<MAX_HUPAI_TYPE;j++)
		{
			pDesk->sGameData.T_CountFen.byHuType[i][j] = pDesk->sGameData.T_HuPai.byHuType[i][j];
		}
	}
	OutputDebugString("20160615::ExecuteCountFen-1");
	//�׷�
	//int iBase = 5;
	pDesk->sGameData.T_CountFen.iBase = pDesk->m_iBaseMoney;
	//�������������÷���
	CalculateGangFen();
	OutputDebugString("20160615::ExecuteCountFen-2");
	//��ׯ����(���32��)
	pDesk->sGameData.T_CountFen.iHuangZhuangBase = ::pow(2.0,pDesk->sGameData.m_iHuangZhuangCount);
	if(pDesk->sGameData.T_CountFen.iHuangZhuangBase >= 32)
	{
		pDesk->sGameData.T_CountFen.iHuangZhuangBase = 32;
	}
	OutputDebugString("20160615::ExecuteCountFen-3");
	//����
	if(pDesk->sGameData.T_HuPai.bIsLiuJu)
	{
		/*for(int i=0; i<PLAY_COUNT; i++)
		{
		pDesk->sGameData.T_CountFen.iZongFen[i] = 0;
		}*/
		//��ׯ����+1
		pDesk->sGameData.m_iHuangZhuangCount++;
		if(pDesk->sGameData.m_iHuangZhuangCount >= 5)
		{
			pDesk->sGameData.m_iHuangZhuangCount = 5;
		}
		//������Ҷ����۷� ֻ��ȡ ̨��
		//for(int i=0; i<PLAY_COUNT; i++)
		//{
			//pDesk->sGameData.T_CountFen.iZongFen[i] = 0;
			//pDesk->sGameData.T_CountFen.iZongFen[i] = pDesk->sGameData.T_CountFen.iFanCount[i] * pDesk->m_iBaseMoney;
		//}

		//��һ���¼�����Ϸ�����¼�
		nextID = THING_ENG_HANDLE;
		time = 5;
	}
	else
	{
		OutputDebugString("20160615::ExecuteCountFen-4");
		//��һ���¼�����Ϸ�����¼�
		nextID = THING_ENG_HANDLE;
		time = 5;
		//��ׯ������0
		pDesk->sGameData.m_iHuangZhuangCount = 0;
		int iHuaCount	= 0;	//������
		//int iFanCount	= 1;	//����
		int iZongFen	= 0;	//�ܷ�

		//ֻ��һ���˺���
		if(1 == pDesk->sGameData.T_HuPai.byUserNum)
		{
			OutputDebugString("20160615::ExecuteCountFen-5");
			bool bHaveFourZhong(false);
			for(int i = 0; i < MAX_HUPAI_TYPE; i++)
			{
				//if(pDesk->sGameData.T_CPGNotify[pDesk->sGameData.T_HuPai.byUser].byHuType[i] == 255)
				if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == 255)
					break;
				else
				{
					if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == HUPAI_TYPE_FOUR_ZHONG)
					{
						bHaveFourZhong = true;
						break;
					}
				}
			}
			OutputDebugString("20160615::ExecuteCountFen-6");
			if(bHaveFourZhong)
			{
				for(int i = 0; i < PLAY_COUNT; i++)
				{
					if(i == pDesk->sGameData.T_HuPai.byUser)
					{
						pDesk->sGameData.T_CountFen.iHuFan[i] += 18;
					}
					else
					{
						pDesk->sGameData.T_CountFen.iHuFan[i] -= 6;
					}
				}
			}
			else
			{
				OutputDebugString("20160615::ExecuteCountFen-7");
				//�Ƿ�����
				CString dwjlog;
				dwjlog.Format("20160615::pDesk->sGameData.T_HuPai.byUser=%d",pDesk->sGameData.T_HuPai.byUser);
				OutputDebugString(dwjlog);
				bool bHaveZhong = pDesk->sUserData.IsHaveAHandPai(pDesk->sGameData.T_HuPai.byUser,35);
				OutputDebugString("20160615::ExecuteCountFen-8");
				if(pDesk->sGameData.T_CountFen.bZiMo)
				{
					for(int i=0; i<PLAY_COUNT; i++)
					{

						if(i == pDesk->sGameData.T_HuPai.byUser)
						{

							if(bHaveZhong)
							{
								pDesk->sGameData.T_CountFen.iHuFan[i] += 3;
							}
							else
							{
								pDesk->sGameData.T_CountFen.iHuFan[i] += 6;
							}
						}
						else
						{
							if(bHaveZhong)
							{
								pDesk->sGameData.T_CountFen.iHuFan[i] -= 1;
							}
							else
							{
								pDesk->sGameData.T_CountFen.iHuFan[i] -= 2;
							}
						}
					}
				}
				else	//��������
				{
					OutputDebugString("20160615::ExecuteCountFen-9");
					for(int i = 0; i < PLAY_COUNT; i++)
					{
						if(i == pDesk->sGameData.T_HuPai.byDianPao)
						{
							pDesk->sGameData.T_CountFen.iGangFan[i] -= 3;
						}
						else
						{
							pDesk->sGameData.T_CountFen.iGangFan[i] += 1;
						}
					}

					if(bHaveZhong)
					{
						pDesk->sGameData.T_CountFen.iHuFan[pDesk->sGameData.T_HuPai.byUser] += 3;
						pDesk->sGameData.T_CountFen.iHuFan[pDesk->sGameData.T_HuPai.byDianPao] -= 3;
					}
					else
					{
						pDesk->sGameData.T_CountFen.iHuFan[pDesk->sGameData.T_HuPai.byUser] += 6;
						pDesk->sGameData.T_CountFen.iHuFan[pDesk->sGameData.T_HuPai.byDianPao] -= 6;
					}
				}
			}
			//������� �¾ֵ�ׯ
			pDesk->sGameData.m_byNtStation = pDesk->sGameData.T_HuPai.byUser;
			OutputDebugString("20160615::ExecuteCountFen-10");
		}
		else	//һ�ڶ���
		{
			for(int i=0; i<PLAY_COUNT; i++)
			{
				for(int i = 0; i < PLAY_COUNT; i++)
				{
					if(i == pDesk->sGameData.T_HuPai.byDianPao)
					{
						pDesk->sGameData.T_CountFen.iGangFan[i] -= 3;
					}
					else
					{
						pDesk->sGameData.T_CountFen.iGangFan[i] += 1;
					}
				}

				if(pDesk->sGameData.T_HuPai.bHaveHu[i])
				{
					bool bHaveZhong = pDesk->sUserData.IsHaveAHandPai(i,35);

					if(bHaveZhong)
					{
						pDesk->sGameData.T_CountFen.iHuFan[i] += 3;
						pDesk->sGameData.T_CountFen.iHuFan[pDesk->sGameData.T_HuPai.byDianPao] -= 3;
					}
					else
					{
						pDesk->sGameData.T_CountFen.iHuFan[i] += 6;
						pDesk->sGameData.T_CountFen.iHuFan[pDesk->sGameData.T_HuPai.byDianPao] -= 6;
					}
				}
			}
			//һ�ڶ��� ������ҵ�ׯ
			pDesk->sGameData.m_byNtStation = pDesk->sGameData.T_HuPai.byDianPao;
		}
	}
	OutputDebugString("20160615::ExecuteCountFen-11");
	//���������ܷ�
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		pDesk->sGameData.T_CountFen.iZongFen[i] = pDesk->m_iBaseMoney * (pDesk->sGameData.T_CountFen.iGangFan[i] + pDesk->sGameData.T_CountFen.iHuFan[i] + pDesk->sGameData.T_CountFen.iZhongFan[i]);
	}
	OutputDebugString("20160615::ExecuteCountFen-12");
}
/*--------------------------------------------------------------------------------------------------*/
//���㷬���ͻ���
void	ThingHandleEx::CountFanAndHua(BYTE byDeskStation,int &iHuaCount, int &iFanCount)
{
	//���㷬��
	for(int i=0;i<MAX_HUPAI_TYPE;i++)
	{
		if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == 255)
		{
			continue;
		}

		if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == HUPAI_PENG_PENG_HU)		//������
		{
			iFanCount += 2;
		}
		else if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == HUPAI_HUN_YI_SE)	//��һɫ
		{
			iFanCount += 2;
		}
		else if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == HUPAI_QING_LONG)	//����(һ����)
		{
			iFanCount += 2;
		}
		else if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == HUPAI_TYPE_GANG_KAI)// �ܿ�
		{
			iFanCount += 2;
		}
		else if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == HUPAI_AN_QI_DUI)	//���߶�
		{
			iFanCount += 2;
		}
		else if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == HUPAI_QING_YI_SE)	//��һɫ
		{
			iFanCount += 4;
		}
		else if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == HUPAI_JIANG_YI_SE)	//��һɫ
		{
			iFanCount += 5;
		}
		else if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == HUPAI_TYPE_DI_HU)	// �غ�
		{
			iFanCount += 5;
		}
		else if(pDesk->sGameData.T_HuPai.byHuType[pDesk->sGameData.T_HuPai.byUser][i] == HUPAI_TYPE_TIAN_HU)// ���
		{
			iFanCount += 6;
		}
	}

	//���㻨��
	for(int i=0;i<HUA_CARD_NUM;i++)
	{
		if(pDesk->sUserData.m_byArHuaPai[pDesk->sGameData.T_HuPai.byUser][i] != 255)
		{
			iHuaCount +=1;
		}
	}
	//���������еĻ�����
	for(int i=0;i<5;i++)
	{
		//�������
		if(pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_HuPai.byUser][i].byType == AT_TRIPLET)
		{
			//����� ���� ���� ���� 
			if(pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_HuPai.byUser][i].byData[0] >= CMjEnum::MJ_TYPE_FD && pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_HuPai.byUser][i].byData[0] <= CMjEnum::MJ_TYPE_BAI)
			{
				iHuaCount +=1;
			}
		}
		else if(pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_HuPai.byUser][i].byType == AT_QUADRUPLET || pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_HuPai.byUser][i].byType == AT_QUADRUPLET_REVEALED)
		{//����ǲ���||���� 
			//����||���� +2
			if(pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_HuPai.byUser][i].byData[0] >= CMjEnum::MJ_TYPE_FD && pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_HuPai.byUser][i].byData[0] <= CMjEnum::MJ_TYPE_BAI)
			{
				iHuaCount +=2;
			}
			else	//����Ͳ +1
			{
				iHuaCount +=1;
			}
		}
		else if(pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_HuPai.byUser][i].byType == AT_QUADRUPLET_CONCEALED)	
		{//����
			//����||���� +2
			if(pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_HuPai.byUser][i].byData[0] >= CMjEnum::MJ_TYPE_FD && pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_HuPai.byUser][i].byData[0] <= CMjEnum::MJ_TYPE_BAI)
			{
				iHuaCount +=3;
			}
			else	//����Ͳ +1
			{
				iHuaCount +=2;
			}
		}
	}
}
/*--------------------------------------------------------------------------------------------------*/
///������Ϸ�����¼�
void ThingHandleEx::ExecuteNormalEnd(BYTE &nextID,BYTE &time)
{    
	//��ǰִ���¼�
	pDesk->sGameData.m_byThingDoing = THING_ENG_HANDLE;

	pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_CountFen.m_byArHandPai,0,true);
	
	//��������
	for(int i=0; i<PLAY_COUNT; i++)
	{
		pDesk->sGameData.T_CountFen.m_byArHandPaiCount[i] = pDesk->sUserData.GetHandPaiCount(i);
	}

	pDesk->KillTimer(TIME_CHECK_GAME_MSG);
	//����ƽ̨������Ϸ�ӿ�
	pDesk->GameFinish(pDesk->sGameData.T_HuPai.byUser,GF_NORMAL);

	pDesk->KillTimer(TIME_WAIT_MSG);
	this->ReSetOutTime();
}




































///�쳣��Ϸ�����¼��¼�
void ThingHandleEx::ExecuteUnNormalEnd(BYTE &nextID,BYTE &time)
{
	ThingHandle::ExecuteUnNormalEnd(nextID,time);
}



///�����Զ�������
void ThingHandleEx::NetCutAutoHandle()
{
	//ThingHandle::NetCutAutoHandle();
	switch(pDesk->sGameData.m_byThingDoing)
	{			        				         
	case THING_2SEZI_NT://�׾���2��ɫ�ӵĵ�����Ϊׯ�¼�		   
	case THING_2SEZI_NT_GP://�׾���2��ɫ�Ӷ�ׯ�Һ����Ƶ��¼�	
	case THING_TURN_2SEZI_NT://�׾�������2��ɫ�Ӷ�ׯ���¼�		
	case THING_2SEZI_DIR://��2��ɫ���¼������Ʒ����¼�		
	case THING_2SEZI_DIR_GP://��2��ɫ���¼������Ʒ�������Ƶ��¼�
	case THING_2SEZI_GP://��2��ɫ���¼������Ƶ��¼�												
	case THING_2SEZI_JING://��2��ɫ�Ӷ������¼�			
	case THING_1SEZI_JIN://��1��ɫ�Ӷ������¼�
		{
			for(int i=0;i<PLAY_COUNT;++i)
			{
				if(!pDesk->sUserData.m_bSiziFinish[i] && pDesk->sUserData.m_bFoceLeave[i])//�����û�з�������Ϣ
				{
					ReceiveSeziFinish(i,NULL,0,false);
				}
			}
		}break;
	case THING_ALL_BUHUA://Ϊ������Ҳ����¼�
		{
		}
		break;
	case THING_ONE_BUHUA://������Ҳ����¼�	
	case THING_CHI_PAI://�����¼�
	case THING_PENG_PAI://�����¼�
	case THING_GANG_PAI://�����¼�
		{
			if(pDesk->sUserData.m_bFoceLeave[pDesk->sUserData.m_byNowOutStation])//���������Ҷ���
			{
				AutoOutOrGet();//������ҳ���
			}
		}
		break;
	case THING_BEGIN_OUT://ׯ�ҿ�ʼ����֪ͨ�¼�	
		{
			if(pDesk->sUserData.m_bFoceLeave[pDesk->sGameData.m_byNtStation])//���ׯ�Ҷ���
			{
				AutoOutOrGet();
			}
		}
		break;
	case THING_OUT_PAI://�����¼�
		{
			for(int i=0;i<PLAY_COUNT;++i)
			{
				if(pDesk->sUserData.m_bFoceLeave[i] && pDesk->sUserData.m_byNowOutStation != i)//�����Ҷ���
				{
					ReceiveUserPassCheck(i,NULL,0,false);//���ͷ�����Ϣ
				}
			}
		}
		break;
	case THING_ZHUA_PAI://ץ���¼�
		{
			OutputDebugString("��ʱ����NetCutAutoHandle 000");
			if(pDesk->sUserData.m_bFoceLeave[pDesk->sUserData.m_byNowOutStation])//���������Ҷ���
			{
				AutoOutOrGet();
			}
		}
		
	}
}






















//-------------------------------------------��������------------------------------------------------------//
/*---------------------------------------------------------------------------------------------------------*/
///�����¼�
void	ThingHandleEx::HandleChiPai(BYTE &nextID,BYTE &time)
{	

	pDesk->sGameData.m_byThingDoing = THING_CHI_PAI;//��¼����ִ�е��¼�
	///�������¼�
//	pDesk->sGameData.T_ChiPai = pDesk->sGameData.temp_ChiPai;//��ȡ����
//	pDesk->sUserData.m_byNowOutStation = pDesk->sGameData.temp_ChiPai.byUser;

	//��ӳ���
	GCPStructEx chi;
	chi.Init();
	chi.byType		= AT_COLLECT;	//��������
	chi.iBeStation	= pDesk->sGameData.T_OutPai.byUser;	//���Ե���� ����֮ǰ���Ƶ����
	chi.iOutpai		= pDesk->sGameData.T_OutPai.byPs;	//���Ե�������
	chi.iStation	= pDesk->sGameData.T_ChiPai.byUser;	//���Ƶ����
	//��������
	for(int i=0;i<3;i++)
	{	
		chi.byData[i] = pDesk->sGameData.T_ChiPai.byChiPs[i];
		if(pDesk->sGameData.T_ChiPai.byChiPs[i] == pDesk->sGameData.T_ChiPai.byPs)
		{
			continue;
		}
		//ɾ������
		pDesk->sUserData.DelleteAHandPai(pDesk->sGameData.T_ChiPai.byUser,pDesk->sGameData.T_ChiPai.byChiPs[i]);
	}

	//ɾ��������ҵ����һ����
	pDesk->sUserData.DelleteLastOutPai(pDesk->sGameData.T_OutPai.byUser);
	//�����������
	pDesk->sUserData.AddToGCP(chi.iStation,&chi);

	//������������
	pDesk->sUserData.CopyGCPData(chi.iStation,pDesk->sGameData.T_ChiPai.m_UserGCPData[chi.iStation]);
	//����������ҵĳ�������
	pDesk->sGameData.T_ChiPai.m_byArOutPaiCount[chi.iBeStation] = pDesk->sUserData.CopyOutPai(chi.iBeStation,pDesk->sGameData.T_ChiPai.m_byArOutPai[chi.iBeStation]);

	//������ҵ���������
	pDesk->sGameData.T_ChiPai.m_byArHandPaiCount[chi.iStation] = pDesk->sUserData.GetHandPaiCount(chi.iStation);

	//֪ͨ�������
	for (int i=0;i<PLAY_COUNT;i++)
	{	
		memset(pDesk->sGameData.T_ChiPai.m_byArHandPai,255,sizeof(pDesk->sGameData.T_PengPai.m_byArHandPai));
		pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_ChiPai.m_byArHandPai,i,true);

		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_ChiPai, sizeof(pDesk->sGameData.T_ChiPai), MDM_GM_GAME_NOTIFY, THING_CHI_PAI, 0);
		pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_ChiPai, sizeof(pDesk->sGameData.T_ChiPai), MDM_GM_GAME_NOTIFY, THING_CHI_PAI, 0);
	}

	//�������ҵ�������������
	pDesk->sGameData.T_CPGNotify[chi.iStation].Clear();
	//��¼��һ�Ӧ
	pDesk->sGameData.T_CPGNotify[chi.iStation].bHaveReturn = true;

	//��ǰ���Ƶ����
	pDesk->sUserData.m_byNowOutStation = chi.iStation;

	//�����ƺ��⶯��
	if(CheckAction(pDesk->sUserData.m_byNowOutStation,pDesk->sUserData.GetLastHandPai(pDesk->sUserData.m_byNowOutStation),4))
	{
		//�����ץ�ƺ��ж���(�ͻ���������ʾ����ʱʱ���)
		pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].bZhuaPai	 = true;
		pDesk->SendGameDataEx(pDesk->sGameData.m_byNtStation, &pDesk->sGameData.T_CPGNotify ,sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
		pDesk->SendWatchDataEx(pDesk->sGameData.m_byNtStation, &pDesk->sGameData.T_CPGNotify ,sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
	}
	

	//�����ƺ�ֱ��������ʱ���Ƽ�ʱ��(��Ϊ�����ƺ󲻿����к��Ƶ����)
	SetDoingThing(pDesk->sUserData.m_byNowOutStation,THING_OUT_PAI);

}
/*---------------------------------------------------------------------------------------------------------*/
///�����¼�
void	ThingHandleEx::HandlePengPai(BYTE &nextID,BYTE &time)	
{
	pDesk->sGameData.m_byThingDoing = THING_PENG_PAI;//��¼����ִ�е��¼�
	
	
	//�������¼�
	GCPStructEx peng;
	peng.Init();
	peng.byType		= AT_TRIPLET;	//������
	peng.iStation	= pDesk->sGameData.T_PengPai.byUser;	//���������λ��
	peng.iBeStation	= pDesk->sGameData.T_OutPai.byUser;		//��������� ����֮ǰ���Ƶ����
	peng.iOutpai	= pDesk->sGameData.T_OutPai.byPs;		//������������

	//��������������
	for(int i=0;i<3;i++)
	{
		peng.byData[i] = pDesk->sGameData.T_OutPai.byPs;
	}

	//�������ɾ�����һ�ų���
	pDesk->sUserData.DelleteLastOutPai(peng.iBeStation);
	//ɾ��������ҵ�����������
	for(int i=0;i<2;i++)
	{	
		pDesk->sUserData.DelleteAHandPai(peng.iStation, peng.iOutpai);
	}
	
	//�������
	pDesk->sUserData.AddToGCP(peng.iStation,&peng);

	//������������
	pDesk->sUserData.CopyGCPData(peng.iStation,pDesk->sGameData.T_PengPai.m_UserGCPData[peng.iStation]);
	//����������ҵĳ�������
	pDesk->sGameData.T_PengPai.m_byArOutPaiCount[peng.iBeStation] = pDesk->sUserData.CopyOutPai(peng.iBeStation,pDesk->sGameData.T_PengPai.m_byArOutPai[peng.iBeStation]);

	//������ҵ���������
	pDesk->sGameData.T_PengPai.m_byArHandPaiCount[peng.iStation] = pDesk->sUserData.GetHandPaiCount(peng.iStation);

	//֪ͨ�������
	for (int i=0;i<PLAY_COUNT;i++)
	{	
		memset(pDesk->sGameData.T_PengPai.m_byArHandPai,255,sizeof(pDesk->sGameData.T_PengPai.m_byArHandPai));
		pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_PengPai.m_byArHandPai,i,true);

		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_PengPai, sizeof(pDesk->sGameData.T_PengPai), MDM_GM_GAME_NOTIFY, THING_PENG_PAI, 0);
		pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_PengPai, sizeof(pDesk->sGameData.T_PengPai), MDM_GM_GAME_NOTIFY, THING_PENG_PAI, 0);
	}
	//�������ҵ�������������
	pDesk->sGameData.T_CPGNotify[peng.iStation].Clear();
	//��¼��һ�Ӧ
	pDesk->sGameData.T_CPGNotify[peng.iStation].bHaveReturn = true;

	//��ǰ���Ƶ����
	pDesk->sUserData.m_byNowOutStation = peng.iStation;

	//�����ƺ��⶯��
	if(CheckAction(pDesk->sUserData.m_byNowOutStation,pDesk->sUserData.GetLastHandPai(pDesk->sUserData.m_byNowOutStation),4))
	{
		//�����ץ�ƺ��ж���(�ͻ���������ʾ����ʱʱ���)
		pDesk->sGameData.T_CPGNotify[pDesk->sUserData.m_byNowOutStation].bZhuaPai	 = true;
		pDesk->SendGameDataEx(pDesk->sGameData.m_byNtStation, &pDesk->sGameData.T_CPGNotify ,sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
		pDesk->SendWatchDataEx(pDesk->sGameData.m_byNtStation, &pDesk->sGameData.T_CPGNotify ,sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
	}

	//�����ƺ�ֱ��������ʱ���Ƽ�ʱ��(��Ϊ�����ƺ󲻿����к��Ƶ����)
	SetDoingThing(pDesk->sUserData.m_byNowOutStation,THING_OUT_PAI);
}

//����¼�(��Ҫ����Ը������)
void    ThingHandleEx::CalculateGangFen(void)
{
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			GCPStructEx gang;
			gang = pDesk->sUserData.m_UserGCPData[i][j];
			switch(gang.byType)
			{
			case AT_QUADRUPLET_CONCEALED:
				{
					pDesk->sGameData.T_CountFen.iGangFan[i] += 6;
					for(int k = 0; k < PLAY_COUNT; k++)
					{
						if(i == k)
							continue;
						pDesk->sGameData.T_CountFen.iGangFan[k] -= 2;
					}
					break;
				}
			case AT_QUADRUPLET:
				{
					pDesk->sGameData.T_CountFen.iGangFan[i] += 3;
					for(int k = 0; k < PLAY_COUNT; k++)
					{
						if(i == k)
							continue;
						pDesk->sGameData.T_CountFen.iGangFan[k] -= 1;
					}
					break;
				}
			case AT_QUADRUPLET_REVEALED:
				{
					pDesk->sGameData.T_CountFen.iGangFan[i] += 3;
					BYTE fangGang = pDesk->sUserData.m_UserGCPData[i][j].iBeStation;
					pDesk->sGameData.T_CountFen.iGangFan[fangGang] -= 3;
					break;
				}
			default:
				break;
			}
		}
	}
}
/*---------------------------------------------------------------------------------------------------------*/
///���¼�
void	ThingHandleEx::HandleGangPai(BYTE &nextID,BYTE &time)	
{
	/*���ж���ʲô���͵ĸ�*/
	//�����ǰ�ǳ���ʱ�� ��ô��������
	BYTE byGangType = 255;
	if(pDesk->sGameData.m_byThingDoing == THING_OUT_PAI)
	{
		byGangType = AT_QUADRUPLET_REVEALED;
	}
	else if(pDesk->sGameData.m_byThingDoing == THING_ZHUA_PAI || THING_ONE_BUHUA == pDesk->sGameData.m_byThingDoing || THING_BEGIN_OUT == pDesk->sGameData.m_byThingDoing)	
	{
		//�����ץ��״̬ || ����״̬ || ��ʼ����״̬ ��ô��ȥ�ж��ǰ��� ���ǲ���
		for(int i=0; i<5; i++)
		{
			//�����������и����� && ��������
			if(pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_GangPai.byUser][i].byData[0] == pDesk->sGameData.T_GangPai.byPs && pDesk->sUserData.m_UserGCPData[pDesk->sGameData.T_GangPai.byUser][i].byType == ACTION_PENG)
			{
				byGangType = AT_QUADRUPLET;
				break;
			}
		}

		if(byGangType != AT_QUADRUPLET)
		{
			byGangType = AT_QUADRUPLET_CONCEALED;
		}
	}

	pDesk->sGameData.m_byThingDoing = THING_GANG_PAI;//��¼����ִ�е��¼�

	pDesk->sUserData.m_byNowOutStation = pDesk->sGameData.T_GangPai.byUser;

	//��������
	GCPStructEx gang;
	gang.Init();
	gang.byType		= byGangType;	//������
	gang.iStation	= pDesk->sGameData.T_GangPai.byUser;	//���������λ��
	gang.iBeStation	= pDesk->sGameData.T_GangPai.byBeGang;	//���ܵ���� ����֮ǰ���Ƶ����
	gang.iOutpai	= pDesk->sGameData.T_GangPai.byPs;		//���ܵ�������
	//��������
	for(int i=0; i<4; i++)
	{
		gang.byData[i] = pDesk->sGameData.T_GangPai.byPs;
	}

	//���������
	if(byGangType == AT_QUADRUPLET_REVEALED)
	{
		//ɾ��������ҵ�����3����
		for(int i=0;i<3;i++)
		{	
			pDesk->sUserData.DelleteAHandPai(gang.iStation, gang.iOutpai);
		}

		//ɾ������������һ�ų���
		pDesk->sUserData.DelleteLastOutPai(gang.iBeStation);
		//����������ҵĳ�������
		pDesk->sGameData.T_GangPai.m_byArOutPaiCount[gang.iBeStation] = pDesk->sUserData.CopyOutPai(gang.iBeStation,pDesk->sGameData.T_GangPai.m_byArOutPai[gang.iBeStation]);

		//��¼��������ڸ���״̬(�����ж��Ƿ�ܿ�) ֻ�������� ����ܿ�
		pDesk->sGameData.m_bGangState[gang.iStation] = true;
	}
	else if(byGangType == AT_QUADRUPLET)	//����
	{
		//ɾ��������ҵ�����1����
		for(int i=0;i<1;i++)
		{	
			pDesk->sUserData.DelleteAHandPai(gang.iStation, gang.iOutpai);
		}
		//ɾ����Ӧ����
		pDesk->sUserData.DelleteAGCPData(gang.iStation,ACTION_PENG,gang.iOutpai);
	}
	else if(byGangType == AT_QUADRUPLET_CONCEALED)	//����
	{
		//ɾ��������ҵ�����4����
		for(int i=0;i<4;i++)
		{	
			pDesk->sUserData.DelleteAHandPai(gang.iStation, gang.iOutpai);
		}
	}
	
	//��Ӹ�������
	pDesk->sUserData.AddToGCP(gang.iStation,&gang);

	//������������
	pDesk->sUserData.CopyGCPData(gang.iStation,pDesk->sGameData.T_GangPai.m_UserGCPData[gang.iStation]);

	//������ҵ���������
	pDesk->sGameData.T_GangPai.m_byArHandPaiCount[gang.iStation] = pDesk->sUserData.GetHandPaiCount(gang.iStation);
//	pDesk->sUserData.m_bIsGangPao[pDesk->sGameData.temp_GangPai.byUser] = true;//���ڸܺ��ڵ�״̬
	pDesk->sGameData.T_GangPai.byType = byGangType;
	//֪ͨ�������
	for (int i=0;i<PLAY_COUNT;i++)
	{	
		memset(pDesk->sGameData.T_GangPai.m_byArHandPai,255,sizeof(pDesk->sGameData.T_GangPai.m_byArHandPai));
		pDesk->sUserData.CopyHandPai(pDesk->sGameData.T_GangPai.m_byArHandPai,i,true);

		pDesk->SendGameDataEx(i, &pDesk->sGameData.T_GangPai, sizeof(pDesk->sGameData.T_GangPai), MDM_GM_GAME_NOTIFY, THING_GANG_PAI, 0);
		pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_GangPai, sizeof(pDesk->sGameData.T_GangPai), MDM_GM_GAME_NOTIFY, THING_GANG_PAI, 0);
	}


	//�������ҵ�������������
	pDesk->sGameData.T_CPGNotify[gang.iStation].Clear();
	//��¼��һ�Ӧ
	pDesk->sGameData.T_CPGNotify[gang.iStation].bHaveReturn = true;

	//��ǰ���Ƶ����
	pDesk->sUserData.m_byNowOutStation = gang.iStation;

	////����ǲ��ܾ�Ҫ����Ƿ��������ܺ� 
	if(byGangType == AT_QUADRUPLET && CheckAction(gang.iStation,gang.iOutpai,3))
	{
		bool bHaveSend = false;	//�Ƿ��Ѿ���������Ϣ
		BYTE byCanActionDesk = 255;	//���������������
		//�������ܺ� ��ô�ͷ���������Ϣ
		for(int i=0; i<PLAY_COUNT; i++)
		{
			if(i == gang.iStation)
			{
				continue;
			}
			if(pDesk->sGameData.T_CPGNotify[i].bHu)
			{
				/*pDesk->SendGameDataEx(i, &pDesk->sGameData.T_CPGNotify ,sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);
				pDesk->SendWatchDataEx(i, &pDesk->sGameData.T_CPGNotify ,sizeof(pDesk->sGameData.T_CPGNotify), MDM_GM_GAME_NOTIFY, THING_CPG_NOTIFY, 0);*/

				HaveAction action;
				action.byDeskStation = pDesk->sUserData.m_byNowOutStation;
				memcpy(action.T_CPGNotify,pDesk->sGameData.T_CPGNotify,sizeof(action.T_CPGNotify));
				for(int j = 0; j < PLAY_COUNT; j++)
				{
					pDesk->SendGameDataEx(j, &action, sizeof(HaveAction), MDM_GM_GAME_NOTIFY, THING_HAVE_ACTION, 0);
				}
				bHaveSend		= true;	//�Ƿ��Ѿ���������Ϣ
				byCanActionDesk = i;	//���������������
				break;
			}
		}
		if(bHaveSend)
		{
			SetDoingThing(byCanActionDesk,THING_HU_PAI);
		}
		else
		{
			//ץ���¼�
			pDesk->sGameData.ApplyThingID = THING_ZHUA_PAI;
			ApplyAction(THING_ZHUA_PAI,100);
		}
	}
	else
	{
		//ץ���¼�
		pDesk->sGameData.ApplyThingID = THING_ZHUA_PAI;
		ApplyAction(THING_ZHUA_PAI,100);
	}
}

/*--------------------------------------------------------------------------------------*/
/*	��ʱ������Ⱥ
/*--------------------------------------------------------------------------------------*/
/// �����ʱ����Ϣ	DWJ
void	ThingHandleEx::OnGameTimer(UINT uTimerID)
{
	switch(uTimerID)
	{
	case 10://�¼���ʱ��id
		{
			pDesk->KillTimer(uTimerID);
			GameCPU(pDesk->sGameData.ApplyThingID);
			break;  
		}
	case TIME_WAIT_MSG: // �����¼�����Ӧ���ǳ�ʱ�Զ�����ϵͳ�Զ�����
		{
			OutTimeHandle();
			break;
		}





	//case TIME_CHECK_GAME_MSG://��Ϣ��ʱ���Զ�����
	//	{
	//		//pDesk->KillTimer(uTimerID);
	//		//AutoHandle();
	//		ThingHandle::OnGameTimer( uTimerID);
	//	}
	//	break;

	}
}

/*--------------------------------------------------------------------------------------*/
//	���õ�ǰ���ڷ������¼�	DWJ
void	ThingHandleEx::SetDoingThing(BYTE byStation,BYTE thing, BYTE byTime)
{
	pDesk->sGameData.m_byWatingThing = thing;
	// �ָ���ʱ����
	ReSetOutTime();
	switch(thing)
	{		
	case THING_BEGIN_OUT://ׯ�ҿ�ʼ����֪ͨ�¼�		
		{
			if(255 == byTime)
			{
				SetOutTime(byStation,pDesk->sGameData.m_mjRule.byOutTime,thing,true);
			}
			else
			{
				SetOutTime(byStation,byTime,thing,true);
			}
			
			break;
		}
	case THING_OUT_PAI://�����¼�
		{
			//������ �����й��� 1�����
			if(pDesk->sUserData.m_bTing[byStation] || pDesk->sUserData.m_bTuoGuan[byStation])
			{
				SetOutTime(byStation,pDesk->sGameData.m_mjRule.byAutoOutTime,thing,true);
			}
			else
			{
				if(255 == byTime)
				{
					SetOutTime(byStation,pDesk->sGameData.m_mjRule.byOutTime,thing,true);
				}
				else
				{
					SetOutTime(byStation,byTime,thing,true);
				}
			}
			break;
		}
	case THING_CHI_PAI:		//�����¼�				DWJ
	case THING_PENG_PAI:	//�����¼�				DWJ
	case THING_GANG_PAI:	//�����¼�				DWJ
		{
			//������ �����й��� 1�봦��
			if(pDesk->sUserData.m_bTing[byStation] || pDesk->sUserData.m_bTuoGuan[byStation])
			{
				SetOutTime(byStation,pDesk->sGameData.m_mjRule.byAutoOutTime,thing,true);
			}
			else
			{
				if(255 == byTime)
				{
					SetOutTime(byStation,pDesk->sGameData.m_mjRule.byBlockTime,thing,true);
				}
				else
				{
					SetOutTime(byStation,byTime,thing,true);
				}
			}
			break;
		}
	case THING_HU_PAI:		//�����¼�				DWJ
		{
			//�й��� 1�����
			if(pDesk->sUserData.m_bTuoGuan[byStation])
			{
				SetOutTime(byStation,pDesk->sGameData.m_mjRule.byAutoOutTime,thing,true);
			}
			else
			{
				if(255 == byTime)
				{
					SetOutTime(byStation,pDesk->sGameData.m_mjRule.byBlockTime,thing,true);
				}
				else
				{
					SetOutTime(byStation,byTime,thing,true);
				}
			}
			break;
		}
	case THING_MAI_MA:	//�����¼�
		{
			SetOutTime(byStation,byTime,thing,true);
			break;
		}
	}
}
/*--------------------------------------------------------------------------------------*/
/// �ָ���ʱ����	DWJ
void	ThingHandleEx::ReSetOutTime()
{
	pDesk->sGameData.m_MjWait.Clear();
}
/*--------------------------------------------------------------------------------------*/
/// ���ó�ʱ����
void	ThingHandleEx::SetOutTime(BYTE station,BYTE time,BYTE thing,bool effect)
{
	pDesk->sGameData.m_MjWait.byWaitThing	= thing;
	pDesk->sGameData.m_MjWait.byCurChair	= station;
	pDesk->sGameData.m_MjWait.byTime		= time + 1;
	pDesk->sGameData.m_MjWait.bEffect		= effect;
}
/*--------------------------------------------------------------------------------------*/
/// ��ʱ����
void	ThingHandleEx::OutTimeHandle()
{
	if(!pDesk->sGameData.m_MjWait.bEffect || pDesk->sGameData.m_MjWait.byCurChair == 255 || pDesk->sGameData.m_MjWait.byTime == 255)
	{
		return;
	}
	if(pDesk->sGameData.m_MjWait.byWaitThing != pDesk->sGameData.m_byWatingThing)
	{
		return;
	}
	if(pDesk->sGameData.m_MjWait.byTime == 255)
	{
		return;
	}
	pDesk->sGameData.m_MjWait.byTime -= 1;
	if(pDesk->sGameData.m_MjWait.byTime == 0)
	{
		if(!pDesk->sUserData.m_bTuoGuan[pDesk->sGameData.m_MjWait.byCurChair])
			pDesk->m_byAutoTime[pDesk->sGameData.m_MjWait.byCurChair]++;
		if(pDesk->m_byAutoTime[pDesk->sGameData.m_MjWait.byCurChair] > 1 && !pDesk->sUserData.m_bTuoGuan[pDesk->sGameData.m_MjWait.byCurChair])
		{
			pDesk->sUserData.m_bTuoGuan[pDesk->sGameData.m_MjWait.byCurChair] = true;
			UserTouGuan	TUserTouGuan;
			TUserTouGuan.byStation	= pDesk->sGameData.m_MjWait.byCurChair;
			TUserTouGuan.bIsTuoGuan	= true;
			for(int i=0;i<PLAY_COUNT;++i)
			{
				TUserTouGuan.bTuoGuan[i] = pDesk->sUserData.m_bTuoGuan[i];		
			}
			for(int i=0;i<PLAY_COUNT;i++)
			{
				pDesk->SendGameDataEx(i,&TUserTouGuan,sizeof(TUserTouGuan),MDM_GM_GAME_NOTIFY,ASS_AUTOCARD,0);
				pDesk->SendWatchDataEx(i,&TUserTouGuan,sizeof(TUserTouGuan),MDM_GM_GAME_NOTIFY,ASS_AUTOCARD,0);
			}
		}
		this->AutoHandle();
	}
}
/*--------------------------------------------------------------------------------------*/
//��Ҵ���ʱ��ϵͳ�Զ�����	DWJ
void	ThingHandleEx::AutoHandle()
{
	switch(pDesk->sGameData.m_byWatingThing)
	{	
	case THING_BEGIN_OUT:	//ׯ�ҿ�ʼ�����Զ�������Ӧ
		{
			//ϵͳ������ҳ���
			AutoOutOrGet();
			break;
		}
	case THING_OUT_PAI:		//����Զ�����		DWJ
		{
			//ϵͳ������ҳ���
			AutoOutOrGet();
			break;
		}
	case THING_CHI_PAI:		//�Զ�������Ӧ		DWJ
	case THING_PENG_PAI:	//�Զ�������Ӧ		DWJ
	case THING_GANG_PAI:	//�Զ�������Ӧ		DWJ
		{
			//ϵͳ���������������
			AutoDoAction();
			break;
		}
	case THING_HU_PAI:		//�Զ�������Ӧ		DWJ
		{
			AutoHuPai();
			break;
		}
	case THING_MAI_MA:	//�Զ����봦��
		{
			ApplyAction(THING_ZHONG_NIAO,10);
			break;
		}
	}
}
/*--------------------------------------------------------------------------------------*/
///�Զ�������ƻ���ץ��
void	ThingHandleEx::AutoOutOrGet()
{
	bool haveout = false;
	for(int i=0;i<PLAY_COUNT;++i)
	{
		if(!pDesk->sUserData.IsOutPaiPeople(i))//�������
		{
			continue;
		}
		haveout = true;
		tagOutPaiEx outpai;
		outpai.Clear();
		outpai.byUser = i;
		outpai.byPs = pDesk->sUserData.GetLastHandPai(i);

		ReceiveUserOutCard(i,&outpai,sizeof(outpai),false);
		break;

	}
	//û�г��Ƶ���ң�����ץ���¼�
	if(!haveout)
	{
		pDesk->sGameData.ApplyThingID = THING_ZHUA_PAI;
		ApplyAction(pDesk->sGameData.ApplyThingID,200);//������һ���¼�
	}
}
/*--------------------------------------------------------------------------------------*/
//	�Զ�������	DWJ
void	ThingHandleEx::AutoDoAction()
{
	//����Ҫ��������͸��ͻ��� ���߿ͻ��� ��ʱ���� ϵͳ�Զ�������
	ReceiveUserPassCheck(pDesk->sGameData.m_MjWait.byCurChair,NULL,0,false,true);

}
/*--------------------------------------------------------------------------------------*/
//	�Զ��������	DWJ
void	ThingHandleEx::AutoHuPai()
{
	//�������
	tagHuPaiEx hu;
	hu.Clear();
	hu.byUser	= pDesk->sGameData.m_MjWait.byCurChair;
	ReceiveUserHuCard(pDesk->sGameData.m_MjWait.byCurChair, &hu, sizeof(hu), false);
}