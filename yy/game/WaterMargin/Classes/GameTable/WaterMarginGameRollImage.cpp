#include "WaterMarginGameRollImage.h"
#include "HNNetExport.h"

namespace WaterMargin
{

	bool RollImage::init()
	{
		if (!HNSprite::init()) return false;
		return true;
	}
	RollImage* RollImage::create(string name)
	{
		//���ع���ͼƬ
		auto cache = SpriteFrameCache::getInstance();
		auto image = new RollImage();
		if (image && image->init())
		{
			image->autorelease();
			image->m_run = true;
			image->initWithSpriteFrame(cache->getSpriteFrameByName(name));
			return image;
		}
		delete image;
		image = NULL;
		return image;
	}

	//ͼƬ����
	void RollImage::imageRun()
	{
		auto cache = SpriteFrameCache::getInstance();
		this->setPositionY(this->getPositionY()-20);//ÿ֡ͼƬ�߶ȼ�20
		if (this->getPositionY() < -60)
		{
			INT num = rand() % 9 + 1;
			sprintf(name, "image%d.png", num);
			//��ͼƬ��ȫ�������ʾ�������������һ��frame������λ���ƶ�����ʾ�����·�
			this->setSpriteFrame(cache->getSpriteFrameByName(name));
			this->setPositionY(180);
		}
	}

	//ͼƬֹͣ���滻frame
	void RollImage::imageStopAndSetFrame(string name)
	{
		auto cache = SpriteFrameCache::getInstance();
		this->setSpriteFrame(cache->getSpriteFrameByName(name));
		this->stopAllActions();
		this->setPositionY(80);
		this->runAction(MoveBy::create(0.03f, Vec2(0, -20)));
	}
	
	//��λ�ڶ���ͼƬ
	void RollImage::resetReadyImage()
	{
		this->stopAllActions();
		this->setPositionY(180);
		//this->setVisible(false);
	}

}