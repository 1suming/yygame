#include "stdafx.h"
#include "CheckHuPaiEx.h"

#include "../server/ServerManage.h"

#include<iostream>
#include<cstring>
using namespace std;

CheckHuPaiEx::CheckHuPaiEx(void)
{
	m_byTingMaxFan = 0;//���ƿ��ܵ����
	m_iMaxFan = -1;
	///����Ƿ���������
	m_bZimo = false;
}
CheckHuPaiEx::~CheckHuPaiEx(void)
{
}

///����������ܷ�����
///@param  handpai[] ��������, pai ���һ����, gcpData[] ����������, MainJing ����, LessJing ����, CanOut[] ����������Ƶ��ƣ������ж��ţ�
///@return true ������false ������
bool CheckHuPaiEx::CanTing(BYTE station,BYTE CanOut[])
{
	if(station<0 || station>PLAY_COUNT)
	{
		return false;
	}
	
	pDesk->sUserData.m_StrJing.sort();
	m_byStation = station;
	m_byTingMaxFan  = 0;//���ƿ��ܵ����
	///����Ƿ���������
	m_bZimo = true;
	memset(CanOut,255,sizeof(CanOut));
	int num=0;
	bool canhu = false;
	for(int i=1;i<30;++i)
	{
		if(i%10 == 0)
		{
			continue;
		}
		memset(m_byArHandPai,255,sizeof(m_byArHandPai));
		for(int j=0;j<HAND_CARD_NUM;j++)
		{
			m_byArHandPai[j] = pDesk->sUserData.m_byArHandPai[station][j];
		}
		for(int j=0;j<HAND_CARD_NUM;j++)
		{
			if(m_byArHandPai[j] == 255)
			{
				m_byArHandPai[j] = i;
				break;
			}
		}
		//��С��������
		MaoPaoSort(m_byArHandPai,HAND_CARD_NUM,false);
		///������ת��
		ChangeHandPaiData(m_byArHandPai,255);
		m_byJingNum = 0;
			
		bool tmphu = false,bqidui=false;
		int tempFan = 1;
		if(CheckQiDui()>0)//�߶�
		{
			int count = CountLongQi();//����
			//if(CheckYaoJiuQiDui())//�۾��߶� 8��
			//{
			//	tempFan = 128;//�۾��߶�  8�� 
			//}
			/*else*/ 
			if (CheckQingYiSe())    //���߶�
			{
				if(count >= 1)
				{
					tempFan = 8;//�����߶� 1�� 3��
				}
				else 
				{            
					tempFan = 8; // ���߶�3�� 
				}
			}
			else
			{		
				if(count >= 1)
				{
					tempFan = 8;//���߶� 1�� 3�� 
				}
				else 
				{            
					tempFan = 4; // ���߶�4�� 
				}
			}				
			bqidui=true;
			canhu = true;
			tmphu = true;
		}
		else if(CheckPingHu(255,true,false))//ƽ����
		{
			CheckPingHuData();//�Ȼ�ȡ�����
			if (CheckQingYiSe())    //��һɫ
			{
				tempFan  = 4;//���ƿ��ܵ���� 2�� 
			}
			if (CheckQuanDaiYao())  //ȫ����   
			{
				//if(CheckQingYiSe())// ��һɫ ����� 
				//{
				//	tempFan  = 16;//����� 5�� 
				//}
				//else
				//{
					tempFan  = 4;//���ƿ��ܵ���� 2��
				//}
			}
			if(CheckPengPengHu())   //�����
			{
				if (CheckQingYiSe())    //���
				{
					tempFan  = 8;//���ƿ��ܵ���� 4�� 
				}
				//else if (CheckJiangDui())   //����
				//{
				//	tempFan  = 8;//���ƿ��ܵ���� 3�� 
				//}
				else
				{
					tempFan  = 2;//���ƿ��ܵ����1 �� 2��
				}
			}

			canhu = true;
			tmphu = true;
		}

		if(tmphu)
		{
			if(tempFan < 1)
			{
				tempFan = 1;
			}
			//��ȡ����
			int gen = 0;
			if(!bqidui)//�߶Բ����,2011-11-29
			{
				gen = CountTingGen(m_byArHandPai);
			}
			if(gen>0)
			{
				for(int j=0;j<gen;++j)
				{
					tempFan *= 2;
				}
			}
			//��¼���ֵ
			if(m_byTingMaxFan < tempFan || m_byTingMaxFan==255)
			{
				m_byTingMaxFan = tempFan;
			}
			char strdug[500];
			sprintf(strdug,"��ֲ��ԣ� λ��%d �� %d ��%d �������%d  ������%d",m_byStation,tempFan,gen,i,m_byTingMaxFan);
			OutputDebugString(strdug);
		}
	}
	if(canhu)
	{
		char strdug[500];
		sprintf(strdug,"��ֲ��ԣ����ղ��� λ��%d �� %d ",m_byStation,m_byTingMaxFan);
		OutputDebugString(strdug);
	}

	return canhu;
}

