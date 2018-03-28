/****************** Copyright (c) 2017 **************
ģ����       : 
�ļ���       :CBaseApp.hpp
����ļ�     : 
�ļ�ʵ�ֹ��� :Ӧ�ù�����
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#ifndef __BASEAPP_HPP__
#define __BASEAPP_HPP__

#include "MsgQueue.hpp"
#include "TimerManager.hpp"
#include "NodeBase.hpp"
#include "Packet.hpp"
#include "HelloSrvThread.hpp"
#include "HttpSrvThread.hpp"
#include "ServiceSet.hpp"

//**********************************
//Ӧ�ù��̻���                       
//**********************************
class CBaseApp
{
private:
	//��ʱ����Ա
	CTimerManager mTimer;

protected:
	//Ӧ�ó����ʼ������
	virtual bool InitApplication(void);
public:	
	//���뵥�ض�ʱ��	
	bool InsertSingleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong));
	//����ѭ����ʱ��	
	bool InsertCircleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong));

	//ɾ��ѭ����ʱ��
	bool DeleteCircleTimer(unsigned long callid);
	//ɾ��������ʱ��
	bool DeleteSingleTimer(unsigned long callid);

	//Ӧ�ó�����������
	void Run();

	//���캯��
	CBaseApp();
	//��������
	virtual ~CBaseApp();
};

//**********************************
//���·���豸������                       
//**********************************
class CSuperVPNApp: public CBaseApp
{
private:
	//��Ϣ������
	CMsgFIFO<CPacket*> mPktQueue;	
	
	//ϵͳ���ݳ�ʼ��
	bool InitSystem(void);
	//�������б���
	ndStatus ServerListCheck();
 	//ϵͳ���л������
 	ndStatus RunEnvCheck();
	//�ڵ��ʼ����ż��
	ndStatus NodeInitCheck();
	//����http������
	ndStatus StartHttpd();
	//���ݰ�����ѭ������
	void TranslatePkt(void);
	
protected:
	//Ӧ�ó����ʼ������
	bool InitApplication(void);

public:
	//�ڵ�
	CNodeBase *mPNode;
	//hello����
	CHelloSrvThread mHelloSrv;
	//http����
	CHttpSrvThread mHttpSrv;
	//����ڵ㼯����
	CServiceSet mServiceSet;
	//���ʶ�������
	CIdentifySet mIdentifySet;
	
	//���캯��
	CSuperVPNApp();
	//��������
	~CSuperVPNApp();	

	//�ڵ�����
	static void NodeRestartFunc(ndULong param);	

	//��Ϣ���ݰ����
	void InsertPktToQueue(CPacket *pkt);	
};

#endif
