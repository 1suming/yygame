#include "stdafx.h"
//#include "../Client/stdafx.h"
#include "GameDataEx.h"

//ȫ�ֶ���
GameDataEx g_GameData;

GameDataEx::GameDataEx(void)
{
	GameData::GameData();
	
	LoadIni();

}

GameDataEx::~GameDataEx(void)
{
	GameData::~GameData();
}

	  
///���������¼��ĺ���¼�
void GameDataEx::SetThingNext()
{
	GameData::SetThingNext();

}

///����ĳ���¼��ĺ���¼�
void GameDataEx::SetThingNext(BYTE id)
{
	GameData::SetThingNext(id);
}

//virtual ��ʼ������
void GameDataEx::InitData()
{
    memset(m_byThingRecord,255,sizeof(m_byThingRecord));
    ///��ִ���¼�
    m_byNextAction = ACTION_NO;
	
    ///���ڷ������¼���
    m_byThingDoing = 255;  

    ///�������¼�,��ʱ����
    temp_ChiPai.Clear();
    ///�������¼�,��ʱ����
    temp_PengPai.Clear();
    ///�������¼�,��ʱ����
    temp_GangPai.Clear();

    ///��Ϸ��ʼ�¼�
    T_Begin.Clear();

    ///�Զ�Ϊׯ�¼�
    T_DongNt.Clear();

    ///��2��ɫ�ӵĵ�����Ϊׯ�¼�
    T_TwoSeziNt.Clear();

    ///��2��ɫ��ȷ��ׯ�Һ����Ƶ�λ���¼�
    T_TwoSeziNtAndGetPai.Clear();

    ///������2��ɫ��ȷ��ׯ��
    T_TurnSeziNt.Clear();

    ///��2��ɫ��ȷ������λ���¼�
    T_TwoSeziDir.Clear();

    ///��2��ɫ��ȷ������λ�ã������ͣ������Ƶ㣨��С�㣩�¼�
    T_TwoSeziDirAndGetPai.Clear();

    ///��2��ɫ��ȷ�����ƶ����¼�
    T_TwoSeziGetPai.Clear();

    ///�����¼�
    T_SendPai.Clear();

    ///�����¼�
    T_TiaoPai.Clear();

    ///��ɫ��2��ɫ�Ӷ������¼�
    T_TwoSeziJing.Clear();

    ///��ɫ��1��ɫ�Ӷ������¼�
    T_OneSeziJin.Clear();

    ///������Ҳ����¼�
    T_AllBuHua.Clear();

    ///������Ҳ����¼�
    T_OneBuHua.Clear();

    ///��ʼ����֪ͨ�¼�
    T_BeginOutPai.Clear();

    ///�����¼�
    T_OutPai.Clear();

    ///ץ�����¼�
    T_ZhuaPai.Clear();

    /////�����ܺ���֪ͨ�¼����¼�
    //T_CPGNotify[4];

    ///�������¼�
    T_ChiPai.Clear();

    ///�������¼�
    T_PengPai.Clear();



  

    ///�������¼�
    T_GangPai.Clear();

    ///�������¼�
    T_TingPai.Clear();

    ///�������¼�
    T_HuPai.Clear();

    ///����¼�
    T_CountFen.Clear();

    ///��Ϸ���������¼�
    T_EndHandle.Clear();

    ///���������������¼�
    T_UnnormalEndHandle.Clear();

	m_ZhongNiao.clear();
	m_TMaiMa.clear();
}

///��ʼ������
void GameDataEx::LoadIni()
{
	GameData::LoadIni();
}


