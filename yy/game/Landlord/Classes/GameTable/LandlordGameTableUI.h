#ifndef __Landlord_Game_Table_UI_H__
#define __Landlord_Game_Table_UI_H__

#include "LandlordGameTableUICallback.h"
#include "LandlordGamePlayer.h"
#include "LandlordPokerCard.h"
#include "LandlordGameCardType.h"
#include "HNLobbyExport.h"
#include "HNNetExport.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>

USING_NS_CC_EXT;

namespace landlord
{
	
	/*
	 * game ui unit.
	 */

	class GameTableLogic;
	//class Dashboard;
	class CardOutList;
	class CardListBoard;
	class Toolbar;

	class GameTableUI : public HN::HNGameUIBase, public GameTableUICallback
	{
	public:
		static HNGameUIBase* create(BYTE bDeskIndex, bool bAutoCreate);
		virtual bool init(BYTE bDeskIndex, bool bAutoCreate);
		virtual void onEnter();
		virtual void onExit();
		void standUp();
		void forceQuit();
		bool sendAuto(bool bAuto);
		void dropDownEventCallBack(cocos2d::Node* pNode);
		void autoOutCallFunction(cocos2d::Node* pNode);
		int getMaxZorder();
		virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;
	private:
		cocos2d::Vec2		_dealerPostion;
		Sprite*				_loadSprite;	
		Sprite*				_NoticeBG;
		GameNotice*			_notic;
		PlayerUI*			_players[PLAY_COUNT];
		Label*				_lRoomInfo;
		GameTableLogic*		_tableLogic;
		//Dashboard* _dashboard;
		Layout*				_tableWidget;
		ui::ImageView*		_lordCardBg;
		ui::ImageView*		_lordCardBack[3];
		PokerCard*			_lordCardFront[3];
		ui::Button*			_btnAction[3];//before game 
		ui::Button*			_btnOut[3];//play game
		CardListBoard*		_cardListBoard[PLAY_COUNT];
		CardOutList*		_cardOutList[PLAY_COUNT];
		Toolbar*			_toolbar;
		ui::TextAtlas*		_timerText;

		bool                _bWaitOver;                                 //�ȴ���Ϸ����
		int					_iContestNum;								//��¼�Լ�����
		int					_contestIndex;								//�����ڼ���

		ImageView*			_cardType[2];								//�������͡�����
		INT					_iValue;									//����i��ֵ
		bool				_isViewBox;									//���ص����Ƿ���ʾ

	private:
		virtual bool onTouchBegan(Touch *touch, Event *unused_event);

	private:
		void menuClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		void outClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		void seatClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		void pullDownItemCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);

		void menuTouchReset(Button* btn);

		// �߼��ӿ��鷽��
	public:
		virtual void addUser(BYTE seatNo, bool bMe) override;
		virtual void removeUser(BYTE seatNo, bool bMe) override;

		virtual void downUserCards(BYTE seatNo, bool bMe);

		virtual void setUserName(BYTE seatNo,const std::string& name) override;
		virtual void showUserMoney(BYTE seatNo, bool visible) override;
		virtual void setUserMoney(BYTE seatNo, const std::string& money);
		virtual void setUserAutoHead(BYTE seatNo) override;
		virtual void setUserHead(BYTE seatNo, bool isEmpty) override;
		virtual void setUserGameHead(BYTE seatNo, bool isLord) override;
		virtual void showUserHandCard(BYTE seatNo, const std::vector<BYTE>& values, bool isUp, bool isOnce) override;
		virtual void showUserHandCardCount(BYTE seatNo, bool visible) override;
		virtual void setUserHandCardCount(BYTE seatNo, INT count) override;
		virtual void showUserHandCardOut(BYTE seatNo, const std::vector<BYTE>& cards) override;
		virtual void showUserOutCardType(BYTE seatNo, BYTE type) override;
		virtual void setUpCardList(BYTE seatNo, const std::vector<BYTE>& upCards) override;
		virtual void getUpCardList(BYTE seatNo, std::vector<BYTE>* upCards) override;

