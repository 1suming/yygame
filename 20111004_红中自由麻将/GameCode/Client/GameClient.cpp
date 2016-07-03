#include "Stdafx.h"
#include <afxdllx.h>
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE GameClientDLL = { NULL, NULL };

//DLL ��ں���
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(GameClientDLL, hInstance)) return 0;
		new CDynLinkLibrary(GameClientDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(GameClientDLL);
	}
	return 1;
}

/********************************************************************************************/
//�ȼ��жϺ���
extern "C" __declspec(dllexport) TCHAR * GetGameOrderName(__int64 dwPoint)
{
	static __int64 uBasePoint[13]=
	{
		0,1000000,10000000,50000000,
		100000000,200000000,500000000,
		1000000000,2000000000,5000000000,10000000000,50000000000,100000000000
	};
	static TCHAR * szOrderName[13]=
	{
		TEXT("����"),TEXT("�ִ�"),TEXT("����"),TEXT("���"),
		TEXT("���Դ�"),TEXT("��ɣ"),TEXT("����"),TEXT("����"),TEXT("�µ�"),
		TEXT("����"),TEXT("����"),TEXT("����"),TEXT("��˹��˹")
	};

	for (int i = 1; i < 13; i++)	
	{
		if (dwPoint < uBasePoint[i]) 
		{
			return szOrderName[i-1];
		}
	}

	return TEXT("��˹��˹");
}
/********************************************************************************************/

//������Ϸ�ͻ���
//�ͻ�����������
GETCLIENTINFO;
GETGAMEICON(NAME_ID);
//GETFRAMEINTERFACE(CClientGameDlg)
GETORDERFUNCTION(GetGameOrderName);

/********************************************************************************************/
