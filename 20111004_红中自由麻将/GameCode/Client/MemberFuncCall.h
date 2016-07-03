#pragma once
#include <map>
/**
* @brief ȡ�ó�Ա����ָ���ַ
*/
template <typename ToType, typename FromType>
void GetMemberFuncAddr_VC6(ToType& addr,FromType f)
{
    union 
    {
        FromType _f;
        ToType   _t;
    }ut;

    ut._f = f;

    addr = ut._t;
}
//�������Ա����
//callflag:��Ա��������Լ������,0--thiscall,��0--��������. (ע�⣬ thiscall ��VC��Ĭ������)��
//funcaddr:��Ա������ַ.
//This:�����ĵ�ַ.
//count:��Ա������������.
//...:��Ա�����Ĳ����б�.
DWORD CallMemberFunc(int callflag,DWORD funcaddr,void *This,int count,...)
{
    DWORD re;

    if(count>0)//�в���,������ѹ��ջ.
    {
        __asm
        {
            mov  ecx,count;//��������,ecx,ѭ��������.
            mov  edx,ecx;
            shl  edx,2;    
            add  edx,0x14;  edx = count*4+0x14;

next:	push  dword ptr[ebp+edx];
            sub   edx,0x4;
            dec   ecx  
                jnz   next;
        }
    }

    //����thisָ��.
    if(callflag==0) //__thiscall,vcĬ�ϵĳ�Ա������������.
    {
        __asm mov ecx,This;
    }
    else//__stdcall
    {
        __asm push This;
    }

    __asm//���ú���
    {
        call funcaddr;
        mov  re,eax;
    }

    return re;
}

// ��MFC��BEGIN_MESSAGE_MAPʹ�÷�����ͬ
// ����ͷ�ļ���ʹ��DECLARE_UIMESSAGE_MAP
#define DECLARE_UIMESSAGE_MAP() \
    private:\
    std::map<int, DWORD_PTR>       m_MessageMapUILButtonHandler;\
    std::map<int, DWORD_PTR>       m_MessageMapUILButtonDownHandler;\
    std::map<int, DWORD_PTR>       m_MessageMapUIAniFinish;\
    void MessageMapFunc();          
//
#define BEGIN_UIMESSAGE_MAP(CLASS_NAME)\
    void CLASS_NAME::MessageMapFunc(){
// ���������
#define ON_LBUTTONCLICK(ID, FUNCNAME) \
    GetMemberFuncAddr_VC6(m_MessageMapUILButtonHandler[ID], &FUNCNAME);
// ������º�
#define ON_LBUTTONDOWNID(ID, FUNCNAME) \
    GetMemberFuncAddr_VC6(m_MessageMapUILButtonDownHandler[ID], &FUNCNAME);
// ����������
#define ON_ANIFINISH(ID, FUNCNAME) \
    GetMemberFuncAddr_VC6(m_MessageMapUIAniFinish[ID], &FUNCNAME);
//
#define END_UIMESSAGE_MAP()\
    };
// ���⻹��Ҫ������������
// 1���ڹ��캯���е���CALL_UIMESSAGE_MAP()
// 2����OnUIMessage�е��� CALL_UIMESSAGE(pMessage)
#define CALL_UIMESSAGE_MAP()\
    MessageMapFunc();
#define CALL_UIMESSAGE(pMessage)\
    if(pMessage->_uMessageType==UI_LBUTTONUP)\
{\
    if (m_MessageMapUILButtonHandler[pMessage->_uControlID]!=0)\
    CallMemberFunc(0, m_MessageMapUILButtonHandler[pMessage->_uControlID], this, 1, pMessage);\
}\
    else if(pMessage->_uMessageType==UI_CATOONFINISH)\
{\
    if (m_MessageMapUIAniFinish[pMessage->_uControlID]!=0)\
    CallMemberFunc(0, m_MessageMapUIAniFinish[pMessage->_uControlID], this, 1, pMessage);\
}\
    else if (pMessage->_uMessageType==UI_LBUTTONDOWN)\
{\
    if (m_MessageMapUILButtonDownHandler[pMessage->_uControlID]!=0)\
    CallMemberFunc(0, m_MessageMapUILButtonDownHandler[pMessage->_uControlID], this, 1, pMessage);\
}

////////////////////////////////////////////////////////////////////////////////
// SOCKET��Ϣ����
#define DECLARE_SOCKETMESSAGE_MAP() \
    private:\
    std::map<int, DWORD_PTR>       m_MessageMapSocketHandler;\
    void SocketMessageMapFunc();          
//
#define BEGIN_SOCKETMESSAGE_MAP(CLASS_NAME)\
    void CLASS_NAME::SocketMessageMapFunc(){
// ���������
#define ON_SOCKETRECV(nSubId, FUNCNAME) \
    GetMemberFuncAddr_VC6(m_MessageMapSocketHandler[nSubId], &FUNCNAME);
//
#define END_SOCKETMESSAGE_MAP()\
    };
// ���⻹��Ҫ������������
// 1���ڹ��캯���е���CALL_SOCKETMESSAGE_MAP()
// 2����HandleGameMessage�е��� CALL_SOCKETMESSAGE(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen)
#define CALL_SOCKETMESSAGE_MAP()\
    SocketMessageMapFunc();
#define CALL_SOCKETMESSAGE(nMainID,nSubId,buffer,nLen)\
    if (m_MessageMapSocketHandler[nSubId]!=0)\
    CallMemberFunc(0, m_MessageMapSocketHandler[nSubId], this, 2, buffer,nLen); 


