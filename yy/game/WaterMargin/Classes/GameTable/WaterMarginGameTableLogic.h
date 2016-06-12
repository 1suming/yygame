#ifndef __HN_WaterMargin_JLBDTableLogic_H__
#define __HN_WaterMargin_JLBDTableLogic_H__


#include "cocos2d.h"
#include "HNUIExport.h"
#include "HNNetExport.h"
#include "HNLogicExport.h"
using namespace std;

namespace WaterMargin
{

	class GameTableUICallback;

	class GameTableLogic : public HN::HNGameLogicBase
	{
	public:
		GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool bAutoCreate);
		~GameTableLogic();
	
	protected:
		GameTableUICallback*	_uiCallback;

	public:
		//�����Ϣ
		virtual void dealGameStationResp(void* object, INT objectSize);
		virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize);
		virtual void dealUserUpResp(MSG_GR_R_UserUp * userSit, UserInfoStruct* user);

		void dealScoreResult(void* object, INT objectSize);
		void dealAddNoteResult(void* object, INT objectSize);
		void dealRollResult(void* object, INT objectSize);
		void dealBibeiHistoryResult(void* object, INT objectSize);
		void dealWinScoreResult(void* object, INT objectSize);
		void dealBiBeiResult(void* object, INT objectSize);
		void dealMaLiResult(void* object, INT objectSize);

		
		void sendUpOrDownMessage(bool value);
		void sendAddNoteMessage();
		void sendGetWinMessage(int value);
		void sendStartRollMessage();
		void sendBibeiBtnMessage();
		void sendDaXiaoMessage(int iBibeiType,int iBibeiMax);
		void sendMaLiMessage();

		//����ת��
		int* DataSwitch(int* str);

		//����ȱ���÷�
		void handleSecond();
		//�����ٴαȱ�
		void handleBiBeiAgain();
		//MaLiתһ�εĽ����ʾ
		void handleMaLiViewGold();
		void MaLiJudge();
		//mali����ת��ȱ�
		void changeMaLiToBB();
		//mali�Ե÷ֽ���
		void changeMaLiToDF();
		//վ����
		void SendStandUp();

		
		int test2;
	};
}


#endif // __HN_SlotMachine_JLBDTableLogic_H__