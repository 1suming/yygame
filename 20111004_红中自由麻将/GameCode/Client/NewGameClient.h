#include "IGameImpl.h"
#include "UI_ForLogic/IUserInterface.h"

#include "..\ChangeFile\UserDataEx.h"
#include "..\ChangeFile\GameDataEx.h"
#include "sounddefines.h"

#include <irrKlang.h>
using namespace irrklang;
//#include "SoundsDef.h"
#include <queue>

interface IGameFrame;
interface IUserInterface;


#define     BN_VOLUM_UP            13      ///< ������
#define     BN_VOLUM_DOWN          14      ///< ������
#define     BN_VOLUM_OFF           15      ///< ���ֹ�
#define     BN_VOLUM_ON            16      ///< ���ֿ�



#define TIME_OUT_CARD			10		//���Ƽ�ʱ��
#define TIME_BLOCK_CARD			11		//���Ƽ�ʱ��
#define TIME_ZHUAPAI			12		// ץ�Ƽ�ʱ��
#define TIME_HIDE_SEZI_ANI		13      // ɫ�Ӷ������ź��������Ķ�ʱ��
#define TIME_BEGIN				14      // �Զ���ʼ

#define ID_WARMING_TIME			17		// ���涨ʱ��

#define TIME_ONE_SECOND			20      // һ�뵹��ʱ

#define TIME_SHOW_HU			21      // ��ʾ���ƶԻ���ʱ��
#define ID_BEGIN_MOPAI          22      //�ͻ�������չ�����ƶ���
#define TIME_SHOW_NIAO          23      //չʾ���񶯻�



/*---------------------------------------------------------------------------*/
//const	POINT	G_PointStart[PLAY_COUNT]	= {{380,140},{620,360},{400,600},{175,345}};
//const	POINT	G_PointEnd[2][PLAY_COUNT]	= {{{360,335},{380,395},{430,390},{370,400}},
//											   {{460,350},{380,330},{365,380},{335,320}}};
const	POINT	G_PointStart[PLAY_COUNT]	= {{536,140},{876,360},{526,600},{175,345}};
const	POINT	G_PointEnd[2][PLAY_COUNT]	= {{{516,335},{536,395},{536,370},{506,400}},
												{{606,350},{536,330},{471,360},{451,320}}};
