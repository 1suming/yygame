#ifndef __BJLGameTableUI_h__
#define __BJLGameTableUI_h__

#include "HNNetExport.h"
#include "platform/CCCommon.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "BJLGameLogic.h"
#include "BJLMessageHead.h"
#include "BJLGameTableUICallback.h"
#include "BJLResultCard.h"
#include "BJLResultDialog.h"
#include "BJLHistoryDialog.h"
#include "BJLGameTableUiData.h"

#include <string>

using namespace cocostudio;
using namespace ui;
using namespace std;

#define CHOUMANUM			 7									//����İ�ť�ĸ���
#define NUMOFBETAREA		 8									//���ٸ���ע����

namespace BJL
{

	class GameTableLogic;

	//��Ϸ����UI����
	struct GameView
	{
		Layout*      myLayout;
		Layout*      dashbroad_layout;
		Layout*      gametable_layout;
		ImageView*   table;

		ImageView* dash_bg_imgview;

		//ׯ���б�(׼��ɾ��  ������)
		ui::ScrollView* scrollView;
		Text* svDealerName[4];

		// ׯ���б�
		ListView* ListView_NTList;

		// ׯ���б�����
		ImageView* ImageView_ListItem;

		// 8����ע����
		Button* Button_BetArea[8];

		// 8����ע��ʾ����
		ImageView* ImageView_BetWarning[8];

		// 8��Ӯ����ʾ���� ��˸Ӯ������
		ImageView* ImageView_BetWin[8];									

		// ����ע
		ImageView* ImageView_betHint;

		// �ȴ�ʱ��
		ImageView* ImageView_WaitHint;		
		
		// 7�����밴ť
		Button* Button_Bet[7];

		// 7�����밴ť����
		ImageView* ImageView_BetBg[7];				

		// ������ׯ
		Button* Button_applyNT;

		// ������ׯ
		Button* Button_applyNoNT;

		// �˳�
		Button* Button_Exit;

		// ·����Ϣ
		Button* Button_Records;

		// 8����ע��ť ����ڷŰ�ť����0-7�±�ֱ��ʾ8������
		NoteViewInfo pnoteview[8];    

		// ����ע���룬0-7�±�ֱ��ʾ8������
		TextAtlas* TextAtlas_TotalBet[8];

		// �����ע���룬0-7�±�ֱ��ʾ8������
		TextAtlas* TextAtlas_UserBet[8];

		// ׯ����Ϣ��ͼ
		ImageView* ImageView_NTContainer;

		// ׯ���ǳ�λ��
		ImageView* ImageView_NTNickName;

		// ׯ�ҽ��λ��
		ImageView* ImageView_NTMoney;

		// ��ʱ������
		ImageView* ImageView_TimerBg;

		// ��ʱ������
		TextAtlas* TextAtlas_TimerText;
	};

	typedef struct tagBetAndType 
	{
		int whichJetton;	//��һ������
		int whichType;		//��һ������
	} BET_TYPE;

	class GameTableUI : public HNGameUIBase , public GameTableUICallback
	{
	public:
		// ��ȡ��Ϸ����
		static HNGameUIBase* create(BYTE bDeskIndex, bool bAutoCreate);

		// ��ʼ��
		bool init(BYTE bDeskIndex, bool bAutoCreate);

		// �˳�
		virtual void onExit() override;

		// ����
		virtual void onEnter() override;

		// ���캯��
		GameTableUI();

		// ��������
		virtual ~GameTableUI();
	private:
		bool initUI();
		void initSprite();
		void loadRes();
		void initTheNewGameData();
	private:
		// ��ע����
		void onBetAreaClickCallback(cocos2d::Ref* pSender,Widget::TouchEventType touchtype);

		// ��ע����
		void onBetClickCallback(cocos2d::Ref* pSender,Widget::TouchEventType touchtype);

		// ���ð�ť
		void onCommonClickCallback(cocos2d::Ref* pSender,Widget::TouchEventType touchtype);

