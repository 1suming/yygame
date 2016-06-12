#ifndef PlatformConfig_h__
#define PlatformConfig_h__

#include "HNNetExport.h"
#include "cocos2d.h"
#include <string>

USING_NS_CC;

class PlatformConfig
{
public:
	enum ACCOUNT_TYPE
	{
		UNKNOWN = 0,
		GUEST,
		NORMAL
	};
public:
	// ��ȡ���õ���
	static PlatformConfig* getInstance();

	// �������õ���
	static void destroyInstance();

	// ��ȡ���¼���ַ
	std::string getAppInfoUrl();

	// ��ȡ�ƹ�Ա��ַ
	std::string getPromoterUrl();

	// ��ȡiOS���߰�װ��ַ
	std::string getOnlineInstallUrl_iOS(const std::string& url);

	// ��ȡƻ���ڹ���̨��֤��ַ
	std::string getPayCallbackUrl_iOS();

	// ��ȡ�����ַ
	std::string getNoticeUrl();

	// ��ȡ��Ʒ�б��ַ
	std::string getGoodsListUrl(const std::string& platForm_Type);

	// ��ȡ������Ϣ�ӿڵ�ַ
	std::string getOrderUrl();

	// ��ȡ�һ���Ʒ��ַ
	std::string getPrizeUrl();

	// ��ȡ�����ַ
	std::string getShareUrl(int id);

	// ��ȡ�û�Э���ַ
	std::string getProtocolUrl();

	// ��ȡ�ͷ���Ϣ
	std::string getEditUrl();

	// ��ȡ֧���ص���ַ
	//std::string getPayCallbackUrl_iOS();

	// APP��Ȩ��
	CC_SYNTHESIZE_PASS_BY_REF(std::string, _appKey, AppKey);

	// APP����id
	CC_SYNTHESIZE_PASS_BY_REF(int, _appId, AppId);

	// ��Ϸlogo
	CC_SYNTHESIZE_PASS_BY_REF(std::string, _gameLogo, GameLogo);

	// ��ϷlogoBg
	CC_SYNTHESIZE_PASS_BY_REF(std::string, _gameLogoBg, GameLogoBg);

	// ��ϷlogoTitle
	CC_SYNTHESIZE_PASS_BY_REF(std::string, _gameLogoTitle, GameLogoTitle);

	// ���̶���ͼƬ
	CC_SYNTHESIZE_PASS_BY_REF(std::string, _splashLogo, SplashLogo);

	// ��Ϸ��Ƴߴ�
	CC_SYNTHESIZE_PASS_BY_REF(cocos2d::Size, _gameDesignSize, GameDesignSize);

	// ƽ̨��Ƴߴ�
	CC_SYNTHESIZE_PASS_BY_REF(cocos2d::Size, _platformDesignSize, PlatformDesignSize);

	// ��ȡ��½������ͣ������Ļ����οͣ�
	CC_SYNTHESIZE_PASS_BY_REF(ACCOUNT_TYPE, _accountType, AccountType);

	// plist���ص�ַ
	CC_SYNTHESIZE_PASS_BY_REF(std::string, _downloadPlistUrl, DownloadPlistUrl);

	// �Ƿ�IAP֧��(ƻ��ƽ̨����Ч��
	bool isIAP() const { return _isIAP; }

protected:
	// ����Http����·��
	std::string buildHttp(const std::string& url, const std::string& path);

	// ����Https����·��
	std::string buildHttps(const std::string& url, const std::string& path);

	// ���캯��
	PlatformConfig();

	// ��������
	~PlatformConfig();

	// �Ƿ�IAP֧��
	bool _isIAP;
};

#endif // PlatformConfig_h__
