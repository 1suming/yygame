#ifndef _ZYHZ_CARDPOOL_H_
#define _ZYHZ_CARDPOOL_H_

#include "ZYHZ_Card.h"

namespace ZYHZ
{
	class PoolAction;

	class CardPool :
		public cocos2d::Node
	{
	public:
		CardPool(void);
		~CardPool(void);

		virtual void addCard(Card* card);            // ���ƶ�����һ����
		virtual void removeCardMsg(Card* card) {};
		virtual void removeCard();      // ���һ����
		virtual void sendSomeCard(std::vector<Card *> vec);     // ������һЩ��
		virtual void sortCard() {};                        // �����ƶ�
		virtual bool init() override;       
		virtual void setCatchPos(const Vec2& pos) { _catchPos = pos; _outToDeskPos = pos; };   // ���������ưڷų�ʼλ�úͳ���λ��
		void setStartSortPos(const Vec2& pos) { _startSortPos = pos; }
		virtual void setHandCardPos(Card* noCard = nullptr) {};  // ��������
		virtual Card* getAutoSendCard();  // ȡ�Զ����Ƶ�����
		const Vec2& getOutToDeskPos() { return _outToDeskPos; }  // ��������λ��
		virtual void setCanOper(bool bs = true) {};  // �ɴ���
		list<Card *> & getListCard() { return _listCard; }   // ����
		virtual void setPoolAction(PoolAction* action);    // ���ö���
		void addLeftOutCount(const INT& count);   // ������ߵ���
		const INT& getLeftOutCount();                    // ������
		const sitDir& getSitDir();                               // ����
		const Vec2& getStartSortPos() { return _startSortPos; }        // ��ʼ�ڷ�λ��
		const Vec2& getCurrHandCardStartSortPos() { return _currSortStartPos; }
		virtual void afterCatchCheckSomething(Card* card) {};		// ץ�Ƽ�⶯��
		virtual void afterOutCheckSomething(Card* card) {};		// ���Ƽ�⶯��

		virtual void addSomeOutCards(const INT& count, const INT& number, bool hideLastOutCard = true, bool isMingGang = true); // ����
		const Vec2& getActionPos();    // ���Ŷ���λ��i

		virtual void afterAction(const INT& removeCount);

		void setActionCardNumber(const INT& number) { _actionNumber = number; };    // ������
		void setTouchActionVector(const std::vector<Card *> vecCard) {_vecTouchCard = vecCard; };  // �������ƶ�����
		std::vector<Card *>& getTouchVec() { return _vecTouchCard; };

		void setOutCardCount(const INT& count) { _sendCardCount = count; }
		const INT& getOutCardCount() { return _sendCardCount; }
		virtual void huCard(bool isZimo = false, bool isQiangGang = false);  // ����
		virtual void finishGame();

		std::list<Card *>& getHuListCard() { return _listHuCard; }  // �����б�
		bool getHasHu() { return _hasHuCard; }  // �Ƿ��Ѿ�����
		void setHasHu(const bool& bs = true) { _hasHuCard = bs; }

		void reSetData();  // �������ݣ��¾ֿ�ʼ
		void setCanCheckAction() { _needCheckPeng = true; _needCheckHu = true; };  // ���ÿɽ��ܶ���
		void setLastActionCardNumber(const INT& number) { _lastActionCardNumber = number; }   // ���ú�����
		const INT& getLastActionCardNumber() { return _lastActionCardNumber; }   // ��ȡ������
		float getSortInterval() { return _sortIntervel; }
		float getActionCardInterval() { return _actionCardIntervel; }

	protected:
		std::list<Card *> _listCard;  // ��Ƭ����
		std::list<Card *>::iterator _listIterStart;   // list������
		std::list<Card *>::iterator _listIterEnd;
		bool _isInSort;   // ����ģʽ
		cocos2d::Vec2 _catchPos;  // ץ��λ��
		cocos2d::Vec2 _startSortPos;
		cocos2d::Vec2 _currSortStartPos;  // ��ǰ������ʼ��
		float _sortIntervel;		// �������м��
		float _actionCardIntervel;  // �����ļ��
		float _outCardInterval;
		Vec2 _outToDeskPos; // ����λ��
		PoolAction* _poolAction;   // ���ƶ���
		sitDir _dir;                    // ��λ
		INT _leftCount;      // �Ѿ���������Ŀ
		Size _cardSize; // �������Ƴߴ�
		INT _actionNumber;  // �����ı���
		Card* _rbeginActionCard;  // �����ֱ߶�����
		Vec2 _actionCardMoveV;     // ������ƫ���� ˮƽ
		Vec2 _actionCardMoveT;		// ������ƫ���� ��ֱ

		std::vector<Card *> _vecTouchCard; // ���Ƶ�����
		std::vector<Card *> _vecKongCard; //  ��������
		std::vector<Card *> _vecTmpKongCard;  // ������ʱ��

		INT _sendCardCount;  // ������
		bool _hasHuCard;   // �Ƿ��Ѿ�����
		INT _lastActionCardNumber;    // ���Ķ�����

		std::list<Card *> _listHuCard;   // ���Ƶ�����

	public:
		void setNotCheckHu() { _needCheckHu = false; };
		void setNotCheckPeng() { _needCheckPeng = false; }
		const bool& getCanPeng() { return _checkedCanPeng; }
		const bool& getCanHu() { return _checkedCanHu; }

	protected:
		bool _needCheckHu; // ����Ƿ�ɺ�����
		bool _needCheckPeng; // ����
		bool _checkedCanHu;  // ��⵽�ܺ�
		bool _checkedCanPeng; // ��⵽����
	};

}

#endif // _CARDPOOL_H_
