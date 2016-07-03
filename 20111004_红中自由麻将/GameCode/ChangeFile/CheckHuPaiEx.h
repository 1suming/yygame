#pragma once

#include "../GameMessage/CheckHuPai.h"
///�¼������ࣨ���أ�
class CheckHuPaiEx:public CheckHuPai
{
public:
	CheckHuPaiEx(void);
	~CheckHuPaiEx(void);
	
public:
	int   m_byTingMaxFan ;//���ƿ��ܵ����

public:
	
	///����ܷ�����
	virtual bool CanTing(BYTE station,BYTE CanOut[]);
	

	///����������Ҹ�������
	int CountTingGen(BYTE pai[]);
    ///���ƣ�CanHu
    ///��������ϸ������
    ///@param station ���Ƽ��λ�ã�hutype[] �ܺ��Ƶ����ͣ�zimo �Ƿ�����
    ///@return true  �ܺ��� false ����
    virtual bool CanHu(BYTE station,BYTE lastpai,BYTE hutype[],bool zimo);
	///ƽ�����
	virtual bool CheckPingHu(BYTE lastzhuapai=255,bool zimo=false,bool jing=false,bool CheckTing=false);

	///��ȡ�����������
	virtual bool  GetMaxCombination(bool zimo,bool jing,BYTE lastpai = 255);


    bool CheckJiangDui();

	//������
	bool CheckAllJiang();

	///����Ƿ�ȫ����
	bool CheckQuanYao();

	///������Ҹ�������
	int CountGen();
	///�����߶��������Ƶ�����
	int CountLongQi();
	///�����߶��������Ƶ�����
	bool CheckHuGen(BYTE lastpai);

	//����Ƿ��۾��߶�
	bool CheckYaoJiuQiDui();



};
