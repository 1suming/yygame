#include "stdafx.h"
#include "NewGameClient.h"
#include "IGameFrame.h"
#include "Define.h"
#include "UI_ForLogic/GameIDDefines.h"
#include "Common/UIMessageDefines.h"
#include "..\GameMessage\UpgradeMessage.h"
#include "UI8_ControlsDefine.h"

#include "SoundDefines.h"

#include "LoveSendClassInExe.h"



#define GETCTRL(CTRL_TYPE,PRE,pUI,CTRL_ID)\
	(( PRE = dynamic_cast<CTRL_TYPE *>(pUI->GetIControlbyID(CTRL_ID)) )!=NULL)

//���ֽڱ���ת���ɿ��ֽڱ���
void char2Wchar(char* pchar,wchar_t* wchar)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP,0,pchar,-1,NULL,0);
	MultiByteToWideChar(CP_ACP,0,pchar,-1,wchar,nwLen);
}

//���ֽڱ���ת���ɶ��ֽڱ���
void wchar2char(wchar_t* wchar,char* pchar)
{
	int len = WideCharToMultiByte(CP_ACP,0,wchar,(int)wcslen(wchar),NULL,0,NULL,NULL);   
	WideCharToMultiByte(CP_ACP,0,wchar,(int)wcslen(wchar),pchar,len,NULL,NULL);
}

IGameImpl *CreateNewGameImpl(IGameFrame* pIBase, IUserInterface *pIUI)
{
    return dynamic_cast<IGameImpl*>(new CNewGameClient(pIBase, pIUI));
}
void DestroyNewGameImpl(IGameImpl **ppImp)
{
   if (*ppImp!=NULL)
    {
        delete *ppImp;
        *ppImp = NULL;
    }
}

CNewGameClient::CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI)
{
	//��Ϸ���
    m_pGameFrame = pGameFrame;
	//������������
	m_SoundEnginBg = createIrrKlangDevice();
	//��Ч����
	m_SoundEngineGame = createIrrKlangDevice();
	//��Ϸ����������Ч����
	m_bBgMusic = true;
	//��Ϸ��Ч����
	m_bGameSound = true;
	//��ϷUI
    m_pUI = pUI;
	//��Ϸ״̬
 	m_bGameStation = GS_WAIT_SETGAME;

	m_bIsSuperUser = false;

	m_nDeskStation = 0;
	m_GameData.InitData();
	m_UserData.InitData();
}

CNewGameClient::~CNewGameClient()
{
	//�رձ�����������
	if(m_SoundEnginBg != NULL)
	{
		m_SoundEnginBg->drop();
	}
	//�ر���Ϸ��Ч����
	if(m_SoundEngineGame != NULL)
	{
		m_SoundEngineGame->drop();
	}
}

/*-----------------------------------------------------------------------------------------------------------*/
/*	һϵ��ƽ̨�ṩ����Ҫ������������Ϸ�ĺ���ʵ�֣������ô������Ϸ�Ŀ�������ÿ��������ʵ������Ϸ���졣		 */
/*-----------------------------------------------------------------------------------------------------------*/
//��ʼ��
int CNewGameClient::Initial()
{
	wchar_t wszUIFile[MAX_PATH];
	::swprintf_s(wszUIFile,L"%d_UI.dat",NAME_ID);
	m_pUI->LoadData(wszUIFile);
	LoadGameSet();
    return 0;
}
/*----------------------------------------------------------------------------------------------*/
//��ʼ��UI
int CNewGameClient::InitUI()
{		

	ReStartGameInitUI();
	for(int i=0; i<PLAY_COUNT; i++)
	{
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010010+i,false);
		//���������Ϣ�ؼ�
		UIShowUserInfo(i,false);
	}
	
	return 0;
}
/*----------------------------------------------------------------------------------------------*/
///���¿���ʱ�ĳ�ʼ��
void CNewGameClient::ReStartGameInitUI()
{
	//��ʼ��ť
	SetBtVisible(CTN_1000000_BTN_10001,false);
	//ʱ���
	SetTimePlay(CTN_1000000_CTN_2006000_Time_2006001,false,0);
	///���ϽǶ���X��
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_20020100,false);
	///���Ͻǻ�ׯX
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_20020101,false);
	///���Ͻǻ�ׯ����
	SetInumInfo(CTN_1000000_CTN_2002000_Num_20020102,0,false);
	ShowSuperDlag(false);
	for(int i=0; i<PLAY_COUNT; i++)
	{
		//ʱ�����ĸ�����
		SetImageVisible(CTN_1000000_CTN_2006000_IMG_2006010+i,false);
		//׼����ʶ
		//SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010010+i,false);
		//ׯ�ұ�ʶ
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010020+i,false);
		//�йܱ�ʶ
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010030+i,false);
		//����������ʶ
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010040+i,false);
		//������ͼ
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010050+i,false);
		//����
		SetInumInfo(CTN_1000000_CTN_2002000_Num_2010060+i,0,false);
		//���Ʊ�ʶ
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010070+i,false);
		//�йܰ�ť
		SetBtVisible(CTN_1000000_BTN_2010101,true);
		//ȡ���йܰ�ť
		SetBtVisible(CTN_1000000_BTN_2010102,false);

	
		//��������е�������Ϣ ��һ�������
		ClearMjHandBlock(CTN_1000000_CTN_2005000_MjHandCtrl_2005010 + i);
		//�齫����
		SetMjHandVisible(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+i,false);

		//����������
		SetMjHandLayDown(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+i,false);
		//�������һ�Ų�����
		SetMjHandGiveMode(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+i,false);
		//��ǽ�ؼ�
		SetMjWallInfo(CTN_1000000_CTN_2005000_MjWallCtrl_2005030+i,0,false);
		//���ƿؼ�
		SetMjGiveInfo(CTN_1000000_CTN_2005000_MjGiveCtrl_2005020+i,8,NULL,0,false);	
	}

	//��������
	UIShowBlockContain(false);
	//��������
	UIShowChiContain(false);
	//�����
	UIShowResultDlg(false);
	//��������
	ShowMaiMaContain(false);

	//���ƶ���
	POINT point;
	point.x = point.y = 0;
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000003,false,point,false);
	
}
/*----------------------------------------------------------------------------------------------*/
//��Ϸ��Ϣ	DWJ
int		CNewGameClient::HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen)
{
    switch(nSubId)
	{
	case ASS_GM_GAME_STATION:	///<����״̬(����������Ϣ)	DWJ
		{
			SetGameStation(buffer,nLen);
			break;
		}
	case ASS_GM_AGREE_GAME:		//	���ͬ����Ϣ	DWJ
		{
			OnUserAgree( buffer,  nLen);           
			break;
		}
	case ASS_AUTOCARD:			//����й�֪ͨ/// �й���Ϣ
		{
			OnUserTuoguan( buffer,  nLen);
			break;
		}
	case THING_GAME_BEGIN:		//	��Ϸ��ʼ�¼�	DWJ
		{
			OnGameStart( buffer,  nLen);
			break;
		}
	case THING_DING_NT:			//	��׮��Ϣ	DWJ
		{
			OnDingNt( buffer,  nLen);					
			break;
		}
	case THING_2SEZI_NT:        //��һ��ɸ�ӣ�ȷ��ׯ�ҷ���
		{
			On2SeZiNT(buffer,nLen);
			break;
		}
	case THING_2SEZI_DIR:		//�ڶ���ɸ�� ȷ�����Ʒ�����Ϣ	DWJ
		{
			if(m_GameData.m_byNtStation == 255)
				break;
			On2SeZiDir(buffer,  nLen);
			break;
		}
	case THING_2SEZI_GP://	������ɸ��ȷ�����ƶ�����Ϣ	DWJ
		{
			if(m_GameData.m_byNtStation == 255)
				break;
			On2SeZiGP(buffer,nLen);
			break;
		}
	case THING_SEND_PAI:		//	������Ϣ	DWJ
		{
			if(m_GameData.m_byNtStation == 255)
				break;
			OnSendAllCard( buffer,  nLen);    
			break;
		}
	case THING_ALL_BUHUA:		//	������Ҳ����¼�	DWJ
		{
			OnAllBuHua( buffer,  nLen);
			break;
		}
	case THING_BEGIN_OUT:		//	��ʼ����֪ͨ	DWJ
		{
			OnBeginOutCard( buffer,  nLen);              
			break;
		}
	case THING_OUT_PAI:			//	��ҳ�����Ϣ	DWJ
		{
			OnNotifyOutCard( buffer,  nLen);          
			break;
		}
	case THING_CPG_NOTIFY:		//	�ԡ������ܡ������ƶ�����Ϣ
		{
			//OnNotifyBlock( buffer,  nLen);         
			break;
		}
	case THING_HAVE_ACTION:
		{
			OnNotifyAction(buffer,nLen);
			break;
		}
	case THING_ZHUA_PAI:		//	ץ����Ϣ	
		{
			OnNotifyZhuaPai( buffer,  nLen);          
			break;
		}
	case THING_ONE_BUHUA:		//	������Ҳ���
		{
			OnNotifyOneBuHua( buffer,  nLen);          
			break;
		}
	case THING_GUO_PAI:			//	��������	
		{
			OnNotifyCanCelBlock(buffer,  nLen);
			break;
		}
	case THING_CHI_PAI:			//	���ƽ����Ϣ
		{
			OnNotifyChiPai( buffer,  nLen);           
			break;
		}
	case THING_PENG_PAI:		//	���ƽ����Ϣ
		{
			OnNotifyPengPai( buffer,  nLen);          
			break;
		}
	case THING_GANG_PAI:		//	���ƽ����Ϣ
		{
			OnNotifyGangPai( buffer,  nLen);          
			break;
		}
	case THING_HU_PAI:			 //	���ƽ����Ϣ
		{
			OnNotifyHuPai( buffer,  nLen);            
			break;
		}
	case THING_MAI_MA:	//����
		{
			OnNotifyMaiMa(buffer,  nLen);
			break;
		}
	case THING_ZHONG_NIAO:
		{
			OnNotifyZhongNiao(buffer,nLen);
			break;
		}
	case THING_ENG_HANDLE:		/// ��֣���Ϸ����
		{
			OnNotifyFinish( buffer,  nLen);         
			break;
		}
	case ASS_CHANGE_PAI_RESULT:	//��һ��ƽ��
		{
			OnUserChangeCard(buffer,nLen);
			break;
		}
	default:
		{
			break;
		}
	}
    return 0;
}
/*----------------------------------------------------------------------------------------------*/
//������Ϣ	
int		CNewGameClient::OnUIMessage(TUIMessage* pMessage)
{
	if(pMessage->_uMessageType==UI_MJCTRL_MOVEONMJ) // �齫�ؼ���Ϣ���ڽ�ֹʹ���������£��Ƶ����Ͼͷ���Ϣ
    {
        OnMoveOnMjCtrl(pMessage);
    }
    else if (pMessage->_uMessageType==UI_MJCTRL_LBUTTON)
    {
        OnLButtonOnMjCtrl(pMessage);
    }
	else if(pMessage->_uMessageType == UI_MOUSEENTER)	//��������Ƴ�
	{
		switch (pMessage->_uControlID)
		{
		case CTN_1_BTN_10001:
		case CTN_1000000_BTN_2010101:				///<�Ϲ�  ��Ť GameGrand\Bn_tuoguan.png
		case CTN_1000000_BTN_2010102 :				///<ȡ���Ϲ�  ��Ť GameGrand\Bn_quxiaotuoguan.png
		case CTN_1000000_CTN_2003000_BTN_2003001:   ///<��  ��Ť GameGrand\guo_bt.png
		case CTN_1000000_CTN_2003000_BTN_2003010:	///<��  ��Ť GameGrand\chi_bt.png
		case CTN_1000000_CTN_2003000_BTN_2003011:   ///<��  ��Ť GameGrand\peng_bt.png
		case CTN_1000000_CTN_2003000_BTN_2003012:   ///<��  ��Ť GameGrand\gang_bt.png
		case CTN_1000000_CTN_2003000_BTN_2003013:   ///<��  ��Ť GameGrand\gang_bt.png
		case CTN_1000000_CTN_2003000_BTN_2003014:   ///<��  ��Ť GameGrand\hu_bt.png
			{
				PlayGameSound(m_nDeskStation,SOUND_MOUSE_ENTER);
				break;
			}
		case CTN_1000000_CTN_2007000_MjHandCtrl_20070011:
			{
				m_bMouseEnter = !m_bMouseEnter;
				//��ʾ���Ƶ�ͼ
				SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070021,m_bMouseEnter);
				SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070022,false);
				SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070023,false);
				break;
			}
		case CTN_1000000_CTN_2007000_MjHandCtrl_20070012:
			{
				m_bMouseEnter = !m_bMouseEnter;
				SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070021,false);
				SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070022,m_bMouseEnter);
				SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070023,false);
				break;
			}
		case CTN_1000000_CTN_2007000_MjHandCtrl_20070013:
			{
				m_bMouseEnter = !m_bMouseEnter;
				SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070021,false);
				SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070022,false);
				SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070023,m_bMouseEnter);
				break;
			}
		case CTN_1000000_CTN_2008000_MjHandCtrl_20080011:
			{
				m_bMouseEnter = !m_bMouseEnter;
				SetImageVisible(CTN_1000000_CTN_2008000_IMG_20080021,m_bMouseEnter);
				SetImageVisible(CTN_1000000_CTN_2008000_IMG_20080022,false);
				break;
			}
		case CTN_1000000_CTN_2008000_MjHandCtrl_20080012:
			{
				m_bMouseEnter = !m_bMouseEnter;
				SetImageVisible(CTN_1000000_CTN_2008000_IMG_20080021,false);
				SetImageVisible(CTN_1000000_CTN_2008000_IMG_20080022,m_bMouseEnter);
				break;
			}
		case CTN_1000000_CTN_2009000_MjHandCtrl_20090011:
			{
				m_bMouseEnter = !m_bMouseEnter;
				SetImageVisible(CTN_1000000_CTN_2009000_IMG_20090021,m_bMouseEnter);
				break;
			}
		}
	}
	else if(pMessage->_uMessageType==UI_RBUTTONDOWN)
	{
		//�������״̬
		if(m_byHandState == 2)	
		{
			//��������ѡ�����״̬
			SetMjHandInvalidTile(CTN_1000000_CTN_2005000_MjHandCtrl_2005012,NULL,0);
			//����״̬
			m_byHandState = 0xFF;
		}
		else if(m_byHandState == 1)
		{
			//���س�������
			UIShowChiContain(false);
			//��ʾ��������
			UIShowBlockContain(true);
			//����״̬
			m_byHandState = 0xFF;
		}
	}
	else if(pMessage->_uMessageType == UI_LBUTTONDOWN) // ����
	{
		switch (pMessage->_uControlID)
		{
        case CTN_1000000_CTN_2005000_MjHandCtrl_2005012:       // �Լ�����	DWJ
            {
				if(!m_UserData.m_bTuoGuan[m_nDeskStation])//�ֶ����ƣ����ּ��㳬ʱ����
				{
					m_nTimeOutCnt = 0;
				}
				//������ || �й��� ���ܳ���
				if(m_UserData.m_bTing[m_nDeskStation])
				{
					return 0;
				}

				//������ || �й��� ���ܳ���
				if(m_UserData.m_bTuoGuan[m_nDeskStation])
				{
					if(m_UserData.m_byNowOutStation == m_nDeskStation)
					{
						//m_pGameFrame->InsertNormalMessage("��������ȡ���й�!");
					}
					
					return 0;
				}

				//ץ�����Ƶ�ʱ�� ���ܳ���
				if(m_bIsHua)
				{
					return 0;
				}
				
				if(m_byHandState == 0xFF)
				{
					// �������                
					OnUIRequestOutCard(pMessage->_uControlID); 
				}
				else if(m_byHandState == 2)
				{
					//�������
					OnUIRequestGangPai(pMessage->_uControlID);
				}
				break;
            }
		case CTN_1000000_CTN_2007000_MjHandCtrl_20070011:
		case CTN_1000000_CTN_2008000_MjHandCtrl_20080011:
		case CTN_1000000_CTN_2009000_MjHandCtrl_20090011:	//���ȷ�����ƶ���
			{
				if(1 == m_byHandState)
				{
					//�������
					RequestBlockChi(0);
				}
				break;
			}
		case CTN_1000000_CTN_2007000_MjHandCtrl_20070012:
		case CTN_1000000_CTN_2008000_MjHandCtrl_20080012:	//���ȷ�����ƶ���
			{
				if(1 == m_byHandState)
				{
					//�������
					RequestBlockChi(1);
				}
				break;
			}
		case CTN_1000000_CTN_2007000_MjHandCtrl_20070013:	//���ȷ�����ƶ���
			{
				//�������״̬
				if(1 == m_byHandState)
				{
					//�������
					RequestBlockChi(2);
				}
				break;
			}
		case CTN_1000000_CTN_2007000_BTN_20070002:
		case CTN_1000000_CTN_2008000_BTN_20080002:
		case CTN_1000000_CTN_2009000_BTN_20090002:	//ȡ����
			{
				//���س�������
				UIShowChiContain(false);
				//��ʾ��������
				UIShowBlockContain(true);
				//����״̬
				m_byHandState = 0xFF;
				break;
			}
		case CTN_1000000_CTN_2003000_BTN_2003001:	//��
			{
				if(GetBtIsEnable(CTN_1000000_CTN_2003000_BTN_2003001))
				{
					m_nTimeOutCnt = 0;
					RequestCancelBlock();
				}
				break;
			}
		case CTN_1000000_CTN_2003000_BTN_2003010:	//�������
			{
				if(GetBtIsEnable(CTN_1000000_CTN_2003000_BTN_2003010))
				{
					//dwjtest �һ�ȡ������
					OnUiClickChiPai();
				}
				break;
			}
		case CTN_1000000_CTN_2003000_BTN_2003011:	//��������
			{
				if(GetBtIsEnable(CTN_1000000_CTN_2003000_BTN_2003011))
				{
					//	��������
					m_nTimeOutCnt = 0;
					RequestBlockPeng();
				}
				break;
			}
		case CTN_1000000_CTN_2003000_BTN_2003012:	//�������
			{
				if(GetBtIsEnable(CTN_1000000_CTN_2003000_BTN_2003012))
				{
					//	�������
					m_nTimeOutCnt = 0;
					RequestBlockGang();
				}
				break;
			}
		case CTN_1000000_CTN_2003000_BTN_2003013:	//��������
			{
				if(GetBtIsEnable(CTN_1000000_CTN_2003000_BTN_2003013))
				{
					//	��������
					RequestBlockTing();
				}
				break;
			}
		case CTN_1000000_CTN_2003000_BTN_2003014:	//�������
			{
				if(GetBtIsEnable(CTN_1000000_CTN_2003000_BTN_2003014))
				{
					// �������
					m_nTimeOutCnt = 0;
					RequestBlockHu();
				}
				break;
			}
		case 2003015://���ܺ�
			{
				if(GetBtIsEnable(2003015))
				{
					// �������ܺ�
					RequestBlockHu();
				}
				break;
			}
		case CTN_1000000_BTN_2010101:	//�����й�
			{
				RequestTuoGuan();
				break;
			}
		case CTN_1000000_BTN_2010102:	//����ȡ���й�
			{
				RequestTuoGuan(false);
				break;
			}
		case CTN_1000000_CTN_2001000_BTN_2001101:	//�뿪
			{
				m_pGameFrame->CloseClient();
				break;
			}
		case CTN_1000000_CTN_2001000_BTN_2001102:	//����
			{
				InitGameDate();
				ReStartGameInitUI();
				m_pGameFrame->ClickContinue();
				break;
			}
		case CTN_1000000_BTN_1003500: //������Ϸ
			{
				//��ʾ��Ϸ����
				UIShowGameSetting(true);
				break;
			}
		case CTN_100000078_BTN_100000088:	//ȷ������
			{
				SaveGameSet();
				//��ʾ��Ϸ����
				UIShowGameSetting(false);
				break;
			}
		case CTN_100000078_BTN_100000089:	//ȡ������
			{
				//��ʾ��Ϸ����
				UIShowGameSetting(false);
				break;
			}
		case CTN_1000000_CTN_2007000_MjHandCtrl_20070001:
		case CTN_1000000_CTN_2007000_MjHandCtrl_20070002:
		case CTN_1000000_CTN_2007000_MjHandCtrl_20070003:
			{
				// ��ʾ���������
				int iUpTile = 0;
				int iLogicTile = 0;

				IMjHandCtrl *pMjHand = NULL;
				pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(pMessage->_uControlID));
				if (NULL!=pMjHand)
				{
					iUpTile = pMjHand->GetUpTile();

				}  
				if (iUpTile != 0)
				{
					//����Ϊ��ѡ�е��齫��
					SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070022,&iUpTile,1,true);
				}
				break;
			}
		case CTN_1000000_CTN_2007000_MjHandCtrl_20070031:	//�����Լ�������
			{
				int iUpTile = 0;

				IMjHandCtrl *pMjHand = NULL;
				pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(CTN_1000000_CTN_2007000_MjHandCtrl_20070031));
				if (NULL!=pMjHand)
				{
					iUpTile = pMjHand->GetUpTile();

				}  
				if (iUpTile != 0)
				{
					//����Ϊ��ѡ�е��齫��
					SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070021,&iUpTile,1,true);
				}

				break;
			}
		case CTN_1000000_CTN_2007000_BTN_20070051:		//����ȷ��
			{
				OnUiChangeCard();
				break;
			}
		case CTN_1000000_CTN_2007000_BTN_20070052:		//����ȡ��
			{
				ShowSuperDlag(false);
				break;
			}
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100011 :  ///<�齫���ƿؼ�1  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100012 :  ///<�齫���ƿؼ�2  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100013 :  ///<�齫���ƿؼ�3  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100014 :  ///<�齫���ƿؼ�4  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100015 :  ///<�齫���ƿؼ�5  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100016 :  ///<�齫���ƿؼ�6  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100017 :  ///<�齫���ƿؼ�7  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100018 :  ///<�齫���ƿؼ�8  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100019 :  ///<�齫���ƿؼ�9  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100020 :  ///<�齫���ƿؼ�10  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100021 :  ///<�齫���ƿؼ�11  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100022 :  ///<�齫���ƿؼ�12  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100023 :  ///<�齫���ƿؼ�13  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100024 :  ///<�齫���ƿؼ�14  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100025 :  ///<�齫���ƿؼ�15  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100026 :  ///<�齫���ƿؼ�16  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100027 :  ///<�齫���ƿؼ�17  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100028 :  ///<�齫���ƿؼ�18  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100029 :  ///<�齫���ƿؼ�19  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100030 :  ///<�齫���ƿؼ�20  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100031 :  ///<�齫���ƿؼ�21  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100032 :  ///<�齫���ƿؼ�22  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100033 :  ///<�齫���ƿؼ�23  �齫���ƿؼ� 
		case  CTN_1000000_CTN_2010000_MjHandCtrl_20100034 :  ///<�齫���ƿؼ�24  �齫���ƿؼ� 
			{
				//�ж��Ƿ�����
				if (m_byMaiMaIndex >= m_GameData.m_TMaiMa.byCount)
				{
					break;;
				}
				//�ж��Ƿ��Ѿ�����
				if (0 != GetMjHandOneCard(pMessage->_uControlID))
				{
					break;
				}
				int iCard = m_GameData.m_TMaiMa.byCard[m_byMaiMaIndex];
				SetMjHandInfo(pMessage->_uControlID,&iCard,1,true);

				m_byMaiMaIndex++;

				if (m_byMaiMaIndex >= m_GameData.m_TMaiMa.byCount)
				{
					CString sText;
					sText.Format("�����������ȴ�ϵͳȷ�ϣ�");
					SetTextinfo(CTN_1000000_CTN_2010000_TEXT_20100002,sText,true);
				}

				break;
			}
        default:
			{
				break;
			}
        }
    }
    else if (pMessage->_uMessageType==UI_CATOONFINISH)	//��������	DWJ
    {
        OnCartoonFinish(pMessage);
//        if (ZHUAPAI_ACT_MVACT==pMessage->_uControlID && m_GameData.m_byThingDoing < THING_HU_PAI)
//        {
//            // �������ץ�ƶ���
   
//            IMjWallCtrl *pWall = NULL;
//			GETCTRL(IMjWallCtrl, pWall, m_pUI, ZHUAPAI_ACT_MJWALL);
//            if ( pWall != NULL )
//            {             
//                pWall->SetControlVisible(false);
//            }
//        }
    }
	else if(pMessage->_uMessageType==UI_KEYDOWN)
	{
		if (pMessage->_ch == VK_F12 && m_bIsSuperUser)
		{
			ShowSuperDlag(true);
		}
	}
    return 0;
}

