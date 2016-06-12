#ifndef __GoldenToad_GamePlayer_H__
#define __GoldenToad_GamePlayer_H__

#include "HNLobbyExport.h"
#include "cocostudio/CocoStudio.h"
#include "GoldenToadGameFishManager.h"
#include "GoldenToadMessageHead.h"
#include "ui/CocosGUI.h"
#include <string>

using namespace ui;
using namespace std;
using namespace HN;

USING_NS_CC;

namespace GoldenToad
{
	class GameTableUI;
	class GameBullet;

	class GamePlayer : public HNLayer
	{
	public:
		enum CANNON_TYPE{
			DoubleTube = 0,
			ThreeTube
		};
	public:
		TextAtlas*		_AtlasLabel_Money;
		TextAtlas*		_AtlasLabel_CannonNum;
		Layout*         _Layout_Table;
		ImageView*		_Image_Battery;
		ImageView*		_Image_MoneyBox;
		Sprite*			_Sprite_Cannon;
		ImageView*		_Image_Card;
		ImageView*		_Image_Lock;
		Sprite*			_Sprite_Bingo;
		Button*			_Button_Add;			//���ڰ�ť
		Button*			_Button_Cut;			//���ڰ�ť

		GamePlayer();
		virtual ~GamePlayer();

	public:
		static GamePlayer* createGamePlayer(BYTE seatNo, Layout* Panel_Table, GameFishManager* fishManager);
		bool init(BYTE seatNo, Layout* Panel_Table, GameFishManager* fishManager);

	public:
		//���ø��ֲ���
		void setConfig(CMD_S_GameConfig* gConfig);
		//������̨�Ƿ�ɼ�����λ���Ƿ�����ң�
		void setBatteryVisible(bool bVisible);
		//��ʾ��ҽ��
		void setUserMoney(const LLONG money);
		//��ʾ������ڵȼ�
		void setCannonNum(const INT cannonNum);
		//������������
		void setCannonType(BulletKind kind);
		//�����ӵ�����
		void setBulletKind(BulletKind kind);
		//�����Ƿ�������
		void setIsEnergyCannon(bool isEnergyCannon);
		//�����Ƿ�������
		void setIsLockFish(bool bLock, INT fishId);
		//����bingoת��
		void setBingo(LLONG money);

	public:
		//��ȡ��ǰ����Ƿ����
		bool batteryIsVisible();
		//��ȡ��ǰ����λ��
		Vec2 getCannonPoint();
		//��ȡ��ҽ��λ�ã�Ϊ�����������ն���
		Vec2 getMoneyPoint();
		//��ȡ������Χ
		FLOAT getBulletNetRadiusById(BYTE id);
		//�ܷ񿪻�
		bool isFireEnable();
		//����ָʾ��������
		void showLockIndicatorLine(float dt);


	public:
		//��ʾ����
		void gameFire(FLOAT degree, BYTE seatNo, INT bulletId, BulletKind bulletKind, INT bulletMulriple, INT lockFishId, bool isRobot = false);
		//�������ڿ��𶯻�
		void playCannonFireAction();

	private:
		void menuClickCallback(cocos2d::Ref* pSender, ui::Widget::TouchEventType touchtype);

	private:
		CC_SYNTHESIZE(bool,			_isMe, IsMe);
		CC_SYNTHESIZE(bool,			_isRobot, Robot);
		CC_SYNTHESIZE(BYTE,			_seatNo, SeatNo);
		CC_SYNTHESIZE(INT,			_userId, UserID);
		CC_SYNTHESIZE(Vec2,			_gumPos, GumPos);					//�ڹ�λ��
		CC_SYNTHESIZE(LLONG,		_currentMoney, CurrentMoney);
		CC_SYNTHESIZE(INT,			_min_bullet_multiple, MinMultiple);
		CC_SYNTHESIZE(INT,			_max_bullet_multiple, MaxMultiple);
		CC_SYNTHESIZE(INT,			_currentFirepower, Firepower);
		CC_SYNTHESIZE(GameTableUI*,	_tableUI, GameTable);
		CC_SYNTHESIZE(INT,			_bulletNum, BulletNum);
		CC_SYNTHESIZE(bool,			_bLock, Lock_Fish);
		CC_SYNTHESIZE(INT,			_lockFishId, LockFishId);
		CC_SYNTHESIZE(BulletKind,	_currentBulletKind, Bullet_Kind);

		INT							_bulletSpeed[BULLET_KIND_COUNT];
		INT							_netRadius[BULLET_KIND_COUNT];
		std::string					bulletName[BULLET_KIND_COUNT];
		bool						_bEnergyCannon;
		GameFishManager*			_fishManager;
		BulletKind					_cannonKind;

	public:
		std::vector<GameBullet*>	_bulletList;
		std::vector<Sprite*>		_bubbleList;
	};

}


#endif // __GoldenToad_GamePlayer_H__
