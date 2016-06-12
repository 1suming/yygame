#ifndef __HN_WaterMargin_GameTableUICallback_H__
#define __HN_WaterMargin_GameTableUICallback_H__

#include "WaterMarginMessageHead.h"

/************************************************************************/
/* ���ͣ��ص��ӿ�                                                        */
/* ���ܣ����������߼����յ���Ϣʱ���ý���ĺ����ӿ�                        */
/* ˵������user������û���û�������������                                */
/* -1�Ĳ������øı�                                                      */
/************************************************************************/

namespace WaterMargin
{

	class GameTableUICallback : public HN::IHNGameLogicBase
	{
	public:
		
		virtual void leaveDesk() = 0;										//�뿪����

		virtual void setMoney(LLONG money) = 0;

		virtual void setDataText(int value, int num) = 0;					//����5������

		virtual void setImageData(int* str) = 0;							//���չ������ͼƬ����

		virtual void setBtnVisible(int Tag, bool visible) = 0;

		virtual void setWinScore(int value) = 0;

		virtual void getWinScore() = 0;										//����÷ְ�ť

		virtual void setSaiZi(int s1, int s2) = 0;

		virtual void playSecondAnimate(int value, int dianShu) = 0;

		virtual void handleSecondDeFeng() = 0;

		virtual void BiBeiHistory(int* history) = 0;

		virtual void BiBeiAgain() = 0;										//�ٴαȱ�

		virtual void MaLiResult(bool begin, int MaLicount, int Index, int* str) = 0;

		virtual void viewWinGold() = 0;                                     //��ʾС����Ӯ��ң�win�����ݣ�

		virtual void MaLiEndBB() = 0;										//С����������ת��ȱ�

		virtual void MaLIEndDF() = 0;										//С�����Ե÷ֽ���

		virtual void JudgeMaLi() = 0;

		virtual void JudgeConnect() = 0;
	};
}

#endif //__HN_SlotMachine_GameTableUICallback_H__