#ifndef __WaterMargin_Animation_H__
#define __WaterMargin_Animation_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "HNNetExport.h"

USING_NS_CC;

using namespace cocostudio;
using namespace std;
using namespace ui;
using namespace HN;

namespace WaterMargin
{
	class GameAnimation
	{
	public:
		struct AniSpirteInfo
		{
			std::string name;
			int     	count;
			float	    time;
		};

		AniSpirteInfo _aniNameFirst[11];
		AniSpirteInfo _aniNameSecond[9];
		AniSpirteInfo _aniRight[3];
		AniSpirteInfo _aniLeft[3];
		AniSpirteInfo _aniBoss[5];
	public:
		GameAnimation();
		~GameAnimation();

		//���ö���ͼƬ��Ϣ
		void onRender();
		//����֡����
		void addAniData(AniSpirteInfo ani, BYTE type, BYTE kind);//type-�ж��ǵ�һ�ֶ������ǵڶ��ֶ�����kind-�ж�������ͼƬ

		//�ȱ������Ҷ���
		Animate* createBiBeiAni(BYTE type, BYTE kind);
		};
}



#endif