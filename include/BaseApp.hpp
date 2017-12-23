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
	//�ڵ�
	static CNodeBase *mPNode;
	//�Ƿ���Ҫ�˳���ʶ
	ndBool mStopRun;	
	//ϵͳ���ݳ�ʼ��
	bool InitSystem(void);
 	//ϵͳ���л������
 	ndStatus RunEnvCheck();
protected:
	//Ӧ�ó����ʼ������
	bool InitApplication(void);

public:
	//���캯��
	CSuperVPNApp();
	//��������
	~CSuperVPNApp();	
	//�ڵ�hello
	static void NodeHelloFunc(ndULong param);	
};

#endif
