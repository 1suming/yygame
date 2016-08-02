#ifndef __HNPlatformRegist_H__
#define __HNPlatformRegist_H__

#include "HNPlatformLogic/HNPlatformLogicBase.h"

namespace HN
{
	/*
	 * ע��ӿ�
	 */
	class IHNPlatformRegist: public IHNPlatformLogicBase
	{
	public:
		virtual void onPlatformRegistCallback(bool success, bool fastRegist, const std::string& message,
                                              const std::string&name, const std::string& pwd, const std::string& agentid, int loginTimes){}
	};

	/*
	 * ע���߼�
	 */
	class HNPlatformRegist: public HNPlatformLogicBase
	{
	public:
		// ע������
        bool requestRegist(const std::string &name, const std::string pwd, const std::string& agentid, bool fastRegist);

		// ƽ̨����
		virtual void I_P_M_Connect(bool result) override;
		
		// ƽ̨ע��
		virtual void I_P_M_Regist(MSG_GP_S_Register* registerStruct, UINT ErrorCode) override;
		
		// ���캯��
		HNPlatformRegist(IHNPlatformRegist* callback);

		// ��������
		virtual ~HNPlatformRegist();
	protected:
		// �Ƿ����ע��
		bool _fastRegist;

		// ע���û���
		std::string _name;

		// ע������
		std::string _pwd;
        
        // �������˺�
        std::string _agentid;

		// �ص�
		IHNPlatformRegist* _callback;

		// ע���߼�
		void platformRegist();

		// ע�᷵����
		enum REGIST_CODE
		{
			eFAILE     = 0, // ע��ʧ��
			eSUCCESS   = 1, // ע��ɹ�
			eEXIST     = 2, // ��ͬ����
			eSENSITIVE = 3, // ���дʻ�
		};
	};
}


#endif // !__HNPlatformRegist_H__