//��ʾ��������
void	CNewGameClient::ShowMaiMaContain(bool bShow)
{
	SetContainVisible(CTN_1000000_CTN_2010000,false);
	if (bShow)
	{
		SetContainVisible(CTN_1000000_CTN_2010000,true);
		SetImageVisible(CTN_1000000_CTN_2010000_IMG_20100001,true);
		CString sText;
		sText.Format("������%d��",m_GameData.m_TMaiMa.byCount);
		SetTextinfo(CTN_1000000_CTN_2010000_TEXT_20100002,sText,true);
		int iCard = 0;
		for(int i=0; i<24; ++i)
		{
			SetMjHandInfo(CTN_1000000_CTN_2010000_MjHandCtrl_20100011+i,&iCard,1,true);
		}

		SetTimePlay(CTN_1000000_CTN_2010000_Time_20100003,true,m_GameData.m_mjRule.byOutTime,0);
	}
}

//��Ӧ����
void	CNewGameClient::OnUiChangeCard()
{
	if (!m_bIsSuperUser)
	{
		return;
	}
	tagSuperChangeCard TSuperChangeCard;
	TSuperChangeCard.byStation = m_nDeskStation;
	TSuperChangeCard.byChangeCard = GetMjHandOneCard(CTN_1000000_CTN_2007000_MjHandCtrl_20070021);
	TSuperChangeCard.byNewCard = GetMjHandOneCard(CTN_1000000_CTN_2007000_MjHandCtrl_20070022);

	if (TSuperChangeCard.byChangeCard == 0 || TSuperChangeCard.byChangeCard == 255  || TSuperChangeCard.byNewCard == 0 || TSuperChangeCard.byNewCard==255)
	{
		return;
	}

	SendGameDataEx(ASS_CHANGE_PAI, &TSuperChangeCard, sizeof(TSuperChangeCard));
}
//��ȡ�齫����һ��������
int		CNewGameClient::GetMjHandOneCard(const int iMjHandID)
{
	int iLogicTile = 0;
	IMjHandCtrl *pMjHand = NULL;
	pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(iMjHandID));
	if (NULL!=pMjHand)
	{
		pMjHand->GetHandInfo(&iLogicTile,1);

	}  
	return iLogicTile;
}
void	CNewGameClient::ShowSuperDlag(bool bFlag)
{
	SetContainVisible(CTN_1000000_CTN_2007100,bFlag);
	SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070041,bFlag);
	SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070042,bFlag);


	//��ʾ��
	int iMjCard[9];
	for(int i=0; i<9; i++)
	{
		iMjCard[i] = i+1;
	}
	SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070001,iMjCard,9,bFlag);
	//��
	for(int i=0; i<9; i++)
	{
		iMjCard[i] = 10+i+1;
	}
	SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070002,iMjCard,9,bFlag);
	//Ͳ+����
	int iMjCard1[10];
	for(int i=0; i<9; i++)
	{
		iMjCard1[i] = 20+i+1;
	}
	iMjCard1[9] = 35;
	SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070003,iMjCard1,10,bFlag);

	//�Լ��������齫
	int iHandCard[MAX_HAND_PAI];
	for (int j = 0; j < m_UserData.m_byArHandPaiCount[m_nDeskStation]; j ++)
	{
		iHandCard[j] = m_UserData.m_byArHandPai[m_nDeskStation][j];
	}
	SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070031,iHandCard, m_UserData.m_byArHandPaiCount[m_nDeskStation],bFlag);

	//�������齫
	SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070021,NULL,0,false);
	//����Ҫ���齫
	SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070022,NULL,0,false);

}
/*----------------------------------------------------------------------------------------------*/
// �������		DWJ             
void	CNewGameClient::OnUIRequestOutCard(UINT32 uControlID,bool bOuttime )
{
	// ��ʾ���������
	int iUpTile = 0;
	BYTE byLogicTile = 0;

	IMjHandCtrl *pMjHand = NULL;
	pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(uControlID));
	if (NULL!=pMjHand)
	{
		iUpTile = pMjHand->GetUpTile();
	}  

	if(iUpTile == 35)
		return;

	byLogicTile = iUpTile;
	
	if(bOuttime)//��ʱ����
	{
		byLogicTile = m_UserData.m_byLastZhuaPai[m_nDeskStation];
		if(byLogicTile == 255)
		{
			byLogicTile = m_UserData.GetLastHandPai(m_nDeskStation);
		}
		
		RequestOutCard(byLogicTile);
	}
	else
	{
		if (0!=byLogicTile)
		{
			RequestOutCard(byLogicTile);    
		}
	}
}
/*----------------------------------------------------------------------------------------------*/
//	��������������
void	CNewGameClient::OnUIRequestGangPai(const int iControlID)
{
	if(!m_GameData.T_CPGNotify[m_nDeskStation].bGang)
	{
		return ;   
	}

	//����Ҫ�ж��Ƿ���������ҿ��Ժ� ������Ժ� ��ô��Ͳ����� ����Ҫ���������ȼ���	
	bool bCanGang = true;
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(i == m_nDeskStation)
		{
			continue;
		}

		if(m_GameData.T_CPGNotify[i].bHu)
		{
			bCanGang = false;
			break;
		}
	}

	if(!bCanGang)
	{
		//������ʾDWJTEST
		return;
	}

	//��ȡ�������ֵ
	int iUpPai = 255;
	IMjHandCtrl *pMjHand = NULL;
	pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(iControlID));
	if (NULL!=pMjHand)
	{
		iUpPai = pMjHand->GetUpTile();
	}  
	if(255 == iUpPai)
	{
		return;
	}

	tagGangPaiEx gang;
	gang.Clear();
	gang.byPs		= iUpPai;
	gang.byUser		= m_nDeskStation;
	gang.byBeGang	= m_nDeskStation;
	for(int i=0; i<4; i++)
	{
		gang.byGangPai[i] = gang.byPs;
	}
	SendGameDataEx(THING_GANG_PAI, &gang, sizeof(gang));
}
/*----------------------------------------------------------------------------------------------*/
// �������	DWJ
void	CNewGameClient::RequestOutCard(BYTE byData)
{
	if (m_UserData.IsOutPaiPeople(m_nDeskStation))
	{
		tagOutPaiEx TOutPai;
		TOutPai.Clear();
		TOutPai.byPs = byData;
		TOutPai.byUser = m_nDeskStation;
		TOutPai.bTing = false;//����
		SendGameDataEx(THING_OUT_PAI, &TOutPai, sizeof(TOutPai));
	}
}
/*----------------------------------------------------------------------------------------------*/
// ��������
void	CNewGameClient::RequestCancelBlock()
{

	SendGameDataEx(THING_GUO_PAI,NULL, 0);
	//������������
	UIShowBlockContain(false);
	//���ؼ�ʱ��(����������˵��ƣ��������ƺ��Ҫ���ص���ʱ)
	if(!m_GameData.T_CPGNotify[m_nDeskStation].bZhuaPai)
	{
		ShowCountDownTimer(m_nDeskStation,0,false);
	}
	
	//��������ѡ�����״̬
	SetMjHandInvalidTile(CTN_1000000_CTN_2005000_MjHandCtrl_2005012,NULL,0);
	//����״̬
	m_byHandState = 0xFF;
}
/*----------------------------------------------------------------------------------------------*/
//	��������ư�ť
void	CNewGameClient::OnUiClickChiPai()
{
	//���״̬
	m_byHandState = 1;
	//��ʾ��������
	UIShowChiContain(true);
}
/*----------------------------------------------------------------------------------------------*/
//	�������	
void	CNewGameClient::RequestBlockChi(int iIndex)
{
	if(!m_GameData.T_CPGNotify[m_nDeskStation].bChi || m_GameData.T_OutPai.byPs == 255)
	{
		return ;   
	}
	//����Ҫ�ж��Ƿ���������ҿ����� ��������� ��ô��Ͳ��ܳ� ����Ҫ���������ȼ���	
	bool bCanChi = true;
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(i == m_nDeskStation)
		{
			continue;
		}

		if(m_GameData.T_CPGNotify[i].bHu || m_GameData.T_CPGNotify[i].bGang || m_GameData.T_CPGNotify[i].bPeng)
		{
			bCanChi = false;
			break;
		}
	}
	if(bCanChi)
	{
		tagChiPaiEx chi;
		chi.Clear();
		chi.byBeChi = m_GameData.T_OutPai.byUser;
		chi.byPs	= m_GameData.T_OutPai.byPs;
		chi.byUser	= m_nDeskStation;
		for(int i=0; i<3; i++)
		{
			chi.byChiPs[i] = m_GameData.T_CPGNotify[m_nDeskStation].m_iChi[iIndex][i];
		}
		SendGameDataEx(THING_CHI_PAI, &chi, sizeof(chi));
	}
	else
	{
		//dwjtest �������ܳԵ���ʾ
	}
}
/*----------------------------------------------------------------------------------------------*/
//	��������
void	CNewGameClient::RequestBlockPeng()
{
	if(!m_GameData.T_CPGNotify[m_nDeskStation].bPeng || m_GameData.T_OutPai.byPs == 255)
	{
		return ;   
	}

	//����Ҫ�ж��Ƿ���������ҿ��Ժ� ������Ժ� ��ô��Ͳ����� ����Ҫ���������ȼ���	
	bool bCanPeng = true;
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(i == m_nDeskStation)
		{
			continue;
		}

		if(m_GameData.T_CPGNotify[i].bHu)
		{
			bCanPeng = false;
			break;
		}
	}

	if(bCanPeng)
	{
		tagPengPaiEx peng;
		peng.Clear();
		peng.byPs	  = m_GameData.T_OutPai.byPs;
		peng.byUser	  = m_nDeskStation;
		peng.byBePeng = m_GameData.T_OutPai.byUser;

		SendGameDataEx(THING_PENG_PAI, &peng, sizeof(peng));
		//���س�������
		UIShowChiContain(false);
	}
	else
	{
		//�������� ������ʾ dwjtest

	}
}
/*----------------------------------------------------------------------------------------------*/
//	�������
void	CNewGameClient::RequestBlockGang()
{
	if(!m_GameData.T_CPGNotify[m_nDeskStation].bGang)
	{
		return ;   
	}
	//����Ҫ�ж��Ƿ���������ҿ��Ժ� ������Ժ� ��ô��Ͳ����� ����Ҫ���������ȼ���	
	bool bCanGang = true;
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(i == m_nDeskStation)
		{
			continue;
		}

		if(m_GameData.T_CPGNotify[i].bHu)
		{
			bCanGang = false;
			break;
		}
	}

	if(!bCanGang)
	{
		//������ʾDWJTEST
		return;
	}
	//����ǳ��ƵĻ� ��ô���ƾ���ֻ����ҳ�����
	if(m_GameData.m_byThingDoing == THING_OUT_PAI)	//����
	{
		if(m_GameData.T_OutPai.byPs == 255)
		{
			return ;   
		}

		tagGangPaiEx gang;
		gang.Clear();
		gang.byPs		= m_GameData.T_OutPai.byPs;
		gang.byUser		= m_nDeskStation;
		gang.byBeGang	= m_GameData.T_OutPai.byUser;
		//gang.byType     = m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[0][0];
		for(int i=0; i<4; i++)
		{
			gang.byGangPai[i] = m_GameData.T_OutPai.byPs;
		}
		SendGameDataEx(THING_GANG_PAI, &gang, sizeof(gang));
		
	}
	else if(m_GameData.m_byThingDoing == THING_ZHUA_PAI || m_GameData.m_byThingDoing == THING_ONE_BUHUA || m_GameData.m_byThingDoing == THING_BEGIN_OUT)	//��ǰ��ץ�� ||����||��ʼ����
	{
		//�����Ƿ��ж����
		int iGangCount = 0;
		for(int i=0; i<4; i++)
		{
			if(m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[i][0] != 255 && m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[i][1] != 255)
			{
				iGangCount++;
			}
		}
		
		if(iGangCount > 1)
		{
			//��������ѡ�����״̬
			SetChoiceGangPai(iGangCount,true);
		}
		else	//ֻ��һ������
		{
			tagGangPaiEx gang;
			gang.Clear();
			gang.byPs		= m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[0][1];
			gang.byUser		= m_nDeskStation;
			gang.byBeGang	= m_nDeskStation;
			//gang.byType     = m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[0][0];
			for(int i=0; i<4; i++)
			{
				gang.byGangPai[i] = m_GameData.T_OutPai.byPs;
			}
			SendGameDataEx(THING_GANG_PAI, &gang, sizeof(gang));
		}
	}
	//���س�������
	UIShowChiContain(false);
}
/*----------------------------------------------------------------------------------------------*/
//	��������
void	CNewGameClient::RequestBlockTing()
{
	if(!m_GameData.T_CPGNotify[m_nDeskStation].bTing)
	{
		return ;   
	}
	
	tagTingPaiEx ting;
	ting.Clear();
	ting.byUser	= m_nDeskStation;
	ting.bTing	= true;
	SendGameDataEx(THING_TING_PAI, &ting, sizeof(ting));

	//����ѡ������״ֻ̬��ѡ���������
	SetChoiceTingPai();

}
/*----------------------------------------------------------------------------------------------*/
// �������
void CNewGameClient::RequestBlockHu()
{

	if(!m_GameData.T_CPGNotify[m_nDeskStation].bHu)
	{
		return ;   
	}

	//�������
	tagHuPaiEx hu;
	hu.Clear();
	hu.byUser	= m_nDeskStation;
	//�Ƿ���˷��� || ���ܺ�
	if(m_GameData.m_byThingDoing == THING_OUT_PAI && m_GameData.T_OutPai.byUser != 255)
	{
		hu.byDianPao = m_GameData.T_OutPai.byUser;
		hu.byPs		 = m_GameData.T_OutPai.byPs;
	}
	else if(m_GameData.m_byThingDoing == THING_GANG_PAI && m_GameData.T_GangPai.byUser != 255)
	{
		hu.byDianPao = m_GameData.T_GangPai.byUser;
		hu.byPs		 = m_GameData.T_GangPai.byPs;
	}
	else if((m_GameData.m_byThingDoing == THING_ZHUA_PAI) || (m_GameData.m_byThingDoing == THING_ONE_BUHUA) || (m_GameData.m_byThingDoing == THING_BEGIN_OUT))	//����
	{
		hu.byPs = 255;
		hu.bZimo = true;
	}
	SendGameDataEx(THING_HU_PAI, &hu, sizeof(hu));

}
/*----------------------------------------------------------------------------------------------*/
// �����й�
void CNewGameClient::RequestTuoGuan(bool bTuoguan )
{
	if(m_bGameStation != GS_PLAY_GAME)
	{
		return;
	}
	
	UserTouGuan TUserTouGuan;
	TUserTouGuan.byStation	= m_nDeskStation;
	TUserTouGuan.bIsTuoGuan = bTuoguan;
	SendGameDataEx(ASS_AUTOCARD, &TUserTouGuan, sizeof(TUserTouGuan));
}
/*----------------------------------------------------------------------------------------------*/















