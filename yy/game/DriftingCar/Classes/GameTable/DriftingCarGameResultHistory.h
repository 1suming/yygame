#ifndef __DriftingCarGameResultHistory_H__
#define __DriftingCarGameResultHistory_H__

#include "HNUIExport.h"
#include "HNNetExport.h"
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include <array>
namespace DriftingCar
{
	class GameResultHistory: public HN::HNLayer
	{
	public:
		static GameResultHistory* create();
		bool init();

		void LoadLayout(Node* playout);

		void addRecord(const std::vector<BYTE>& records, BYTE byResult);

		void ShowRecord();									//��ʾ��¼
		void ShowLastRecord();								//��ʾ���һҳ����
		void ShowProbability();								//��ʾͳ�Ƹ���

		void ClickLeft();									
		void ClickRight();

	private:

		void ButtonLeft(Ref* pRef,Widget::TouchEventType TouchType);
		void ButtonRight(Ref* pRef, Widget::TouchEventType TouchType);

		void CheckRecordIndex();

		int GetMinIndex();
		int GetMaxIndex();

	protected:
		enum 
		{
			MAX_COUNT = 9,
			AreaCount = 8,
		};
		GameResultHistory();
		virtual ~GameResultHistory();
	private:


		std::array<Text*, AreaCount> _arrayText;				//�����ı�

		std::array<ImageView*, MAX_COUNT> _arrayBigLogos;		//��ͼ��
		std::array<ImageView*, MAX_COUNT> _arraySmallLogos;		//Сͼ��
		std::array<ImageView*, MAX_COUNT> _arrayResult;			//�������

		Button* _buttonLeft;									//��߰�ť
		Button* _buttonRight;									//�ұ߰�ť

		std::vector<BYTE> vtByhistory;							//��ʷ��¼��Ϣ
		std::deque<BYTE> dequeReuslt;
		int _historyIndex;										//·��ͼ���±�����
	};
}

#endif // __GuessCarGameResultHistory_H__
