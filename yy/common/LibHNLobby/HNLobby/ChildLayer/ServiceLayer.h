#ifndef ServiceLayer_h__
#define ServiceLayer_h__

#include "HNUIExport.h"
#include "HNNetExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class ServiceLayer : public HNLayer, public HNHttpDelegate, public ui::EditBoxDelegate
{
	struct SHANGBIDATA_UI
	{
		ImageView* img_BG;
		Text* Txt_Lotteries;
		Text* Txt_BiLi;
		Text* Txt_Info;
		HNEditBox* edit_Num;
		Button * Button_ShangBiOK;
	}_ChangeDataUi;


public:
	// ��ʾ
	void showService(Node* parent, Vec2 vec, int zorder, int tag);

	//��ʾ�һ�ҳ��
	//bShangbi--Ĭ����ʾ�бҶһ�����
	void	ShowChangeView();

	// ����
	void closeService();

	// ��ʼ��
	virtual bool init() override; 

	// ����λ��
	void setBGPositon(Vec2 vec);

	// ����
	CREATE_FUNC(ServiceLayer);

protected:
	// ��ť�ص�
	void onServiceClick(Ref* pRef);
	//�һ��ص�
	void onChangeClick(Ref* pRef);
	// �ͷ���Ϣ�ص�
	virtual void onHttpResponse(const std::string& requestName, bool isSucceed, const std::string &responseData) override;

private:

	// ���в���������
	bool ExchangeEventSelector(HNSocketMessage* socketMessage);


	// ���캯��
	ServiceLayer();

	// ��������
	virtual ~ServiceLayer();

	// ��ȡ�ͷ���Ϣ
	void requestServiceInfo(int iNum);

private:
	// �����ļ�
	Node*           _csNode;
	
	// �ͷ��绰
	std::string		_phone;

	// �ͷ���Ϣ
	std::string		_message;

	// �ͷ�����
	std::string		_email;

	// �����С
	Size            _winSize;

	// ��ť
	Button*         _buttonClose;
	Button*         _buttonShangBi;
	Button*         _buttonJiFen;
	//�һ�����  0-�һ��б�  1-�һ�����
	BYTE				_byChangeType;

	INT				_iIntegralRatio;		//���ֶһ�����

	virtual void editBoxReturn(ui::EditBox* editBox) {};
};

#endif // ServiceLayer_h__
