#ifndef __WaterMargin_GAME_MALI_H__
#define __WaterMargin_GAME_MALI_H__

#include "HNNetExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "HNUIExport.h"
#include "ui/UICheckBox.h"
#include <vector>

USING_NS_CC;

using namespace cocostudio;
using namespace std;
using namespace ui;
using namespace HN;


namespace WaterMargin
{
	class GameTableLogic;
	class RollImage;

	class GameMaLi : public  HNLayer
	{
	protected:
		GameMaLi();
		~GameMaLi();
	public:
		static GameMaLi* create(GameTableLogic* tableLogic, Node* node);
		//	CREATE_FUNC(GameBiBei);
		bool init(GameTableLogic* tableLogic, Node* node);

		void startPlay();						//playArmature
		void playgo();
		void stopRoll();
		void rolling(float delay);
		void createResultImage(int* str);
		//5s����ʱ
		void runTime(float delay);

		void menuClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);		//��ť�ص�����

		void createImage();

		ImageView* _MaLiBG;
	//	vector<TextAtlas*> _Bdata;
		GameTableLogic* _MaLiLogic;
		vector<ImageView*> _image;
		//4��ͼ��λ��
		vector<ImageView*> _resultpos;
		//4��ͼ��ָ��
		vector<ImageView*> _result;
		//4������
		vector<TextAtlas*> _data;
		float _time;

		//��������
		int                            _Count;
		//ͣͼ
		int                            _imageIndex;
		Node*						   _father;

		vector<RollImage*> _rollImage;					//��һ�Ź���ͼ
		vector<RollImage*> _readyImage;					//�ڶ��Ź���ͼ
		int _imageType[4];

	};
}
#endif