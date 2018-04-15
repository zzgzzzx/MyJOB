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
#include "NodeGeneral.hpp"
#include "Packet.hpp"
#include "HelloSrvThread.hpp"
#include "HttpSrvThread.hpp"
#include "IdentifySet.hpp"

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
	virtual bool InitApplication(int argc,char *argv[]);
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
	void Run(int argc,char *argv[]);

	//���캯��
	CBaseApp();
	//��������
	virtual ~CBaseApp();
};

//**********************************
//SuperVPN������                       
//**********************************
class CSuperVPNApp: public CBaseApp
{
private:
	//��Ϣ������
	CMsgFIFO<CPacket*> mPktQueue;	
	
	//ϵͳ���ݳ�ʼ��
	bool InitSystem(char *appname, bool ifOnlyCheckUpgrade);
	//�������б���
	ndStatus ServerListCheck();
 	//ϵͳ���л������
 	ndStatus RunEnvCheck(char *appname, bool ifOnlyCheckUpgrade);
	//�ڵ��ʼ����ż��
	ndStatus NodeInitCheck();
	//����http������
	ndStatus StartHttpd();
	//���ݰ�����ѭ������
	void TranslatePkt(void);
	//�ӱ���ʼ��
	void KBInit();
	//�������ƻ���
	ndBool GetRunLock();
	//�����������ƻ���
	ndBool GetUpgradeLock();	
	
protected:
	//Ӧ�ó����ʼ������
	bool InitApplication(int argc,char *argv[]);

public:
	//�ڵ�
	CNodeGeneral *mPNode;
	//hello����
	CHelloSrvThread mHelloSrv;
	//http����
	CHttpSrvThread mHttpSrv;
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

//**********************************
//SuperVPN��⹤����                       
//**********************************
class CCheckSuperVPNApp: public CBaseApp
{
private:	
	//ϵͳ���ݳ�ʼ��
	bool InitSystem(void);
	void StopSuperVPNAndUpgrade();
	void CheckVPNAndRun();

protected:
	//Ӧ�ó����ʼ������
	bool InitApplication(int argc,char *argv[]);

public:
	
	//���캯��
	CCheckSuperVPNApp();
	//��������
	~CCheckSuperVPNApp();	
};

#endif
