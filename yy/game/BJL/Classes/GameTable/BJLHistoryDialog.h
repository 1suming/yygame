#ifndef __BJLHistoryDialog_h__
#define __BJLHistoryDialog_h__

#include "HNNetExport.h"
#include "HNUIExport.h"
#include "BJLMessageHead.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

#include <vector>

namespace BJL
{

	#define  BJL_MAX_NUM_VIEW 5				//�������ʾ������¼����  
	#define  BJL_MAX_NUM_AREA 3				//�������ʾ�м�������

	class GameHistoryDialog : public HNDialogBase
	{
	public:
		static GameHistoryDialog* create(cocos2d::Node* parent, const std::vector<LuziData>& history);
		bool init(cocos2d::Node* parent, const std::vector<LuziData>& history);
		
		GameHistoryDialog();
		virtual ~GameHistoryDialog();
	private:
		std::vector<LuziData>	_history;
		int _currentIndex;
		Vec2 _startPoint;
		ui::Layout* _widget;
		ui::ScrollView* _sv;
		ui::ImageView* _bg;
		Vector<Sprite*> spArr;
	private:
		//·�ӻص�
		void luziscrollview_scroll(cocos2d::Ref* pobj, ui::ScrollView::EventType type);
		void move_one_right();

		void move_one_left();

		void reset_pos();
	protected:

		bool onTouchBegan(Touch *touch, Event *unused_event);
		bool initUI();
	};


}

#endif // __BJLHistoryDialog_h__
