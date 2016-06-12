#include "WaterMarginGameAudio.h"
#include "HNUIExport.h"

namespace WaterMargin
{

#define GAME_AUDIO_ROOT "WaterMargin/GameAudio/"

	void GameAudio::playBackground()
	{
		HNAudioEngine::getInstance()->playBackgroundMusic("WaterMargin/GameAudio/xiongdiwushu.mp3", true);
	}

	void GameAudio::playMaLiBackground()
	{
		HNAudioEngine::getInstance()->playBackgroundMusic("WaterMargin/GameAudio/1.mp3", true);
	}

	void GameAudio::stopBackground()
	{
		HNAudioEngine::getInstance()->stopBackgroundMusic();
	}

	void GameAudio::pauseBackgroundMusic()
	{
		HNAudioEngine::getInstance()->pauseBackgroundMusic();
	}

	void GameAudio::resumeBackgroundMusic()
	{
		HNAudioEngine::getInstance()->resumeBackgroundMusic();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void GameAudio::playBiBeiBg()		//�ȱ���������
	{
		HNAudioEngine::getInstance()->playBackgroundMusic("WaterMargin/GameAudio/bibeibgsound.wav", true);
	}

	void GameAudio::playDianshu(int value)
	{
		char name[50] = {0};
		sprintf(name, "waterMargin/GameAudio/%ddian.mp3", value);
		HNAudioEngine::getInstance()->playEffect(name);
	}

	void GameAudio::playGundong()
	{
		HNAudioEngine::getInstance()->playEffect("WaterMargin/GameAudio/gundong.mp3");
	}

	void GameAudio::playFirstAnimate()	//��һ������Ч
	{
		HNAudioEngine::getInstance()->playEffect("WaterMargin/GameAudio/daoguang.mp3");
	}

	void GameAudio::playSecondAnimate(int value)
	{
		char name[40] = {0};
		sprintf(name, "WaterMargin/GameAudio/sound_%d.mp3", value);
		HNAudioEngine::getInstance()->playEffect(name);
	}

	void GameAudio::playDefeng()	//�÷���Ч
	{
		HNAudioEngine::getInstance()->playEffect("WaterMargin/GameAudio/defen.wav");
	}

	void GameAudio::playShangfeng()	//�Ϸ���Ч
	{
		HNAudioEngine::getInstance()->playEffect("WaterMargin/GameAudio/shangfen.mp3");
	}

	/*void GameAudio::playGameButton()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("Audio_Button.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}*/

	void GameAudio::playYaosaizi()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("yaosaizi.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GameAudio::playBiBeiWin()	//�ȱ�Ӯ
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("ying.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GameAudio::playSHZWin()	//����Ӯ
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("winsound.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GameAudio::playLose()		//��
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("shu.mp3");
		HNAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void GameAudio::setEffectsVolume(float volume)
	{
		HNAudioEngine::getInstance()->setEffectsVolume(volume);
	}

	void GameAudio::setBackgroundMusicVolume(float volume)
	{
		HNAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
	}
}