		virtual void showLandlordCard(bool visible, const std::vector<BYTE>& cards) override;	
		virtual void showBackCardTypeAndMutiple(BYTE cardType, BYTE cardMutiple) override;
		virtual void showLandlord(BYTE seatNo, bool visible) override;
		virtual void showActionButton(BYTE flag) override;
		virtual void showWin(BYTE seatNo) override;	
		virtual void showLoading(bool bVisible) override;
		virtual void showNotice(const std::string &message);
		virtual void showUserProfit(BYTE seatNo, LLONG money) override;
		virtual void setWaitTime(BYTE seatNo, BYTE time, bool visible) override;
		virtual void showDashboard(bool visible) override;
		virtual void showUserCallScore(BYTE seatNo) override;
		virtual void showUserCallScoreResult(BYTE seatNo, int score) override;
		virtual void showUserAddDoubleResult(BYTE seatNo, int value) override;
		virtual void setBasePoint(int point) override;
		virtual void setMultiple(int multiple) override;
		virtual void showCancelAutoMask(bool visible) override;
		
		virtual void contestleaveTableDlg() override;                                   //�������뿪��Ϸ�����
		void configBoxEventCallBack(Ref* pSender, Widget::TouchEventType type);			//�����˳�����ص�
		virtual void showGameContestKick() override;                                    //������̭
		virtual void showGameContestWaitOver() override;                                //�ȴ���������
		virtual void showGameContestOver(MSG_GR_ContestAward* contestAward) override;   //��������
		virtual void updateMyRankNum(int value) override;								//��������
		virtual void addContestUI() override;											//��ӱ�������������UI
		virtual void setPaiMing(const string& str) override;							//��������
		virtual void setJuShu(const string& str) override;								//���þ���
		virtual void playContestAnimate() override;										//���ű�������
		virtual void removeContestAnimate() override;									//ɾ����������
		virtual void showLeftUpPNG(GameTaskStruct gameTask) override;

		//��ⱨ���Ķ�ʱ���Ƿ�ʼ
		void checkTimeMatchStart(float dt);
		/*
		 * my action button
		 * @param: seatNo user's seat No.
		 * @param: visible show or not.
		 */
		virtual void setOutVisible(bool visible, bool firstOut);
	
		/*
		 * show user action result
		 * @param: seatNo user's seat No.
		 * @param: visible show or not.
		 */
		virtual void showUserCallNT(BYTE seatNo, bool visible);
		virtual void showUserNotCallNT(BYTE seatNo, bool visible);
		virtual void showUserRobNT(BYTE seatNo, bool visible); 
		virtual void showUserNotRobNT(BYTE seatNo, bool visible);
		virtual void showUserReady(BYTE seatNo, bool visible);
		virtual void showUserDouble(BYTE seatNo, bool visible);
		virtual void showUserNotDouble(BYTE seatNo, bool visible);
		virtual void showUserPublishCard(BYTE seatNo, bool visible);
		virtual void showUserNotOut(BYTE seatNo, bool visible);
		virtual void hideUserStatus(BYTE seatNo);
		virtual void showUserInvalidOut(bool visible);
		virtual void showUserCannotOut(bool visible);
		virtual void hideOutCardTip();
    
		/*
		 * play sound.
		 */
		virtual void playSingle(BYTE seatNo, int value);
		virtual void playDouble(BYTE seatNo, int value);
		virtual void playThree(BYTE seatNo, int value);// value = 0, 1, 2
		virtual void playStraight(BYTE seatNo);
		virtual void playDoubleSequence(BYTE seatNo);
		virtual void playPlan(BYTE seatNo);
		virtual void playFourTake(BYTE seatNo, bool isDouble);
		virtual void playRocket(BYTE seatNo);
		virtual void playBomb(BYTE seatNo);
		virtual void playCardLast(BYTE seatNo, int value);
		virtual void playWin();
		virtual void playLose();

		/*
		 * something about desk.
		 */
		virtual void clearDesk(bool isContestEnd);
		virtual void leaveDesk(bool isForceQuit);

	protected:
		void loadLayout();
		bool isValidSeat(BYTE seatNo);
		void initParams();
		void callEverySecond(float delta);
		void startTimer();
		void stopTimer();
		void loadUserStatus(BYTE seatNo, bool visible, const std::string &filename);
		void loadOutCardTip(bool visible, const std::string &filename);
		void addOutCardList(BYTE seatNo);
		void addHandCardList(BYTE seatNo);

	public:
		void showWaitDistributionTable(bool show);

	public:
		virtual void showUserChatMsg(BYTE seatNo, CHAR msg[]);
		std::vector<std::string > recordMsg;//�����¼
	protected:
		GameTableUI();
		virtual ~GameTableUI();
		friend class GameDelearUI;
	};
}


#endif