#ifndef __DZPokerGameTableUI_h__
#define __DZPokerGameTableUI_h__

#include "DZPokerGameTableUICallback.h"

#include "HNNetExport.h"
#include "cocos2d.h"
#include "DZPokerGamePlayer.h"
#include "DZPokerPokerCard.h"
#include "DZPokerGameDealerUI.h"
#include "DZPokerGameCardType.h"
#include <vector>

/************************************************************************/
/* ��Ϸ�������                                                          */
/************************************************************************/


namespace DZPoker
{

	typedef struct TableView
	{
		Node*		table;
		Node*		dashboard;
		Node*		pulldownMenu;
		ImageView*	bg;
		ImageView*	dealer;
		ImageView*	card_type;
		Button*		btnMenu;
		Button*		btnPosition[PLAY_COUNT];	// 8�����Ӱ�ť
		int			sitposition;				// ��ǵ�ǰ������µ���λ�ţ����û������Ϊ-1��
		Button*		btnJetton[4];
		ImageView*	idashbroad;
		Button*		btnControll[3];				// �ֱ��ʾ ���ƣ���ע������
		int			btnControllTag[3];			// ��ʾ��3����ť�ӹ��ܵ�Tag
		ImageView*	addSliderbg;
		Button*		btnCardType;
		Slider*		addSlider;
		TextAtlas*	addSliderMoney;
		ImageView*  pullDownMume;
		Button*		pullDownItemMume[3];
		int			silderMaxMoney;
		int			sliderMinMoney;
	} TableView;

	class GameTableLogic;

	class GameTableUI : public HNGameUIBase, public GameTableUICallback
	{

		static const int NUM_PUBLIC_CARD	= 5;		// ����������
		static const int NUM_TABLE_JETTON	= 5;		// �����ϵĳ���

	public:
		//bDeskIndex:���ӵ�����
		static HNGameUIBase* create(BYTE bDeskIndex, bool bAutoCreate);

		bool init(BYTE bDeskIndex, bool bAutoCreate);
		GameTableUI();
		virtual ~GameTableUI();
		PlayerUI* getPlayer(int seatNo);

	public:
		virtual void onExit() override;

	private:
		//json�ļ���ȡλ��
		Vec2 _positionOfPlayer[PLAY_COUNT];
		Vec2 _positionOfJetton[PLAY_COUNT];
		Vec2 _positionOfDealer;
		Vec2 _positionOfPublicCard[5];

		Vec2 _directionOf4[4];				//0-3   ��������
		Vec2 _directionOf8[8];				//0-7    �£����£������ϣ��ϣ����ϣ��ң�����
		void initPosition();

	private:
		virtual bool onTouchBegan(Touch *touch, Event *unused_event);

	private:
		//std::vector<GamePlayer*> _playerlist;
		//GamePlayer* _playerlist[PLAY_COUNT];
		PlayerUI* _players[PLAY_COUNT];
		int getSizeOfPlayerList();
		//GamePlayer* _currentPT;
		TableView* _tableView;
		void showCardTypeCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		void deskClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		void jettonClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		void controllBtnClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		void sliderCallback(cocos2d::Ref* pSender,ui::Slider::EventType type);

		//void todichiCallBack(cocos2d::Ref* pSender);
		//void leavedichiCallBack(cocos2d::Ref* pSender);
		//void xiazhuCallBack(cocos2d::Ref* pSender);
		//void kaipaiCallBack(cocos2d::Ref* pSender);
		//void checkCallback(cocos2d::Ref* pSender);
		//void qipaiCallBack(cocos2d::Ref* pSender);
		//void callCallBack(cocos2d::Ref* pSender);

		void pullDownCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		void pullDownItemCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		//��ť������ܸ�λ
		void menuTouchReset(Button* btn);


		PokerCard*		_publicCard[NUM_PUBLIC_CARD];								//�����ϵĹ�����5����											
		ProgressTimer*	_progressTimer;											//�����ϵļ�ʱ��
		Sprite*			_loadSprite;													//����Ԥ�����ɰ�
		GameCardType*	_cardTypeLayer;


		//GameAnimation* _gameanimation;

	private:
		Label* _roomIdInFo;
		Label* _lTableName;
		Label* _lPlayerMoney;
		Label* _lConstJuShu;			//��������
		Label* _lConstRank;			//��������
		

	public:
		Sprite* money_dichi_jetton[NUM_TABLE_JETTON];									//�����ϵ���Ϸ����
	public://�������Ժ����������ڲ��Զ���Ч���͵��ö�Ӧ�����ķ���  ����ɾ����
	//	//����Test
	//	void startFaPai();
	//	void fapaiTimerFUNC(float dt);

	//	//����Test
	//	void startQiPai();	
	//	//��ʱ��Test
	//	void startTimer();
	//	//��עTest
	//	void startXiaZhu();
	//	//����Test
	//	void startKaiPai();
	//private:
		//��ʼ��һ����Ϸ
		void initGameData();
		//��ʼ�µ�һ����Ϸ��Ҫ�����ݳ�ʼ��
		void initNewturn();