/*---------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
//��ǽ���������õ�
//static int G_iWallChangePos[MEN_CARD_NUM] ={35,34,33,32,31,30,29,28,27,26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
//static int G_iWallChangePos[MEN_CARD_NUM] ={0,1,2,3,4,5,6,7,8,9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};
static int G_iWallChangePos[] ={0,1,2,3,4,5,6,7,8,9,10,11, 12, 13, 14, 15, 16, 17, 18, 19,20,21, 22, 23, 24, 25, 26, 27, 28, 29,30, 31, 32, 33, 34,35};
/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/
struct MjUiInfo
{
    int         m_nUserHandInfo[20];
    int         m_nHandCardCount;

    int         m_nUserOutInfo[OUT_CARD_NUM];
    int         m_nOutCardCount;

	bool        m_bIsBigGang[5];//�Ƿ���
    BYTE        m_ucCPGData[5][4];
    BYTE        m_ucSpeCPGData[5][4];//���������
    BYTE        m_ucSpeCPGNum[5][4];//������Ƶ�����
    BYTE        m_ucChair[5];
    BYTE        m_ucCPGTile[5];
    MJ_LOGIC_TYPE m_CPGtype[5];
    int         m_nCPGCount;
    
    MjUiInfo()
    {
        memset(this, 0, sizeof(MjUiInfo));
    }
    void ClearHand()
    {
        memset(m_nUserHandInfo, 0, sizeof(m_nUserHandInfo));
        m_nHandCardCount = 0;
    }
    void GetFromByte(BYTE ucByteInfo[])
    {
        ClearHand();
        for (int i=0; i<17; ++i)
        {
            if (0==ucByteInfo[i] || 255==ucByteInfo[i])
            {
                break;
            }
            m_nHandCardCount++;
            m_nUserHandInfo[i] = ucByteInfo[i];
        }
    }
    void FillWithBack(int nNum)
    {
        memset(m_nUserHandInfo, 0, sizeof(m_nUserHandInfo));
        m_nHandCardCount = nNum;
    }
    void ClearOut()
    {
        memset(m_nUserOutInfo, 0, sizeof(m_nUserOutInfo));
        m_nOutCardCount = 0;
    }
    void GetOutFromByte(BYTE ucByteInfo[])
    {
        ClearOut();
        for (int i=0; i<OUT_CARD_NUM; ++i)
        {
            if (0==ucByteInfo[i] || 255==ucByteInfo[i])
            {
                break;
            }
            m_nUserOutInfo[i] = ucByteInfo[i];
			m_nOutCardCount++;
        }
    }
    void ClearCPG()
    {
        memset(m_bIsBigGang, 0, sizeof(m_bIsBigGang));//�Ƿ���
        memset(m_ucSpeCPGData, 0, sizeof(m_ucSpeCPGData));//���������
        memset(m_ucSpeCPGNum, 0, sizeof(m_ucSpeCPGNum));//������Ƶ�����
        memset(m_ucCPGData, 0, sizeof(m_ucCPGData));
        memset(m_ucChair, 0, sizeof(m_ucChair));
        memset(m_ucCPGTile, 0, sizeof(m_ucCPGTile));
        memset(m_CPGtype, LT_EMPTY, sizeof(m_CPGtype));
        m_nCPGCount = 0;
    }
    void GetFromCPGData(GCPStructEx cpgstr[],bool show=true)
    {
        ClearCPG();
		int index = 0;
        for (int i=0; i<5; ++i)
        {
			if(cpgstr[i].byType == 255)
				continue;
			if(cpgstr[i].bIsBigGang)
			{
				if (-1==cpgstr[i].byBigGang[0][0] || 255==cpgstr[i].byBigGang[0][0] || 0==cpgstr[i].byBigGang[0][0])
				{
					break;
				}
				for (int j=0; j<4; ++j)
				{
					m_ucSpeCPGData[i][j] = cpgstr[i].byBigGang[j][0];	
					m_ucSpeCPGNum[i][j]	 = cpgstr[i].byBigGang[j][1];	//ÿ���Ƶ�����
				} 
				m_bIsBigGang[i] = true;//��־���
			}
			else
			{
				if (-1==cpgstr[i].byData[0] || 255==cpgstr[i].byData[0] || 0==cpgstr[i].byData[0])
				{
					break;
				}
				for (int j=0; j<4; ++j)
				{
					m_ucCPGData[i][j] = cpgstr[i].byData[j];                
				} 
			}
            m_ucChair[i] = cpgstr[i].iBeStation;
            m_ucCPGTile[i] = cpgstr[i].iOutpai;
            m_nCPGCount ++;

            if (ACTION_CHI==cpgstr[i].byType)
            {
                m_CPGtype[i] = LT_COLLECT;
            }
            else if (ACTION_PENG==cpgstr[i].byType)
            {
                m_CPGtype[i] = LT_TRIPLET;
            }
            else if (ACTION_AN_GANG==cpgstr[i].byType)
            {
                for (int j=0; j<3; ++j)
                {
                    m_ucCPGData[i][j] = 0;
                }
				if(!show)
					m_ucCPGData[i][3] = 0;
                m_CPGtype[i] = LT_QUADRUPLET_CONCEALED;
            }
            else if (ACTION_BU_GANG==cpgstr[i].byType || ACTION_MING_GANG==cpgstr[i].byType)
            {
                m_CPGtype[i] = LT_QUADRUPLET_REVEALED;
            }
        }
    }
};


class CNewGameClient : public IGameImpl
{

public:

    CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI);

    virtual ~CNewGameClient();

private:

    CNewGameClient(const CNewGameClient &other);
    CNewGameClient &operator = (const CNewGameClient &other);

public:

#define SAFE_CTRL_OPERATE(CTRL_TYPE,CTRL_ID, OPERATE)\
	{\
	CTRL_TYPE* PRE=NULL; \
	PRE = dynamic_cast<CTRL_TYPE *>(m_pUI->GetIControlbyID(CTRL_ID));\
	if (PRE!=NULL) PRE->OPERATE; \
	}\

/*-----------------------------------------------------------------------------------------------------------
	һϵ��ƽ̨�ṩ����Ҫ������������Ϸ�ĺ����������ô������Ϸ�Ŀ�������ÿ��������ʵ������Ϸ���졣
  ----------------------------------------------------------------------------------------------------------*/