///����������Ҹ�������
int CheckHuPaiEx::CountTingGen(BYTE pai[])
{	
	int count = 0,num=0;
	BYTE data = 255,type=255;
	for(int i=0;i<9;++i)
	{
		for(int j=0;j<3;++j)
		{
			num = 0;
			data = i+1+j*10;
			num = CLogicBase::GetNumInArr(pai,data,17);
			if(pDesk->sUserData.IsUserHaveGangPai(m_byStation,data,type))
			{
				count++;
			}
			else if(pDesk->sUserData.IsUserHavePengPai(m_byStation,data))
			{
				num+=3;
			}
			else
			{
				if(pDesk->sUserData.GetGCPCount(m_byStation) >0)
				{
					num += pDesk->sUserData.GetOnePaiGCPCount(m_byStation,data);
				}
			}
			if(num>=4)
			{
				count++;
			}
		}
	}
	return count;
}

///��������ϸ������
///@param station ���Ƽ��λ�ã�hutype[] �ܺ��Ƶ����ͣ�zimo �Ƿ�����
///@return true  �ܺ��� false ����
bool CheckHuPaiEx::CanHu(BYTE station,BYTE lastpai,BYTE hutype[],bool zimo)
{
	///����Ƿ���������
	m_bZimo = zimo;
    //�����������ڵ���
    m_byLastPai = lastpai;
    m_byStation = station;
    memset(m_byArHandPai,255,sizeof(m_byArHandPai));
    for(int i=0;i<HAND_CARD_NUM;i++)
    {
        m_byArHandPai[i] = pDesk->sUserData.m_byArHandPai[station][i];
        if(pDesk->sUserData.m_byArHandPai[station][i] == 255 && lastpai != 255 && !zimo)
        {
            m_byArHandPai[i] = lastpai;
            break;
        }
    }
    ///��С��������
    MaoPaoSort(m_byArHandPai,HAND_CARD_NUM,false);
    bool bb = false;
	bool dahu = false;
    ///������ת��
    ChangeHandPaiData(m_byArHandPai,255);	
    ///ͳ�Ƹ���
    int nGengCount = 0;
	nGengCount = CountGen();
  //  if(CheckQiDui()>0)        //���߶�
  //  {
		//SetAHupaiType(HUPAI_AN_QI_DUI,hutype);//�߶�
  //      bb = true;
		//dahu = true;
  //  }
  //  else 
	if( CheckPingHu(lastpai,zimo,false))//ƽ������
    {        
		CheckPingHuData();//�Ȼ�ȡ�����
		//if (CheckQingYiSe())    //��һɫ
		//{
		//	SetAHupaiType(HUPAI_QING_YI_SE,hutype);
		//	dahu = true;
		//}
		////��һɫ
		//if(CheckHunYiSe())	
		//{
		//	SetAHupaiType(HUPAI_HUN_YI_SE,hutype);
		//	dahu = true;
		//}
		////������
		//if(CheckPengPengHu())   
		//{
		//	SetAHupaiType(HUPAI_PENG_PENG_HU,hutype);
		//	dahu = true;
		//}
		////����
		//if(CheckQingLong())
		//{
		//	SetAHupaiType(HUPAI_QING_LONG,hutype);
		//	dahu = true;
		//}

		////��һɫ 
		//if(CheckAllJiang())
		//{
		//	SetAHupaiType(HUPAI_JIANG_YI_SE,hutype);
		//	dahu = true;
		//}

        bb = true;
    }
    if (bb)
    {
		//û�д�������Ƶ���
		if(!dahu)
		{
			SetAHupaiType(HUPAI_HU_PAI,hutype);
		}
		//�ĸ����к�
		if(CheckFourZhong())
		{
			SetAHupaiType(HUPAI_TYPE_FOUR_ZHONG,hutype);
		}
		//����
		if(zimo)
		{
			SetAHupaiType(HUPAI_ZI_MO,hutype);
		}
		//���
		if(CheckTianHu())
		{
			SetAHupaiType(HUPAI_TYPE_TIAN_HU,hutype);
		}
		//�غ�
		if(CheckDiHu())
		{
			SetAHupaiType(HUPAI_TYPE_DI_HU,hutype);
		}
		// �ܿ�
        if (CheckGangKai())     
        {
            SetAHupaiType(HUPAI_TYPE_GANG_KAI, hutype);
        }
		
		// ����(�������ж���)
        /*if (CheckQiangGangHe()) 
        {
            SetAHupaiType(HUPAI_TYPE_QIANG_GANG, hutype);
        }*/
        // ���
        pDesk->sUserData.m_byGengCount[station] = nGengCount;
    }
    return bb;
}
///ƽ�����
bool CheckHuPaiEx::CheckPingHu(BYTE lastzhuapai,bool zimo,bool jing,bool CheckTing)
{
	//ѭ������
	TempHupaistruct.Init();
	hupaistruct.Init();
	m_iMaxFan = -1;
	BYTE pai[4]={255};
	bool caishen[4]={0};
	BYTE byZhongNum(0);
	for(int i = 0; i < HAND_CARD_NUM; i++)
	{
		if(m_byArHandPai[i] == 35)
			byZhongNum++;
	}
	if(byZhongNum >= 4)
		return true;

	//1������ļ��,һ�������һ��������  //û�в���Ĳ������������
	if(m_byJingNum>=1 && m_byJingNum!=255)
	{

		for(int i=0;i<NoJingHuTempData.conut;i++)
		{
			if(NoJingHuTempData.data[i][1]<1 || NoJingHuTempData.data[i][1] == 255 ||NoJingHuTempData.data[i][0]==255)
			{
				continue;
			}
			hupaistruct.Init();//��ʼ�����ƽṹ
			NoJingHuTempData.data[i][1]-=1;

			memset(caishen,0,sizeof(caishen));
			memset(pai,255,sizeof(pai));
			pai[0] = pai[1] = NoJingHuTempData.data[i][0];
			caishen[0] = true;
			//����Ƿ�258����
			/*if((pai[0] % 10 == 2) || (pai[0] % 10 == 5) || (pai[0] % 10 == 8))
			{
			hupaistruct.AddData(TYPE_JIANG_PAI,pai,caishen);
			}
			else
			{
			NoJingHuTempData.data[i][1]+=1;
			continue;
			}*/
			

			MakePingHu(NoJingHuTempData,hupaistruct,m_byJingNum-1);//���Ƽ��ݹ麯�����ڵݹ������ȫ�����
			NoJingHuTempData.data[i][1]+=1;
		}
	}
	//���������ļ��,(�޾����齫HuTempData �� NoJingHuTempData��һ��������)
	for(int i=0;i<NoJingHuTempData.conut;i++)
	{
		if(NoJingHuTempData.data[i][1]<2 || NoJingHuTempData.data[i][0]==255)
		{
			continue;
		}
		hupaistruct.Init();//��ʼ�����ƽṹ
		NoJingHuTempData.data[i][1]-=2;

		memset(caishen,0,sizeof(caishen));
		memset(pai,255,sizeof(pai));
		pai[0] = pai[1] = NoJingHuTempData.data[i][0];
		//dwjtesttest ���ｫ���ж�
		//�Ƿ�258��
		/*if((pai[0] < 30) &&((pai[0] % 10 == 2) || (pai[0] % 10 == 5) || (pai[0] % 10 == 8)))
		{
			hupaistruct.AddData(TYPE_JIANG_PAI,pai,caishen);
		}
		else
		{
			NoJingHuTempData.data[i][1]+=2;
			continue;
		}*/
	
		MakePingHu(NoJingHuTempData,hupaistruct,m_byJingNum);//���Ƽ��ݹ麯�����ڵݹ������ȫ�����
		NoJingHuTempData.data[i][1]+=2;
	}
	if(m_iMaxFan>=0)
	{
		hupaistruct = TempHupaistruct;
		return true;
	}
	return false;
}

