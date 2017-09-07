/*********************************************************
ģ����       : 
�ļ���       :BaseApp.cpp
����ļ�     :BaseApp.hpp
�ļ�ʵ�ֹ��� :Ӧ�ù�����
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#include "BaseApp.hpp"
#include "NDFunc.hpp"

CSuperVPNApp TSuperVPNApp;

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CBaseApp::Run()
{
	//�߳�����
	mTimer.Start();
	//�����������
	InitApplication();
}

/*********************************************************
����˵�������뵥�ض�ʱ��
���˵����
����˵����
����ֵ  ��timeout(����)
*********************************************************/
bool CBaseApp::InsertSingleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong))
{
	if (mTimer.InsertSingleTimer(callid,timeout,CallBack) == 0)
		return true;
	else
		return false;
}

/*********************************************************
����˵��������ѭ����ʱ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CBaseApp::InsertCircleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong))
{
	if (mTimer.InsertCircleTimer(callid,timeout,CallBack) == 0)
		return true;
	else
		return false;
}


/*********************************************************
����˵����ɾ��ѭ����ʱ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CBaseApp::DeleteCircleTimer(unsigned long callid)
{
	if (mTimer.DeleteCircleTimer(callid) == 0)
		return true;
	else
		return false;
}

/*********************************************************
����˵����ɾ��������ʱ��
���˵����callid����ʱ��ID
����˵����
����ֵ  ��
*********************************************************/
bool CBaseApp::DeleteSingleTimer(unsigned long callid)
{
	if (mTimer.DeleteSingleTimer(callid) == 0)
		return true;
	else
		return false;
}

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CBaseApp::InitApplication(void)
{
	return true;
}

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
CBaseApp::CBaseApp()
{

}

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
CBaseApp::~CBaseApp()
{

}