		// ������ׯ����ׯ
		void onApplyNTClickCallback(cocos2d::Ref* pSender,Widget::TouchEventType touchtype);

		// ��ע�����ص�
		void betAnimationCallback(cocos2d::Node* pSender);

		// ����
		void dispatchCard(float delta);
		
		//·�ӻص�
		void luziscrollview_scroll(cocos2d::Ref* pobj, ui::ScrollView::EventType type);

	private:
		void stopBetAnimation();
		void hideXiaZhuBTN();
		void showSettlementDialog(S_C_GameResult* pData);
		void showMyBetAnimation(BYTE whichBTN , BYTE whichArea);
		void resetDealerList(BYTE dealerStation[], SHORT count);

	private:
		ResultDialog*      _gameResult;
//		GameHistoryDialog* _Dialog;
		GameTableLogic*    _logic;
		ResultCard*        _resultCard;
		
		//��Ϸ����UI����
		GameView*          _gameView;
		
		//��ׯ�б�ģ��
		T_DEALER_LIST*     _dealerList;
	public:
		virtual void leaveTheGame() override;
		
		//��һ����ׯ�� ��Ϸֱ�ӽ�����ע
		virtual void OnHandleBegin(S_C_GameBegin * pGameBeginData);
		
		//����
		virtual void showSettlement(S_C_GameResult* pGameResult); 
		
		// ��ʾ��ʾ��Ϣ
		virtual void IShowNoticeMessage(const std::string& msg) override;

		// �����ϡ���ׯ��ť
		virtual void IApplyNT(bool isNT) override;

		// ������ׯ�б�
		virtual void IUpdateNTList() override;

		// ��ʾׯ����Ϣ
		virtual void IUpdateNTInfo(const std::string name, LLONG money) override;

		// ��ʾ�Լ���ע��
		virtual void IDisplayMyBetMoney(int noteArea, LLONG money) override;

		// ������ҽ��
		virtual void IUpdateUserMoney(LLONG money) override;

		// ��ʾ�����ע����
		virtual void IPlayNoteAnimation(BYTE noteType, int noteArea) override;

		// ���ó��밴ť�Ƿ����
		virtual void ISetBetEnable(bool enabled[], int count) override;

		// ��������ʱ
		virtual void IStartTimer(int second) override;

		// ��ָ��������ע����
		virtual void IAreaBet(int area, LLONG betMoney) override;

		// ��ʼ����
		virtual void IInitCard(BYTE cards[][3], int playerCount, int NTCount, int playerCardType[], int NTCardType[]) override;

		// ��ʾ��
		virtual void IShowCards(bool animate) override;

		// ����
		virtual void ISendCard(bool animate) override;

		// ��ʾ��ע
		virtual void IShowBetHint(bool visible) override;

		// ��ʾ�ȴ�ʱ��
		virtual void IShowWaitHint(bool visible) override;

		// ��Ϸ��ʼ
		virtual void IGameStart() override;

		// ��ʾ�м�����
		virtual void IShowWinArea(bool visible) override;

		// �������
		virtual void IShowOpenBoard(bool visible) override;

		//����ʱͼ�ƶ���λ��
		virtual void SetImageTimeMoveNewPostion()override;

		//����ʱͼ�ƶ�����λ��
		virtual void SetImageTimeMoveOldPostion()override;
		
	private:
		//��̬������һЩ����
		Label* _dealerName;
		Label* _dealerMoney;
		Text* _ownName;
		Text* _ownMoney;
		LLONG _currentMaxZhu[NUMOFBETAREA + 1];         // ÿ���������¶���ע
		LLONG _currentMyZhu[NUMOFBETAREA];				// �����ע���
		LLONG _currentDealerLimit;						   
		Vector<Sprite*> spriteArr;
	private:
		// ��ʱ������ʱ��
		int _remainTime;

		// ѡ�е���ע��ť
		int _betIndex;

		void timerUpdate(float delat);
		float _fxScale ;
		float _fyScale ;
	};

}

#endif // __BJLGameTableUI_h__