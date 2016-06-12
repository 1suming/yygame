#ifndef __DriftingCar_Game_Table_UI_Callback_H__
#define __DriftingCar_Game_Table_UI_Callback_H__

#include "HNNetExport.h"
#include "HNLogicExport.h"
#include "DriftingCarMessageHead.h"


namespace DriftingCar
{
	class GameTableUICallback : public HN::IHNGameLogicBase
	{
	public:

		virtual void Test() = 0;

		virtual void OnGameStation() = 0;												//��һ�ν���ʱ����

		virtual void startGameFree() = 0;												//������Ϣ

		virtual void startGameBeginBet() = 0;											//��ע��Ϣ

		virtual void startGamePaoMaDeng() = 0;											//�������Ϣ

		virtual void startGameEndResult() = 0;											//������Ϣ

		virtual void GameBetReuslt() = 0;												//��ע�����Ϣ

		virtual void setBetEnabled(bool enabled) = 0;									//��ע����ѡ��ť�Ƿ����

		virtual void setFreeTime(BYTE time) = 0;										//���ÿռ�ʱ��

		virtual void setBetTime(BYTE time) = 0;											//����Ͷעʱ��

		virtual void setSendTime(BYTE time) = 0;										//���ÿ���ʱ��

		virtual void setMsg(const std::string &msg) = 0;								//��ʾ��ʾ��Ϣ

		virtual void startSend( int end, BYTE time) = 0;								//������������

		virtual void addRecord(const std::vector<BYTE>& records, BYTE byResult) = 0;	//��ӿ�����¼
														
		virtual void addNT(BYTE seatNo, const std::string& name, LLONG money) = 0;		//���ׯ��

		virtual void removeAllNT() = 0;													//�Ƴ�����ׯ��
														
		virtual void setBetAreaTotal(bool isMy, BYTE AreaNo, LLONG value) = 0;			//������ע��������ע
														
		virtual void setBetAreaUser(bool isMy, BYTE AreaNo, LLONG value) = 0;			//������ע���������ע
														
		virtual void setBetTotal(LLONG value) = 0;										//��������ע����
																														
		virtual void setNTName(const std::string& name) = 0;							//����ׯ������
														
		virtual void setNTCount(int count) = 0;											//������ׯ����
														
		virtual void setNTMoney(LLONG value) = 0;										//����ׯ�ҽ��
														
		virtual void setNTGrade(LLONG value) = 0;										//����ׯ�ҳɼ�
														
		virtual void setUserName(const std::string& name) = 0;							//�����������
														
		virtual void setUserMoney(LLONG value) = 0;										//������ҽ��
														
		virtual void setUserGrade(LLONG value) = 0;										//������ҳɼ�

		virtual void setUserBetMoney(LLONG value) = 0;									//������ע���
														
		virtual void leaveDesk() = 0;													//�뿪����
																
		virtual void setOpenArea(int index) = 0;										//��ʾ��������
																
		virtual void hideOpenArea() = 0;												//���ؿ�������
																
		virtual void setApplyAndLeaveButtonEnabled(bool enabled) = 0;					//������ׯ���밴ť�Ƿ����

		virtual void SetGameEndResult(LLONG userScore, LLONG userCapital,				
			LLONG userNTScore, LLONG userNTCapital) = 0;								//���ý������ʾ

		virtual void hideGameEndImage() = 0;											//���ؽ����

		virtual void ShowWaitNTCount(BYTE mySeatNo) = 0;								//��ʾ�Լ���ׯ���б��е�λ��

		virtual void ShowApplyButton() = 0;												//��ʾ��ׯ��ť

		virtual void ShowDownNTButton() = 0;											//��ʾ��ׯ��ť

		virtual void ShowNoNT() = 0;													//��ʾû��ׯ��

		virtual void ShowBetAreaAction() = 0;											//��ʾ��ע����

		virtual void HideBetAreaAction() = 0;											//������ע����

		virtual void ShowImageMyNT() = 0;												//��ʾ������ׯ
			
		virtual void ShowImageMyNoNT() = 0;												//��ʾ������ׯ
	};
}


#endif