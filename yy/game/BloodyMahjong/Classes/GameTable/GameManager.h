#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include "CardPoolUser.h"
#include "ResourceLoader.h"
#include "XZMJ_GameTableLogic.h"
#include "XZMJ_MessageHead.h"
#include "Factory.h"
#include "MahjongCardPoolUser.h"

namespace XZMJ
{
	class GameManager :
		public cocos2d::Node
	{
	public:
		GameManager(void);
		~GameManager(void);

		// ��ʼ����
	public:
		virtual bool init() override;		
		CREATE_FUNC(GameManager);
		virtual void loadResource();						// ������Դ
	private:
		virtual void initData();                               // ��ʼ������
		virtual void addLoader();							// �����Դ������
		virtual void setText();									// �����ı�
		virtual void setButton();								// ���ð�ť
		virtual void addUI();									// ����UI��
		void reParent(std::string name);				// ����cocos_node�ڵ㸸�ڵ�
		void resetDiKuang();									// ���õ׿�ť��״̬

		// cocos ��ť�ص�����
		void startButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void trusteeshepButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void quxiaotuoguangButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void returnButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void queWanButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void queTongButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void queTiaoButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void chiButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void pengButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void gangButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void tingButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void huButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void qiButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void huQueDingButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void confirmButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void cancelButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void continueButtonClickCallBack(Ref* ref,Widget::TouchEventType type);
		void leaveButtonClickCallBack(Ref* ref,Widget::TouchEventType type);

		// ��Ŧ���ʱ������ʵ��
	private:
		void endQueMen(INT queColor = 0, bool isRand = false);												// ������ȱ,��ť���߼�ʱ������
		void endAllTimer();																										// ��ֹ���м�ʱ��
		int getRd_A_N(int a, int n);

		// ��ʱ��
	public:
		void startTimer(const sitDir& timerPointDir, timerType timerType);
	private:
		void waitTimer(float dt);																							    // �Ⱥ�ʼ
		void queMenTimer(float dt);																							// �Ⱥ�ȱ
		void waitOutTimer(float dt);																							// �Ⱥ���ҳ���
		void waitThinkTimer(float dt);																						// �Ⱥ���Ҷ�������
		void playBgTimer(float dt);																							// ���ű�������
		void setRemainTime(const float& time) { _remainTime = time; }									// ����ʣ��ʱ��

		// �Զ����ش���������ʱ
	public:
		void setConnectSuccess() { _isConnectSuccess = true; }
	private:
		void autoReturnDeskWhenLag();
		void checkConnectSuccess(float dt);
		void willReturnTimer(float dt);
		void returnDesk(bool safeLeave = true);
		void trusteeshepSendMsg(bool trusteeshep = true);	// �й�
		bool _isConnectSuccess;

		// ��ʱ���������
	private:
		LabelAtlas* _pAtlasTimerNumber;																					// ��ʱ�����ֱ�ʾ
		float _remainTime;																										// ��ǰ����ʱʣ��ʱ��
		bool _autoBegin;																										   // �Ƿ��Զ���ʼ
		unsigned int _clockID;																								   // ����ʱ��ЧID

		// ����ָ��
	public:
		static GameManager* getInstance();
	private:
		static GameManager* _instance;

		// ��Դָ��
	public:
		cocosResourceLoader* getCR() { return _cLoader; }
		spriteResourceLoader* getSR() { return _sLoader; }
		plistResourceLoader* getPR() { return _pLoader; }

	private:
		cocosResourceLoader* _cLoader;																					// cocostudio��Դ
		spriteResourceLoader* _sLoader;																					// sprite��Դ 
		plistResourceLoader* _pLoader;																					// plist��Դ

	private:

		// ���̿���
	public:
		virtual void addUser(const sitDir& dir, const UserInfoStruct& user);																						//  �������
		virtual void userLeave(const sitDir& dir);																																//  ����뿪
		virtual void userTrusteeshep(const sitDir& dir, bool isTrust = true);																														//  ����й�
		virtual void userAgree(const sitDir& dir);																																// ���׼��
		virtual void startGame(const bool& autoBegin = true);																											// ��ʼ��Ϸ
		virtual void startSendBottomCard(const sitDir& seziDir, INT sezi0, INT sezi1, std::vector<std::vector<INT>> vvSouthCard);	// ������
		const vector<CardPoolUser *>& getVecUser();
		virtual void catchCard(const sitDir& dir, const INT& number, const bool& head = true);														// ץ��
		virtual void outCard(const sitDir& dir, const INT& number);																									// ����
		void showMoney();
		void showMoneyCallBack();

		//  '����ʱ��
		void reconnected(																																									// ��������
			std::vector<std::vector<INT>>& vvHandCard,								
			std::vector<std::vector<INT>>& vvOutCard, 
			std::vector<std::vector<std::vector<INT>>>& vvvActionCard,
			std::vector<std::vector<INT>>& vvMenPai,
			std::vector<bool>& vTrusteeshep,
			std::vector<INT>& vQueMen,
			sitDir& ntDir);																																									// end ��������
		void hideAllBottomCard(bool hide = true);																															// �������е���
		void showPatternBottomCard(std::vector<std::vector<INT>>& vvec);																					// ��ʾ���ֵ���
		void showHandCard(std::vector<std::vector<INT>>& vvec);																									// ��ʾ����
		void showActionCard(std::vector<std::vector<std::vector<INT>>>& vvvec);																			// ��ʾ������
		void showOutCard(std::vector<std::vector<INT>>& vvec);																									// ��ʾ�ѳ���
		
		void finishGame(const std::vector<std::vector<int>>& vvNum);																							// ������Ϸ
		void restartGame();																																								// ������Ϸ
		void initAllHandCard(const sitDir& seziDir, INT sezi0, INT sezi1, std::vector<std::vector<INT>> vvSouthCard);						// �����������
		Card* commonCatchCard(const sitDir& dir, const INT& mahjongNumber);																			// ��ǰץ������
		Card* tailCatchCard(const sitDir& dir, const INT& mahjongNumber);																					//  ����β��ץ��
		void addCardPool(CardPool * pool);																																		// ���ӷ�λ��Ƭ��
		std::vector<CardPool *>& getUserCardPool();																														// ��ȡ��Ƭ��
		
		// ����ʱ��
		void hasHu(const sitDir& dir, bool isZiMo = false, bool isQiangGang = false);																		// ����ʱ��
		void setLastActionCard(const sitDir& dir, const INT& number);																								// �����������
		void setHasHu(const sitDir& dir, const bool& bHasHu);																										// ������Һ���

		// ȡ��
	public:
		Card* getCard(const sitDir& dir, const INT& mahjongNumber);																							// ȡ������
		Card* getZhengDaoCard(const INT& mahjongNumber);																										// ����
		Card* getXiaoDaoCard(const sitDir& dir, const INT& mahjongNumber);																				// С��

		// ����
	private:
		Card* getZhengLiCard(const INT& mahjongNumber);																											// ������
		Card* getBeiLiCard();																																								// ������
		Card* getZuoLiCard();																																							// ������
		Card* getYouLiCard();																																							// ������
		
		// ���ƿ���
	public:
		void autoSendCard(const sitDir& dir);																										// �Զ�����
		cocos2d::Vec2 getOutCardDeskPos(const sitDir& dir);																				// ��������ڷ�λ��
		void afterOutCard(const sitDir& dir);																											// ���ƺ�
		void startOutCard(const sitDir& dir);																											// ׯ�ҵ�һ�ο�ʼ����
		void setMinusOutCount(const sitDir& dir);																								// ��һ�δ��������Ŀ
		Card* getLastestOutCard();																														// �õ����³�����
		bool isNoCardCanCatch();																															// �Ƿ�û��ץ��
		void checkHandCardData();																														// У�������վ�
		void setHandCardFormServer(const std::vector<std::vector<int>>& vvNum);											// ͬ���������������
	
	private:
		void sendCard(const sitDir& dir/*��ʼ��λ*/, const INT& heapCount /*��ʼ����*/);									// ���ַ���
		void sendCardToPool(const INT& cardCount);																							// ���͵��Ƹ������, ��ʱ������
		void sendCardTimer(float dt);																													// ���Ƽ�ʱ��
		
		// ��������
	public:
		void setQueMen(INT queMen);                                                // ����ȱ��
		INT& getQueMen();																   // ��ȡȱ��
		void setPlayGame(bool bs = true);										   // �����Ƿ����ڴ���
		const bool& isGamePlaying();												   // ��ȡ�Ƿ����ڴ���
		void setKongType(INT type);													   // ���ø�������
		void setConcealedNumber(INT number);								   // ���ð�����ֵ
		tagHuPaiEx& getHuMsg();													   // ��ȡ������Ϣ
		sitDir& getStartDir();																   // ȡׯ��ֵ

	private:
		std::vector<CardPoolUser *> _vecUser;
		bool _isPlayGame;																		// ��Ϸ״̬�Ƿ�����4�˴���
		bool _isTrusteeShep;																	// ��Ϸ״̬�Ƿ��й�
		bool _isSouthPlayOper;																// �Ƿ��ֵ���Ҳ���
		INT _queMen;																			// ��ǰ���ȱ��
		sitDir _startDir;																			// ׯ��λ��
		INT _kongType;																			// �������� ����������
		INT _concealedNumber;															// ������
		tagHuPaiEx _huMsg;																	// ������Ϣ
		std::vector<Card *> _vecTmpHuCard;										// ����ʱ��
		std::vector<std::vector<Card *>> _vvLastShow;							// �����ʾ���е���

	private:
		Card * _lastestOutCard;																								// ���������
		std::vector<std::vector<int>> _vvNum;																		// ����˴���������
		std::vector<Card *> _vecBottomCard;																		// ��������
		std::vector<std::vector<std::vector<Card *>>> _vvvZhengLiCard;								// ����������
		std::vector<std::vector<std::vector<Card *>>> _vvvZhengDaoCard;							// ����������
		std::vector<Card *> _vecBeiLiCard;																			//  ����������
		std::vector<Card *> _vecZuoLiCard;																			// ����������
		std::vector<Card *> _vecYouLiCard;																			// ����������
		std::vector<std::vector<std::vector<std::vector<Card *>>>> _vvvvXiaoDaoCard;		// ��������
		std::vector<std::vector<Card *>> _vvecHuCard;														// ��������
		INT northSouthCount;																								// �ϱ��ƶ���
		INT westEastCount;																									// �����ƶ���
		INT _currCardIndex;																									// ��ǰץ��λ��
		INT _lastCardIndex;																									// β��λ��
		INT _sendCardCount;																								// ���ƵĴ���
		std::vector<cocos2d::Vec2> _vecCatchEndPos;															// ץ�Ƶİ���λ������
		std::vector<CardPool *> _vecCardPool;																		// 4����Ƭ��
		std::vector<cocos2d::Vec2> _vecCatchPos;																// 4��ץ�ư��õ�
		sitDir _startSendDir;																									// �������ƿ�ʼλ��
		std::vector<std::vector<INT>> _vvStartHandCard;														// ��ʼ����
		std::vector<INT> _vecCurrSendIndex;																		// ���ڷ��������µ�λ�� 4������
		std::vector<cocos2d::Rect> _vecOutCardRect;															// ���ưڷ�rect
		std::vector<INT> _vecHasOutCardCount;																	// �Ѿ����Ƶ���Ŀ
		INT _zorderWest;																										// ����zoder
		INT _zorderEast;																										// ����zoder
		INT _zorderSouth;																										// ����zoder
		INT _zorderNorth;																										// ����zoder
		INT _lineCount;																											// ÿһ�а��齫

		// ��ֱ�����Ӧ
	public:
		void setGlobalScale();																// ����ȫ������
		const float& getGlobalScale();													// ��ȡȫ������
		const Vec2& getNewOrigin();													// ��ȡȫ��ԭ��

	private:
		float _globalScale;																		// ȫ������
		Vec2 _newOrigin;																		// ȫ��ԭ������

		// ������
	public:
		PoolAction* _touchCard;															// ����
		PoolAction* _meldedKong;														// ����
		PoolAction* _concealedKong;													// ����
		PoolAction* _touchKong;															// ����
		PoolAction* _huCard;																// ����

		// ��Ч���
		void playEffect(int a, int n, std::string head, int index);				// ������Ч

		// �����Ϣ
		bool isMan(int index);																// �����˻���Ů��

		// ��ʱ��Դ����, ÿһ����Ϸ��������
	public:
		Card* &getBigShowSp();															// ��ȡ��ʾ�������
		void addBigShowSp(const sitDir& dir, const INT& number);		// �����ʾ�������

	private:
		Card* _bigShowCard;																// ��ʾ�������
	};

}

#endif // _GAMEMANAGER_H_