public:
    //��ʼ��
    virtual int		Initial();
    //��ʼ��UI
    virtual int		InitUI();
    //��Ϸ��Ϣ
    virtual int		HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen);
    //������Ϣ
    virtual int		OnUIMessage(TUIMessage* pMessage);
    //
    virtual int		ResetGameFrame(void);
    //��ҽ���
    virtual int		GameUserCome(void);
    //����뿪
    virtual int		GameUserLeft(int nIndex);
    //��Ҷ���
    virtual int		GameUserOffLine(void);
    //�Ƿ�������Ϸ������Ϸʵ��
    //�Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
    virtual bool	IsPlayingGame();
    //��ʱ����Ϣ
    virtual void OnTimer(int nTimerID);
    //��״̬
    virtual void	SetStationParameter(BYTE bGameStation);
	//������Ϸ״̬����Ҫ�Ƕ����������������Ϣ
    virtual void	SetGameStation(void * pBuffer,int nLen);
	//�˳���Ϸ
	virtual bool	OnGameQuiting();




/*-----------------------------------------------------------------------------------------------------------
	�����߼�����ģ��
  ----------------------------------------------------------------------------------------------------------*/
private:

	//��Ϸ���
    IGameFrame* m_pGameFrame;
	//��ϷUI
    IUserInterface* m_pUI;
	//������������
	ISoundEngine* m_SoundEnginBg;
	//��Ϸ��Ч����
	ISoundEngine* m_SoundEngineGame;
	//��Ϸ����������Ч����
	bool			m_bBgMusic;
	//��Ϸ��Ч����
	bool			m_bGameSound;
	///<�Լ��ڷ������ϵ���λ��
	int             m_nDeskStation;    
	//��Ϸ״̬
	BYTE			m_bGameStation;
	//����״̬ 0����״̬ 1�����ѡ��״̬ 2��ʾѡ�����״̬ 3��ʾѡ������״̬
	int            m_byHandState;      
	//��ʾ����Ƿ�����(ѡ����Ƶ�ʱ������)
	bool			m_bMouseEnter;		
	//��ǰץ������ �ǲ��ǻ��� 
	bool			m_bIsHua;			
	//��Ϸ������ǳ�
	CString			m_sUserNick[PLAY_COUNT];
	//�Ƿ񳬶��û�
	bool			m_bIsSuperUser;
	//��Ϸ����
	GameDataEx      m_GameData;		
	///��ҵ����ݣ����ƣ����ƣ��������Ƶ�
	UserDataEx      m_UserData;	

	






	//�ÿͻ����Ƿ������Թ�
	bool m_bAllowWatch;
    BYTE            m_ucCurrentPlayer;  //��ֵ���
    MjUiInfo        m_UserDataCard[PLAY_COUNT]; 
    int               m_nTimeOutCnt;
	bool			m_bIsBigGang[2];//�Ƿ���
	BYTE			m_byCheckPai;//ѡ��ĸ���

	//����չ���������ƶ���
	BYTE            m_byMoPaiNum; //���ƴ��������ֹ�Ҫ��16��
	int				m_iDeskHandPai[PLAY_COUNT][MAX_HAND_PAI];
	BYTE            m_byDeskMenPai[PLAY_COUNT][MEN_CARD_NUM];
	BYTE            m_byMoPaiStation;//��ǰ�������λ��
	BYTE			m_byDirect;   //��ʼλ��
	BYTE            m_byGetPai;      //����

	BYTE			m_byMaiMaIndex;	//���������
private:
	//	�û�ͬ��	DWJ
	void	OnUserAgree(void *buffer, int len);
	// �û��й�
	void	OnUserTuoguan(void *buffer, int nLen);
	//	��Ϸ��ʼ	DWJ
	void	OnGameStart(void *buffer, int len);
	//	ȷ��ׯ����Ϣ	DWJ
	void	OnDingNt(void *buffer, int nLen);
	//	��һ����ɸ��ȷ�����Ʒ���	DWJ
	void	On2SeZiDir(void *buffer, int nLen);
	//	�ڶ���ɸ��ȷ�����ƶ�����Ϣ	DWJ
	void	On2SeZiDirGP(void *buffer, int nLen);
	//	������������	DWJ
	void	OnSendAllCard(void *buffer, int nLen);
	//	������Ҳ�����Ϣ
	void	OnAllBuHua( void *buffer, int nLen);
	//	��Ϸ��ʼ������
	void	OnBeginOutCard(void *buffer, int nLen);
	//	���ƽ��
	void	OnNotifyOutCard(void *buffer, int nLen);
	//	����
	void	OnNotifyBlock(void *buffer, int nLen);
	//���� llj
	void	OnNotifyAction(void *buffer, int nLen);
	//	ץ��	DWJ
	void	OnNotifyZhuaPai(void *buffer, int nLen);
	//	������Ҳ���	DWJ
	void	OnNotifyOneBuHua( void *buffer, int nLen);   
	//	��ҷ�������
	void	OnNotifyCanCelBlock(void *buffer, int nLen);
	//	����	
	void	OnNotifyChiPai(void *buffer, int nLen);
	//	����
	void	OnNotifyPengPai(void *buffer, int nLen);
	// ����
	void	OnNotifyGangPai(void *buffer, int nLen);
	// ����
	void	OnNotifyHuPai(void *buffer, int nLen);  
	// ����
	void	OnNotifyFinish(void *buffer, int nLen);

private:
	//��һ��ƽ��
	void	OnUserChangeCard(void *buffer, int nLen);
	///���¿���ʱ�ĳ�ʼ��UI����	DWJ
	void	ReStartGameInitUI();
	//���������Ϣ�ؼ�
	void	UIShowUserInfo(BYTE byStation, bool bFlag);
	//	��ʾ��ǽ����	DWJ
	//	���ݸ�ʽ��255��û���ƣ� 0���Ʊ�������������һ��
	void	UIShowWall(BYTE byStation, BYTE byArrMenPai[]);
	//	��ʾ��������	DWJ
	void	UIShowAllHandPai(bool bFlag,bool bFinish = false);
	//	�������е�����ʾ�Ʊ�	DWJ
	void	UIShowAllHandPaiBack(bool bFlag, bool bLayDown);
	//	��ʾ������		DWJ
	void	UIShowHuaCount(bool bFlag);
	//	����������ʾ��λ
	void	UIShowDNXB(bool bFlag);
	//	��ʾׯ�Ҿ�����Ϣ
	void	UIShowNtInfo(bool bFlag);
	//	��ʾ���Ʊ�ʶ	DWJ
	void	UIShowTingLog(bool bFlag);
	//	��ʾ����ʱ	DWJ
	void	ShowCountDownTimer(BYTE byDeskStation, BYTE byTime,bool bFlag);
	//	��ʾ������ҳ�������
	void	UIShowOutCard(BYTE byStation, bool bShow);
	//	��ʾ��������	DWJ
	void	UIShowBlockContain(bool bFlag);
	//	��ʾ��������	DWJ
	void	UIShowChiContain(bool bFlag);
	//	��ʾ��������1	DWJ
	void	ShowChiContain1(bool bFlag);
	//	��ʾ��������2	DWJ
	void	ShowChiContain2(bool bFlag);
	//	��ʾ��������3	DWJ
	void	ShowChiContain3(bool bFlag);
	//	����ѡ�����	DWJ
	void	SetChoiceGangPai(int iGangCount,bool bFlag);
	//	����ѡ������״ֻ̬��ѡ���������
	void	SetChoiceTingPai();
	// ��ʾ�����
	void	UIShowResultDlg(bool bFlag);
	//�й���ʾ
	void	UIShowTuoGuan();
	//��ʾ��Ϸ����
	void	UIShowGameSetting(bool bFlag);
	//��ʾ��������
	void	ShowSuperDlag(bool bFlag);
	//��Ӧ����
	void	OnUiChangeCard();
