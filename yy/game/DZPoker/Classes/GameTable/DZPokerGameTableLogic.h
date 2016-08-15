#ifndef __DZPokerGameTableLogic_h__
#define __DZPokerGameTableLogic_h__

#include "HNNetExport.h"
#include "HNLogicExport.h"
#include "DZPokerGameClientMessage.h"

namespace DZPoker
{
	
class GameTableUICallback;

/************************************************************************/
/* ��Ϸ�߼����̴���                                                      */
/************************************************************************/
class GameTableLogic: public HN::HNGameLogicBase
{
public:
	GameTableLogic(GameTableUICallback* uiCallback, BYTE bDeskIndex, bool bAutoCreate);
	virtual ~GameTableLogic();

public:	
	virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize);

protected:
	/************************�ָ���*********************************/
	//��Ϸ��Ϣ����
	void dealUserSetResp(void* object, INT objectSize);
	void dealUserSetChangeResp(void* object, INT objectSize){}
	void dealGameBeginResp(void* object, INT objectSize);//**
	void dealSysDeskCfgResp(void* object, INT objectSize);//**
	void dealCfgDeskResp(void* object, INT objectSize);
	void dealDeskCfgResp(void* object, INT objectSize);
	void dealAgreeDeskCfgResp(void* object, INT objectSize){}//--
	void dealBetResp(void* object, INT objectSize);//**
	void dealHandCardResp(void* object, INT objectSize);
	void dealSend3CardResp(void* object, INT objectSize);
	void dealSend4_5CardResp(void* object, INT objectSize);
	void dealSendCardFinishResp(void* object, INT objectSize);
	void dealBetPoolUpResp(void* object, INT objectSize);
	void dealBetReqResp(void* object, INT objectSize){}
	void dealBetInfoResp(void* object, INT objectSize);
	void dealNewUserResp(void* object, INT objectSize){}
	void dealPassUserResp(void* object, INT objectSize){}
	void dealSubMoneyResp(void* object, INT objectSize); //**
	void dealCompareCardResp(void* object, INT objectSize);
	void dealCallMandateResp(void* object, INT objectSize){}
	void dealCheckMandateResp(void* object, INT objectSize){}
	void dealCallAnyMandateResp(void* object, INT objectSize){}
	void dealPassAbandonMandateResp(void* object, INT objectSize){}
	void dealUserLeftDeskResp(void* object, INT objectSize);
	void dealSortOutResp(void* object, INT objectSize);
	void dealTokenResp(void* object, INT objectSize);
	void dealResultResp(void* object, INT objectSize);
	void dealBuyMoneyResp(void* object, INT objectSize);
	void dealLookCardResp(void* object, INT objectSize);
	void dealCanShowCardResp(void* object, INT objectSize){}
	void dealShowCardResp(void* object, INT objectSize);
	void dealNoPlayerResp(void* object, INT objectSize);
	void dealAgreeResp(void* object, INT objectSize);
	void dealCfgDeskTimeOutResp(void* object, INT objectSize);
	void dealNotEnoughMoneyResp(void* object, INT objectSize);
	void dealMoneyLackResp(void* object, INT objectSize);//**
	void dealNextRoundReqResp(void* object, INT objectSize);//**
	void dealNewRoundBeginResp(void* object, INT objectSize);
	void dealFinishRoundReqResp(void* object, INT objectSize){}
	

	/************************�ָ���*********************************/
	//�����Ϣ����
	virtual void dealUserAgreeResp(MSG_GR_R_UserAgree* agree);
	virtual void dealGameStartResp(BYTE bDeskNO);
	virtual void dealGameEndResp(BYTE bDeskNO);
	virtual void dealGamePointResp(void* object, INT objectSize);
	virtual void dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);
	//����Ŷ�����
	virtual void dealQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& user);
	virtual void dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);
	virtual void dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo);
	virtual void dealGameStationResp(void* object, INT objectSize);
    
    //--------------------------�����ӿ�---------------------
    //������Ϣ�㲥
    virtual void dealGameContestNotic(MSG_GR_I_ContestInfo* contestInfo) override;
    //�û�������Ϣ
    virtual void dealGameUserContset(MSG_GR_ContestChange* contestChange) override;
    //������̭
    virtual void dealGameContestKick() override;
    //�ȴ���������
    virtual void dealGameContestWaitOver() override;
    //��������
    virtual void dealGameContestOver(MSG_GR_ContestAward* contestAward) override;
    
    
public:
	//������Ϸ
	void enterGame();
	virtual void sendUserUp();
	void loadDeskUsersUI();
	virtual void clearDesk();

public:
	//����ָ��ͻ���==>�����
	//�����Ŷ�
	void sendQueue();
	void sendUserRaise(LLONG money);
	void sendUserBet(LLONG money);
	void sendUserFold();
	void sendUserCall();
	void sendUserCheck();
	void sendUserAllIn();
	void sendUserReady();
	LLONG getMinBuy() const;
	LLONG getMaxBuy() const;
	LLONG getPlanBuy() const;
	BYTE getMeStation() const;
protected:
	GameTableUICallback*	_uiCallback;
	TableInfo				_tableInfo;
	UserInfoStruct*         _myInfo;
	void sendUserBet(emType type, LLONG money);
	void setTableOffsetStation(BYTE bDeskStation);
protected:
	void initData();
	/**������������**/
	void saveTableInfo(void* object);
	/**��ʾ�����Ϣ**/
	void showUserInfo();
	/**��ʾ��ҳ���**/
	void showUserMoney(const TSubMoney* pSubMoney);
	/**��ʾ������Ի���**/
	void showBuyMoney(LLONG min, LLONG max, LLONG plan);
	/**��ȡ��һ����ҵ�λ��**/
	BYTE getNextUserStation(BYTE byStation);
	/**��ȡ��һ����ҵ�λ��**/
	BYTE getLastUserStation(BYTE byStation);
	/**�߼�λ��<==>��ͼλ��**/
	BYTE viewToLogicStation(BYTE byStation);
	BYTE logicToViewStation(BYTE byStation);
	/**��ʾ������ť**/
	void showAction(bool bVisible, void* object);
	//��ʾ��ѧ
	void showHelp(bool bVisible);
	//��ʾ�й�
	void showAutoAction(bool bVisible);
	//�йܴ���
	bool doUserAutoAction(void* object);
	//���û�����������ֵ
	void setSliderValue();

	//���������Ϣ���
	void	ClearDeskUser();

public:
	//��¼�Ƿ�Ϊ������׼��
	bool _isReadyQueue;
	// �Ƿ������
	bool	_bContestRoom;
	//�Ƿ�������� 
	bool	_bContestEnd;
};


}

#endif // __DZPokerGameTableLogic_h__