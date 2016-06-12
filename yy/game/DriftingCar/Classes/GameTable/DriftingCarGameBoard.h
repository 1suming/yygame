#ifndef __DriftingCarGameBoard_H__
#define __DriftingCarGameBoard_H__

/*
 *	����ư���
 */

#include "HNUIExport.h"
#include "HNNetExport.h"
#include <array>
#include "DriftingCarMessageHead.h"

namespace DriftingCar
{
	class GameCell;

	class GameBoard:public HN::HNLayer
	{
	public:

		typedef std::function<void (cocos2d::Node*)> PlayCallBack;//�ܳ�����ʱ���ص�����

		typedef std::function<void (int)>CallCarAnimation;

		static GameBoard* create(Node* pNode);

		virtual bool init(Node* pNode);

		void CarPlay(float);

		void startPlay( int endIndex, BYTE btime);			//���Ŷ���

		void HideCarAndAllCell();							//�����ܳ��������ܵ�

		void ShowCarBlinkLogos(int index);					//ָ���������ܵ���˸

		void showCarStart();								//���ƶ���ָ����ʼλ��

		PlayCallBack CallBackPlayAnimation;					//����������Ļص�����

		CallCarAnimation CallBackCarIndex;					//�ܳ�����ʱ�����ص�����
		//
	protected:
		static float speed_level1;
		static float speed_level2;
		static float speed_level3;
		enum 
		{
			LEVEL_COUNT = 3,
			CAR_LOGO_TYPE = 8,//����
		};
		GameBoard();
		~GameBoard();
		void loadCells(Node* layout);
		void switchCell(float delta);
		void InitCarAnimation();			//��ʼ��������

		void showAction(int index, float fduration);
		void HideAction(int index);

		
		
		void InitCarCoord();					//������������ϵ
		void SetCarStartPosition();				//����car�ص���ʼλ��
		
		void StartPlayOther(int endIndex, int btime);					//����һ���ܳ�����ʵ�ַ�ʽ

	private:
		std::vector<GameCell*> _cells;		

		std::array<Vec2,CAR_LOGO_COUNT> _Vec2Car;

		Vector<FiniteTimeAction*> _vtAction;

		Sprite* _spriteCar;						//������

		int _startIndex;
		int _endIndex;
		int _currentIndex;
		float _totalTime;
		float _runTime;
		int _level1Count;
		int _level2Count;
		int _level3Count;
		float _levelTime[LEVEL_COUNT];//20%,70%,10%

		float _jiashudu;				//���ٶ�
		int _iRoundcount;				//�ƶ�����
		int _iSumCount;					//�ܲ���
	};
}

#endif // __GuessCarGameBoard_H__
