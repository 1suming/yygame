#include "AppDelegate.h"

#include "HNLobbyExport.h"

#include "json/rapidjson.h"
#include "json/document.h"

/*******************************�����Ϸ****************************************/

// ������
#include "LandlordMessageHead.h"
#include "LandlordGameTableUI.h"
    

// ��󸲶��
#include "GoldenToadMessageHead.h"
#include "GoldenToadGameTableUI.h"

// �����˿�
#include "DZPokerUpgradeMessage.h"
#include "DZPokerGameTableUI.h"

// ��͸ը��
#include "GoldenFlowerMessageHead.h"
#include "GoldenFlowerGameTableUI.h"

// ������
#include "TRACTORMessageHead.h"
#include "TRACTORGameTableUI.h"


// ������ʮ��
#include "BJLMessageHead.h"
#include "BJLGameTableUI.h"


// ����Ʈ��
#include "DriftingCarMessageHead.h"
#include "DriftingCarGameTableUI.h"

// ˮ䰴�
#include "WaterMarginMessageHead.h"
#include "WaterMarginGameTableUI.h"

// �����齫
#include "ZYHZ_MessageHead.h"
#include "ZYHZ_GameTableUI.h"

// Ѫս�齫
#include "XZMJ_MessageHead.h"
#include "XZMJ_GameTableUI.h"
/******************************************************************************/

USING_NS_CC;

// ��Ϸ��Ƴߴ�
static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);

// ������Ȩ��
static std::string APP_INFO_KEY("mixproject");

// ��Ϸѡ��
struct GameItem
{
	UINT nameId;
	HNGameCreator::GAMETYPE type;
	GAME_CREATE_SELECTOR create;
};

/*******************************�����Ϸ****************************************/
static GameItem GameList[] = 
{
	// ������ʮ��
	{ BJL::GAME_NAME_ID, HNGameCreator::NORMAL, BJL::GameTableUI::create },

	// ������
	{ landlord::GAME_NAME_ID, HNGameCreator::NORMAL, landlord::GameTableUI::create },

	// ��󸲶��
	{ GoldenToad::GAME_NAME_ID, HNGameCreator::NORMAL, GoldenToad::GameTableUI::create },

	// �����˿�
	{ DZPoker::GAME_NAME_ID, HNGameCreator::NORMAL, DZPoker::GameTableUI::create },

	// ��͸ը��
	{ goldenflower::GAME_NAME_ID, HNGameCreator::NORMAL, goldenflower::GameTableUI::create },

	// Ѫս�齫
	{ XZMJ::NAME_ID, HNGameCreator::NORMAL, XZMJ::GameTableUI::create },

	// ���ɺ���
	{ ZYHZ::NAME_ID, HNGameCreator::NORMAL, ZYHZ::GameTableUI::create },

	// ������
	{ TRACTOR::GAME_NAME_ID, HNGameCreator::NORMAL, TRACTOR::GameTableUI::create },

	// ����Ʈ��
	{ DriftingCar::NAME_ID, HNGameCreator::NORMAL, DriftingCar::GameTableUI::create },

	// ˮ䰴�
	{ WaterMargin::NAME_ID, HNGameCreator::NORMAL, WaterMargin::GameTableUI::create },

	
};
/******************************************************************************/

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		Size size = getConfigSize();
		glview = GLViewImpl::createWithRect("Ley", Rect(0, 0, size.width, size.height));
#else
		glview = GLViewImpl::create("Ley");
#endif
		director->setOpenGLView(glview);
	}

	// turn on display FPS
	//director->setDisplayStats(true);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0f / 60);

	// Set the design resolution
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);
	Size frameSize = glview->getFrameSize();

	register_all_packages();

	initGameConfig();

	//auto scene = GameInitial::createScene();
	//director->runWithScene(scene);
	GameMenu::createMenu();

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	//Director::getInstance()->pause();
	Director::getInstance()->stopAnimation();
	HNAudioEngine::getInstance()->pauseBackgroundMusic();

	auto userDefault = UserDefault::getInstance();
	userDefault->setBoolForKey("bEnterBackground", true);
	userDefault->flush();

	// if you use SimpleAudioEngine, it must be pause
	//SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	//Director::getInstance()->resume();
	Director::getInstance()->startAnimation();
	HNAudioEngine::getInstance()->resumeBackgroundMusic();

	auto userDefault = UserDefault::getInstance();
	userDefault->setBoolForKey("bEnterBackground", false);
	userDefault->flush();

	// if you use SimpleAudioEngine, it must resume here
	//SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

Size AppDelegate::getConfigSize()
{
	Size size = designResolutionSize;
	do
	{
		std::string filename("config.json");
		if (FileUtils::getInstance()->isFileExist(filename))
		{
			std::string json = FileUtils::getInstance()->getStringFromFile(filename);
			rapidjson::Document doc;
			doc.Parse<rapidjson::kParseDefaultFlags>(json.c_str());
			if (doc.HasParseError() || !doc.IsObject())
			{
				break;
			}

			if (doc.HasMember("width"))
			{
				size.width = doc["width"].GetInt();
			}
			if (doc.HasMember("height"))
			{
				size.height = doc["height"].GetInt();
			}
		}
	} while (0);

	return size;
}

void AppDelegate::initGameConfig()
{
	FileUtils::getInstance()->addSearchPath("Games");
	FileUtils::getInstance()->setXXTeaKey(getXXTEA_KEY());
	PlatformConfig::getInstance()->setGameLogo("game_logo.png");

	// �����Ϸ
	int gameCount = sizeof(GameList) / sizeof(GameItem);
	for (int i = 0; i < gameCount; i++)
	{
		HNGameCreator::getInstance()->addGame(GameList[i].nameId, GameList[i].type, GameList[i].create);
	}

	// ������Ϸ��Ȩ��
	PlatformConfig::getInstance()->setAppKey(APP_INFO_KEY);

	// ƽ̨��Ƴߴ�
	PlatformConfig::getInstance()->setPlatformDesignSize(designResolutionSize);

	// ��Ϸ��Ƴߴ�
	PlatformConfig::getInstance()->setGameDesignSize(designResolutionSize);

	// ��ȡ��Ʒ�б�
	ProductManger::getInstance()->addProducts("mixproject");

}