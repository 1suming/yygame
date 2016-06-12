#ifndef __GoldenToad_GAMECHAT_LAYER_H__
#define __GoldenToad_GAMECHAT_LAYER_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "HNNetExport.h"
#include "HNUIExport.h"
USING_NS_CC;

using namespace cocostudio;
using namespace ui;

namespace GoldenToad
{

	class GameChatDlgLayer : public HNLayer, public ui::EditBoxDelegate
	{
	public:

		typedef std::function<void (const std::string& text)> SendSpeakeCallBack;
		SendSpeakeCallBack	onSpeakeCallBack;

		typedef std::function<void()> OnCloseCallBack;
		OnCloseCallBack	onCloseCallBack;

		struct CHAT_UI
		{
			ImageView*		ImageView_BG;

			Button*			Button_common;
			Button*			Button_face;
			Button*			Button_send;
			Button*			Button_record;
			HNEditBox*		TextField_text;
			ui::ScrollView* ScrollView_face;
			ui::ListView*	ListView_common;
			ui::ListView*	ListView_chatRecord;

			Widget*			chatUI;

		}_chatUI;

	public:
		GameChatDlgLayer();
		virtual ~GameChatDlgLayer();

	public:
		virtual bool init() override;

		//��ʾ�������������
		void showOrHideChatDlg(bool bShow);

		//��������¼
		void addChatRecord(std::string msg);

	private:
		void createCommonList(INT idx);

	private:
		//������水ť�ص�
		void chatUiButtonCallBack(Ref* pSender, Widget::TouchEventType type);
		//�������ص�
		void faceTouchEventCallBack(Ref* pSender, Widget::TouchEventType type);
		//�������б����ص�
		void commonListEventCallBack(Ref* pSender, ui::ListView::EventType type);

		virtual void editBoxReturn(ui::EditBox* editBox) {};

	public:
		CREATE_FUNC(GameChatDlgLayer);
	};

}

#endif // __GAMECHAT_LAYER_H__