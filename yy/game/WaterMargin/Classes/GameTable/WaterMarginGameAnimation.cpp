#include "WaterMarginGameAnimation.h"
#include <string>

namespace WaterMargin
{
	GameAnimation::GameAnimation()
	{
		memset(_aniNameFirst, NULL, sizeof(_aniNameFirst));
		memset(_aniNameSecond, NULL, sizeof(_aniNameSecond));
		memset(_aniRight, NULL, sizeof(_aniRight));
		memset(_aniLeft, NULL, sizeof(_aniLeft));
		memset(_aniBoss, NULL, sizeof(_aniBoss));
	}
	GameAnimation::~GameAnimation()
	{

	}

	//���ö���ͼƬ��Ϣ
	void GameAnimation::onRender()
	{
		_aniNameSecond[0].name = "SH_2_";					//ˮ�1
		_aniNameSecond[0].count = 54;
		_aniNameSecond[0].time = 0.05f;

		_aniNameSecond[1].name = "ZYT_2_";					//������1
		_aniNameSecond[1].count = 49;
		_aniNameSecond[1].time = 0.05f;

		_aniNameSecond[2].name = "TT_2_";					//��������1
		_aniNameSecond[2].count = 47;
		_aniNameSecond[2].time = 0.057f;

		_aniNameSecond[3].name = "S_2_";					//��1
		_aniNameSecond[3].count = 35;
		_aniNameSecond[3].time = 0.077f;

		_aniNameSecond[4].name = "LC_2_";					//�ֳ�1
		_aniNameSecond[4].count = 35;
		_aniNameSecond[4].time = 0.077f;

		_aniNameSecond[5].name = "LZS_2_";					//³����1
		_aniNameSecond[5].count = 54;
		_aniNameSecond[5].time = 0.05f;

		_aniNameSecond[6].name = "D_2_";					//��1
		_aniNameSecond[6].count = 30;
		_aniNameSecond[6].time = 0.09f;

		_aniNameSecond[7].name = "M_2_";					//ì1
		_aniNameSecond[7].count = 44;
		_aniNameSecond[7].time = 0.061f;

		_aniNameSecond[8].name = "F_2_";					//��1
		_aniNameSecond[8].count = 42;
		_aniNameSecond[8].time = 0.064f;

		_aniNameFirst[0].name = "SH_1_";					//ˮ�2
		_aniNameFirst[0].count = 12;
		_aniNameFirst[0].time = 0.067f;

		_aniNameFirst[1].name = "ZYT_1_";					//������2
		_aniNameFirst[1].count = 12;
		_aniNameFirst[1].time = 0.067f;

		_aniNameFirst[2].name = "TT_1_";					//��������2
		_aniNameFirst[2].count = 12;
		_aniNameFirst[2].time = 0.067f;

		_aniNameFirst[3].name = "S_1_";						//��2
		_aniNameFirst[3].count = 12;
		_aniNameFirst[3].time = 0.067f;

		_aniNameFirst[4].name = "LC_1_";					//�ֳ�2
		_aniNameFirst[4].count = 12;
		_aniNameFirst[4].time = 0.067f;

		_aniNameFirst[5].name = "LZS_1_";					//³����2
		_aniNameFirst[5].count = 12;
		_aniNameFirst[5].time = 0.067f;

		_aniNameFirst[6].name = "D_1_";						//��2
		_aniNameFirst[6].count = 12;
		_aniNameFirst[6].time = 0.067f;

		_aniNameFirst[7].name = "M_1_";						//ì2
		_aniNameFirst[7].count = 12;
		_aniNameFirst[7].time = 0.067f;

		_aniNameFirst[8].name = "F_1_";						//��2
		_aniNameFirst[8].count = 11;
		_aniNameFirst[8].time = 0.072f;

		_aniNameFirst[9].name = "yaoqi_";					 //ҡ��
		_aniNameFirst[9].count = 42;
		_aniNameFirst[9].time = 0.064f;

		_aniNameFirst[10].name = "frame_";					 //��
		_aniNameFirst[10].count = 5;
		_aniNameFirst[10].time = 0.54f;

		_aniRight[0].name = "Rightlost_";                    //����
		_aniRight[0].count = 25;
		_aniRight[0].time = 0.15f;

		_aniRight[1].name = "Rightwait_";                    //�ҵȴ�
		_aniRight[1].count = 31;
		_aniRight[1].time = 0.15f;

		_aniRight[2].name = "Rightwin_";                     //��Ӯ
		_aniRight[2].count = 18;
		_aniRight[2].time = 0.15f;

		_aniLeft[0].name = "Leftlost_";                      //����
		_aniLeft[0].count = 30; 
		_aniLeft[0].time = 0.15f;

		_aniLeft[1].name = "Leftwait_";                      //��ȴ�
		_aniLeft[1].count = 27;
		_aniLeft[1].time = 0.15f;

		_aniLeft[2].name = "Leftwin_";                       //��Ӯ
		_aniLeft[2].count = 30;
		_aniLeft[2].time = 0.15f;

		_aniBoss[0].name = "Bosslost_";                      //boss��
		_aniBoss[0].count = 25;
		_aniBoss[0].time = 0.15f;

		_aniBoss[1].name = "Bossopen_";                      //boss����
		_aniBoss[1].count = 14;
		_aniBoss[1].time = 0.15f;

		_aniBoss[2].name = "Bossthrow_";                     //bossҡɫ��
		_aniBoss[2].count = 25;
		_aniBoss[2].time = 0.15f;

		_aniBoss[3].name = "Bosswait_";                      //boss�ȴ�
		_aniBoss[3].count = 8;
		_aniBoss[3].time = 0.15f;

		_aniBoss[4].name = "BossWin_";                       //bossӮ
		_aniBoss[4].count = 15;
		_aniBoss[4].time = 0.15f;
	}

	//��֡�����ӵ������
	void GameAnimation::addAniData(AniSpirteInfo ani, BYTE type, BYTE kind)
	{
		char str[32];
		//��������
		Animation *bMove = Animation::create();
		bMove->setDelayPerUnit(ani.time);
		for (int i = 1; i <= ani.count; i++)
		{
			std::string name(ani.name);
			sprintf(str, "%d.png", i);
			name.append(str);
			bMove->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
		}
		
		sprintf(str, "ani_%d_%d", type, kind);//1-��һ������2-�ڶ�������3-�ȱ��ұ��˵Ķ�����4-����˵Ķ�����5-boss����
		AnimationCache::getInstance()->addAnimation(bMove, str);
	}

	Animate* GameAnimation::createBiBeiAni(BYTE type, BYTE kind)
	{
		char name[30] = { 0 };
		sprintf(name, "ani_%d_%d", type, kind);
		auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(name));
		auto animate = Animate::create(animation);
		return animate;
	}
}