///��ȡ�����������
bool CheckHuPaiEx:: GetMaxCombination(bool zimo,bool jing,BYTE lastpai)
{
	CheckPingHuData();//�Ȼ�ȡ�����

	ClearHupaiType(m_byHuType);//�����������
	int fan = 0;
	bool bMax =  false;//�Ƿ���ڸ��������

    if(CheckQingYiSe())    //��һɫ
    {
		fan = 4;
    }
    if(CheckQuanDaiYao())  //ȫ����  
    {
		if(CheckQingYiSe())//��һɫ
		{
			fan = 16;//����� 5 �� 16��
		}
		else
		{
			fan = 4;
		}
    }
	if(CheckPengPengHu())   //�����
	{
		if (CheckQingYiSe())    //���
		{
			fan = 8;
		}
		else if (CheckJiangDui())   //����
		{
			fan = 8;
		}
		else
		{
			fan += 2;
		}
	}
	if(fan>m_iMaxFan)
	{
		m_iMaxFan = fan;
		bMax =  true;   
	}
	if(bMax)
	{
		TempHupaistruct = hupaistruct;//��¼�������
	}
	return bMax;
}

//������
bool	CheckHuPaiEx::CheckAllJiang()
{
	for(int i=0;i<hupaistruct.count;++i)
	{
		if(hupaistruct.data[i][0]%10 != 2 && hupaistruct.data[i][0]%10 != 5 && hupaistruct.data[i][0]%10 != 8)
		{
			return false;
		}
	}
	for(int i=0;i<5;i++)
	{
		if(pDesk->sUserData.m_UserGCPData[m_byStation][i].byType == 255)
			continue;
		BYTE pai =255;
		pai = pDesk->sUserData.m_UserGCPData[m_byStation][i].byData[0];
		switch(pDesk->sUserData.m_UserGCPData[m_byStation][i].byType)
		{
		case AT_COLLECT://���ƶ���
			{
				return false;
				break;
			}
		case AT_TRIPLET:
		case AT_QUADRUPLET:
		case AT_QUADRUPLET_CONCEALED:
		case AT_QUADRUPLET_REVEALED:
			{
				if(pai %10 != 2 && pai %10 != 5 && pai %10 != 8)
				{
					return false;
				}
			}
			break;
		}
	}
	return true;
}