/*-----------------------------------------------------------------------------*/
int CNewGameClient::ResetGameFrame(void)
{
    return 0;
}
/*-----------------------------------------------------------------------------*/
//��ҽ���
int CNewGameClient::GameUserCome(void)
{
    return 0;
}
/*-----------------------------------------------------------------------------*/
//����뿪
int		CNewGameClient::GameUserLeft(int nIndex)
{
	UIShowUserInfo(nIndex,false);
    return 0;
}
/*-----------------------------------------------------------------------------*/
//��Ҷ���
int CNewGameClient::GameUserOffLine(void)
{
    return 0;
}
/*-----------------------------------------------------------------------------*/
//�Ƿ�������Ϸ������Ϸʵ��
//�Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
bool	CNewGameClient::IsPlayingGame()
{
 	if(m_bGameStation >= GS_SEND_CARD && m_bGameStation < GS_WAIT_NEXT)
	{
 		return true;
	}
	return false;
}
/*-----------------------------------------------------------------------------*/
//��ʱ����Ϣ
void CNewGameClient::OnTimer(int nTimerID)
{
	switch(nTimerID)
	{
	case TIME_ONE_SECOND:
		{
			m_GameData.m_iRemaindTime--;
			if(m_GameData.m_iRemaindTime <= 5 && m_GameData.m_iRemaindTime > 0)
			{
				PlayGameSound(m_nDeskStation,SOUND_COUNT_DOWN);
			}

			if(m_GameData.m_iRemaindTime <= 0)
			{
				m_pGameFrame->KillTimer(TIME_ONE_SECOND);
			}
			break;
		}

	case ID_BEGIN_MOPAI:
		{
			int iHandCount(0);
			if(m_byMoPaiNum < 12)
			{
				if(m_byGetPai < 24)
				{
					memset(m_byDeskMenPai[m_byDirect]+m_byGetPai,255,sizeof(BYTE) * 4);
					m_byGetPai += 4;
				}
				else if(m_byGetPai == 24)
				{
					memset(m_byDeskMenPai[m_byDirect]+m_byGetPai,255,sizeof(BYTE) * 4);
					m_byDirect = (m_byDirect+1)%PLAY_COUNT;
					m_byGetPai = 0;
				}
				else
				{
					memset(m_byDeskMenPai[m_byDirect]+m_byGetPai,255,sizeof(BYTE) * 2);
					m_byDirect = (m_byDirect+1)%PLAY_COUNT;
					m_byGetPai = 0;
					memset(m_byDeskMenPai[m_byDirect]+m_byGetPai,255,sizeof(BYTE) * 2);
					m_byGetPai += 2;
				}

				iHandCount = 4 * (m_byMoPaiNum/4+1);

			}
			else if(m_byMoPaiNum == 12)
			{
				if(m_byGetPai <= 22)
				{
					memset(m_byDeskMenPai[m_byDirect]+m_byGetPai+1,255,sizeof(BYTE));
					memset(m_byDeskMenPai[m_byDirect]+m_byGetPai+5,255,sizeof(BYTE));
				}
				else if(m_byGetPai <= 24)
				{
					memset(m_byDeskMenPai[m_byDirect]+m_byGetPai+1,255,sizeof(BYTE));
					memset(m_byDeskMenPai[m_byDirect+1]+1,255,sizeof(BYTE));
				}
				else if(m_byGetPai <= 26)
				{
					memset(m_byDeskMenPai[m_byDirect]+m_byGetPai+1,255,sizeof(BYTE));
					memset(m_byDeskMenPai[m_byDirect+1]+3,255,sizeof(BYTE));
				}
				iHandCount = 14;
			}
			else if(m_byMoPaiNum == 13)
			{
				memset(m_byDeskMenPai[m_byDirect]+m_byGetPai,255,sizeof(BYTE));
				m_byGetPai+=3;
			}
			else if(m_byMoPaiNum == 14)
			{
				if(m_byGetPai > 28)
				{
					m_byGetPai = m_byGetPai - 28;
				}
				memset(m_byDeskMenPai[m_byDirect]+m_byGetPai,255,sizeof(BYTE));
				m_byGetPai--;
			}
			else if(m_byMoPaiNum == 15)
			{
				memset(m_byDeskMenPai[m_byDirect]+m_byGetPai,255,sizeof(BYTE));
			}
			
			int iCards[MAX_HAND_PAI] = {255};
			SetMjHandInfo(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+m_byMoPaiStation,m_iDeskHandPai[m_pGameFrame->ViewStation2DeskStation(m_byMoPaiStation)], iHandCount,true);

			for(int i = 0; i < PLAY_COUNT; i++)
			{
				UIShowWall(i,m_byDeskMenPai[i]);
			}
			m_byMoPaiStation = (m_byMoPaiStation + 3)%PLAY_COUNT;
			m_byMoPaiNum++;
			if(m_byMoPaiNum >= 16)
			{
				//UIShowAllHandPai(true);
				m_pGameFrame->KillTimer(ID_BEGIN_MOPAI);
			}
			break;
		}
    case TIME_ZHUAPAI://ץ��
        {
            m_pGameFrame->KillTimer(TIME_ZHUAPAI);
            // ����������ʾ    
            for (int i=0; i<PLAY_COUNT; ++i)
            {
				if(m_UserData.m_bIsHu[i])//������Ҳ�����
					continue;
                int desk = m_pGameFrame->DeskStation2View(i);
				m_UserDataCard[desk].GetFromByte( m_UserData.m_byArHandPai[i] );   
				if(i != m_nDeskStation)
				{
					m_UserDataCard[desk].FillWithBack(m_UserData.GetHandPaiCount(i));
				}
                // �������ץ�ƶ�������������Ϊ��һ��
				// UISetHandCard(desk, m_UserDataCard[desk].m_nUserHandInfo, m_UserDataCard[desk].m_nHandCardCount, i==m_ucCurrentPlayer);
            }
			//ץ�ƶ���
			POINT pt,ptDst;   
            UIShowZhuaMoveAction(pt, ptDst, 0xFF, false);
        }
        break;
	case TIME_OUT_CARD: // ����
		{
			OnKillAllTimer();
			// �������ƿ�
			SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));
			if(m_nDeskStation != m_UserData.m_byNowOutStation)
				break;
            OnUIRequestOutCard(CTN_1_MjHandCtrl_600010,true);//����ʱ�䵽��
			//if (!m_bIsTuoGuan[m_nDeskStation])
            {
                // ��¼��ʱ����
                m_nTimeOutCnt ++;
                if (m_nTimeOutCnt>=3)
                {
                    RequestTuoGuan(true);
                }
                else
                {
                    //std::wostringstream oss;
                    //oss<<L"ע�⣬���Ѿ���ʱ"<<m_nTimeOutCnt<<L"�Σ���ʱ3�ν��Զ��й�"<<std::flush;
                    //m_pGameFrame->InsertSystemMessage((wchar_t *)oss.str().c_str());
                }
            }
			break;
		}
    case TIME_BLOCK_CARD: // ����
        {
			OnKillAllTimer();
			//�ָ����е�����ɫ
			if(m_UserData.m_byNowOutStation == m_nDeskStation)//�Լ�����
			{			
				if((m_UserData.m_bTuoGuan[m_nDeskStation] || m_UserData.m_bTing[m_nDeskStation]) && m_GameData.T_CPGNotify[m_nDeskStation].bHu)//�����й�״̬��b�����ܺ��ͺ���
				{
					RequestBlockHu();//���ͺ�������
				}
				else if(m_UserData.m_bTing[m_nDeskStation] )//�������ܸ�
				{
					
				}
			
			}
			else//�����˵���
			{           
				if((m_UserData.m_bTuoGuan[m_nDeskStation] || m_UserData.m_bTing[m_nDeskStation]) && m_GameData.T_CPGNotify[m_nDeskStation].bHu)//�����й�״̬��b�����ܺ��ͺ���
				{
					RequestBlockHu();//���ͺ�������
				}
				else if(m_UserData.m_bTing[m_nDeskStation] )//�������ܸ�
				{
					RequestBlockGang();//���͸�����
				}
				else
				{
					RequestCancelBlock();
				}
			}
            break;
        }
	case TIME_SHOW_HU:// ��ʾ���ƶԻ���ʱ��
		{
            m_pGameFrame->KillTimer(TIME_SHOW_HU);
			SAFE_CTRL_OPERATE(IContain, CTN_HUPAI_DLG, SetControlVisible(false));//���غ��ƶԻ���
		}
		break;
	case TIME_SHOW_NIAO:
		{
			static bool bShow = true;
			for(int i = 0; i < 4; i++)
			{
				BYTE card = m_GameData.m_ZhongNiao.byCard[i];
				if(((card % 10)== 2 || (card % 10)== 3 || (card )== 35))
				{
					IImage* pImage = NULL;
					pImage = dynamic_cast<IImage*>(m_pUI->GetIControlbyID(2008106+i));
					pImage->SetControlVisible(bShow);
				}
			}
			bShow = !bShow;
		}
		break;
	}
}
/*----------------------------------------------------------------------------------------------*/
//��״̬
void	CNewGameClient::SetStationParameter(BYTE bGameStation)
{
	m_bGameStation = bGameStation;
}
/*----------------------------------------------------------------------------------------------*/
//������Ϸ״̬����Ҫ�Ƕ����������������Ϣ
void CNewGameClient::SetGameStation(void* pBuffer,int nLen)
{
	GameStation_Base *pGameStation = (GameStation_Base *) pBuffer;
	//������Ϸ״̬
	SetStationParameter(pGameStation->byGameStation);
	//�����������
	m_GameData.m_mjRule.byBeginTime		= pGameStation->byBeginTime;	//��ʼ�ȴ�ʱ��
	m_GameData.m_mjRule.byOutTime		= pGameStation->byOutTime;		//����ʱ��
	m_GameData.m_mjRule.byBlockTime		= pGameStation->byBlockTime;	//����˼��ʱ��
	m_GameData.m_mjRule.bForceTuoGuan	= pGameStation->bForceTuoGuan;	//ǿ���Ƿ��й�
	m_bIsSuperUser = pGameStation->bIsSuperUser;


	m_nDeskStation = m_pGameFrame->GetMyDeskStation();

	CString cs;
	cs.Format("llj::pGameStation->byGameStation = %d",pGameStation->byGameStation);
	OutputDebugString(cs);

	switch(pGameStation->byGameStation)
    {
	case GS_WAIT_SETGAME:	//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:		//�ȴ���ҿ�ʼ״̬
	case GS_WAIT_NEXT:		//�ȴ���һ����Ϸ��ʼ
		{
			if (nLen != sizeof(GameStation_Wait_Agree))
			{
				return;
			}

			GameStation_Wait_Agree *pGameStation = (GameStation_Wait_Agree *)pBuffer;
			if (NULL == pGameStation)
			{
				return;
			}
			//��ʾ����ҵ�׼����ʶ
			int iView = 0;
			for(int i=0; i<PLAY_COUNT; i++)
			{
				iView = m_pGameFrame->DeskStation2View(i);
				SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010010+iView,pGameStation->bUserAgree[i]);
			}
			//����׼������ʱ	
			SetTimePlay(CTN_1000000_CTN_2006000_Time_2006001,true,m_GameData.m_mjRule.byBeginTime);
			//��ʾ׼����ť
			SetBtVisible(CTN_1000000_BTN_10001,true);

			//����һ�뵹��ʱ 
			SetOneSecondCountDown(m_GameData.m_mjRule.byBeginTime);
			break;
		}
        case GS_SEND_CARD:		//����״̬(��δ����,�����Ѿ����ƣ�����û�г���)
            {
				GameStationData* pGameStation = (GameStationData*)pBuffer;
				m_GameData.m_byNtStation = pGameStation->byNtStation;
				int iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
				SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010020+iView,true);
				//��ʾׯ�ҷ�λ
				UIShowNtInfo(true);
				for(int i=0;i<PLAY_COUNT;++i)
				{
					//������ҵ���������
					m_UserData.SetHandPaiData(i,pGameStation->m_byArHandPai[i],MAX_HAND_PAI);
					//��������
					//m_UserData.m_byArHandPaiCount[i] = pSendPai->m_byArHandPaiCount[i];
					//������ҵ���������
					SetMjWallInfo(CTN_1000000_CTN_2005000_MjWallCtrl_2005030+i,MEN_CARD_NUM,true);
					m_UserData.SetMenPaiData(i,pGameStation->m_iArMenPai[i]);
					// ��ǽ
					UIShowWall(i, m_UserData.m_iArMenPai[i]);		//��ʾ��ǽ�����ƣ�//�Ȳ���ʾ�����ǽ

					m_UserData.m_bTuoGuan[i] = pGameStation->m_bistuoguan[i];

					m_UserDataCard[i].GetFromByte( pGameStation->m_byArHandPai[i] );	//dwjdelete

					if(i != m_nDeskStation)//dwjdelete
					{
						m_UserDataCard[i].FillWithBack(m_UserData.GetHandPaiCount(i));
					}
				}

				for(int i = 0; i < PLAY_COUNT; i++)
				{
					for(int j = 0; j < MAX_HAND_PAI; j++)
					{
						m_iDeskHandPai[i][j] = m_UserData.m_byArHandPai[i][j];
					}
				}
				UIShowAllHandPai(true);
				//����������ʾ��λ
				UIShowDNXB(true);
				//��������ǳ���Ϣ
				GetUserName();
				//��ʾ�й���Ϣ
				UIShowTuoGuan();
            }
            break;        
       case GS_PLAY_GAME:	//��Ϸ��״̬
            {
				GameStationDataEx* pGameStation = (GameStationDataEx*)pBuffer;
				m_GameData.m_byNtStation = pGameStation->byNtStation;
				int iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
				SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010020+iView,true);
				//��ʾׯ�ҷ�λ
				UIShowNtInfo(true);
				for(int i=0;i<PLAY_COUNT;++i)
				{
					//������ҵ���������
					m_UserData.SetHandPaiData(i,pGameStation->m_byArHandPai[i],MAX_HAND_PAI);
					m_UserData.SetGCPData(i,pGameStation->m_UserGCPData[i]);
					//��������
					//m_UserData.m_byArHandPaiCount[i] = pSendPai->m_byArHandPaiCount[i];
					//������ҵ���������
					m_UserData.SetMenPaiData(i,pGameStation->m_iArMenPai[i]);
					SetMjWallInfo(CTN_1000000_CTN_2005000_MjWallCtrl_2005030+i,MEN_CARD_NUM,true);
					// ��ǽ
					UIShowWall(i, m_UserData.m_iArMenPai[i]);		//��ʾ��ǽ�����ƣ�//�Ȳ���ʾ�����ǽ
					m_UserData.SetOutPaiData(i,pGameStation->m_byArOutPai[i],OUT_CARD_NUM);
					UIShowOutCard(i,true);
					m_UserData.m_bTuoGuan[i] = pGameStation->m_bistuoguan[i];
					m_UserDataCard[i].GetFromByte( pGameStation->m_byArHandPai[i] );	//dwjdelete
					if(i != m_nDeskStation)//dwjdelete
					{
						m_UserDataCard[i].FillWithBack(m_UserData.GetHandPaiCount(i));
					}
				}

				for(int i = 0; i < PLAY_COUNT; i++)
				{
					for(int j = 0; j < MAX_HAND_PAI; j++)
					{
						m_iDeskHandPai[i][j] = m_UserData.m_byArHandPai[i][j];
					}
				}
				if(pGameStation->m_byNowOutStation == m_nDeskStation)
				{
					int iView = m_pGameFrame->DeskStation2View(pGameStation->m_byNowOutStation);
					SetMjHandGiveMode(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+iView,true);
				}
				
				UIShowAllHandPai(true);
				//����������ʾ��λ
				UIShowDNXB(true);
				//��������ǳ���Ϣ
				GetUserName();
				//��ʾ�й���Ϣ
				UIShowTuoGuan();
           }
           break;
        default:
            break;
    }

    //��ʼ���ű�����Ч
	PlayBgSound(true);
}
/*----------------------------------------------------------------------------------------------*/
///����Ƴ�
bool	CNewGameClient::OnGameQuiting()
{
	if(m_bGameStation >= GS_SEND_CARD && m_bGameStation <= GS_PLAY_GAME)
	{
		if (!m_pGameFrame->GetIsWatching())
		{
			//ǿ���й�
			if(m_GameData.m_mjRule.bForceTuoGuan)
			{
				if(IDOK != AFCMessageBox("��������Ϸ��, �����뿪��Ϸ�����йܴ��� ���Ƿ�Ҫ�뿪��", "��ʾ",AFC_YESNO)) 
				{
					return false ; 
				}
				
			}
			else	//ǿ�˽�����Ϸ
			{
				if(IDOK != AFCMessageBox("��������Ϸ��, �����뿪���ᱻ�۷����Ƿ����Ҫ�뿪��", "��ʾ",AFC_YESNO)) 
				{
					return false ; 
				} 
				//m_pGameFrame->SendGameData(NULL , 0 ,MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
			}
		}
	}
	m_pGameFrame->SendGameData(NULL , 0 ,MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*-------------------------------------------------------------------------------------------------*/
// �û��й�
void	CNewGameClient::OnUserTuoguan(void *buffer, int nLen)
{
	if(nLen != sizeof(UserTouGuan))
	{
		return;
	}
    UserTouGuan *pUserTouGuan = (UserTouGuan *)buffer;
	if(NULL == pUserTouGuan)
	{
		return;
	}
 
	//��¼�й�״̬
	for(int i=0;i<PLAY_COUNT;++i)
	{
		m_UserData.m_bTuoGuan[i] = pUserTouGuan->bTuoGuan[i];
	}
	//�й���ʾ
	UIShowTuoGuan();
}
/*-------------------------------------------------------------------------------------------------*/
// �û�ͬ��	DWJ
void	CNewGameClient::OnUserAgree(void *buffer, int len)
{
	///��Ϸ�в�����ͬ����Ϣ
	if(m_bGameStation == GS_PLAY_GAME || m_bGameStation == GS_SEND_CARD)
	{
		return; 
	}
	if(len!=sizeof(MSG_GR_R_UserAgree))
	{
		return ;
	}
	MSG_GR_R_UserAgree * pUserAgree =(MSG_GR_R_UserAgree *)buffer;
	if(pUserAgree == NULL)
	{
		return;
	}

	if(pUserAgree->bDeskStation	== m_nDeskStation)         // ����
	{
		ReStartGameInitUI();
		m_pGameFrame->KillTimer(TIME_ONE_SECOND);
	}
	//��ʾ׼����ʶ
	int iView = m_pGameFrame->DeskStation2View(pUserAgree->bDeskStation);
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010010+iView,true);
	PlayGameSound(m_nDeskStation,SOUND_READY);
}
/*-------------------------------------------------------------------------------------------------*/
// ��Ϸ��ʼ	DWJ
void	CNewGameClient::OnGameStart(void *buffer, int len)
{
	if(len != sizeof(tagBeginEx))
	{
		return;
	}
	tagBeginEx *pBegin = (tagBeginEx *)buffer;
	if(NULL == pBegin)
	{
		return;
	}
	SetStationParameter(GS_SEND_CARD);	
	//������Ϸ����
	ReStartGameInitUI();

	for (int i = 0; i < PLAY_COUNT; i ++)
	{
		//��������׼����ʶ
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010010+i,false);
		//��ʾ�Ƴ�ǽ
		SetMjWallInfo(CTN_1000000_CTN_2005000_MjWallCtrl_2005030+i,MEN_CARD_NUM,true);
	}
	InitGameDate();
	m_GameData.m_byThingDoing = THING_GAME_BEGIN;//���ڷ������¼�

	//ׯ��λ��
	m_GameData.m_byNtStation		= pBegin->byNt;
	m_GameData.m_iHuangZhuangCount	= pBegin->iHuangZhuangCount;

	if(m_GameData.m_byNtStation != 255)
	{
		//��ʾׯ��
		int iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010020+iView,true);
		//��ʾׯ�ҷ�λ
		UIShowNtInfo(true);
	}
	//��ʾ������
	//UIShowHuaCount(true);
	//����������ʾ��λ
	UIShowDNXB(true);
	//��������ǳ���Ϣ
	GetUserName();
	//��ʼ����
    PlayGameSound(m_nDeskStation,SOUND_BEGIN);
}
/*-------------------------------------------------------------------------------------------------*/
// ȷ��ׯ����Ϣ	DWJ
void	CNewGameClient::OnDingNt(void *buffer, int nLen)
{	
	if(nLen != sizeof(tagDongNtEx))
	{
		return;
	}
	tagDongNtEx *pDingNt = (tagDongNtEx*)buffer;
	if(NULL == pDingNt)
	{
		return;
	}
	m_GameData.m_byThingDoing = THING_DING_NT;	//����ִ�е��¼�
	//ׯ��λ��
	m_GameData.m_byNtStation		= pDingNt->byNt;
	m_GameData.m_iHuangZhuangCount	= pDingNt->iHuangZhuangCount;
	
	//��ʾׯ��
	int iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010020+iView,true);
	//��ʾׯ�ҷ�λ
	UIShowNtInfo(true);

}
/*-------------------------------------------------------------------------------------------------*/

