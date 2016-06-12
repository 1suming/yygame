#ifndef __TRACTORGameLogic_h__
#define __TRACTORGameLogic_h__


#include "HNBaseType.h"
namespace TRACTOR
{
	//////////////////////////////////////////////////////////////////////////////////
	enum
	{
		//��ֵ����
		LOGIC_MASK_COLOR	=		0xF0	,							//��ɫ����
		LOGIC_MASK_VALUE	=		0x0F	,							//��ֵ����

		//�˿�����
		CT_SINGLE			=		1		,							//��������
		CT_ONE_LONG		=		2		,							//��������
		CT_TWO_LONG		=		3		,							//��������
		CT_THREE_TIAO		=		4		,							//��������
		CT_SHUN_ZI		=		5		,							//˳������
		CT_TONG_HUA		=		6		,							//ͬ������
		CT_HU_LU			=		7		,							//��«����
		CT_TIE_ZHI			=		8		,							//��֧����
		CT_TONG_HUA_SHUN	=		9		,							//ͬ��˳��
		CT_KING_TONG_HUA_SHUN	=	10		,							//�ʼ�ͬ��˳

		//////////////////////////////////////////////////////////////////////////////////

		GAME_PLAYER			=	8		,							//��Ҹ���
		MAX_CENTERCOUNT		=	5		,							//�����Ŀ
		FULL_COUNT				=	52		,							//ȫ����Ŀ
		MAX_COUNT				=	2		,							//�����Ŀ
	};

	//ʤ����Ϣ�ṹ
	struct UserWinList
	{
		BYTE bSameCount;
		WORD wWinerList[GAME_PLAYER];
	};

	//�����ṹ
	struct CardAnalyseResult
	{
		BYTE 							cbFourCount;						//������Ŀ
		BYTE 							cbThreeCount;						//������Ŀ
		BYTE 							cbLONGCount;						//������Ŀ
		BYTE							cbSignedCount;						//������Ŀ
		BYTE 							cbFourLogicVolue[1];				//�����б�
		BYTE 							cbThreeLogicVolue[1];				//�����б�
		BYTE 							cbLONGLogicVolue[2];				//�����б�
		BYTE 							cbSignedLogicVolue[5];				//�����б�
		BYTE							cbFourCardData[MAX_CENTERCOUNT];	//�����б�
		BYTE							cbThreeCardData[MAX_CENTERCOUNT];	//�����б�
		BYTE							cbLONGCardData[MAX_CENTERCOUNT];	//�����б�
		BYTE							cbSignedCardData[MAX_CENTERCOUNT];	//������Ŀ
	};
	//////////////////////////////////////////////////////////////////////////////////

	// ��Ϸ�߼�
	class GameLogic
	{
		//��������
	public:
		GameLogic();
		~GameLogic();

		//���ƺ���
	public:
		//�����˿�
		void sortCard(BYTE cbCardData[], BYTE cbCardCount);
		//�����˿�
		void randCard(BYTE cbCardBuffer[], BYTE cbBufferCount);

		//���ͺ���
	public:
		//��ȡ����
		BYTE getCardType(BYTE cbCardData[], BYTE cbCardCount);
		//��ȡ��ֵ
		BYTE getCardValue(BYTE cbCardData) { return cbCardData & LOGIC_MASK_VALUE; }
		//��ȡ��ɫ
		BYTE getCardColor(BYTE cbCardData) { return cbCardData & LOGIC_MASK_COLOR; }
		//�����˿�
		BYTE getSameCard(const BYTE bCardData[], const BYTE bMaxCard[], BYTE bCardCount, BYTE bMaxCardCount, BYTE bResultData[]);

		//���ܺ���
	public:
		//�߼���ֵ
		BYTE getCardLogicValue(BYTE cbCardData);
		//�Ա��˿�
		BYTE compareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);
		//�����˿�
		void analysebCardData(const BYTE cbCardData[], BYTE cbCardCount, CardAnalyseResult & AnalyseResult);
		//7��5
		BYTE fiveFromSeven(BYTE cbHandCardData[],BYTE cbHandCardCount,BYTE cbCenterCardData[],BYTE cbCenterCardCount,BYTE cbLastCardData[],BYTE cbLastCardCount);
		//�������
		bool selectMaxUser(BYTE bCardData[GAME_PLAYER][MAX_CENTERCOUNT],UserWinList &EndResult,const INT lAddScore[]);

	};
}

//////////////////////////////////////////////////////////////////////////


#endif // __TRACTORGameLogic_h__