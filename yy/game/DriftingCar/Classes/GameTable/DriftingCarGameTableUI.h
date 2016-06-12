#ifndef __DriftingCarGame_Table_UI_H__
#define __DriftingCarGame_Table_UI_H__

#include "DriftingCarGameTableUICallback.h"

#include "HNNetExport.h"
#include "cocos2d.h"
#include <vector>
#include <array>

namespace DriftingCar
{
	class GameTableLogic;
	class GameBoard;
	class GameResultHistory;
	class GameApplyList;

#define Bet_Button_Tag 1000			//���밴ť
#define betArea_button_tag 2000		//��ע����

	std::string ValueToString(LLONG value);//ת�ɴ�,���ַ��� "123456789"->"123,456,789"

	class GameTableUI : public HN::HNGameUIBase, public GameTableUICallback
	{
	protected:
		GameTableUI();
		virtual ~GameTableUI();

	public:
		static GameTableUI* create(BYTE bDeskIndex, bool bAutoCreate);
		virtual bool init(BYTE bDeskIndex, bool bAutoCreate);
		virtual void onEnter();
		virtual void onExit();
		void areaClickCallback(cocos2d::Node* pNode);
	public:

		virtual void Test()override;

		virtual void OnGameStation()override;										//��һ�ν���ʱ����

		virtual void startGameFree()override;										//������Ϣ

		virtual void startGameBeginBet()override;									//��ע��Ϣ

		virtual void startGamePaoMaDeng()override;									//�������Ϣ

		virtual void startGameEndResult()override;									//������Ϣ

		virtual void GameBetReuslt()override;										//��ע�����Ϣ

		virtual void setBetEnabled(bool enabled);									//��ע����ѡ��ť�Ƿ����
		
		virtual void setFreeTime(BYTE time);										//���ÿռ�ʱ��

		virtual void setBetTime(BYTE time);											//����Ͷעʱ��
		
		virtual void setSendTime(BYTE time);										//���ÿ���ʱ��
		
		virtual void setMsg(const std::string &msg);								//��ʾ��ʾ��Ϣ
		
		virtual void startSend(int end, BYTE time);									//������������
		
		virtual void addRecord(const std::vector<BYTE>& records,BYTE byResult);		//��ӿ�����¼
		
		virtual void addNT(BYTE seatNo, const std::string& name, LLONG money);		//���ׯ��
		
		virtual void removeAllNT();													//�Ƴ�����ׯ��

//		virtual void removeNT(BYTE seatNo);											//�Ƴ�ׯ��
		
		virtual void setBetAreaTotal(bool isMy, BYTE AreaNo, LLONG value);			//������ע��������ע
		
		virtual void setBetAreaUser(bool isMy, BYTE AreaNo, LLONG value);			//������ע���������ע
		
		virtual void setBetTotal(LLONG value);										//��������ע����
	
		virtual void setNTName(const std::string& name);							//����ׯ������
		
		virtual void setNTCount(int count);											//������ׯ����
		
		virtual void setNTMoney(LLONG value);										//����ׯ�ҽ��
		
		virtual void setNTGrade(LLONG value);										//����ׯ�ҳɼ�
		
		virtual void setUserName(const std::string& name);							//�����������
		
		virtual void setUserMoney(LLONG value);										//������ҽ��
		
		virtual void setUserGrade(LLONG value);										//������ҳɼ�
		
		virtual void setUserBetMoney(LLONG value)override ;							//������ע���

		virtual void leaveDesk();													//�뿪����
		
		virtual void setOpenArea(int index);										//��ʾ��������
		
		virtual void hideOpenArea();												//���ؿ�������
		
		virtual void setApplyAndLeaveButtonEnabled(bool enabled);					//������ׯ���밴ť�Ƿ����

		virtual void SetGameEndResult(LLONG userScore,   LLONG userCapital, 
									  LLONG userNTScore, LLONG userNTCapital);		//���ý������ʾ

		virtual void hideGameEndImage();											//���ؽ����

		virtual void ShowWaitNTCount(BYTE mySeatNo);								//��ʾ�Լ���ׯ���б��е�λ��

		virtual void ShowApplyButton();												//��ʾ��ׯ��ť

		virtual void ShowDownNTButton() ;											//��ʾ��ׯ��ť

		virtual void ShowNoNT()override;											//��ʾû��ׯ��

		virtual void ShowBetAreaAction()override;									//��ʾ��ע����

		virtual void HideBetAreaAction()override;									//������ע����

		virtual void ShowImageMyNT()override;										//��ʾ������ׯ

		virtual void ShowImageMyNoNT()override;										//��ʾ������ׯ
	protected:	
		void initParams();															//��ʼ������

		void loadLayout();															//����UI����

		void startTimer();															//��ʼ��ʱ

		void stopTimer();															//ֹͣ��ʱ

		void reduceTime(float delta);												//ʱ������

		void SetShowTime( BYTE time );												//��ʾʱ��

		void LoadBetButton(Node*);													//������ע��ť