void CNewGameClient::On2SeZiNT(void * buffer, int nLen)
{
	if(nLen != sizeof(m_GameData.T_TwoSeziNt))
		return;

	tagTwoSeziNtEx* pTwoSeziNt = (tagTwoSeziNtEx*)buffer;

	if(!pTwoSeziNt)
		return;

	m_GameData.m_byThingDoing = THING_2SEZI_NT;
	m_GameData.T_TwoSeziNt.Clear();
	memcpy(&m_GameData.T_TwoSeziNt,pTwoSeziNt,sizeof(m_GameData.T_TwoSeziNt));
	m_GameData.m_byNtStation = pTwoSeziNt->byNt;

	int iView = m_pGameFrame->DeskStation2View(0);

	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000010 + iView,true,G_PointStart[iView],G_PointEnd[0][iView],CT_STRAIGHT);
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000020 + iView,true,G_PointStart[iView],G_PointEnd[1][iView],CT_STRAIGHT);

	//����ɫ����Ч
	PlayGameSound(m_nDeskStation,SOUND_SEZI);
}
// ��һ����ɸ��ȷ�����Ʒ���	DWJ
void	CNewGameClient::On2SeZiDir(void *buffer, int nLen)
{
	if(nLen != sizeof(tagTwoSeziDirEx))
	{
		return;
	}
	tagTwoSeziDirEx *pTwoSeziDir = (tagTwoSeziDirEx*)buffer;
	if(NULL == pTwoSeziDir)
	{
		return;
	}
	m_GameData.m_byThingDoing = THING_2SEZI_DIR;	//����ִ�е��¼�

	int iView = m_pGameFrame->DeskStation2View(0);
	//����֮ǰ������ɫ��
	POINT	point;
	point.x = 0; 
	point.y = 0;
	SetAnimateShowFrame(CTN_1000000_CTN_1000100_ExAnimate_2000010 + iView,false,-1,point);
	SetAnimateShowFrame(CTN_1000000_CTN_1000100_ExAnimate_2000020 + iView,false,-1,point);
	//�����һ��ɸ�ӵ�����
	m_GameData.T_TwoSeziDir.Clear();
	memcpy(&m_GameData.T_TwoSeziDir, pTwoSeziDir, sizeof(m_GameData.T_TwoSeziDir));
	iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
	//��ʼ������ɸ�Ӷ���(����ׯ��λ��ȥ����)
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000010 + iView,true,G_PointStart[iView],G_PointEnd[0][iView],CT_STRAIGHT);
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000020 + iView,true,G_PointStart[iView],G_PointEnd[1][iView],CT_STRAIGHT);

	//����ɫ����Ч
	PlayGameSound(m_nDeskStation,SOUND_SEZI);
	
}
void CNewGameClient::On2SeZiGP(void* buffer, int nLen)
{
	if(nLen != sizeof(tagTwoSeziGetPaiEx))
	{
		return;
	}

	tagTwoSeziGetPaiEx* pTwoSeziGP = (tagTwoSeziGetPaiEx*)buffer;

	if(NULL == pTwoSeziGP)
	{
		return;
	}

	m_GameData.m_byThingDoing = THING_2SEZI_GP;

	int iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);

	//����֮ǰ������ɫ��
	POINT	point;
	point.x = 0; 
	point.y = 0;
	SetAnimateShowFrame(CTN_1000000_CTN_1000100_ExAnimate_2000010 + iView,false,-1,point);
	SetAnimateShowFrame(CTN_1000000_CTN_1000100_ExAnimate_2000020 + iView,false,-1,point);

	m_GameData.T_TwoSeziGetPai.Clear();
	memcpy(&m_GameData.T_TwoSeziGetPai,pTwoSeziGP,sizeof(m_GameData.T_TwoSeziGetPai));

	//��ʼ������ɸ�Ӷ���(����ׯ��λ��ȥ����)
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000010 + iView,true,G_PointStart[iView],G_PointEnd[0][iView],CT_STRAIGHT);
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000020 + iView,true,G_PointStart[iView],G_PointEnd[1][iView],CT_STRAIGHT);
	//����ɫ����Ч
	PlayGameSound(m_nDeskStation,SOUND_SEZI);
}
/*-------------------------------------------------------------------------------------------------*/
//�ڶ���ɸ��ȷ�����ƶ�����Ϣ	DWJ
void	CNewGameClient::On2SeZiDirGP(void *buffer, int nLen)
{
	if(nLen != sizeof(tagTwoSeziDirAndGetPaiEx))
	{
		return;
	}
	tagTwoSeziDirAndGetPaiEx *pTwoSeziDir = (tagTwoSeziDirAndGetPaiEx*)buffer;
	if(NULL == pTwoSeziDir)
	{
		return;
	}
	m_GameData.m_byThingDoing = THING_2SEZI_DIR_GP;	//����ִ�е��¼�

	int iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
	//����֮ǰ������ɫ��
	POINT	point;
	point.x = 0; 
	point.y = 0;
	SetAnimateShowFrame(CTN_1000000_CTN_1000100_ExAnimate_2000010 + iView,false,-1,point);
	SetAnimateShowFrame(CTN_1000000_CTN_1000100_ExAnimate_2000020 + iView,false,-1,point);
	//�����2��ɸ�ӵ�����
	m_GameData.T_TwoSeziDirAndGetPai.Clear();
	memcpy(&m_GameData.T_TwoSeziDirAndGetPai, pTwoSeziDir, sizeof(m_GameData.T_TwoSeziDirAndGetPai));

	//��ʼ������ɸ�Ӷ���(����ׯ��λ��ȥ����)
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000010 + iView,true,G_PointStart[iView],G_PointEnd[0][iView],CT_STRAIGHT);
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000020 + iView,true,G_PointStart[iView],G_PointEnd[1][iView],CT_STRAIGHT);
	//����ɫ����Ч
	PlayGameSound(m_nDeskStation,SOUND_SEZI);
}
/*-------------------------------------------------------------------------------------------------*/
// ������������	DWJ
// ע���ʱû�������ݣ����ƺ����
void	CNewGameClient::OnSendAllCard(void *buffer, int nLen)
{
	if(nLen != sizeof(tagSendPaiEx))
	{
		return;
	}
	tagSendPaiEx *pSendPai = (tagSendPaiEx*)buffer;
	if(NULL == pSendPai)
	{
		return;
	}
	//����ִ�е��¼�
	m_GameData.m_byThingDoing = THING_SEND_PAI;
	SetStationParameter(GS_SEND_CARD);
	//����֮ǰ������ɫ��
	int iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
	POINT	point;
	point.x = 0; 
	point.y = 0;
	SetAnimateShowFrame(CTN_1000000_CTN_1000100_ExAnimate_2000010 + iView,false,-1,point);
	SetAnimateShowFrame(CTN_1000000_CTN_1000100_ExAnimate_2000020 + iView,false,-1,point);


	//������������
	memcpy(&m_GameData.T_SendPai, pSendPai, sizeof(m_GameData.T_SendPai));
	for(int i=0;i<PLAY_COUNT;++i)
	{
		//������ҵ���������
		m_UserData.SetHandPaiData(i,pSendPai->m_byArHandPai[i],pSendPai->m_byArHandPaiCount[i]);
		//��������
		//m_UserData.m_byArHandPaiCount[i] = pSendPai->m_byArHandPaiCount[i];
		//������ҵ���������
		m_UserData.SetMenPaiData(i,pSendPai->byMenPai[i]);

		// ��ǽ
		//UIShowWall(i, m_UserData.m_iArMenPai[i]);		//��ʾ��ǽ�����ƣ�//�Ȳ���ʾ�����ǽ



		m_UserDataCard[i].GetFromByte( pSendPai->m_byArHandPai[i] );	//dwjdelete
		
		if(i != m_nDeskStation)//dwjdelete
		{
			m_UserDataCard[i].FillWithBack(m_UserData.GetHandPaiCount(i));
		}
	}

	for(int i = 0; i < PLAY_COUNT; i++)
	{
		for(int j = 0; j < MAX_HAND_PAI; j++)
		{
			m_iDeskHandPai[i][j] = m_UserData.m_byArHandPai[i][j];
		}
	}
	memset(m_byDeskMenPai,0,sizeof(m_byDeskMenPai));
	m_byDirect = m_GameData.T_TwoSeziDir.byDirection;
	m_byGetPai = m_GameData.T_TwoSeziGetPai.byGetPai*2;
	m_byMoPaiNum = 0;
	m_byMoPaiStation = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
	m_pGameFrame->SetTimer(ID_BEGIN_MOPAI,300);
	//��ʾ������ҵ�����
	//UIShowAllHandPai(true);//�Ȳ���ʾȫ������
}
/*-------------------------------------------------------------------------------------------------*/
//	������Ҳ�����Ϣ	DWJ
void	CNewGameClient::OnAllBuHua( void *buffer, int nLen)
{
	if(nLen != sizeof(tagAllBuHuaEx))
	{
		return;
	}
	tagAllBuHuaEx *pAllBuHua = (tagAllBuHuaEx*)buffer;
	if(NULL == pAllBuHua)
	{
		return;
	}
	//����ִ�е��¼�
	m_GameData.m_byThingDoing = THING_ALL_BUHUA;
	SetStationParameter(GS_SEND_CARD);

	//������������
	memcpy(&m_GameData.T_AllBuHua, pAllBuHua, sizeof(m_GameData.T_AllBuHua));

	//������������
	for(int i=0; i<PLAY_COUNT;i++)
	{
		m_UserData.SetHuaPaiData(i,pAllBuHua->m_byArHuaPai[i]);
	}
	
	//����Ƹ��Ų��Ҵ�
	UIShowAllHandPaiBack(true,true);
	//������Ч
	PlayGameSound(m_nDeskStation,SOUND_BLOCK);	
}
/*-------------------------------------------------------------------------------------------------*/
// ��Ϸ��ʼ������	DWJ
void	CNewGameClient::OnBeginOutCard(void *buffer, int nLen)
{
	if(nLen != sizeof(tagBeginOutPaiEx))
	{
		return;
	}
	tagBeginOutPaiEx *pBeginOutPai = (tagBeginOutPaiEx*)buffer;
	if(NULL == pBeginOutPai)
	{
		return;
	}
	m_GameData.m_byThingDoing = THING_BEGIN_OUT;//����ִ�е��¼�
	SetStationParameter(GS_PLAY_GAME);

	//������ʼ��������
	m_GameData.T_BeginOutPai.Clear();
	memcpy(&m_GameData.T_BeginOutPai, pBeginOutPai, sizeof(m_GameData.T_BeginOutPai));


	for(int i=0;i<PLAY_COUNT;++i)
	{
		//������ҵ���������
		m_UserData.SetHandPaiData(i,pBeginOutPai->m_byArHandPai[i],pBeginOutPai->m_byArHandPaiCount[i]);
		//��������
		m_UserData.m_byArHandPaiCount[i] = pBeginOutPai->m_byArHandPaiCount[i];
		//������ҵ���������
		m_UserData.SetMenPaiData(i,pBeginOutPai->byMenPai[i]);

		// ��ǽ //��ʾ��ǽ�����ƣ�
		UIShowWall(i, m_UserData.m_iArMenPai[i]);				
	}

	//��ǰ����λ��
	m_UserData.m_byNowOutStation = pBeginOutPai->byNt; 


	//�����Լ�����������
	m_UserData.SortHandPai(m_nDeskStation,false);
	//��ʾ������ҵ�����
	UIShowAllHandPai(true);

	//���ó�����ҵ����Ƹ���
	int iView = m_pGameFrame->DeskStation2View(m_UserData.m_byNowOutStation);
	SetMjHandGiveMode(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+iView,true);

	//��ʾ������
	//UIShowHuaCount(true);

	//��ʾ����ʱ
	ShowCountDownTimer(m_UserData.m_byNowOutStation,m_GameData.m_mjRule.byOutTime,true);

	if(pBeginOutPai->byUser == m_nDeskStation)
	{
		//����һ�뵹��ʱ 
		SetOneSecondCountDown(m_GameData.m_mjRule.byOutTime);
	}
	//����״̬
	m_byHandState = 0xFF;
}
/*-------------------------------------------------------------------------------------------------*/
// ���ƽ��	DWJ
void	CNewGameClient::OnNotifyOutCard(void *buffer, int nLen)
{
	if(nLen != sizeof(tagOutPaiEx))
	{
		return;
	}

	tagOutPaiEx *pOutPaiResult = (tagOutPaiEx *)buffer;
	if(NULL == pOutPaiResult)
	{
		return;
	}
	//���ų�������
	PlayGameSound(m_nDeskStation,SOUND_OUTCARD);
	SetStationParameter(GS_PLAY_GAME);
	//����ִ�е��¼�
	m_GameData.m_byThingDoing = THING_OUT_PAI;
	//���ص���ʱ
	ShowCountDownTimer(m_nDeskStation,0,false);
	//������������
	UIShowBlockContain(false);

	//������������
	m_GameData.T_OutPai.Clear();
	memcpy(&m_GameData.T_OutPai, pOutPaiResult, sizeof(m_GameData.T_OutPai));

	//�����������
	m_UserData.SetOutPaiData(pOutPaiResult->byUser,pOutPaiResult->m_byArOutPai[pOutPaiResult->byUser],pOutPaiResult->m_byArOutPaiCount[pOutPaiResult->byUser]);

	//��ʾ��������
	UIShowOutCard(pOutPaiResult->byUser, true);

	//��ʾ����λ�ö���
	int iView = m_pGameFrame->DeskStation2View(pOutPaiResult->byUser);
	RECT rect = GetMjGiveRect(CTN_1000000_CTN_2005000_MjGiveCtrl_2005020+iView,pOutPaiResult->m_byArOutPaiCount[pOutPaiResult->byUser]-1);
	POINT point;
	point.x	= rect.left;
	point.y	= rect.top-20;
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000003,true,point,true);

	//���֮ǰû������ ����������
	if(!m_UserData.m_bTing[pOutPaiResult->byUser] && pOutPaiResult->bUserTing[pOutPaiResult->byUser])
	{
		//��ʶ���������
		m_UserData.m_bTing[pOutPaiResult->byUser] = pOutPaiResult->bUserTing[pOutPaiResult->byUser];
		//����������Ч
		PlayGameSound(pOutPaiResult->byUser,SOUND_TING);
	}
	//��ȡ���Ʊ�ʶ
	for(int i=0;i<PLAY_COUNT;i++ )
	{
		m_UserData.m_bTing[i] = pOutPaiResult->bUserTing[i];
	}
	//��ʾ���Ʊ�ʶ
	UIShowTingLog(true);
	//��������ѡ�����״̬
	SetMjHandInvalidTile(CTN_1000000_CTN_2005000_MjHandCtrl_2005012,NULL,0);


	for(int i=0;i<PLAY_COUNT;i++ )
	{
		//��������
		m_UserData.SetHandPaiData(i,pOutPaiResult->m_byArHandPai[i],pOutPaiResult->m_byArHandPaiCount[i]);
	}

	//��ʾ��������
	UIShowAllHandPai(true);

	//���ó�����ҵ����Ʋ�����
	iView = m_pGameFrame->DeskStation2View(pOutPaiResult->byUser);
	SetMjHandGiveMode(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+iView,false);


	//���������ҵĳ�����֪ͨ����
	for(int i=0;i<PLAY_COUNT;++i)
	{
		m_GameData.T_CPGNotify[i].Clear();
	}
	//���س�������
	UIShowChiContain(false);
	//����״̬
	m_byHandState = 0xFF;

	//������ ɱ��һ�뵹��ʱ��ʱ��
	m_pGameFrame->KillTimer(TIME_ONE_SECOND);
	
	//������ֵ����
	PlayGameSound(pOutPaiResult->byUser,pOutPaiResult->byPs);
}
/*-------------------------------------------------------------------------------------------------*/
// ����
void	CNewGameClient::OnNotifyBlock(void *buffer, int nLen)
{
	//��Ϊ���������������ʽ�����������ݰ� �������ﳤ���ж�Ҫ*PLAY_COUNT
	if(nLen != sizeof(tagCPGNotifyEx)*PLAY_COUNT)
	{
		return;
	}

	tagCPGNotifyEx *pCPGNotify = (tagCPGNotifyEx *)buffer;
	if(NULL == pCPGNotify)
	{
		return;
	}
	//������������
	memcpy(&m_GameData.T_CPGNotify, pCPGNotify, sizeof(m_GameData.T_CPGNotify));
	
	if(		m_GameData.T_CPGNotify[m_nDeskStation].bChi || m_GameData.T_CPGNotify[m_nDeskStation].bPeng || m_GameData.T_CPGNotify[m_nDeskStation].bGang
		||	m_GameData.T_CPGNotify[m_nDeskStation].bTing|| m_GameData.T_CPGNotify[m_nDeskStation].bHu)
	{
		//��ʾ��������
		UIShowBlockContain(true);
		FlashWindow();
	}

	//ץ�ƺ��ж�����ô�Ͳ�����ʾ����ʱ�� ��Ϊǰ��ᷢ��һ��ץ����Ϣ��ʱ���Ѿ���ʾ��
	if(m_GameData.T_CPGNotify[m_nDeskStation].bZhuaPai)
	{
		OutputDebugString("dwjsdmj::ɶҲ������");
	}
	else	//����ץ���ж���(�� ���˳��� �Լ������ƶ��� ) ��ô��Ҫ��ʾ����ʱ
	{
		//���õ���ʱ
		ShowCountDownTimer(m_nDeskStation,m_GameData.m_mjRule.byBlockTime,true);
	}
}
/*-------------------------------------------------------------------------------------------------*/

void CNewGameClient::OnNotifyAction(void *buffer, int nLen)
{
	//��Ϊ���������������ʽ�����������ݰ� �������ﳤ���ж�Ҫ*PLAY_COUNT
	if(nLen != sizeof(HaveAction))
	{
		return;
	}

	HaveAction *pCPGNotify = (HaveAction *)buffer;
	if(NULL == pCPGNotify)
	{
		return;
	}
	bool bIHaveAction = false;
	//������������
	memcpy(m_GameData.T_CPGNotify, pCPGNotify->T_CPGNotify, sizeof(m_GameData.T_CPGNotify));

	if(		m_GameData.T_CPGNotify[m_nDeskStation].bChi || m_GameData.T_CPGNotify[m_nDeskStation].bPeng || m_GameData.T_CPGNotify[m_nDeskStation].bGang
		||	m_GameData.T_CPGNotify[m_nDeskStation].bTing|| m_GameData.T_CPGNotify[m_nDeskStation].bHu)
	{
		//��ʾ��������
		UIShowBlockContain(true);
		bIHaveAction = true;
		FlashWindow();
	}

	//ץ�ƺ��ж�����ô�Ͳ�����ʾ����ʱ�� ��Ϊǰ��ᷢ��һ��ץ����Ϣ��ʱ���Ѿ���ʾ��
	if(m_GameData.T_CPGNotify[m_nDeskStation].bZhuaPai)
	{
		OutputDebugString("dwjsdmj::ɶҲ������");
	}
	else	//����ץ���ж���(�� ���˳��� �Լ������ƶ��� ) ��ô��Ҫ��ʾ����ʱ
	{
		//���õ���ʱ
		if(bIHaveAction)
			ShowCountDownTimer(m_nDeskStation,m_GameData.m_mjRule.byBlockTime,true);
		else
			ShowCountDownTimer(pCPGNotify->byDeskStation,m_GameData.m_mjRule.byBlockTime,true);
	}
}