//��⽫�ԣ�258��������
bool CheckHuPaiEx::CheckJiangDui()
{
	for(int i=0;i<hupaistruct.count;++i)
	{
		if(hupaistruct.byType[i] == TYPE_SHUN_ZI)//˳��
		{
			return false;
		}
		if(hupaistruct.data[i][0]%10 != 2 && hupaistruct.data[i][0]%10 != 5 && hupaistruct.data[i][0]%10 != 8)
		{
			return false;
		}
	}
	for(int i=0;i<5;i++)
	{
		if(pDesk->sUserData.m_UserGCPData[m_byStation][i].byType == 255)
			continue;
		BYTE pai =255;
		pai = pDesk->sUserData.m_UserGCPData[m_byStation][i].byData[0];
		switch(pDesk->sUserData.m_UserGCPData[m_byStation][i].byType)
		{
		case ACTION_CHI://���ƶ���
			{
				return false;
			}
			break;
		case ACTION_AN_GANG:
		case ACTION_MING_GANG:
		case ACTION_BU_GANG:
		case ACTION_PENG:
			{
				if(pai %10 != 2 && pai %10 != 5 && pai %10 != 8)
				{
					return false;
				}
			}
			break;
		}
	}
    return true;
}

///����Ƿ�ȫ����
bool CheckHuPaiEx::CheckQuanYao()
{
	if(pDesk->sUserData.GetGCPCount(m_byStation) > 0)
		return false;
    for(int i=0;i<HAND_CARD_NUM;i++)
    {
		if(m_byArHandPai[i] == 255 )
			continue;
		if( m_byArHandPai[i] > CMjEnum::MJ_TYPE_B9 || (m_byArHandPai[i]%10 != 1 && m_byArHandPai[i]%10 != 9))
			return false;
    }
	return true;
}

///������Ҹ�������
int CheckHuPaiEx::CountGen()
{	
	int count = 0,num=0;
	BYTE pai = 255;
	char strdug[500];
	for(int i=0;i<9;++i)
	{
		for(int j=0;j<3;++j)
		{
			num = 0;
			pai = i+1+j*10;
			num += HuTempData.GetPaiCount(pai);
			num += pDesk->sUserData.GetOnePaiGCPCount(m_byStation,pai);
			if(num>=4)
			{
				count++;
			}
		}
	}
	return count;
}

///�����߶��������Ƶ�����
int CheckHuPaiEx::CountLongQi()
{	
	int count = 0;
	for(int i=0;i<HuTempData.conut;++i)
	{
		if(HuTempData.data[i][0] == 255 || HuTempData.data[i][1] < 4 || HuTempData.data[i][1] == 255)
			continue;
		count++;
	}
	return count;
}

///�����߶��������Ƶ�����
bool CheckHuPaiEx::CheckHuGen(BYTE lastpai)
{	
	if(lastpai == 255)
		return false;
	if(CLogicBase::GetNumInArr(m_byArHandPai,lastpai ,HAND_CARD_NUM) >= 4)
		return true;
	return false;
}

//����Ƿ��۾��߶� 8 
bool CheckHuPaiEx::CheckYaoJiuQiDui()
{
	if(CheckQiDui() <= 0)
		return false;
	for(int i=0;i<HuTempData.conut;++i)
	{
		if(HuTempData.data[i][0] == 255)
			continue;
		if(HuTempData.data[i][0] %10 != 1 && HuTempData.data[i][0] %10 != 9)
			return false;
	}
	return true;
}


