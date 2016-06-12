#ifndef __HN_WaterMargin_GameTableUI_H__
#define __HN_WaterMargin_GameTableUI_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "ui/UICheckBox.h"
#include <vector>
#include "WaterMarginGameTableLogic.h"
#include "WaterMarginGameTableUICallback.h"
#include "WaterMarginMessageHead.h"
#include "HNUIExport.h"
#include "WaterMarginGameAnimation.h"


#define Tag_UP				25
#define Tag_DOWN			26
#define Tag_AUTO			27
#define Tag_WIN				32
#define Tag_ADD				28
#define Tag_BIBEI			31
#define Tag_START			29
#define Tag_ALLSTOP			30
#define Tag_HANDLE			33
#define Tag_QUIT		   195

USING_NS_CC;

using namespace cocostudio;
using namespace std;
using namespace ui;
using namespace HN;

namespace WaterMargin
{
	class GameRule;
	class GameNotice;
	class GameTableLogic;
	class RollImage;
	class GameBiBei;
	class GameMaLi;
	class BDBtn;

	class GameTableUI : public HNGameUIBase, public GameTableUICallback
	{
	public:
		GameTableUI();
		virtual ~GameTableUI();

		static GameTableUI* create(BYTE bDeskIndex, bool bAutoCreate);

		virtual bool init(BYTE bDeskIndex, bool bAutoCreate);
	
		virtual void leaveDesk() override;

		virtual void setMoney(LLONG money) override;

		virtual void setDataText(int value, int num) override;

		virtual void setImageData(int* str) override;

		virtual void setBtnVisible(int Tag, bool visible) override;

		virtual void setWinScore(int value) override;

		virtual void getWinScore() override;
		

		virtual void setSaiZi(int s1, int s2) override;

		virtual void playSecondAnimate(int value, int dianShu) override;

		virtual void handleSecondDeFeng() override;

		virtual void BiBeiHistory(int* history) override;

		virtual void BiBeiAgain() override;

		virtual void MaLiResult(bool begin, int MaLicount, int Index, int* str) override;

		virtual void viewWinGold() override;

		void hideWinGold();

		virtual void MaLiEndBB() override;

		virtual void MaLIEndDF() override;

		virtual void JudgeMaLi() override;

		virtual void JudgeConnect() override;

		void MaLiGetScore(float delay);

		void secondResult();

		void secondDeFeng(float delay);

		void EnterMaLi();

		void createMaLi();

		void initData();

		void createImage();						//�������Ź���ͼ

		void startRoll();						//��ʼ����
		void stopRoll();						//ֹͣ����
		
		void Rolling(float delay);				//����ʱ�����
		void queueStop(float delay);			//ֹͣʱ�����
		void JudgeRule(float delay);						//�ж��Ƿ��н�
		void useRule();							//����GameRule�ӿ�
		void lineToImageFunction();				//���߼��Ŷ�Ӧ����ģ��
		void playFirstAnimate(int value);
		void playSecondAnimate();
		void changeLayer();						//���л�
		void FirstResult();						//��һ��Ľ���
		
		void changeScore(float delay);
		void setWinData();

		void BibeiChangeDefeng(float delay);	//�ȱ����÷��л�
		void changeOver();
		void changeBegin();
		
		void SetLayer();						//roll->bibei
		
		void btnRule();							//��ť��Ҫ���ϵ�һЩ����

		void menuClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);		//��ť�ص�����

		void headCallBack(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);

		void loading();
		void addLoadding(float delay);
		void setNotic(float delay);

		void loadingCallback(Texture2D* textureData, std::string plist);
		void loadingAni();
		void gameINIT();

		void asyncLoading();
		void loadSrcUpdate(float delta);
		void StartBiBei();

		//�����Զ��ֶ���ť��״̬
		void controlBtnVisible(bool visible);

		void loadMaLiBtnPanel();
		void showMaLiBDbtn();
		void maLiClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);

	public:
		GameTableLogic*		_tableLogic;									

		GameRule*			_gameRule;
		vector<TextAtlas*>	_dataText;						//�����ʾ����
		ImageView*			_tableBG;						//��һ�㱳��
		Widget*				_btnWidget;
		vector<Button*>		_btn;							//9����ť
		vector<RollImage*>	_rollImage;						//��һ�Ź���ͼ
		vector<RollImage*>	_readyImage;					//�ڶ��Ź���ͼ
		vector<RollImage*>	_blackImage;					//�ڰ�ͼƬ
		vector<ImageView*>	_Frame;							//ִ�е�һ����ʱ�Ŀ��
		int					_imageType[15];					//Ҫ��ʾ��ͼƬ�������
		int					_lineToImage[9][5];				//���߼��Ŷ�Ӧ����ģ��
		int					_line;							//�ڼ���-1
		vector<int>			_playAnimate;					//Ҫ���ŵڶ��ζ�����λ��
		int					_winScore;						//���
		bool				_stop;							//���ȫͣ��ֵ��true


		vector<Sprite*>		_history;						//�ȱ������ʷ
		ImageView*			_secondBG;
		GameBiBei*			_BiBei;
		GameMaLi*			_MaLi;
		CMD_S_MaLi			_MaLiData;
		//BDBtn*				_bdBtn;
		LLONG				_userMoney;
		ImageView*			_menuBg;
		LoadingBar*			_loading;
		ImageView*			_notic;
		cocos2d::Node*		_loadingLayer;

		BYTE				_desk;
		bool				_bool;

		int					_addResources;
		Sprite*				_leftDoor;
		Sprite*				_rightDoor;
		int					_LayerIndex;
		int					_scoreChange;					//Ӯ�Ľ�ҵ�ʮ��֮һ
		bool				_isReply;						//�Ƿ�ظ��������
		int					_connetIndex;					//15������˳�

		bool                _isFirst;//�ж��Ƿ��ǵ�һ��ת���������Զ��ֶ���ťʱ�õ�
		Layout*		_btnPanel;

		GameAnimation* _gameAni;
	};
}


#endif // __HN_WaterMargin_GameTableUI_H__