// ץ��
void	CNewGameClient::OnNotifyZhuaPai(void *buffer, int nLen)
{
	if(nLen != sizeof(tagZhuaPaiEx))
	{
		return;
	}
	tagZhuaPaiEx *pZhuaPai = (tagZhuaPaiEx *)buffer;
	if(NULL == pZhuaPai)
	{
		return;
	}
	//�����ץ������
	m_GameData.T_ZhuaPai.Clear();
	//����ץ������
	memcpy(&m_GameData.T_ZhuaPai, pZhuaPai, sizeof(m_GameData.T_ZhuaPai));

	SetStationParameter(GS_PLAY_GAME);
	m_GameData.m_byThingDoing = THING_ZHUA_PAI;//����ִ�е��¼�

	m_UserData.m_byNowOutStation = pZhuaPai->byUser; //��ǰ����λ��

	for(int i=0;i<PLAY_COUNT;++i)
	{
		//������ҵ���������
		m_UserData.SetMenPaiData(i,pZhuaPai->byMenPai[i]);
		//��ʾ��ǽ�����ƣ�	
		UIShowWall(i, m_UserData.m_iArMenPai[i]);			
	}

	//��������
	//m_UserData.m_byArHandPaiCount[pZhuaPai->byUser] = pZhuaPai->m_byArHandPaiCount[pZhuaPai->byUser];
	//��������
	m_UserData.SetHandPaiData(pZhuaPai->byUser,pZhuaPai->m_byArHandPai[pZhuaPai->byUser],pZhuaPai->m_byArHandPaiCount[pZhuaPai->byUser]);
	//��ʾ����
	UIShowAllHandPai(true);
	
	//�ǻ��ƾͲ���ʾ����ʱ  ��Ϊ�����Ҫ������
	if(!m_UserData.CheckIsHuaPai(pZhuaPai->byPs))
	{
		//��ʾ���Ƶ���ʱ
		ShowCountDownTimer(pZhuaPai->byUser,m_GameData.m_mjRule.byOutTime,true);
		//��ʶ�ǻ���  ��ʱ���ܳ���
		m_bIsHua	  = false;
	}
	else
	{
		OutputDebugString("dwjsdmj::C_�ǻ���_�ȵ����油��֮������ʾ����ʱ");
		//��ʶ�ǻ���  ��ʱ�ܳ���
		m_bIsHua	  = true;
	}
	

	//��¼ץ������
	m_UserData.m_byLastZhuaPai[pZhuaPai->byUser] = pZhuaPai->byPs;

	//���ó�����ҵ����Ʋ�����
	int iView = m_pGameFrame->DeskStation2View(pZhuaPai->byUser);
	SetMjHandGiveMode(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+iView,true);

	//������������
	UIShowBlockContain(false);
	//���س�������
	UIShowChiContain(false);
	//����״̬
	m_byHandState = 0xFF;
	//����ץ����Ч
	PlayGameSound(m_nDeskStation,SOUND_ZHUAPAI);
	if(pZhuaPai->byUser == this->m_nDeskStation)
	{
		//��˸����
		FlashWindow();
		//����һ�뵹��ʱ 
		SetOneSecondCountDown(m_GameData.m_mjRule.byOutTime);
	}
}
/*-------------------------------------------------------------------------------------------------*/
//	������Ҳ���	DWJ
void	CNewGameClient::OnNotifyOneBuHua( void *buffer, int nLen)
{
	if(nLen != sizeof(tagOneBuHuaEx))
	{
		return;
	}
	tagOneBuHuaEx *pOneBuHua = (tagOneBuHuaEx *)buffer;
	if(NULL == pOneBuHua)
	{
		return;
	}
	//���岹������
	m_GameData.T_OneBuHua.Clear();
	//����ץ������
	memcpy(&m_GameData.T_OneBuHua, pOneBuHua, sizeof(m_GameData.T_OneBuHua));

	SetStationParameter(GS_PLAY_GAME);
	//����ִ�е��¼�
	m_GameData.m_byThingDoing = THING_ONE_BUHUA;

	//��ǰ����λ��
	m_UserData.m_byNowOutStation = pOneBuHua->byUser; 

	for(int i=0;i<PLAY_COUNT;++i)
	{
		//������ҵ���������
		m_UserData.SetMenPaiData(i,pOneBuHua->byMenPai[i]);
		//��ʾ��ǽ�����ƣ�	
		UIShowWall(i, m_UserData.m_iArMenPai[i]);		
	}

	//������������
	m_UserData.SetHuaPaiData(pOneBuHua->byUser,pOneBuHua->m_byArHuaPai[pOneBuHua->byUser]);
	//��ʾ���Ƹ���
	UIShowHuaCount(true);

	//��������
	//m_UserData.m_byArHandPaiCount[pOneBuHua->byUser] = pOneBuHua->m_byArHandPaiCount[pOneBuHua->byUser];
	//��������
	m_UserData.SetHandPaiData(pOneBuHua->byUser,pOneBuHua->m_byArHandPai[pOneBuHua->byUser],pOneBuHua->m_byArHandPaiCount[pOneBuHua->byUser]);
	//��ʾ����
	UIShowAllHandPai(true);

	//�ǻ��ƾͲ���ʾ����ʱ  ��Ϊ�����Ҫ������
	if(!m_UserData.CheckIsHuaPai(pOneBuHua->byPs))
	{
		//��ʾ���Ƶ���ʱ
		ShowCountDownTimer(pOneBuHua->byUser,m_GameData.m_mjRule.byOutTime,true);
		//��ʶ�ǻ���  ��ʱ���ܳ���
		m_bIsHua	  = false;
	}
	else
	{
		//��ʶ�ǻ���  ��ʱ���ܳ���
		m_bIsHua	  = true;
	}


	//��¼ץ������
	m_UserData.m_byLastZhuaPai[pOneBuHua->byUser] = pOneBuHua->byPs;

	//���ó�����ҵ����Ʋ�����
	int iView = m_pGameFrame->DeskStation2View(pOneBuHua->byUser);
	SetMjHandGiveMode(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+iView,true);


	//����ץ����Ч
	PlayGameSound(m_nDeskStation,SOUND_ZHUAPAI);
	if(pOneBuHua->byUser == this->m_nDeskStation)
	{
		//��˸����
		FlashWindow();
		//����һ�뵹��ʱ 
		SetOneSecondCountDown(m_GameData.m_mjRule.byOutTime);
	}

}
/*-------------------------------------------------------------------------------------------------*/
//	��ҷ�������
void	CNewGameClient::OnNotifyCanCelBlock(void *buffer, int nLen)
{
	//��ǰ�����Լ����� ��Ҫ���ؼ�ʱ��
	if(m_UserData.m_byNowOutStation != m_nDeskStation)
	{
		//������������
		UIShowBlockContain(false);
		//���ص���ʱ����
		ShowCountDownTimer(m_nDeskStation,0,false);
	}
}
/*-------------------------------------------------------------------------------------------------*/
//	����
void	CNewGameClient::OnNotifyChiPai(void *buffer, int nLen)
{
	if(nLen != sizeof(tagChiPaiEx))
	{
		return;
	}
	tagChiPaiEx *pChiPai = (tagChiPaiEx *)buffer;
	if(NULL == pChiPai)
	{
		return;
	}

	//�����������
	m_GameData.T_ChiPai.Clear();
	//������������
	memcpy(&m_GameData.T_ChiPai, pChiPai, sizeof(m_GameData.T_ChiPai));
	//����ִ�е��¼�
	m_GameData.m_byThingDoing = THING_CHI_PAI;

	//��ǰ����λ��
	m_UserData.m_byNowOutStation = pChiPai->byUser; 

	//���س�������
	UIShowChiContain(false);
	//������������
	UIShowBlockContain(false);

	//��������
	m_UserData.SetGCPData(pChiPai->byUser,pChiPai->m_UserGCPData[pChiPai->byUser]);

	//��������
	//m_UserData.m_byArHandPaiCount[pChiPai->byUser] = pChiPai->m_byArHandPaiCount[pChiPai->byUser];
	//��������
	m_UserData.SetHandPaiData(pChiPai->byUser,pChiPai->m_byArHandPai[pChiPai->byUser],pChiPai->m_byArHandPaiCount[pChiPai->byUser]);
	//��ʾ����
	UIShowAllHandPai(true);

	//��������
	m_UserData.SetOutPaiData(pChiPai->byBeChi, pChiPai->m_byArOutPai[pChiPai->byBeChi],pChiPai->m_byArOutPaiCount[pChiPai->byBeChi]);
	//��ʾ��������
	UIShowOutCard(pChiPai->byBeChi, true);

	//�������Ƽ�ʱ��
	ShowCountDownTimer(pChiPai->byUser,m_GameData.m_mjRule.byOutTime,true);

	//����״̬
	m_byHandState = 0xFF;

	//���ų�����Ч
	PlayGameSound(pChiPai->byUser,SOUND_CHI);

	//���Ʊ����� �����س��ƶ���
	POINT point;
	point.x = point.y = 0;
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000003,false,point,false);
	

}
/*-------------------------------------------------------------------------------------------------*/
// ����
void	CNewGameClient::OnNotifyPengPai(void *buffer, int nLen)
{
	if(nLen != sizeof(tagPengPaiEx))
	{
		return;
	}
	tagPengPaiEx *pPengPai = (tagPengPaiEx *)buffer;
	if(NULL == pPengPai)
	{
		return;
	}

	//������������
	m_GameData.T_PengPai.Clear();
	//������������
	memcpy(&m_GameData.T_PengPai, pPengPai, sizeof(m_GameData.T_PengPai));
	//����ִ�е��¼�
	m_GameData.m_byThingDoing = THING_PENG_PAI;

	//��ǰ����λ��
	m_UserData.m_byNowOutStation = pPengPai->byUser; 

	//���س�������
	UIShowChiContain(false);
	//������������
	UIShowBlockContain(false);

	//��������
	m_UserData.SetGCPData(pPengPai->byUser,pPengPai->m_UserGCPData[pPengPai->byUser]);

	//��������
	m_UserData.SetHandPaiData(pPengPai->byUser,pPengPai->m_byArHandPai[pPengPai->byUser],pPengPai->m_byArHandPaiCount[pPengPai->byUser]);
	//��ʾ����
	UIShowAllHandPai(true);

	//��������
	m_UserData.SetOutPaiData(pPengPai->byBePeng, pPengPai->m_byArOutPai[pPengPai->byBePeng],pPengPai->m_byArOutPaiCount[pPengPai->byBePeng]);
	//��ʾ��������
	UIShowOutCard(pPengPai->byBePeng, true);

	//�������Ƽ�ʱ��
	ShowCountDownTimer(pPengPai->byUser,m_GameData.m_mjRule.byOutTime,true);

	//����״̬
	m_byHandState = 0xFF;

	//// �������ƶ���

	//����������
	PlayGameSound(pPengPai->byUser,SOUND_PENG);

	//���Ʊ����� �����س��ƶ���
	POINT point;
	point.x = point.y = 0;
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000003,false,point,false);
}
/*-------------------------------------------------------------------------------------------------*/
// ����
void	CNewGameClient::OnNotifyGangPai(void *buffer, int nLen)
{
	if(nLen != sizeof(tagGangPaiEx))
	{
		return;
	}
	tagGangPaiEx *pGangPai = (tagGangPaiEx *)buffer;
	if(NULL == pGangPai)
	{
		return;
	}

	//������������
	m_GameData.T_GangPai.Clear();
	//������������
	memcpy(&m_GameData.T_GangPai, pGangPai, sizeof(m_GameData.T_GangPai));
	//����ִ�е��¼�
	m_GameData.m_byThingDoing = THING_GANG_PAI;

	//��ǰ����λ��
	m_UserData.m_byNowOutStation = pGangPai->byUser; 

	//���س�������
	UIShowChiContain(false);
	//������������
	UIShowBlockContain(false);

	//��������
	m_UserData.SetGCPData(pGangPai->byUser,pGangPai->m_UserGCPData[pGangPai->byUser]);

	//��������
	m_UserData.SetHandPaiData(pGangPai->byUser,pGangPai->m_byArHandPai[pGangPai->byUser],pGangPai->m_byArHandPaiCount[pGangPai->byUser]);
	//��ʾ����
	UIShowAllHandPai(true);

	//���ó�����ҵ����Ƹ���
	int iView = m_pGameFrame->DeskStation2View(pGangPai->byUser);
	SetMjHandGiveMode(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+iView,false);
	
	//��������
	m_UserData.SetOutPaiData(pGangPai->byBeGang, pGangPai->m_byArOutPai[pGangPai->byBeGang],pGangPai->m_byArOutPaiCount[pGangPai->byBeGang]);
	//��ʾ��������
	UIShowOutCard(pGangPai->byBeGang, true);

	//// ���Ÿ��ƶ���
	//���Ÿ�����
	PlayGameSound(pGangPai->byUser,SOUND_GANG);
	PlayGameSound(m_nDeskStation,SOUND_BLOCK);
	

	//���Ʊ����� �����س��ƶ���
	POINT point;
	point.x = point.y = 0;
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000003,false,point,false);

	//��������ѡ�����״̬
	SetMjHandInvalidTile(CTN_1000000_CTN_2005000_MjHandCtrl_2005012,NULL,0);
	//����״̬
	m_byHandState = 0xFF;
}
/*-------------------------------------------------------------------------------------------------*/
// ����
void CNewGameClient::OnNotifyHuPai(void *buffer, int nLen)  
{
	if(nLen != sizeof(tagHuPaiEx))
	{
		return;
	}
	tagHuPaiEx *pHuPai = (tagHuPaiEx *)buffer;
	if(NULL == pHuPai)
	{
		return;
	}

	//������������
	memcpy(&m_GameData.T_HuPai, pHuPai, sizeof(m_GameData.T_HuPai));

	//���س�������
	UIShowChiContain(false);
	//������������
	UIShowBlockContain(false);

	// ���ź��ƶ���
	POINT point;
	point.x=point.y=0;
	SetAnimatePlay(CTN_1000000_CTN_1000100_ExAnimate_2000001,true,point,point);
	//SetAnimatePlay(2000003,false);
	//��������� ����������Ч
	if (pHuPai->bZimo)
	{
		PlayGameSound(pHuPai->byUser,SOUND_ZIMO);//��������
	}
	else 
	{		 
		PlayGameSound(pHuPai->byUser,SOUND_HU);
	}

	//��¼�������
	for(int i=0;i<PLAY_COUNT;++i)
	{
		if(pHuPai->bHaveHu[i])
		{
			m_UserData.m_bIsHu[i] = true;
		}
	}

	//������ҵ���������
	for(int i = 0; i < PLAY_COUNT; i++)
	{
		//m_UserData.SetHandPaiData(pHuPai->byUser,pHuPai->m_byArHandPai[pHuPai->byUser],pHuPai->m_byArHandPaiCount[pHuPai->byUser]);
		m_UserData.SetHandPaiData(i,pHuPai->m_byArHandPai[i],pHuPai->m_byArHandPaiCount[i]);
	}
	
	//��ʾ����
	UIShowAllHandPai(true,true);

	//�����Լ��ĵ���ʱ
	ShowCountDownTimer(pHuPai->byUser,0,false);

}
/*-------------------------------------------------------------------------------------------------*/
// ����
void CNewGameClient::OnNotifyFinish(void *buffer, int nLen)
{
	if(nLen != sizeof(tagCountFenEx))
	{
		return;
	}
	tagCountFenEx *pCountFen = (tagCountFenEx *)buffer;
	if(NULL == pCountFen)
	{
		return;
	}
	m_pGameFrame->KillTimer(TIME_SHOW_NIAO);
	SetStationParameter(GS_WAIT_NEXT);
	//����ִ�е��¼�
	m_GameData.m_byThingDoing = THING_ENG_HANDLE;
	m_GameData.T_CountFen.Clear();
	memcpy(&m_GameData.T_CountFen, pCountFen, sizeof(m_GameData.T_CountFen));
	IExAnimate * pExAnimate = NULL;
	pExAnimate = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(2000003));
	if(pExAnimate)
		pExAnimate->SetControlVisible(false);
	//SetAnimatePlay(2000003,false);
	SetContainVisible(2008100,false);
	//��ʾ�����
	UIShowResultDlg(true);
	//����׼������ʱ	
	//SetTimePlay(CTN_1000000_CTN_2006000_Time_2006001,false,m_GameData.m_mjRule.byBeginTime);
	//����һ�뵹��ʱ 
	SetOneSecondCountDown(m_GameData.m_mjRule.byBeginTime);
}

//��һ��ƽ��
void	CNewGameClient::OnUserChangeCard(void *buffer, int nLen)
{
	OutputDebugString("xxmj::OnUserChangeCard-0");
	if(nLen != sizeof(tagSuperChangeCard))
	{
		return;
	}
	tagSuperChangeCard *pChangeCard = (tagSuperChangeCard *)buffer;
	if(NULL == pChangeCard)
	{
		return;
	}
	OutputDebugString("xxmj::OnUserChangeCard-1");

	//���Ƴɹ�
	if (pChangeCard->bSuccese)
	{
		OutputDebugString("xxmj::OnUserChangeCard-2");
		//������ҵ���������
		m_UserData.SetHandPaiData(m_nDeskStation,pChangeCard->byArHandPai,pChangeCard->byArHandPaiCount);

		//�Լ��������齫
		int iHandCard[MAX_HAND_PAI];
		for (int j = 0; j < m_UserData.m_byArHandPaiCount[m_nDeskStation]; j ++)
		{
			iHandCard[j] = m_UserData.m_byArHandPai[m_nDeskStation][j];
		}
		SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070031,iHandCard, m_UserData.m_byArHandPaiCount[m_nDeskStation],true);

		//�������齫
		SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070021,NULL,0,true);
		//����Ҫ���齫
		SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070022,NULL,0,true);

		//��ʾ������ҵ�����
		UIShowAllHandPai(true);
		OutputDebugString("xxmj::OnUserChangeCard-3");
	}
}
































 /// ǿ�˽�����Ϸ
void CNewGameClient::OnForceFinish(void *buffer, int nLen)     
{
	tagCountFenEx *pCmd = (tagCountFenEx *)buffer;   
	if(pCmd->byCloseFlag == GFF_FORCE_FINISH)
	{//��ʾǿ�˽��������
		OnSetTimer(2,TIME_BEGIN);
		// �й�״̬
		SAFE_CTRL_OPERATE(IButton, CTN_1_BTN_10002, SetControlVisible(true));
		SAFE_CTRL_OPERATE(IButton, CTN_1_BTN_10003, SetControlVisible(false));
		// ��ʾ��ʼ��ť
		SAFE_CTRL_OPERATE(IButton, CTN_1_BTN_10001, SetControlVisible(false));
		// �����������
		SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));
		UIShowTimer(0xFF, 0, false);
		SAFE_CTRL_OPERATE(IContain, CTN_HUPAI_DLG, SetControlVisible(false));///���غ��ƶԻ���
		//������
		for(int i=IMG_Liu_Ju;i<TEXT_JS_ZhuanYi_3+1;++i)
		{
			SAFE_CTRL_OPERATE(IText,  i, SetControlVisible(false));
			SAFE_CTRL_OPERATE(IImage,  i, SetControlVisible(false));
		}
		//�����ֽ��㣺����Һ���
		SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_20500, SetControlVisible(true));//��ʾ�����GetGameXY 
		wchar_t wszFen[500]=L"";
		for (int i=0; i<PLAY_COUNT; ++i)
		{					
			//�ܽ���
			//�ǳ�
			SAFE_CTRL_OPERATE(IText, TEXT_JS_User_0 + i, SetControlVisible(true));
			
			//��
			SAFE_CTRL_OPERATE(IText, TEXT_JS_ZongFen_0 + i, SetControlVisible(true));
			::swprintf_s(wszFen, L"%I64d", pCmd->iZongFen[i]);
			SAFE_CTRL_OPERATE(IText, TEXT_JS_ZongFen_0 + i, SetText(wszFen));
			//���
			SAFE_CTRL_OPERATE(IText, TEXT_JS_JinBi_0 + i, SetControlVisible(true));
			::swprintf_s(wszFen, L"%I64d", pCmd->i64Money[i]);
			SAFE_CTRL_OPERATE(IText, TEXT_JS_JinBi_0 + i, SetText(wszFen));
			// ����
			IText *pFan = NULL;
			GETCTRL(IText, pFan, m_pUI, TEXT_JS_Hu_Type_1);
			if (pFan)
			{
				wchar_t wszFan[500]=L"ǿ�˽�����Ϸ";
				pFan->SetText(wszFan);
				pFan->SetControlVisible(true);
			}
		}
	}
}
















