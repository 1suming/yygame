#pragma once
#include "../GameMessage/GameData.h"

#pragma pack(1)
typedef struct tagZhongNiao
{
	BYTE byMenPai[PLAY_COUNT][MEN_CARD_NUM];//��������
	int byCard[4];//��������
	tagZhongNiao()
	{
		clear();
	}
	void clear()
	{
		memset(this, 255 ,sizeof(tagZhongNiao));
	}
}ZhongNiao;

struct	tagMaiMa
{
	BYTE byUser;	//��������
	BYTE byCount;	//����ĸ���
	BYTE byCard[4];	//��������

	tagMaiMa()
	{
		clear();
	}

	void clear()
	{
		memset(this, 255 ,sizeof(tagMaiMa));
	}
};

///��Ϸ���ݹ�����
class GameDataEx: public GameData
{

public:
	GameDataEx(void);
	~GameDataEx(void);
public:
	///�齫ʱ��ȴ���ʱ�䵽�˷�������Ϊ����
	MJ_WaitTimeEx   m_MjWait;
	ZhongNiao m_ZhongNiao;

	tagMaiMa	m_TMaiMa;

public:
	///���������¼��ĺ���¼�
	virtual void SetThingNext();
	///����ĳ���¼��ĺ���¼�
	virtual void SetThingNext(BYTE id);
	//virtual ��ʼ������
	virtual void InitData();
	///��ʼ������
	virtual void LoadIni();

};
#pragma pack()
//ȫ�ֶ���
extern GameDataEx g_GameData;