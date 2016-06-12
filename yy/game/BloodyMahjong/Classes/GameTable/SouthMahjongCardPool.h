#ifndef _SOUTHMAHJONGCARDPOOL_H_
#define _SOUTHMAHJONGCARDPOOL_H_

#include "MahjongCardPool.h"

namespace XZMJ
{
	class SouthMahjongCardPool :
		public MahjongCardPool
	{
	public:
		SouthMahjongCardPool(void);
		~SouthMahjongCardPool(void);

		CREATE_COUNT(SouthMahjongCardPool);
		virtual bool init(INT count);

		virtual void setHandCardPos(Card* noCard = nullptr) override;						// ��������
		virtual void sortCard() override;									// ��������
		virtual void removeCardMsg(Card* card) override;	
		virtual void removeCard() override;
		void removeCardMsgCallBack();  
		virtual void setCanOper(bool bs = true);                        // �����ܷ������
		virtual Card* getAutoSendCard() override;  // ȡ�Զ����Ƶ�����
		bool hasNoQueColor();  // ��ȱ�Ż�ɫ
		bool checkTouchCard(Card* card); // ����Ƿ����
		bool checkMeldedKong(Card* card);  // �������
		bool checkConcealedKong();  // ��ⰵ��
		bool checkTouchKong(Card* card);  // �������
		virtual void afterAction(const INT& removeCount) override;   
		virtual void afterCatchCheckSomething(Card* card) override;  // ץ�Ƽ��
		virtual void afterOutCheckSomething(Card* card) override;               // ���Ƽ��
		void sortListCard(std::list<Card *>& list);
		bool canTing();
		bool canHu(HUPAI_TYPE_EX& typeBase, HUPAI_TYPE_EX& typeAdd, bool& hasAdd, Card* card = nullptr);

	private:
		void sendBlockMsg();
		void canBlock();
		bool isShunZi(std::list<INT>& listNum);  // ���˳��
		void calcCardCount(std::list<Card *>& listCard,std::list<INT>& listCardNum, std::vector<INT>& vecCard, std::vector<INT>& vCount);
		void calCOCOS_NODEumCount(std::list<INT>& listCardNum,std::vector<INT>& vecCard, std::vector<INT>& vCount);
		// ���Ƽ��
		bool isPingHu(Card* card = nullptr);   // ƽ��  
		bool isDuiDuiHu(Card* card = nullptr); // �ԶԺ�
		bool isQingYiSe(Card* card = nullptr);  // ��һɫ
		bool isDaiJiuYao(Card* card = nullptr);  // ���۾�
		bool isXiaoQiDui(Card* card = nullptr);    // С�߶�
		bool isJiangDui(Card* card = nullptr);   // ����
		bool isQingDui(Card* card = nullptr);    // ���
		bool isLongQiDui(Card* card = nullptr);  // ���߶�
		bool isQingQiDui(Card* card = nullptr);  // ���߶�
		bool isQingYaoJiu(Card* card = nullptr); // ���۾�
		bool isLongQingQiDui(Card* card = nullptr);  // �����߶�
		bool isYaoJiuQiDui(Card* card = nullptr);    // �۾��߶�
		bool isTianHu(Card* card = nullptr); // ���
		bool isDiHu(Card* card = nullptr);     // �غ�


	private:
		mahjongColor _queColor; // ȱ��
		Card* _willRemoveCard; // ���Ƴ�����
		bool _hasSelectOutCard;
	};

}

#endif