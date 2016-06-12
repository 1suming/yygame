#ifndef __DZPokerGamePlayer_h__
#define __DZPokerGamePlayer_h__

#include "cocos2d.h"
#include "HNNetExport.h"
#include "platform/CCCommon.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>
#include "DZPokerPokerCard.h"
#include "HNUIExport.h"

using namespace ui;
using namespace std;

namespace DZPoker
{
	class PlayerUI: public HN::HNLayer
	{
	public:
		/*�������*/
		static PlayerUI* create(LLONG userId);
		/*��������*/
		void setName(const std::string& name);
		/*�����Ա�*/
		void setSex(bool men);
		/*���ó���*/
		void setChip(LLONG chip);
		/*�������״̬*/
		void setStatus(const char* status,int value);
		/*�����������*/
		std::string getName() const;
		/*�����������*/
		void setHandCard(int index, BYTE value);
		/*��������*/
		void hideHandCard();
		/*����ʱ*/
		void startWait(int time, int total);
		/*ֹͣ����ʱ*/
		void stopWait();
		/*��ʾӮ��Ч��*/
		void playWin(int type);
		/*ֹͣ��ʾӮ��Ч��*/
		void stopPlayWin();
		/*��ʼ������*/
		virtual bool init(LLONG userId);
		PlayerUI();
		virtual ~PlayerUI();
	protected:
		bool onTouchBegan(Touch *touch, Event *unused_event);
		void onTouchEnded(Touch *touch, Event *unused_event);
		void resetName(cocos2d::Node* pNode);
	private:
		BYTE _deskPosition;							//��ҵı�����ͼ�е����Ӻ�
	private:
		HNSprite* _bg;
		HNSprite* _headSp;
		Text* _nameText;
		Text* _chipText;
		std::string _name;
		LLONG _chip;
		PokerCard* _handCard[2];
		LLONG _userId;
		ProgressTimer* _progressTimer;
		void onWaitTimer(float delta);
		float _leftTime;
		float _totalTime;
	};

}

#endif // __DZPokerGamePlayer_h__
