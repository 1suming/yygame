
#pragma once
#include <string>
namespace DriftingCar{

	class GameAudio
	{
	public:
		static void PlayBackground();
		static void pauseBackground();
		static void resumeBackground();
		static void StopBackground();


		static void StartPaoMaDeng();		//��ʼ�ܳ�����

		static void PlayBetChouma();		//��ע����

		static void BetTishi();				//��ע��ʾ

		static void StopCarAnimation();		//ֹͣ�ܳ�����

		static void TimeWarnining();		//����ʱ����

	private:
		static void GameAudioPlaySound(const std::string& str);

	};


}