		void LoadBetArea(Node*);													//������ע����

		void LoadHistory(Node*);													//������ʷ��¼

		void LoadPaoMaDeng(Node*);													//���������

		void LoadGameEndResult(Node*);												//�������

		void LoadNTList(Node*);														//������ׯ�б�

		void ShowWaitNTList();														//��ʾ�ȴ���ׯ������Ϣ
		
		void Playanimation(int index);												//���ſ�������

		void ShowSmallAwardCar(int index);											//����С��ͼ

		void CheckWaitNTList();														//����±�

		void ShowCarLogoblink(int index);											//�����ܵ���˸

		void HideAllCarLogos();														//���������ܵ�

		void ShowblinkBetButton(int index);											//ָ��������˸ ��1��ʼ

		void StopAllBetButton();													//ȫ������ֹͣ��˸

		void ShowGameStatusImage(int iTime);										//��ʾ״̬ͼƬ

		void PlaytimeWaring(int itime);												//���ž���ʱ��

		void LoadImageGame(int itype);												//������ʾͼ

		void ShowImageCarBlink();													//���ñ���car��˸

		void HideImageCarBlink();													//���ر���car

	private:
		void menuClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);	//����ׯ��ť���˳���ť

		void betClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);		//��ע����

		void betAreaClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);	//��ע������Ӧ

		void WaitNTListClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchType);//������ׯ��������ׯ��ť

	private:
		enum 
		{
			BET_COUNT  = 8,		//������ע����
			AREA_COUNT = 8,		//��ע����
			Wait_NTList = 5,	//�ȴ���ׯUI��ʾ��
			CAR_LIGHT = 4,		//���ƹ�
		};
		struct GameBetText
		{
			ui::TextBMFont* _userText;		//�����ע���
			ui::TextBMFont* _userTotalText;//�ܵ���ע���
			GameBetText():_userTotalText(nullptr),_userText(nullptr){}
		};
		struct NTInfo
		{
			BYTE byseatno;			//�û���λ��
			std::string UserName;	//�û��ǳ�
			LLONG lmoney;			//�û������
		};

		int _betIndex;											//��ǰ��ע��ť������0-7
		int _iWinAward;											//��������
		int _iGameStatus;										//��Ϸ״̬

		std::array<ui::Button*,BET_COUNT> _betButton;			//���밴ť
		std::array<ui::Button*,AREA_COUNT> _gameBetArea;		//��ע����
		std::array<GameBetText,AREA_COUNT>_gameBetAreaText;		//��ע�ı���ʾ

		ImageView* _ImageStatus;					//��Ϸ״̬ͼ

		ui::Button* _exitButton;					//�˳���ť

		ui::Button* _applyButton;					//��ׯ��ť
		ui::Button* _leaveButton;					//��ׯ��ť
		ui::ImageView* _ImagenoNT;					//�����ȴ�ͼƬ

		//�ȴ���ׯ����б�
		std::array<Text*,Wait_NTList> _arrayWaitNt;	//�ȴ���ׯ����
		ui::Text*   _waitNTNum;						//��ׯ������λ��
		std::vector<NTInfo>	vtNtInfp;				//�ȴ��û���ׯ������Ϣ
		int _iWaitNtIndex;							//�û��ȴ���ׯ��������

		std::array<ImageView*, CAR_LIGHT> _arrayLight;	//����
		ImageView* _image_bet_bg;						//����ͼ			
		ImageView* _image_bet_car;						//����ͼƬ
		cocos2d::Sprite* _spriteAnimation;				//������

		Layer*	_layoutBetArea;						//��ע��������

		ui::Button* _waitUpButton;					//��һҳ �ȴ�ׯ���б�
		ui::Button* _waitDwonButton;				//��һҳ

		ui::TextAtlas* _timeAtlat;					//ʱ������

		ui::Text* _stateTimeText;					//ʱ������
		ui::Text* _totalBetText;					//��ע����

		ui::Text*		_NTNameText;				//ׯ������
		ui::TextBMFont* _NTCountText;				//��ׯ����
		ui::TextBMFont* _NTMoneyText;				//ׯ�ҽ��
		ui::TextBMFont* _NTGradeText;				//ׯ�ҳɼ�

		ui::Text*		_userNameText;				//����ǳ�
		ui::TextBMFont* _userMoneyText;				//��ҽ��
		ui::TextBMFont* _userGradeText;				//��ҳɼ�
		ui::TextBMFont* _userBetText;				//�����ע

		ImageView*		_imageEnd;					//����ͼ
		ui::Text*		_UserCapital;				//��ұ���
		ui::Text*		_UserWinScore;				//�����Ӯ
		ui::Text*		_UserNTScore;				//ׯ����Ӯ
		ui::Text*		_UserNTCapital;				//ׯ�ұ���

		GameBoard* _gameBoard;						//�������
		GameResultHistory* _resultHistory;			//��������ʷ
		GameTableLogic* _tableLogic;				//�߼��� ������Ϣ
		
	};

}


#endif