private:	/*-----------UI��Ӧ�¼�--------------*/
	//	�������                
	void	OnUIRequestOutCard(UINT32 uControlID,bool outtime = false);
	//	�������
	void	RequestOutCard(BYTE byData);
	//	��������
	void	RequestCancelBlock();
	//	��������ư�ť
	void	OnUiClickChiPai();
	//	�������
	void	RequestBlockChi(int iIndex);
	//	��������
	void	RequestBlockPeng();
	//	�������
	void	RequestBlockGang();
	//	��������������
	void	OnUIRequestGangPai(const int iControlID);
	//	��������
	void	RequestBlockTing();
	//	�������
	void	RequestBlockHu();
	// �����й�
	void	RequestTuoGuan(bool bTuoguan = true);
	// �����ؼ���Ϣ
	int		OnCartoonFinish(TUIMessage *pMessage);


	
	
	



	 /// ǿ�˽�����Ϸ
	void OnForceFinish( void *buffer, int nLen);

    // @brief ��ʾ�ƶ������ؼ�
    void UIShowZhuaMoveAction(POINT ptStart, POINT ptEnd, BYTE ucViewStatin, bool bShow);

    // ���齫�ؼ�������ʱ������ƶ����ؼ���
    int OnMoveOnMjCtrl(TUIMessage* pMessage);
    // ���齫�ؼ�������ʱ����굥���ؼ�
    int OnLButtonOnMjCtrl(TUIMessage* pMessage);
    



public:
	

	//���ü�ʱ��
	void OnSetTimer(BYTE station,int type);
	//ɾ������ʱ��
	void OnKillAllTimer();
	/// ������ʱ��
	BOOL KillGameTimer(int TimerID);
    // ���ö�ʱ��
    void UIShowTimer(BYTE bViewStation, int nTime, bool bShow = true);


private:
	//������Ϸ����:����֪ͨ��Ϣ������������
	void	SendGameDataEx(BYTE id);
	//������Ϸ����
	void	SendGameDataEx(BYTE id,void *pData,UINT uSize);
	//��˸����
	void	FlashWindow();



	//���ַ���ת����������
	void GetPaiFormChar(int &count ,BYTE data[],const wchar_t pszPai[],int type);
	//�������ַ�ת������
	BYTE ChangeChar(wchar_t data1,wchar_t data2);






	
private:
	//��ʼ������
	void InitGameDate();
	//������Ϸ����
	void	LoadGameSet();
	//������Ϸ����
	void	SaveGameSet();
	//���ű�����Ч	DWJ
	void	PlayBgSound(bool bISLoop);

	//������Ϸ��Ч	DWJ
	void	PlayGameSound(BYTE byDeskStation,int SoundID, bool bISLoop = false);
	//������ҵĵ��ǳ�
	void	GetUserName();
	//����һ�뵹��ʱ m_iRemaindTime
	void	SetOneSecondCountDown(int iTime);
	