		//////////////////////////////////////////////////////////////////////////
	private:
		void updatePlayerByDeskStation(BYTE bDeskStation);
		void updatePlayerByDeskStation(std::vector<BYTE>& byDeskStations);
	public:
		/**����ص�**/
		//��Ҳ���(��ס�����ơ���ע�����ơ����ơ���ʱ������)
		virtual void showUserBet(BYTE byDeskStation);			//ע������showBetMoney	��ע
		virtual void showUserCall(BYTE byDeskStation);			//��ע
		virtual void showUserAdd(BYTE byDeskStation);			//��������ע
		virtual void showUserCheck(BYTE byDeskStation);			//����
		virtual void showUserFold(BYTE byDeskStation);			//����
		virtual void showUserAllIn(BYTE byDeskStation);			//ȫ��

		//��Ҳ�����ť����
		virtual void showBet(bool bVisible, bool bEnable, LLONG benMoney);
		virtual void showCall(bool bVisible, bool bEnable);
		virtual void showRaise(bool bVisible, bool bEnable);
		virtual void showCheck(bool bVisible, bool bEnable);
		virtual void showFold(bool bVisible, bool bEnable);
		virtual void showAllIn(bool bVisible, bool bEnable);
		virtual void showAdd(bool bVisible, bool bEnable);
		virtual void showSub(bool bVisible, bool bEnable);
		virtual void showSlider(bool bVisible, bool bEnable, LLONG max, LLONG min, LLONG current);
		virtual void setSliderValue(LLONG value);
		virtual LLONG getSliderValue();

		//��Ҳ�����ť����
		virtual void showBetMoney(bool bVisible, LLONG money);
		virtual void showCallMoney(bool bVisible, LLONG money);
		virtual void showRaiseMoney(bool bVisible, LLONG money);

		//���µ׳�
		virtual void showPot(BYTE index);
		//������
		virtual void showHandCard(std::vector<THandCard>& handCards);
		//����
		virtual void showFlopCard(const std::vector<BYTE>& byCards);
		//ת��
		virtual void showTurnCard(BYTE card);
		//����
		virtual void showRiverCard(BYTE card);
		//�����û���Ϣ
		virtual void showUser(BYTE byDeskStation, bool bMe, bool sex);
		virtual void showUserUp(BYTE byDeskStation, bool bMe);
		virtual void showUserNickName(BYTE byDeskStation, const std::string& nickName);
		virtual void showUserMoney(BYTE byDeskStation,LLONG MONEY);
		//��ʾ������ϵ�����
		virtual void showUserCardType(BYTE seatNo, INT type);
		//��ʾׯ��
		virtual void showDealer(BYTE byDeskStation);
		//��ʾСäע
		virtual void showSmallBlind(BYTE byDeskStation);
		//��ʾ��äע
		virtual void showBigBlind(BYTE byDeskStation);
		//��ʾ��ע���루�����ϵģ� 
		virtual void showUserBetMoney(BYTE byDeskStation,LLONG money);
		//�ɽ�
		virtual void showWinPool(const std::vector<std::vector<LLONG>> &winPool);
		//��ʾ�������
		virtual void showUserHandCard(BYTE byDeskStaion, const std::vector<BYTE>& byCards);
		//��ʾ������
		virtual void upCommunityCard(BYTE index);
		//����������
		virtual void upUserHandCard(BYTE byDeskStation, BYTE index);
		//��ʾ���ʣ�����ʱ��
		virtual void showUserLeftTime(BYTE byDeskStation,BYTE byTime, BYTE total, bool isMe);
		//�Զ�������ť
		virtual void showAutoCall(bool bShow, bool bSelected);
		virtual void showAutoCallAny(bool bShow, bool bSelected);
		virtual void showAutoCheck(bool bShow, bool bSelected);
		virtual void showAutoCheckFold(bool bShow, bool bSelected);
		//��ʾ���ֽ�ѧ
		virtual void showHelp(bool bVisible);
		//�������Ǯ����
		virtual void showUserLackMoney(BYTE byDeskStation);
		//��������
		virtual void clearDesk();
		//����������
		virtual void clearDeskCard();
		//�뿪����
		virtual void leaveDesk() ;
		//��ʾ����׼��
		virtual void showTableReady(bool bVisible);
		//ת����λ
		virtual void rotateStation(int offset);
		//��ʾ����Ի���
		virtual void showBuyMoneyDialog(BYTE deskStation, LLONG min, LLONG max, LLONG plan, LLONG total);
		//֪ͨ��ʾ
		virtual void noticeMessage(const std::string &message);
		//��������
		virtual void changeToken(bool isMe,BYTE who);
		//��ʾ������Ϣ
		virtual void showTableInfo(const std::string& tableName);
		virtual void showMyMoney(LLONG money);

	public://����ϵ��
		//������̭
		virtual void showGameContestKick();       
		//�ȴ���������
		virtual void showGameContestWaitOver();
		//��������
		virtual void showGameContestOver(MSG_GR_ContestAward* contestAward);
		//��������
		virtual void showConstJuShu(int Index);
		//��ʾ����
		virtual void ShowConstRank(int iRankNum, int iRemainPeople) ;		
		//�����Լ�������	
		virtual void updateMyRankNum(int iValue);								
	protected:
		bool				_firstOnEnter;
		GameTableLogic*		_tableLogic;
		GameDelearUI*		_dealer;
		//���������ʼ��
		void inflateLayout();
		bool seatNoIsOk(BYTE seatNo);
		friend class GameDelearUI;

	public:
		int _waitTime;
		void isWaitTime(bool isWait);
		void updateTime(float dt);

		int		_iContestNum;							//��¼�Լ�����
	};
}


#endif // __DZPokerGameTableUI_h__