/*---------------------------------------------------------------------------------------------*/
//���������Ϣ�ؼ�
void	CNewGameClient::UIShowUserInfo(BYTE byStation, bool bFlag)
{
	int iView = m_pGameFrame->DeskStation2View(byStation);
	//�ǳ�����
	SetTextinfo(CTN_1_TEXT_70+iView,"",bFlag);
	//�ǳƵ�ͼ
	SetImageVisible(CTN_1_IMG_80+iView,bFlag);
	//���ͷ��
	SetImageVisible(CTN_1_IMG_90+iView,bFlag);
	//���
	SetTextinfo(CTN_1_TEXT_100+iView,"",bFlag);

	//׼����ʶ
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010010+iView,bFlag);
	//ׯ�ұ�ʶ
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010020+iView,bFlag);
	//�йܱ�ʶ
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010030+iView,bFlag);
	//����������ʶ
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010040+iView,bFlag);
	//������ͼ
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010050+iView,bFlag);
	//����
	SetInumInfo(CTN_1000000_CTN_2002000_Num_2010060+iView,0,bFlag);
	//���Ʊ�ʶ
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010070+iView,bFlag);
}
/*---------------------------------------------------------------------------------------------*/
// ���ݸ�ʽ��255��û���ƣ� 0���Ʊ�������������һ��	DWJ
void	CNewGameClient::UIShowWall(BYTE byStation, BYTE byArrMenPai[])
{
	int iViewStation = m_pGameFrame->DeskStation2View(byStation);
	BYTE byTmpArrMenPai[MEN_CARD_NUM];
	memcpy(byTmpArrMenPai, byArrMenPai, sizeof(byTmpArrMenPai));

	for (int i=0; i<MEN_CARD_NUM; i++)
	{
		int nCurIndex = i;
		if (nCurIndex%2==1)
		{            
			if (255 == byTmpArrMenPai[nCurIndex] /*���������棬����*/ &&  255 != byTmpArrMenPai[nCurIndex-1] /*���������棬����*/)
			{
				// ���������Ժ����ԣ��������ƣ��������ƣ�����Ӧ�ý���                
				byTmpArrMenPai[nCurIndex] = byTmpArrMenPai[nCurIndex-1];
				byTmpArrMenPai[nCurIndex-1] = 255;
			}        
		}
	}

	int count = 0;
	for (int i=0; i<MEN_CARD_NUM; ++i)       // ��������
	{   
		if (255 == byTmpArrMenPai[i])    // ����
		{
			SetMjWallTittle(CTN_1000000_CTN_2005000_MjWallCtrl_2005030+iViewStation, -1 ,G_iWallChangePos[i],true);
		}
		else if (0 == byTmpArrMenPai[i])	//�Ʊ�
		{
			SetMjWallTittle(CTN_1000000_CTN_2005000_MjWallCtrl_2005030+iViewStation, 0 ,G_iWallChangePos[i],true);
		}
		else if (byTmpArrMenPai[i]>0 && byTmpArrMenPai[i]<38 && byTmpArrMenPai[i]%10!=0)	//ĳ�Ƶ�����
		{
			SetMjWallTittle(CTN_1000000_CTN_2005000_MjWallCtrl_2005030+iViewStation, 0 ,G_iWallChangePos[i],true);
		}
	}
}
/*---------------------------------------------------------------------------------------------*/
//	��ʾ��������	DWJ
void	CNewGameClient::UIShowAllHandPai(bool bFlag,bool bFinish)
{
	int iHandCard[MAX_HAND_PAI];

	for (int i = 0; i <PLAY_COUNT; i++)
	{
		for (int m = 0; m < HAND_CARD_NUM; m ++)
		{
			iHandCard[m] = 255;
		}

		int iViewDesk = m_pGameFrame->DeskStation2View(i);
		//��������е�������Ϣ ��һ�������
		ClearMjHandBlock(CTN_1000000_CTN_2005000_MjHandCtrl_2005010 + iViewDesk);
		//�Լ�������
		if (i == m_nDeskStation)
		{
			for (int j = 0; j < m_UserData.m_byArHandPaiCount[m_nDeskStation]; j ++)
			{
				iHandCard[j] = m_UserData.m_byArHandPai[m_nDeskStation][j];
			}
		}
		else if(m_UserData.m_bTing[i] || m_UserData.m_bIsHu[i] || bFinish)
		{
			for (int j = 0; j < m_UserData.m_byArHandPaiCount[i]; j ++)
			{
				iHandCard[j] = m_UserData.m_byArHandPai[i][j];
			}
		}
		
		SetMjHandInfo(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+iViewDesk,iHandCard, m_UserData.m_byArHandPaiCount[i],bFlag);
		//�Ƿ������� ������ ��Ҫ��
		SetMjHandLayDown(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+iViewDesk,(m_UserData.m_bTing[i] || m_UserData.m_bIsHu[i] || bFinish));

		
		int iBeView = 0;
		//�������� 
		for(int j=0; j<5;j++)
		{
			if(m_UserData.m_UserGCPData[i][j].byType != 255)
			{
				iBeView = m_pGameFrame->DeskStation2View(m_UserData.m_UserGCPData[i][j].iBeStation);
				AddMjHandOneBlock(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+iViewDesk, (MJ_ACTION_TYPE)m_UserData.m_UserGCPData[i][j].byType, m_UserData.m_UserGCPData[i][j].byData, iBeView/*m_UserData.m_UserGCPData[i][j].iBeStation*/);
			}
		}

	}	
}
/*---------------------------------------------------------------------------------------------*/
//��ʾ������	DWJ
void	CNewGameClient::UIShowHuaCount(bool bFlag)
{
	int iView = 0;
	int iHuaNum = 0;
	for(int i=0;i<PLAY_COUNT; i++)
	{
		iView = m_pGameFrame->DeskStation2View(i);
		//������ͼ
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010050+iView,bFlag);

		//��������
		iHuaNum = 0;
		for(int j=0; j<HUA_CARD_NUM;j++)
		{
			if(255 != m_UserData.m_byArHuaPai[i][j])
			{
				iHuaNum++;
			}
		}
		SetInumInfo(CTN_1000000_CTN_2002000_Num_2010060+iView,(__int64)iHuaNum,bFlag);
	}
}
/*---------------------------------------------------------------------------------------------*/
//����������ʾ��λ
void	CNewGameClient::UIShowDNXB(bool bFlag)
{
	int iView = 0;
	for(int i=0;i<PLAY_COUNT; i++)
	{
		iView = m_pGameFrame->DeskStation2View(i);
		//����������λͼ
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010040+iView,bFlag,i*21);
	}
}
/*---------------------------------------------------------------------------------------------*/
//	��ʾׯ�Ҿ�����Ϣ
void	CNewGameClient::UIShowNtInfo(bool bFlag)
{

	if(m_GameData.m_byNtStation != 255)
	{
		int iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010020+iView,true);
	}
	return;
	///���ϽǶ���X��
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_20020100,bFlag,113*m_GameData.m_byNtStation);
	///���Ͻǻ�ׯX
	SetImageVisible(CTN_1000000_CTN_2002000_IMG_20020101,bFlag);
	
	if(m_GameData.m_iHuangZhuangCount >= 5)
	{
		m_GameData.m_iHuangZhuangCount = 5;
	}
	int iNum = ::pow(2.0,m_GameData.m_iHuangZhuangCount);

	///���Ͻǻ�ׯ����
	SetInumInfo(CTN_1000000_CTN_2002000_Num_20020102,(__int64)iNum,bFlag);
}
/*---------------------------------------------------------------------------------------------*/
//	��ʾ���Ʊ�ʶ	DWJ
void	CNewGameClient::UIShowTingLog(bool bFlag)
{
	int iView = 0;
	for(int i=0; i<PLAY_COUNT; i++)
	{
		iView = m_pGameFrame->DeskStation2View(i);
		if(m_UserData.m_bTing[i])
		{
			SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010070+iView,bFlag);
		}
		else
		{
			SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010070+iView,false);
		}
	}
}
/*---------------------------------------------------------------------------------------------*/
//	�������е�����ʾ�Ʊ�	DWJ
void	CNewGameClient::UIShowAllHandPaiBack(bool bFlag, bool bLayDown)
{
	int iHandCard[MAX_HAND_PAI];

	for (int i = 0; i <PLAY_COUNT; i++)
	{
		for (int m = 0; m < MAX_HAND_PAI; m ++)
		{
			iHandCard[m] = 0;
		}

		int iViewDesk = m_pGameFrame->DeskStation2View(i);
		SetMjHandInfo(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+iViewDesk,iHandCard, m_UserData.m_byArHandPaiCount[i],bFlag);
		SetMjHandLayDown(CTN_1000000_CTN_2005000_MjHandCtrl_2005010+iViewDesk,bLayDown);
	}	
}
/*---------------------------------------------------------------------------------------------*/
//��ʾ���Ƶ���ʱ	DWJ
void	CNewGameClient::ShowCountDownTimer(BYTE byDeskStation, BYTE byTime, bool bFlag)
{
	if(byDeskStation < 0 && byDeskStation >= PLAY_COUNT)
	{
		return;
	}
	for(int i=0; i<PLAY_COUNT;i++)
	{
		SetImageVisible(CTN_1000000_CTN_2006000_IMG_2006010+i,false);
	}
	
	SetTimePlay(CTN_1000000_CTN_2006000_Time_2006001,bFlag,byTime);
	int iView = m_pGameFrame->DeskStation2View(byDeskStation);
	SetImageVisible(CTN_1000000_CTN_2006000_IMG_2006010+iView,bFlag);

}
/*---------------------------------------------------------------------------------------------*/
// ���ó���
void	CNewGameClient::UIShowOutCard(BYTE byStation, bool bShow)
{
	SetContainVisible(CTN_1000000_CTN_2006000,bShow);
	int iView = 0;
	int iArOutPai[OUT_CARD_NUM]={0};
	for(int i=0; i<PLAY_COUNT;  i++)
	{
		memset(iArOutPai, 0, sizeof(iArOutPai));

		for(int j = 0; j < OUT_CARD_NUM; j++)
		{
			iArOutPai[j] = m_UserData.m_byArOutPai[i][j];
		}

		iView = m_pGameFrame->DeskStation2View(i);
		SetMjGiveInfo(CTN_1000000_CTN_2005000_MjGiveCtrl_2005020+iView,8,iArOutPai,m_UserData.m_byArOutPaiCount[i],bShow);
	}
}
/*---------------------------------------------------------------------------------------------*/
//	��ʾ��������	DWJ
void	CNewGameClient::UIShowBlockContain(bool bFlag)
{
	//����
	SetContainVisible(CTN_1000000_CTN_2003000,bFlag);

	SetBtEnable(CTN_1000000_CTN_2003000_BTN_2003001,bFlag);	///<��
	SetBtEnable(CTN_1000000_CTN_2003000_BTN_2003010,m_GameData.T_CPGNotify[m_nDeskStation].bChi&&bFlag);	///<��
	SetBtEnable(CTN_1000000_CTN_2003000_BTN_2003011,m_GameData.T_CPGNotify[m_nDeskStation].bPeng&&bFlag);	///<�� 
	SetBtEnable(CTN_1000000_CTN_2003000_BTN_2003012,m_GameData.T_CPGNotify[m_nDeskStation].bGang&&bFlag);	///<��
	SetBtEnable(CTN_1000000_CTN_2003000_BTN_2003013,m_GameData.T_CPGNotify[m_nDeskStation].bTing&&bFlag);	///<��
	if(m_GameData.T_CPGNotify[m_nDeskStation].bQiangGang)
	{
		SetBtEnable(2003014,m_GameData.T_CPGNotify[m_nDeskStation].bQiangGang && bFlag);
		return;
	}
	SetBtEnable(CTN_1000000_CTN_2003000_BTN_2003014,m_GameData.T_CPGNotify[m_nDeskStation].bHu&&bFlag);	///<��

}
/*---------------------------------------------------------------------------------------------*/
//	��ʾ��������	DWJ
void	CNewGameClient::UIShowChiContain(bool bFlag)
{
	ShowChiContain1(false);
	ShowChiContain2(false);
	ShowChiContain3(false);

	//�����м������Ƶ�����
	int iChiCount = 0;
	for(int i=0; i<3; i++)
	{
		if(m_GameData.T_CPGNotify[m_nDeskStation].m_iChi[i][0] != 255)
		{
			iChiCount++;
		}
	}

	if(1 == iChiCount)
	{
		ShowChiContain1(bFlag);
	}
	else if(2 == iChiCount)
	{
		ShowChiContain2(bFlag);
	}
	else if(3 == iChiCount)
	{
		ShowChiContain3(bFlag);
	}

	//��껹û������
	m_bMouseEnter = false;

}
/*---------------------------------------------------------------------------------------------*/
//	��ʾ��������1	DWJ
void	CNewGameClient::ShowChiContain1(bool bFlag)
{
	SetContainVisible(CTN_1000000_CTN_2009000,bFlag);
	int iHandCard[3] = {0};

	for(int i=0; i<2; i++)
	{
		//�����2����������
		SetMjHandInfo(CTN_1000000_CTN_2009000_MjHandCtrl_20090011+i,iHandCard,0,false);
		//��ʹ����괩͸
		//SetMjHandUseMouse(CTN_1000000_CTN_2009000_MjHandCtrl_20090011+i,false);
		//����2����ͼ
		SetImageVisible(CTN_1000000_CTN_2009000_IMG_20090021,false);
	}
	for(int i=0; i<2; i++)
	{
		for(int j=0; j<3;j++)
		{
			iHandCard[j] = 255;
			iHandCard[j] = m_GameData.T_CPGNotify[m_nDeskStation].m_iChi[i][j];
		}
		SetMjHandInfo(CTN_1000000_CTN_2009000_MjHandCtrl_20090011+i,iHandCard,3,bFlag);
	}
}
/*---------------------------------------------------------------------------------------------*/
//	��ʾ��������1	DWJ
void	CNewGameClient::ShowChiContain2(bool bFlag)
{
	SetContainVisible(CTN_1000000_CTN_2008000,bFlag);
	int iHandCard[3] = {0};

	for(int i=0; i<2; i++)
	{
		//�����2����������
		SetMjHandInfo(CTN_1000000_CTN_2008000_MjHandCtrl_20080011+i,iHandCard,0,false);
		//��ʹ����괩͸
		//SetMjHandUseMouse(CTN_1000000_CTN_2008000_MjHandCtrl_20080011+i,false);
		//����2����ͼ
		SetImageVisible(CTN_1000000_CTN_2008000_IMG_20080021,false);
	}
	for(int i=0; i<2; i++)
	{
		for(int j=0; j<3;j++)
		{
			iHandCard[j] = 255;
			iHandCard[j] = m_GameData.T_CPGNotify[m_nDeskStation].m_iChi[i][j];
		}
		SetMjHandInfo(CTN_1000000_CTN_2008000_MjHandCtrl_20080011+i,iHandCard,3,bFlag);
	}
}
/*---------------------------------------------------------------------------------------------*/
//	��ʾ��������1	DWJ
void	CNewGameClient::ShowChiContain3(bool bFlag)
{
	SetContainVisible(CTN_1000000_CTN_2007000,bFlag);

	int iHandCard[3] = {0};

	for(int i=0; i<3; i++)
	{
		//�����������������
		SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070011+i,iHandCard,0,false);
		//��ʹ����괩͸
		//SetMjHandUseMouse(CTN_1000000_CTN_2007000_MjHandCtrl_20070011+i,false);
		//����������ͼ
		SetImageVisible(CTN_1000000_CTN_2007000_IMG_20070021,false);
	}
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3;j++)
		{
			iHandCard[j] = 255;
			iHandCard[j] = m_GameData.T_CPGNotify[m_nDeskStation].m_iChi[i][j];
		}
		SetMjHandInfo(CTN_1000000_CTN_2007000_MjHandCtrl_20070011+i,iHandCard,3,bFlag);
	}
	
}
/*---------------------------------------------------------------------------------------------*/
//	����ѡ�����	DWJ
void	CNewGameClient::SetChoiceGangPai(int iGangCount,bool bFlag)
{
	int iHand[HAND_CARD_NUM];
	memset(iHand ,255, sizeof(iHand));
	if(iGangCount > 0 && bFlag)
	{
		for(int i=0; i<HAND_CARD_NUM; i++)
		{
			iHand[i] = m_UserData.m_byArHandPai[m_nDeskStation][i];
			for(int j=0; j<iGangCount; j++)
			{
				if(m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[j][1] == m_UserData.m_byArHandPai[m_nDeskStation][i])
				{
					iHand[i] = 255;
					break;
				}
			}
		}
		int iTmpHand[HAND_CARD_NUM];
		int iCount = 0;
		for(int i=0; i<HAND_CARD_NUM; i++)
		{
			iTmpHand[i] = 255;
			if(iHand[i] != 255)
			{
				iTmpHand[iCount] = iHand[i];
				iCount++;
			}
		}
		//��ʼ�������Ʋ���ѡ
		SetMjHandInvalidTile(CTN_1000000_CTN_2005000_MjHandCtrl_2005012,iTmpHand,iCount);
		m_byHandState = 2;
	}
	else
	{
		//��ʼ�������Ʋ���ѡ
		SetMjHandInvalidTile(CTN_1000000_CTN_2005000_MjHandCtrl_2005012,NULL,0);
		m_byHandState = 0xFF;
	}
	
}
/*---------------------------------------------------------------------------------------------*/
//����ѡ������״ֻ̬��ѡ���������
void	CNewGameClient::SetChoiceTingPai()
{
	//�������Ƶ�����
	int iTingCount = 0;
	for(int i=0; i<HAND_CARD_NUM; i++)
	{
		if(m_GameData.T_CPGNotify[m_nDeskStation].m_byTingCanOut[i] != 255)
		{
			iTingCount++;
		}
	}

	int iHand[HAND_CARD_NUM];
	memset(iHand ,255, sizeof(iHand));
	for(int i=0; i<HAND_CARD_NUM; i++)
	{
		iHand[i] = m_UserData.m_byArHandPai[m_nDeskStation][i];
		for(int j=0; j<iTingCount; j++)
		{
			if(m_GameData.T_CPGNotify[m_nDeskStation].m_byTingCanOut[j] == iHand[i])
			{
				iHand[i] = 255;
				break;
			}
		}
	}

	int iTmpHand[HAND_CARD_NUM];
	int iCount = 0;
	for(int i=0; i<HAND_CARD_NUM; i++)
	{
		iTmpHand[i] = 255;
		if(iHand[i] != 255)
		{
			iTmpHand[iCount] = iHand[i];
			iCount++;
		}
	}
	//��ʼ�������Ʋ���ѡ
	SetMjHandInvalidTile(CTN_1000000_CTN_2005000_MjHandCtrl_2005012,iTmpHand,iCount);
	//m_byHandState = 3;
}
/*---------------------------------------------------------------------------------------------*/
//�й���ʾ
void	CNewGameClient::UIShowTuoGuan()
{
	//�й�logo
	int iView = 0;
	for(int i=0;i<PLAY_COUNT;++i)
	{
		iView = m_pGameFrame->DeskStation2View(i);
		SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010030+iView,m_UserData.m_bTuoGuan[i]);
	}

	//�йܰ�ť
	SetBtVisible(CTN_1000000_BTN_2010101,!m_UserData.m_bTuoGuan[m_nDeskStation]);
	//ȡ���йܰ�ť
	SetBtVisible(CTN_1000000_BTN_2010102,m_UserData.m_bTuoGuan[m_nDeskStation]);
}
/*---------------------------------------------------------------------------------------------*/
// ��ʾ�����
void	CNewGameClient::UIShowResultDlg(bool bFlag)
{
	//���������������еĿؼ�
	SetImageVisible(CTN_1000000_CTN_2001000_IMG_2001103,false);	///<��ׯ
	SetImageVisible(CTN_1000000_CTN_2001000_IMG_2001001,false);	///<���㱳��1
	SetImageVisible(CTN_1000000_CTN_2001000_IMG_2001002,false);	///<���㱳��2
	SetImageVisible(CTN_1000000_CTN_2001000_IMG_2001003,false);	///<���㱳��3
	for(int i=0; i<4; i++)
	{
		///<�ǳ�
		SetTextinfo(CTN_1000000_CTN_2001000_TEXT_2001011+i,"",false);
		///�ܷ�
		SetTextinfo(CTN_1000000_CTN_2001000_TEXT_2001021+i,"",false);
		///����
		SetTextinfo(CTN_1000000_CTN_2001000_TEXT_2001031+i,"",false);
		//ǹ��
		SetTextinfo(2001071+i,"",false);
		//��Ӯ
		SetTextinfo(2001061+i,"",false);
		//ׯ�ұ�ʶ
		SetImageVisible(CTN_1000000_CTN_2001000_IMG_2001051+i,false);
	}
	for(int i=0; i<3; i++)
	{

		//��������е�������Ϣ 
		ClearMjHandBlock(CTN_1000000_CTN_2001000_MjHandCtrl_2001041 + i);
		///<�齫���ƿؼ�
		SetMjHandVisible(CTN_1000000_CTN_2001000_MjHandCtrl_2001041+i,false);
		///<�������
		SetTextinfo(CTN_1000000_CTN_2001000_TEXT_2001044+i,"",false);
		///<����
		SetTextinfo(CTN_1000000_CTN_2001000_TEXT_2001047+i,"",false);
	}


	//��ʾ����
	SetContainVisible(CTN_1000000_CTN_2001000,bFlag);
	///<�˳�
	SetBtVisible(CTN_1000000_CTN_2001000_BTN_2001101,bFlag);
	///<����
	SetBtVisible(CTN_1000000_CTN_2001000_BTN_2001102,bFlag);
	

	//ׯ�ұ�ʶ
	SetImageVisible(CTN_1000000_CTN_2001000_IMG_2001051+m_GameData.m_byNtStation,bFlag);

	//�Ƿ��ׯ
	if(m_GameData.T_CountFen.bIsLiuJu)
	{
		SetImageVisible(CTN_1000000_CTN_2001000_IMG_2001103,bFlag);
		//���ݺ��Ƶ����� ��ʾ��ͬ�ı���
		SetImageVisible(CTN_1000000_CTN_2001000_IMG_2001001,bFlag);
	}
	else
	{
		//���ݺ��Ƶ����� ��ʾ��ͬ�ı���
		SetImageVisible(CTN_1000000_CTN_2001000_IMG_2001001+m_GameData.T_CountFen.byHuUserNum-1,bFlag);
	}
	

	CString sTextInfo;
	for(int i=0; i<PLAY_COUNT; i++)
	{
		//��ʾ�����ǳ�
		SetTextinfo(CTN_1000000_CTN_2001000_TEXT_2001011+i,m_sUserNick[i],bFlag);
		///<��Ӯ
		if(m_GameData.T_CountFen.i64Money[i] > 0)
		{
			sTextInfo.Format("+%I64d",m_GameData.T_CountFen.i64Money[i]);
		}
		else if(m_GameData.T_CountFen.i64Money[i] < 0)
		{
			sTextInfo.Format("%I64d",m_GameData.T_CountFen.i64Money[i]);
		}
		else
		{
			sTextInfo.Format("%I64d",m_GameData.T_CountFen.i64Money[i]);
		}
		//sTextInfo.Format("%d",);
		SetTextinfo(2001061+i,sTextInfo,bFlag);

		//��ʾ�ܷ�
		sTextInfo.Format("%d",m_GameData.T_CountFen.iGangFan[i]);
		SetTextinfo(2001021+i,sTextInfo,bFlag);
		//��ʾ����
		sTextInfo.Format("%d",m_GameData.T_CountFen.iHuFan[i]);
		SetTextinfo(2001031+i,sTextInfo,bFlag);
		//��ʾǹ��
		sTextInfo.Format("%d",m_GameData.T_CountFen.iZhongFan[i]);
		SetTextinfo(2001071+i,sTextInfo,bFlag);
	}

	//�Ƿ����� ����ֻ��һ���˺���
	//SetImageVisible(CTN_1000000_CTN_2001000_IMG_2001050,bFlag&&m_GameData.T_CountFen.bZiMo);

	int iShowCount = 0;
	int iHandPai[HAND_CARD_NUM]={0};
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(m_GameData.T_CountFen.m_bIsHu[i])
		{
			///<�������
			//SetTextinfo(CTN_1000000_CTN_2001000_TEXT_2001044+iShowCount,m_sUserNick[i],bFlag);

			//���ý�����ϱ��齫���Ƶ���Ϣ
			for(int j=0;j<HAND_CARD_NUM;j++)
			{
				iHandPai[j] = 0;
				iHandPai[j] = m_GameData.T_CountFen.m_byArHandPai[i][j];
			}
			//��������
			SetMjHandInfo(CTN_1000000_CTN_2001000_MjHandCtrl_2001041+iShowCount,iHandPai,m_GameData.T_CountFen.m_byArHandPaiCount[i],bFlag);
			//�齫�ֿ�
			SetMjHandGiveMode(CTN_1000000_CTN_2001000_MjHandCtrl_2001041+iShowCount,bFlag);
			//��������
			int iBeView = 0;
			//�������� 
			for(int j=0; j<5;j++)
			{
				if(m_UserData.m_UserGCPData[i][j].byType != 255)
				{
					iBeView = m_pGameFrame->DeskStation2View(m_UserData.m_UserGCPData[i][j].iBeStation);
					AddMjHandOneBlock(CTN_1000000_CTN_2001000_MjHandCtrl_2001041+iShowCount, (MJ_ACTION_TYPE)m_UserData.m_UserGCPData[i][j].byType, m_UserData.m_UserGCPData[i][j].byData, iBeView);
				}
			}
			//��������
			sTextInfo.Format("����:");
			//�Ƿ�����һɫ
			bool bIsQingYiSe = false;
			for(int j=0;j<MAX_HUPAI_TYPE;j++)
			{
				if(m_GameData.T_CountFen.byHuType[i][j] == 255)
				{
					continue;
				}
				if(m_GameData.T_CountFen.byHuType[i][j] == HUPAI_QING_YI_SE)
				{
					bIsQingYiSe = true;
					break;
				}
			}
			if(bIsQingYiSe)
			{
				sTextInfo.Format("%s ��һɫ[5��]",sTextInfo);
			}
			else
			{
				sTextInfo.Format("%s �Ƶ���[1��]",sTextInfo);
			}

			for(int j=0;j<MAX_HUPAI_TYPE;j++)
			{
				if(m_GameData.T_CountFen.byHuType[i][j] == 255)
				{
					continue;
				}
				if(m_GameData.T_CountFen.byHuType[i][j] == HUPAI_TYPE_TIAN_HU)
				{
					sTextInfo.Format("%s ���[6��]",sTextInfo);
				}
				else if(m_GameData.T_CountFen.byHuType[i][j] == HUPAI_TYPE_DI_HU)
				{
					sTextInfo.Format("%s �غ�[5��]",sTextInfo);
				}
				else if(m_GameData.T_CountFen.byHuType[i][j] == HUPAI_JIANG_YI_SE)
				{
					sTextInfo.Format("%s ��һɫ[5��]",sTextInfo);
				}
				else if(m_GameData.T_CountFen.byHuType[i][j] == HUPAI_HUN_YI_SE)
				{
					sTextInfo.Format("%s ��һɫ[2��]",sTextInfo);
				}
				else if(m_GameData.T_CountFen.byHuType[i][j] == HUPAI_QING_LONG)
				{
					sTextInfo.Format("%s һ����[2��]",sTextInfo);
				}
				else if(m_GameData.T_CountFen.byHuType[i][j] == HUPAI_TYPE_GANG_KAI)
				{
					sTextInfo.Format("%s ���ϻ�[2��]",sTextInfo);
				}
				else if(m_GameData.T_CountFen.byHuType[i][j] == HUPAI_PENG_PENG_HU)
				{
					sTextInfo.Format("%s ������[2��]",sTextInfo);
				}
				else if(m_GameData.T_CountFen.byHuType[i][j] == HUPAI_AN_QI_DUI)
				{
					sTextInfo.Format("%s �߶�[2��]",sTextInfo);
				}
			}

			///<����
			//SetTextinfo(CTN_1000000_CTN_2001000_TEXT_2001047+iShowCount,sTextInfo,bFlag);

			iShowCount++;
		}
	}
	
	ITime *pTime = NULL;
	pTime = dynamic_cast<ITime *>(m_pUI->GetIControlbyID(2001004));
	if (NULL != pTime)
	{
		pTime->SetShowMaxNum(m_GameData.m_mjRule.byBeginTime);
		pTime->SetShowMinNum(0);
		pTime->SetPLay(true);
		pTime->SetControlVisible(true);
	}
	//������Ч
	if(bFlag)
	{
		if(m_GameData.T_CountFen.i64Money[m_nDeskStation] > 0)
		{
			//Ӯ����
			PlayGameSound(m_nDeskStation,SOUND_WIN);
		}
		else if(m_GameData.T_CountFen.i64Money[m_nDeskStation] < 0)
		{
			//������
			PlayGameSound(m_nDeskStation,SOUND_LOST);
		}
	}
}
/*---------------------------------------------------------------------------------------------*/
//��ʾ��Ϸ����
void	CNewGameClient::UIShowGameSetting(bool bFlag)
{
	SetContainVisible(CTN_100000078,bFlag);
	//������Ч
	SetRButtonSelected(CTN_100000078_RButton_100000079,m_bBgMusic,bFlag);
	//��Ϸ��Ч
	SetRButtonSelected(CTN_100000078_RButton_100000081,m_bGameSound,bFlag);
}
/*---------------------------------------------------------------------------------------------*/















//��ʾ�ƶ������ؼ�
void CNewGameClient::UIShowZhuaMoveAction(POINT ptStart, POINT ptEnd, BYTE ucViewStatin, bool bShow)
{
    // �ƶ��ؼ�
    IContain *pCtn = NULL;
    if ( GETCTRL(IContain, pCtn, m_pUI, CONTAIN_ZHUAPAI_ACT) )
    {
        pCtn->SetControlVisible(bShow);
    }
    if (!bShow)
    {
        m_pGameFrame->KillTimer(TIME_ZHUAPAI);
        return;
    }
    IMjWallCtrl *pWall = NULL;
    if ( GETCTRL(IMjWallCtrl, pWall, m_pUI, ZHUAPAI_ACT_MJWALL) )
    {
        if (ucViewStatin<PLAY_COUNT)
        {
            pWall->SetViewDirection(ucViewStatin);
        }
        pWall->FillWallBack(1);
        pWall->SetControlVisible(true);
    }
    IMoveAction *pMvact = NULL;
    GETCTRL(IMoveAction, pMvact, m_pUI, ZHUAPAI_ACT_MVACT);
    if (pMvact)
    {                                    
        pMvact->SetStartP(ptStart);
        pMvact->SetEndP(ptEnd);
        pMvact->SetPlayState(true);
    }
}