/*----------------------------------------------һ�º�������UI�༭���汾����Ϸ������ʹ��---------------------------------------------------------------------*/
public:
	//���ö����ؼ����Ŷ���
	void	SetAnimatePlay(const int AnimateID,bool bFlag,POINT pointStart,POINT pointEnd,CURVE_TYPE CT_TYPE = CT_NONE,BYTE byPlayNum = 1, bool bIsLoop = false);
	//���ö����ؼ����Ŷ���
	void	SetAnimatePlay(const int AnimateID,bool bFlag,POINT position,bool bIsLoop = false,CURVE_TYPE CT_TYPE = CT_NONE);
	//���ö����ؼ�����ָ������
	void	SetAnimatePlay(const int AnimateID,wchar_t wsFilePath[],bool bFlag,BYTE byPlayNum = 1, bool bIsLoop = false);
	//���ö�����ʾĳһ֡
	void	SetAnimateShowFrame(const int AnimateID,bool bFlag,int iFrame,POINT point);

	//���ð�ť�Ƿ����
	void	SetBtEnable(const int iButtonID, bool bFlag);
	//���ð�ť�Ƿ�ɼ�
	void	SetBtVisible(const int iButtonID, bool bFlag);
	//��ȡ��ť�Ƿ����
	bool	GetBtIsEnable(const int iButtonID);

	//����ʱ�����ʾ����ʱ
	void	SetTimePlay(const int iTimeID,bool bFlag, BYTE byMaxTime, BYTE byMinTime = 0);

	//����ͼƬ�ؼ���λ��
	void	SetImagePosition(const int iImageID, int iPositonX, int iPositonY, bool bFlag);
	//����ͼƬ�ؼ��Ƿ�ɼ�
	void	SetImageVisible(const int iImageID, bool bFlag,int iSrcX = 0, int iSrcY = 0);
	//����ͼƬ�ؼ�������ʾָ�����ļ�
	void	SetImageLoadSrc(const int iImageID,CString sImagSrc,bool bFlag);

	//���������ؼ��ɼ�
	void	SetContainVisible(const int iContainID,bool bFlag);

	//�����ı��ؼ���ʾ����
	void	SetTextinfo(const int iTextID,__int64 iNum, bool bFlag);
	//�����ı��ؼ���ʾ����
	void	SetTextinfo(const int iTextID,TCHAR sChar[], bool bFlag);
	//�����ı��ؼ���ʾ����
	void	SetTextinfo(const int iTextID,CString sText, bool bFlag);

	//�������ֿռ���ʾ����
	void	SetInumInfo(const int iNumID, __int64 i64Num, bool bFlag);

	//���ó���ؼ���ʾֵ
	void	SetShCtrlInfo(const int iShCtrlID,int iNum, bool bFlag);

	//���õ�ѡ��ؼ��Ƿ�ѡ��
	void	SetRButtonSelected(const int iRButtonID,bool bSelected, bool bFlag);

	//�����齫��ǽ�ؼ�����С&&�Ƿ�ɼ�
	void	SetMjWallInfo(const int iMjWallID,int iFillNum,bool bFlag);
	//�����齫��ǽָ�����齫��Ϣ
	void	SetMjWallTittle(const int iMjWallID,int iValue,int iPosition,bool bFlag);

	//�����齫���ƿؼ���Ϣ
	void	SetMjGiveInfo(const int iMjGiveID, int iLineNum, int iHandTiles[], int iCount, bool bFlag);
	//��ȡ�齫���Ƶľ���
	RECT	GetMjGiveRect(const int iMjGiveID,int iIndex);

	//�����齫���ƿؼ��Ƿ�ɼ�
	void	SetMjHandVisible(const int iMjHandID, bool bFlag);
	//�����齫���ƿؼ���Ϣ
	void	SetMjHandInfo(const int iMjHandID, int byCards[], int iCardCount,bool bFlag,bool bLaydown = false);
	//�����齫���ƿؼ���
	void	SetMjHandLayDown(const int iMjHandID, bool bLayDown);
	//�����齫���ƿؼ����һ���Ƿ����
	void	SetMjHandGiveMode(const int iMjHandID, bool bMode);
	//����齫�ؼ�������Ϣ
	void	ClearMjHandBlock(const int iMjHandID);
	//����������Ϣ
	void	AddMjHandOneBlock(const int iMjHandID, MJ_ACTION_TYPE BlockType, BYTE byCardData[], BYTE byBeChair);
	//�����齫���ƿؼ��Ƿ���괩͸pMjHand->SetIsUseMouse(false);
	void	SetMjHandUseMouse(const int iMjHandID, bool bFlag);
	//���������齫����ѡ��
	void	SetMjHandInvalidTile(const int iMjHandID, int iHandCard[], int iCount);

	//�����������ʾ����
	void	InsertTalkInfo(const int iTalkID, CString sText);
	//����������Ϣ
	void	ClearTalkInfo(const int iTalkID);


	void On2SeZiNT(void * buffer, int nLen);
	void On2SeZiGP(void* buffer, int nLen);
	void OnNotifyMaiMa(BYTE* buffer, int nLen);
	void OnNotifyZhongNiao(BYTE* buffer, int nLen);

	//��ȡ�齫����һ��������
	int		GetMjHandOneCard(const int iMjHandID);

	//��ʾ��������
	void	ShowMaiMaContain(bool bShow = true);
};