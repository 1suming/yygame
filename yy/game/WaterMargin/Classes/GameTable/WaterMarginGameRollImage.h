#ifndef __HN_WaterMargin__RollImage__
#define __HN_WaterMargin__RollImage__

#include "cocos2d.h"
#include "HNUIExport.h"
#include <string>

using namespace std;
using namespace HN;

USING_NS_CC;

namespace WaterMargin
{

	class RollImage : public HNSprite
	{
		char name[30];
	public:
		virtual bool init();
		static RollImage* create(string name);
		//�������
		CC_SYNTHESIZE(bool, m_run, Run);

	public:
		//ͼƬ����
		void imageRun();
		//ͼƬֹͣ���滻frame
		void imageStopAndSetFrame(string name);

		void resetReadyImage();
	};
}

#endif