// ���齫�ؼ�������ʱ������ƶ����ؼ���
int CNewGameClient::OnMoveOnMjCtrl(TUIMessage* pMessage)
{
	CString cs;
	cs.Format("hzmj::pMessage->_ch=%d",pMessage->_ch);
	OutputDebugString(cs);

   if (pMessage->_uControlID == CTN_1_MjHandCtrl_600010)
    {
		if (2==m_byHandState)  // �������״̬
        {
			IMjHandCtrl *pHand = NULL;
            GETCTRL(IMjHandCtrl, pHand, m_pUI, CTN_1_MjHandCtrl_600010);
            if (NULL!=pHand)
            {
                MJ_TILE_UIINFO uiInfo[MAX_TILES_INCTRL];
                pHand->GetTilesInfo(uiInfo, MAX_TILES_INCTRL);
                // ��ѡ�еĵ��� 
                for (int i=0; i<pHand->GetTileCount(); ++i)
                {   
                    if (LT_EMPTY==uiInfo[i].ucActFlag && uiInfo[i].ucTiles[0]==pMessage->_ch)
                    {
                        uiInfo[i].bSelect = true;
                    }
                    else
                    {
                        uiInfo[i].bSelect = false;
                    }        
                }
                pHand->SetTilesInfo(uiInfo, pHand->GetTileCount());
            }
        }
    }
	return 0;
}
// ���齫�ؼ�������ʱ����굥���ؼ�
int CNewGameClient::OnLButtonOnMjCtrl(TUIMessage* pMessage)
{
	if(pMessage->_uControlID == CTN_1_MjGangCtrl_600011)//���ƿؼ�1
	{
		
        tagGangPaiEx gang;
        gang.Clear();
		gang.bIsBigGang = m_bIsBigGang[0];
    
       
		
        gang.byUser = m_nDeskStation;
        gang.byBeGang = m_ucCurrentPlayer;
        SendGameDataEx(THING_GANG_PAI, &gang, sizeof(gang));
        // �����������
        SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));
        // �����
        SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetInvalidTile(NULL, 0));
        SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetIsUseMouse(true));
        m_byHandState = 0xFF;//����״̬
		
		return 0;
	}
	
    if (pMessage->_uControlID!=CTN_1_MjHandCtrl_600010)
    {
		if (2==m_byHandState)  // �������״̬
		{
			BYTE type = 255;
			BYTE byPs = pMessage->_ch;
			for(int i=0;i<4;++i)
			{
				if(byPs == m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[i][1])
				{
					type = m_GameData.T_CPGNotify[m_nDeskStation].m_iGangData[i][0];
				}
			}
			tagGangPaiEx gang;
			gang.Clear();
			gang.byPs = byPs;
			gang.byType = type;
			memset(gang.byGangPai,byPs,sizeof(gang.byGangPai));
			gang.byUser = m_nDeskStation;
			gang.byBeGang = m_ucCurrentPlayer;
			SendGameDataEx(THING_GANG_PAI, &gang, sizeof(gang));
			// �����������
			SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600000, SetControlVisible(false));
			// �����
			SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetInvalidTile(NULL, 0));
			SAFE_CTRL_OPERATE(IMjHandCtrl, CTN_1_MjHandCtrl_600010, SetIsUseMouse(true));
			m_byHandState = 0xFF;//����״̬
		}
        return 0;
    }
	return 0;
}
/*--------------------------------------------------------------------------------------------*/
// �����ؼ���Ϣ	DWJ
int CNewGameClient::OnCartoonFinish(TUIMessage *pMessage)
{
    
	switch(pMessage->_uControlID)
	{
	case CTN_1000000_CTN_1000100_ExAnimate_2000010:
	case CTN_1000000_CTN_1000100_ExAnimate_2000011:
	case CTN_1000000_CTN_1000100_ExAnimate_2000012:
	case CTN_1000000_CTN_1000100_ExAnimate_2000013:
		{
			int iFramNum = 0;
			int iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
			if(m_GameData.m_byThingDoing == THING_2SEZI_NT)
			{
				iView = m_pGameFrame->DeskStation2View(0);
				iFramNum = 14 + m_GameData.T_TwoSeziNt.bySezi0;
				SetAnimateShowFrame(pMessage->_uControlID,true,iFramNum,G_PointEnd[0][iView]);

				if(m_GameData.m_byNtStation != 255)
				{
					//��ʾׯ��
					int iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
					SetImageVisible(CTN_1000000_CTN_2002000_IMG_2010020+iView,true);
					//��ʾׯ�ҷ�λ
					UIShowNtInfo(true);
				}
			}
			else if(m_GameData.m_byThingDoing == THING_2SEZI_DIR)
			{
				iFramNum = 14 + m_GameData.T_TwoSeziDir.bySezi0;
				SetAnimateShowFrame(pMessage->_uControlID,true,iFramNum,G_PointEnd[0][iView]);
			}
			else if (m_GameData.m_byThingDoing == THING_2SEZI_GP)
			{
				iFramNum = 14 + m_GameData.T_TwoSeziGetPai.bySezi0;
				SetAnimateShowFrame(pMessage->_uControlID,true,iFramNum,G_PointEnd[0][iView]);
			}
			break;
		}
	case CTN_1000000_CTN_1000100_ExAnimate_2000020:
	case CTN_1000000_CTN_1000100_ExAnimate_2000021:
	case CTN_1000000_CTN_1000100_ExAnimate_2000022:
	case CTN_1000000_CTN_1000100_ExAnimate_2000023:
		{
			int iFramNum = 0;
			int iView = m_pGameFrame->DeskStation2View(m_GameData.m_byNtStation);
			if(m_GameData.m_byThingDoing == THING_2SEZI_NT)
			{
				iView = m_pGameFrame->DeskStation2View(0);
				iFramNum = 14 + m_GameData.T_TwoSeziNt.bySezi1;
				SetAnimateShowFrame(pMessage->_uControlID,true,iFramNum,G_PointEnd[1][iView]);
			}
			else if(m_GameData.m_byThingDoing == THING_2SEZI_DIR)
			{
				iFramNum = 14 + m_GameData.T_TwoSeziDir.bySezi1;
				SetAnimateShowFrame(pMessage->_uControlID,true,iFramNum,G_PointEnd[1][iView]);
			}
			else if (m_GameData.m_byThingDoing == THING_2SEZI_GP)
			{
				iFramNum = 14 + m_GameData.T_TwoSeziGetPai.bySezi1;
				SetAnimateShowFrame(pMessage->_uControlID,true,iFramNum,G_PointEnd[1][iView]);
			}
			break;
		}
	case CTN_1000000_CTN_2006000_Time_2006001:	//ʱ���ʱ�䵽
	case 2001004:
		{
			if(m_bGameStation == GS_WAIT_ARGEE || m_bGameStation == GS_WAIT_NEXT || m_bGameStation == GS_WAIT_SETGAME)
			{
				m_pGameFrame->CloseClient();
			}
			else
			{
				ShowCountDownTimer(m_nDeskStation,0,false);
			}
			break;
		}
	case CTN_1000000_CTN_2010000_Time_20100003:	//����ʱ�䵽
		{
			ShowMaiMaContain(false);
			break;
		}
	}
   
	return 0;
}
/*--------------------------------------------------------------------------------------------*/
//������ҵĵ��ǳ�
void	CNewGameClient::GetUserName()
{
	UserInfoForGame UserInfo;
    for (int i=0; i<PLAY_COUNT; ++i)
    {
		if(m_pGameFrame->GetUserInfo(i,UserInfo))
		{
			m_sUserNick[i].Format("%s",UserInfo.szNickName);
		}
	}
}
/*--------------------------------------------------------------------------------------------*/
//����һ�뵹��ʱ m_iRemaindTime
void	CNewGameClient::SetOneSecondCountDown(int iTime)
{
	OutputDebugString("dwjsdmj::SetOneSecondCountDown");
	m_pGameFrame->KillTimer(TIME_ONE_SECOND);
	m_GameData.m_iRemaindTime = iTime;
	m_pGameFrame->SetTimer(TIME_ONE_SECOND, 1000);
}
/*--------------------------------------------------------------------------------------------*/
//���ü�ʱ��
void CNewGameClient::OnSetTimer(BYTE station,int type)
{
	OnKillAllTimer();
	BYTE viewstation = m_pGameFrame->DeskStation2View(station);//�����ͼλ��
	int itime = 1;
	switch(type)
	{
	case TIME_OUT_CARD://���ƶ�ʱ��
		{
			//itime = m_MJSetting.ucOutcardTime;
			//if(station == m_nDeskStation && (m_UserData.m_bTuoGuan[station] || m_UserData.m_bTing[station]))//�Լ����������й�״̬
			//{
			//	itime = m_MJSetting.nAutoTime;
			//}
		}
		break;
	case TIME_BLOCK_CARD://���ƶ�ʱ��
		{
// 			itime = m_MJSetting.ucBlockTime;
// 			if(station == m_nDeskStation && m_UserData.m_bTuoGuan[station] )//�Լ����������й�״̬
// 			{
// 				itime = m_MJSetting.nAutoTime;
// 			}
		}
		break;
	case TIME_BEGIN:// �Զ���ʼ
		{
/*			itime = m_MJSetting.ucBeginTime;*/
		}
		break;
	
	}
	m_pGameFrame->SetTimer(type, itime*1000);
    UIShowTimer(viewstation, itime, true);//��ʾ��ʱ������
}
//ɾ������ʱ��
void CNewGameClient::OnKillAllTimer()
{
    UIShowTimer(0, 0, false);//���ؼ�ʱ������
	m_pGameFrame->KillTimer(TIME_OUT_CARD);
	m_pGameFrame->KillTimer(TIME_BLOCK_CARD);
	m_pGameFrame->KillTimer(TIME_BEGIN);
}
/// ������ʱ��
BOOL CNewGameClient::KillGameTimer(int TimerID)
{
	m_pGameFrame->KillTimer(TimerID);
	m_pGameFrame->KillTimer(ID_WARMING_TIME);//ɾ�������ʱ��
	UIShowTimer(0,0,false);//���ؼ�ʱ��
	return FALSE;
}

// ���ö�ʱ��
void CNewGameClient::UIShowTimer(BYTE bViewStation, int nTime, bool bShow )
{
    // ����ؼ�ID
    static int nCtrl[PLAY_COUNT] = {CTN_1_CTN_600031_IMG_600020  , CTN_1_CTN_600031_IMG_600020+1  , CTN_1_CTN_600031_IMG_600020+2  , CTN_1_CTN_600031_IMG_600020+3  };
    if (!bShow)
    {
        for (int i=0; i<PLAY_COUNT; ++i)
        {
            SAFE_CTRL_OPERATE(IImage, nCtrl[i], SetControlVisible(false) );
        }
        SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600031, SetControlVisible(bShow));
        return;
    }
    SAFE_CTRL_OPERATE(ITime, CTN_1_CTN_600031_Time_600030, SetControlVisible(true));
    SAFE_CTRL_OPERATE(ITime, CTN_1_CTN_600031_Time_600030, SetShowMaxNum(nTime));
    SAFE_CTRL_OPERATE(ITime, CTN_1_CTN_600031_Time_600030, SetShowMinNum(1));
    SAFE_CTRL_OPERATE(ITime, CTN_1_CTN_600031_Time_600030, SetPLay(true));
    for (int i=0; i<PLAY_COUNT; ++i)
    {
        SAFE_CTRL_OPERATE(IImage, nCtrl[i], SetControlVisible(i==bViewStation));
    }
    SAFE_CTRL_OPERATE(IContain, CTN_1_CTN_600031, SetControlVisible(bShow));
}


//������Ϸ����:����֪ͨ��Ϣ������������
void CNewGameClient::SendGameDataEx(BYTE id)
{
	m_pGameFrame->SendGameData(id, NULL, 0);
}

//������Ϸ����
void CNewGameClient::SendGameDataEx(BYTE id,void *pData,UINT uSize)
{
	m_pGameFrame->SendGameData( id,pData, uSize);
}

//��˸����
void CNewGameClient::FlashWindow()
{
	HWND hwnd = m_pUI->GetWindowsHwnd();
	FLASHWINFO info;
	info.cbSize = sizeof(FLASHWINFO);
	info.dwFlags = FLASHW_TRAY|FLASHW_TIMERNOFG;
	info.hwnd = hwnd;
	info.uCount = 10;
	info.dwTimeout = 500;
	FlashWindowEx(&info);
}








///���ƿ���
//���ַ���ת����������
void CNewGameClient::GetPaiFormChar(int &count ,BYTE data[],const wchar_t pszPai[],int type)
{
	memset(data,255,sizeof(data));
	count=0;
	switch(type)
	{
	case 0://��ȫ������
		{
			for(int i=2;i<MAX_HAND_PAI*2 + 2;i+=2)
			{
				if(pszPai[i] == 0)
					break;
				data[i/2-1] = ChangeChar(pszPai[i],pszPai[i+1]);
				count++;
			}
		}
		break;
	case 1://�����ƻ���ĳЩ��
		{
			for(int i=0;i<MAX_HAND_PAI*2;i+=2)
			{
				if(pszPai[i] == 0)
					break;
				data[i/2] = ChangeChar(pszPai[i],pszPai[i+1]);
				count++;
			}
		}
		break;
	case 2://������һ��ץ��
		{
			data[0] = ChangeChar(pszPai[2],pszPai[3]);
			count = 1;
		}
		break;
	}
}
//�������ַ�ת������
BYTE CNewGameClient::ChangeChar(wchar_t data1,wchar_t data2)
{
	BYTE pai = 255;
	BYTE num1 = 0,num2 = 0;

	if(data2 == 'b')//ͬ
	{
		num2 = 20;
	}
	else if(data2 == 't')//��
	{
		num2 = 10;
	}
	if(data1 >= '1' && data1<='9' && (data2 == 'w' || data2 == 'b' || data2 == 't'))
	{
		num1 = data1 - '1' +1;
		pai = num1+num2;
	}
	else if(data1 == 'd' && data2=='f')//����
	{
		pai = 31;
	}
	else if(data1 == 'n' && data2=='f')//�Ϸ�
	{
		pai = 32;
	}
	else if(data1 == 'x' && data2=='f')//����
	{
		pai = 33;
	}
	else if(data1 == 'b' && data2=='f')//����
	{
		pai = 34;
	}
	else if(data1 == 'h' && data2=='z')//����
	{
		pai = 35;
	}
	else if(data1 == 'f' && data2=='c')//����
	{
		pai = 36;
	}
	else if(data1 == 'b' && data2=='b')//�װ�
	{
		pai = 37;
	}
	return pai;
}





/*----------------------------------------------------------------------------------------------*/
//������Ϸ����
void CNewGameClient::LoadGameSet()
{
	CString nid;
	nid.Format("%d",NAME_ID);
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +nid +"_c.ini");
	CString key = TEXT("Game");	
	//������Ч
	m_bBgMusic		= ("1" == f.GetKeyVal(key,"BgMusic","1"));
	//��Ϸ��Ч
	m_bGameSound	= ("1" == f.GetKeyVal(key,"GameSound","1"));
	return;
}
/*----------------------------------------------------------------------------------------------*/
//������Ϸ����
void CNewGameClient::SaveGameSet()
{
	CString nid;
	nid.Format("%d",NAME_ID);
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +nid +"_c.ini");
	CString key = TEXT("Game");	

	IRadioButton *pRadioButton = NULL;
	//������Ч
	pRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(CTN_100000078_RButton_100000079));
	if (NULL != pRadioButton)
	{
		if (pRadioButton->GetIsSelect())
		{
			//����û�в��� ��ô�����Ҫ��ʼ���ű���������
			if (!m_bBgMusic)
			{
				m_bBgMusic = true;
				PlayBgSound(true);
			}
			f.SetKeyValString(key,"BgMusic","1");
		}
		else
		{
			//������ڲ��� ��ô��Ҫֹͣ��
			if (m_bBgMusic)
			{
				m_bBgMusic = false;
				if (NULL != m_SoundEnginBg)
				{
					m_SoundEnginBg->stopAllSounds();
				}
			}
			f.SetKeyValString(key,"BgMusic","0");
		}
	}
	//��Ϸ��Ч
	pRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(CTN_100000078_RButton_100000081));
	if (NULL != pRadioButton)
	{
		if (pRadioButton->GetIsSelect())
		{
			m_bGameSound = true;
			f.SetKeyValString(key,"GameSound","1");
		}
		else
		{
			//������ڲ�����Ч ��ʱ��Ҫֹͣ
			if (m_bGameSound)
			{
				m_bGameSound = false;
				if (NULL != m_SoundEngineGame)
				{
					m_SoundEngineGame->stopAllSounds();
				}
			}
			f.SetKeyValString(key,"GameSound","0");
		}
	}
	return;
}
/*----------------------------------------------------------------------------------------------*/
//���ű�����Ч	DWJ
void	CNewGameClient::PlayBgSound(bool bISLoop)
{
	if(!m_SoundEnginBg)
	{
		return ;
	}
	if(!m_bBgMusic)
	{
		return ;
	}

	char szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)) ; 
	wsprintf(szFileName,"sound\\bg.ogg");
	m_SoundEnginBg->setSoundVolume(0.5);
	m_SoundEnginBg->play2D(szFileName,bISLoop);
}
/*----------------------------------------------------------------------------------------------*/
//������Ϸ��Ч
void	CNewGameClient::PlayGameSound(BYTE byDeskStation, int iSoundID, bool bISLoop )
{
	if(!m_SoundEngineGame)
	{
		return ;
	}
	if(!m_bGameSound)
	{
		return ;
	}

	char szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)); 
	if(iSoundID >= 100)
	{
		switch(iSoundID)
		{
		case SOUND_BEGIN:	//��ʼ
			{
				wsprintf(szFileName,"sound\\Begin.ogg");
				break;
			}
		case SOUND_READY:	//׼��
			{
				wsprintf(szFileName,"sound\\Ready.ogg");
				break;
			}
		case SOUND_ZHUAPAI:	//ץ��
			{
				wsprintf(szFileName,"sound\\Zhuapai.ogg");
				break;
			}
		case SOUND_BLOCK:	//����
			{
				wsprintf(szFileName,"sound\\Block.ogg");
				break;
			}
		case SOUND_OUTCARD:	//����
			{
				wsprintf(szFileName,"sound\\OutcardWeight.ogg");
				break;
			}
		case SOUND_WIN:		//Ӯ��
			{
				wsprintf(szFileName,"sound\\win.ogg");
				break;
			}
		case SOUND_LOST:	//����
			{
				wsprintf(szFileName,"sound\\lost.ogg");
				break;
			}
		case SOUND_SEZI:	//ɫ��
			{
				wsprintf(szFileName,"sound\\Sezi.ogg");
				break;
			}
		case SOUND_MOUSE_ENTER:	//������
			{
				wsprintf(szFileName,"sound\\MouseEnter.ogg");
				break;
			}
		case SOUND_COUNT_DOWN:
			{
				wsprintf(szFileName,"sound\\CountDown.ogg");
				break;
			}
		}
	}
	else
	{
		UserInfoForGame user;
		if (m_pGameFrame->GetUserInfo(byDeskStation,user))
		{
			if (user.bBoy)
			{
				//��
				if(iSoundID >=1 && iSoundID <=9)
				{
					wsprintf(szFileName,"sound\\putong\\man\\%dwan.wav",iSoundID);
				}
				else if(iSoundID >=11 && iSoundID <=19)	//��
				{
					iSoundID = iSoundID%10;
					wsprintf(szFileName,"sound\\putong\\man\\%dtiao.wav",iSoundID);
				}
				else if(iSoundID >=21 && iSoundID <=29)	//Ͳ
				{
					iSoundID = iSoundID%10;
					wsprintf(szFileName,"sound\\putong\\man\\%dtong.wav",iSoundID);
				}
				else if(iSoundID == 31)	//����
				{
					wsprintf(szFileName,"sound\\putong\\man\\dongfeng.wav",iSoundID);
				}
				else if(iSoundID == 32)	//�Ϸ�
				{
					wsprintf(szFileName,"sound\\putong\\man\\nanfeng.wav",iSoundID);
				}
				else if(iSoundID == 33)	//����
				{
					wsprintf(szFileName,"sound\\putong\\man\\xifeng.wav",iSoundID);
				}
				else if(iSoundID == 34)	//����
				{
					wsprintf(szFileName,"sound\\putong\\man\\beifeng.wav",iSoundID);
				}
				else if(iSoundID == 35)	//����
				{
					wsprintf(szFileName,"sound\\putong\\man\\zhong.wav",iSoundID);
				}
				else if(iSoundID == 36)	//����
				{
					wsprintf(szFileName,"sound\\putong\\man\\fa.wav",iSoundID);
				}
				else if(iSoundID == 37)	//�װ�
				{
					wsprintf(szFileName,"sound\\putong\\man\\bai.wav",iSoundID);
				}
				else if(iSoundID == SOUND_CHI)	//��
				{
					wsprintf(szFileName,"sound\\putong\\man\\chi.wav",iSoundID);
				}
				else if(iSoundID == SOUND_PENG)	//��
				{
					wsprintf(szFileName,"sound\\putong\\man\\peng.wav",iSoundID);
				}
				else if(iSoundID == SOUND_GANG)	//��
				{
					wsprintf(szFileName,"sound\\putong\\man\\gang.wav",iSoundID);
				}
				else if(iSoundID == SOUND_TING)	//��
				{
					wsprintf(szFileName,"sound\\putong\\man\\ting.wav",iSoundID);
				}
				else if(iSoundID == SOUND_HU)	//��
				{
					wsprintf(szFileName,"sound\\putong\\man\\hu.wav",iSoundID);
				}
				else if(iSoundID == SOUND_ZIMO)	//��
				{
					wsprintf(szFileName,"sound\\putong\\man\\zimo.ogg",iSoundID);
				}
			}
			else
			{
				//��
				if(iSoundID >=1 && iSoundID <=9)
				{
					wsprintf(szFileName,"sound\\putong\\wom\\%dwan.wav",iSoundID);
				}
				else if(iSoundID >=11 && iSoundID <=19)	//��
				{
					iSoundID = iSoundID%10;
					wsprintf(szFileName,"sound\\putong\\wom\\%dtiao.wav",iSoundID);
				}
				else if(iSoundID >=21 && iSoundID <=29)	//Ͳ
				{
					iSoundID = iSoundID%10;
					wsprintf(szFileName,"sound\\putong\\wom\\%dtong.wav",iSoundID);
				}
				else if(iSoundID == 31)	//����
				{
					wsprintf(szFileName,"sound\\putong\\wom\\dongfeng.wav",iSoundID);
				}
				else if(iSoundID == 32)	//�Ϸ�
				{
					wsprintf(szFileName,"sound\\putong\\wom\\nanfeng.wav",iSoundID);
				}
				else if(iSoundID == 33)	//����
				{
					wsprintf(szFileName,"sound\\putong\\wom\\xifeng.wav",iSoundID);
				}
				else if(iSoundID == 34)	//����
				{
					wsprintf(szFileName,"sound\\putong\\wom\\beifeng.wav",iSoundID);
				}
				else if(iSoundID == 35)	//����
				{
					wsprintf(szFileName,"sound\\putong\\wom\\zhong.wav",iSoundID);
				}
				else if(iSoundID == 36)	//����
				{
					wsprintf(szFileName,"sound\\putong\\wom\\fa.wav",iSoundID);
				}
				else if(iSoundID == 37)	//�װ�
				{
					wsprintf(szFileName,"sound\\putong\\wom\\bai.wav",iSoundID);
				}
				else if(iSoundID == SOUND_CHI)	//��
				{
					wsprintf(szFileName,"sound\\putong\\wom\\chi.wav",iSoundID);
				}
				else if(iSoundID == SOUND_PENG)	//��
				{
					wsprintf(szFileName,"sound\\putong\\wom\\peng.wav",iSoundID);
				}
				else if(iSoundID == SOUND_GANG)	//��
				{
					wsprintf(szFileName,"sound\\putong\\wom\\gang.wav",iSoundID);
				}
				else if(iSoundID == SOUND_TING)	//��
				{
					wsprintf(szFileName,"sound\\putong\\wom\\ting.wav",iSoundID);
				}
				else if(iSoundID == SOUND_HU)	//��
				{
					wsprintf(szFileName,"sound\\putong\\wom\\hu.wav",iSoundID);
				}
				else if(iSoundID == SOUND_ZIMO)	//��
				{
					wsprintf(szFileName,"sound\\putong\\wom\\zimo.ogg",iSoundID);
				}
			}
		}
		else
		{
			//��
			if(iSoundID >=1 && iSoundID <=9)
			{
				wsprintf(szFileName,"sound\\putong\\man\\%dwan.wav",iSoundID);
			}
			else if(iSoundID >=11 && iSoundID <=19)	//��
			{
				iSoundID = iSoundID%10;
				wsprintf(szFileName,"sound\\putong\\man\\%dtiao.wav",iSoundID);
			}
			else if(iSoundID >=21 && iSoundID <=29)	//Ͳ
			{
				iSoundID = iSoundID%10;
				wsprintf(szFileName,"sound\\putong\\man\\%dtong.wav",iSoundID);
			}
			else if(iSoundID == 31)	//����
			{
				wsprintf(szFileName,"sound\\putong\\man\\dongfeng.wav",iSoundID);
			}
			else if(iSoundID == 32)	//�Ϸ�
			{
				wsprintf(szFileName,"sound\\putong\\man\\nanfeng.wav",iSoundID);
			}
			else if(iSoundID == 33)	//����
			{
				wsprintf(szFileName,"sound\\putong\\man\\xifeng.wav",iSoundID);
			}
			else if(iSoundID == 34)	//����
			{
				wsprintf(szFileName,"sound\\putong\\man\\beifeng.wav",iSoundID);
			}
			else if(iSoundID == 35)	//����
			{
				wsprintf(szFileName,"sound\\putong\\man\\zhong.wav",iSoundID);
			}
			else if(iSoundID == 36)	//����
			{
				wsprintf(szFileName,"sound\\putong\\man\\fa.wav",iSoundID);
			}
			else if(iSoundID == 37)	//�װ�
			{
				wsprintf(szFileName,"sound\\putong\\man\\bai.wav",iSoundID);
			}else if(iSoundID == SOUND_CHI)	//��
			{
				wsprintf(szFileName,"sound\\putong\\man\\chi.wav",iSoundID);
			}
			else if(iSoundID == SOUND_PENG)	//��
			{
				wsprintf(szFileName,"sound\\putong\\man\\peng.wav",iSoundID);
			}
			else if(iSoundID == SOUND_GANG)	//��
			{
				wsprintf(szFileName,"sound\\putong\\man\\gang.wav",iSoundID);
			}
			else if(iSoundID == SOUND_TING)	//��
			{
				wsprintf(szFileName,"sound\\putong\\man\\ting.wav",iSoundID);
			}
			else if(iSoundID == SOUND_HU)	//��
			{
				wsprintf(szFileName,"sound\\putong\\man\\hu.wav",iSoundID);
			}
			else if(iSoundID == SOUND_ZIMO)	//��
			{
				wsprintf(szFileName,"sound\\putong\\man\\zimo.ogg",iSoundID);
			}
		}
	}


	m_SoundEngineGame->setSoundVolume(1.0);
	m_SoundEngineGame->play2D(szFileName,bISLoop);
}
/*----------------------------------------------------------------------------------------------*/
//��ʼ������
void CNewGameClient::InitGameDate()
{   
	m_bMouseEnter = false;
	m_bIsHua	  = false;


    m_ucCurrentPlayer = 0xFF;                   //��ֵ���
 
    m_byHandState = 0xFF;                       //����״̬ 0����״̬ 1�����ѡ��״̬
  

   
	///��ʼ��һЩ����	
	for (int i=0; i<PLAY_COUNT; ++i)
    {
        m_UserDataCard[i].ClearOut();
        m_UserDataCard[i].ClearCPG();
        m_UserDataCard[i].ClearHand();        
    }
    m_UserData.InitData();
	m_GameData.InitData();
    m_nTimeOutCnt = 0;
	m_GameData.m_iRemaindTime = 0;
}

























//--------------------------------ADD-BY-DWJ--------------------------------------------------------------
/*���º��� ������Ϸ��ͨ��*/
//--------------------------------------------------------------------------------------------------------
//���ö����ؼ����Ŷ���
void	CNewGameClient::SetAnimatePlay(const int AnimateID,bool bFlag,POINT pointStart,POINT pointEnd,CURVE_TYPE CT_TYPE,BYTE byPlayNum, bool bIsLoop)
{
	IExAnimate *pAnimate = NULL;
	pAnimate = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(AnimateID));

	if (NULL!=pAnimate)
	{
		pAnimate->SetPlayNum(byPlayNum);
		if (pointEnd.x != 0 && 0 != pointEnd.y)
		{
			pAnimate->SetEnd(pointEnd);
		}

		if (pointStart.x != 0 && 0 != pointStart.y)
		{
			pAnimate->SetStartP(pointStart);
		}

		pAnimate->SetCurveInstance(CT_TYPE);
		pAnimate->SetPlayState(bFlag);
		pAnimate->SetControlVisible(bFlag);
		pAnimate->SetIsLoop(bIsLoop);
	}

}
//--------------------------------------------------------------------------------------------------------
//���ö����ؼ����Ŷ���
void	CNewGameClient::SetAnimatePlay(const int AnimateID,bool bFlag,POINT position,bool bIsLoop ,CURVE_TYPE CT_TYPE )
{
	IExAnimate *pAnimate = NULL;
	pAnimate = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(AnimateID));

	if (NULL!=pAnimate)
	{
		pAnimate->SetGameXY(position.x,position.y);
		pAnimate->SetCurveInstance(CT_TYPE);
		pAnimate->SetPlayState(bFlag);
		pAnimate->SetControlVisible(bFlag);
		pAnimate->SetIsLoop(bIsLoop);
	}
}
//--------------------------------------------------------------------------------------------------------
//���ö����ؼ�����ָ������
void	CNewGameClient::SetAnimatePlay(const int AnimateID,wchar_t wsFilePath[],bool bFlag,BYTE byPlayNum , bool bIsLoop )
{
	IExAnimate *pAnimate = NULL;
	pAnimate = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(AnimateID));

	if (NULL!=pAnimate)
	{
		pAnimate->SetExtFilePath(wsFilePath) ;
		pAnimate->LoadFromFile();
		pAnimate->SetPlayNum(byPlayNum);
		pAnimate->SetPlayState(bFlag);
		pAnimate->SetControlVisible(bFlag);
		pAnimate->SetIsLoop(bIsLoop);
	}
}
//--------------------------------------------------------------------------------------------------------
//���ö�����ʾĳһ֡
void	CNewGameClient::SetAnimateShowFrame(const int AnimateID,bool bFlag,int iFrame,POINT point)
{
	IExAnimate *pAnimate = NULL;
	pAnimate = dynamic_cast<IExAnimate *>(m_pUI->GetIControlbyID(AnimateID));

	if (NULL!=pAnimate)
	{
		if (-1 == iFrame)
		{
			pAnimate->SetShowFrame(pAnimate->GetFrameNum()-1);
		}
		else
		{
			pAnimate->SetShowFrame(iFrame);
		}
		if(point.x != 0 && point.y != 0)
		{
			pAnimate->SetGameXY(point.x, point.y);
		}
		pAnimate->SetControlVisible(bFlag);

	}
}
//--------------------------------------------------------------------------------------------------------
//���ð�ť�Ƿ����
void	CNewGameClient::SetBtEnable(const int iButtonID, bool bFlag)
{

	IButton *pButton = NULL;
	pButton = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(iButtonID));
	if (NULL != pButton)
	{
		pButton->SetEnable(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//���ð�ť�Ƿ�ɼ�
void	CNewGameClient::SetBtVisible(const int iButtonID, bool bFlag)
{
	IButton *pButton = NULL;
	pButton = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(iButtonID));
	if (NULL != pButton)
	{
		pButton->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//��ȡ��ť�Ƿ����
bool	CNewGameClient::GetBtIsEnable(const int iButtonID)
{
	IButton *pButton = NULL;
	pButton = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(iButtonID));
	if (NULL != pButton)
	{
		return	pButton->GetEnable();
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------
//����ʱ�����ʾ����ʱ
void	CNewGameClient::SetTimePlay(const int iTimeID,bool bFlag, BYTE byMaxTime, BYTE byMinTime)
{
	ITime *pTime = NULL;
	pTime = dynamic_cast<ITime *>(m_pUI->GetIControlbyID(iTimeID));
	if (NULL != pTime)
	{
		pTime->SetShowMaxNum(byMaxTime);
		pTime->SetShowMinNum(byMinTime);
		pTime->SetPLay(bFlag);
		pTime->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//����ͼƬ�ؼ���λ��
void	CNewGameClient::SetImagePosition(const int iImageID, int iPositonX, int iPositonY, bool bFlag)
{
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImageID));
	if (NULL != pImage)
	{
		pImage->SetGameXY(iPositonX,iPositonY);
		pImage->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//����ͼƬ�ؼ��Ƿ�ɼ�
void	CNewGameClient::SetImageVisible(const int iImageID, bool bFlag,int iSrcX, int iSrcY)
{
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImageID));
	if (NULL != pImage)
	{
		pImage->SetControlVisible(bFlag);
		pImage->SetSrcX(iSrcX);
		pImage->SetSrcY(iSrcY);
	}
}
//--------------------------------------------------------------------------------------------------------
//����ͼƬ�ؼ�������ʾָ�����ļ�
void	CNewGameClient::SetImageLoadSrc(const int iImageID,CString sImagSrc,bool bFlag)
{
	//virtual int LoadPic(wchar_t *szFileName)=0;
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImageID));
	if (NULL != pImage)
	{
		pImage->LoadPic(CA2W(sImagSrc));
		pImage->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//���������ؼ��ɼ�
void	CNewGameClient::SetContainVisible(const int iContainID,bool bFlag)
{
	IContain *pContain = NULL;
	pContain = dynamic_cast<IContain *>(m_pUI->GetIControlbyID(iContainID));
	if (NULL != pContain)
	{
		pContain->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//�����ı��ؼ���ʾ����
void	CNewGameClient::SetTextinfo(const int iTextID,__int64 iNum, bool bFlag)
{
	IText *pText = NULL;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(iTextID));
	if (NULL != pText)
	{
		CString sText;
		sText.Format("%I64d",iNum);
		pText->SetText(CA2W(sText));
		pText->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//�����ı��ؼ���ʾ����
void	CNewGameClient::SetTextinfo(const int iTextID,TCHAR sChar[], bool bFlag)
{
	IText *pText = NULL;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(iTextID));
	if (NULL != pText)
	{
		CString sText;
		sText.Format("%s",sChar);
		pText->SetText(CA2W(sText));
		pText->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//�����ı��ؼ���ʾ����
void	CNewGameClient::SetTextinfo(const int iTextID,CString sText, bool bFlag)
{
	IText *pText = NULL;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(iTextID));
	if (NULL != pText)
	{
		pText->SetText(CA2W(sText));
		pText->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//�������ֿռ���ʾ����
void	CNewGameClient::SetInumInfo(const int iNumID, __int64 i64Num, bool bFlag)
{
	INum *pNum = NULL;
	pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(iNumID));
	if (NULL != pNum)
	{
		pNum->SetNum(i64Num);
		pNum->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//���ó���ؼ���ʾֵ
void	CNewGameClient::SetShCtrlInfo(const int iShCtrlID, int iNum, bool bFlag)
{
	IShCtrl *pShCtrl = NULL;
	pShCtrl = dynamic_cast<IShCtrl *>(m_pUI->GetIControlbyID(iShCtrlID));
	if (NULL != pShCtrl)
	{
		pShCtrl->SetShNote(iNum);
		pShCtrl->SetControlVisible(bFlag);
	}

}
//--------------------------------------------------------------------------------------------------------
//���õ�ѡ��ؼ��Ƿ�ѡ��
void	CNewGameClient::SetRButtonSelected(const int iRButtonID,bool bSelected, bool bFlag)
{
	IRadioButton *pRadioButton = NULL;
	pRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(iRButtonID));
	if (NULL != pRadioButton)
	{
		pRadioButton->SetIsSelect(bSelected);
		pRadioButton->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//�����齫��ǽ�ؼ��Ƿ�ɼ�
void	CNewGameClient::SetMjWallInfo(const int iMjWallID,int iFillNum,bool bFlag)
{
	IMjWallCtrl *pMjWall = NULL;
	pMjWall = dynamic_cast<IMjWallCtrl*>(m_pUI->GetIControlbyID(iMjWallID));
	if (NULL != pMjWall)
	{
		pMjWall->FillWallBack(iFillNum);
		pMjWall->SetMaxLength(iFillNum);
		pMjWall->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//�����齫��ǽָ�����齫��Ϣ
void	CNewGameClient::SetMjWallTittle(const int iMjWallID,int iValue,int iPosition,bool bFlag)
{
	IMjWallCtrl *pMjWall = NULL;
	pMjWall = dynamic_cast<IMjWallCtrl*>(m_pUI->GetIControlbyID(iMjWallID));
	if (NULL != pMjWall)
	{
		pMjWall->SetWallTile(iValue,iPosition);
		pMjWall->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//�����齫���ƿؼ���Ϣ
void	CNewGameClient::SetMjGiveInfo(const int iMjGiveID, int iLineNum, int iHandTiles[], int iCount, bool bFlag)
{
	IMjGiveCtrl *pMjGive = NULL;
	pMjGive = dynamic_cast<IMjGiveCtrl*>(m_pUI->GetIControlbyID(iMjGiveID));
	if (NULL != pMjGive)
	{
		pMjGive->SetGiveInfo(iHandTiles,iCount);
		pMjGive->SetGiveLineCnt(iLineNum);
		pMjGive->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//��ȡ�齫���Ƶľ���
RECT	CNewGameClient::GetMjGiveRect(const int iMjGiveID,int iIndex)
{
	RECT r;
	memset(&r,0,sizeof(r));
	IMjGiveCtrl *pMjGive = NULL;
	pMjGive = dynamic_cast<IMjGiveCtrl*>(m_pUI->GetIControlbyID(iMjGiveID));
	if (NULL != pMjGive)
	{
		r = pMjGive->GetTileRectByIndex(iIndex);
	}
	return r;
}
//--------------------------------------------------------------------------------------------------------
//�����齫���ƿؼ��Ƿ�ɼ�
void	CNewGameClient::SetMjHandVisible(const int iMjHandID, bool bFlag)
{
	IMjHandCtrl *pMjHand = NULL;
	pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(iMjHandID));
	if (NULL != pMjHand)
	{
		pMjHand->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//�����齫���ƿؼ���Ϣ
void	CNewGameClient::SetMjHandInfo(const int iMjHandID, int byCards[], int iCardCount,bool bFlag,bool bLaydown)
{
	IMjHandCtrl *pMjHand = NULL;
	pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(iMjHandID));
	if (NULL != pMjHand)
	{
		pMjHand->SetHandInfo(byCards,iCardCount);
		pMjHand->SetControlVisible(bFlag);
		pMjHand->SetIsLaydown(bLaydown);
	}
}
//--------------------------------------------------------------------------------------------------------
//�����齫���ƿؼ���
void	CNewGameClient::SetMjHandLayDown(const int iMjHandID, bool bLayDown)
{
	IMjHandCtrl *pMjHand = NULL;
	pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(iMjHandID));
	if (NULL != pMjHand)
	{
		pMjHand->SetIsLaydown(bLayDown);
	}
}
//--------------------------------------------------------------------------------------------------------
//�����齫���ƿؼ����һ���Ƿ����
void	CNewGameClient::SetMjHandGiveMode(const int iMjHandID, bool bMode)
{
	IMjHandCtrl *pMjHand = NULL;
	pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(iMjHandID));
	if (NULL != pMjHand)
	{
		pMjHand->SetGiveTileMode(bMode);
	}
}
//--------------------------------------------------------------------------------------------------------
//����齫�ؼ�������Ϣ
void	CNewGameClient::ClearMjHandBlock(const int iMjHandID)
{
	IMjHandCtrl *pMjHand = NULL;
	pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(iMjHandID));
	if (NULL != pMjHand)
	{
		pMjHand->ClearAllBlock();
	}
}
//--------------------------------------------------------------------------------------------------------
//����������Ϣ
void	CNewGameClient::AddMjHandOneBlock(const int iMjHandID, MJ_ACTION_TYPE BlockType, BYTE byCardData[], BYTE byBeChair)
{
	IMjHandCtrl *pMjHand = NULL;
	pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(iMjHandID));

	if (NULL != pMjHand)
	{
		BYTE byMyView = m_pGameFrame->DeskStation2View(m_nDeskStation);

		if(BlockType == AT_QUADRUPLET_CONCEALED && byBeChair != byMyView)
		{
			memset(byCardData,0,sizeof(byCardData));
		}
		
		pMjHand->AddOneBlock((MJ_LOGIC_TYPE)BlockType, byCardData, byBeChair);
	}

}
//--------------------------------------------------------------------------------------------------------
void	CNewGameClient::SetMjHandUseMouse(const int iMjHandID, bool bFlag)
{
	IMjHandCtrl *pMjHand = NULL;
	pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(iMjHandID));

	if (NULL != pMjHand)
	{
		pMjHand->SetIsUseMouse(bFlag);
	}
}
/*-----------------------------------------------------------------------*/
//���������齫����ѡ��
void	CNewGameClient::SetMjHandInvalidTile(const int iMjHandID, int iHandCard[], int iCount)
{

	IMjHandCtrl *pMjHand = NULL;
	pMjHand = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(iMjHandID));
	if (NULL != pMjHand)
	{
		pMjHand->SetInvalidTile(iHandCard,iCount);
	}
}

/*-----------------------------------------------------------------------*/
//�����������ʾ����	
void	CNewGameClient::InsertTalkInfo(const int iTalkID,CString sText)
{
	ITalk *pTalk = NULL;
	pTalk = dynamic_cast<ITalk*>(m_pUI->GetIControlbyID(iTalkID));
	if(NULL != pTalk)
	{
		TCHAR* pResult = new TCHAR[MAX_PATH];
		memset(pResult, 0, MAX_PATH * sizeof(TCHAR));
		wsprintf(pResult, TEXT("<font><color=0xffffff00,size=12,style=����> %s</font>"),sText);
		pTalk->SetText(CT2W(pResult));
		delete pResult;
	}
}

/*-----------------------------------------------------------------------*/
//����������Ϣ
void	CNewGameClient::ClearTalkInfo(const int iTalkID)
{
	ITalk *pTalk = NULL;
	pTalk = dynamic_cast<ITalk*>(m_pUI->GetIControlbyID(iTalkID));
	if(NULL != pTalk)
	{
		pTalk->ClearText();
	}
}

void CNewGameClient::OnNotifyMaiMa(BYTE* buffer, int nLen)
{

	if(nLen != sizeof(m_GameData.m_TMaiMa))
	{
		return;
	}

	tagMaiMa* pMaiMa = (tagMaiMa*)buffer;
	if (NULL == pMaiMa)
	{
		return;
	}
	memcpy(&m_GameData.m_TMaiMa,pMaiMa,nLen);

	//��˭����
	if (m_GameData.m_TMaiMa.byUser == m_nDeskStation)
	{
		m_byMaiMaIndex = 0;
		//��ʾ��������
		ShowMaiMaContain();
	}

}


void CNewGameClient::OnNotifyZhongNiao(BYTE* buffer, int nLen)
{
	if(nLen != sizeof(m_GameData.m_ZhongNiao))
	{
		return;
	}

	SetContainVisible(2008100,true);

	ZhongNiao* pZhongNiao = (ZhongNiao*)buffer;
	memcpy(&m_GameData.m_ZhongNiao,pZhongNiao,nLen);


	for(int i = 0; i < 4; i++)
	{
		IMjHandCtrl* pMjHandCtrl = NULL;
		pMjHandCtrl = dynamic_cast<IMjHandCtrl*>(m_pUI->GetIControlbyID(2008102 + i));
		if(!pMjHandCtrl) continue;
		pMjHandCtrl->SetHandInfo(NULL,0);
		int card = {m_GameData.m_ZhongNiao.byCard[i]};
		if (255 != card)
		{
			pMjHandCtrl->SetHandInfo(&card,1);
			pMjHandCtrl->SetControlVisible(true);
		}
	}

	for(int i=0;i<PLAY_COUNT;++i)
	{
		//������ҵ���������
		m_UserData.SetMenPaiData(i,pZhongNiao->byMenPai[i]);
		//��ʾ��ǽ�����ƣ�	
		UIShowWall(i, m_UserData.m_iArMenPai[i]);			
	}

	for(int i = 0; i < 4; i++)
	{
		IImage* pImage = NULL;
		pImage = dynamic_cast<IImage*>(m_pUI->GetIControlbyID(2008106+i));
		pImage->SetControlVisible(false);
	}

	m_pGameFrame->SetTimer(TIME_SHOW_NIAO,500);
	
}
