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
#include "NodeUser.hpp"
#include "NodeSrv.hpp"
#include "HttpRunEnvCKUser.hpp"
#include "HttpRunEnvCKSrv.hpp"

CNodeBase *CSuperVPNApp::mPNode;

/*********************************************************
����˵����
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CSuperVPNApp::InitApplication(void)
{
	//ϵͳ���ݳ�ʼ��
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] SYSTEM START BEGIN INIT SYSTEM...");
	if (!InitSystem()) 
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] Init System Error......");
		cout<<"Init System Error......";
		return false;
	}
	return true;
}

/*********************************************************
����˵����ϵͳ���ݳ�ʼ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CSuperVPNApp::InitSystem(void)
{
	//ϵͳ���л������(����edge\iptable\node-version)
	while(RunEnvCheck() != ND_SUCCESS)
		sleep(8);

	//�ڵ��ʼ����ʼ
	mPNode->NodeInit();

	//���Ӷ�ʱHello
	AfxInsertCircleTimer(TIMER_ID_NODE_HELLO_CHECK, TIMER_VALUE_NODE_HELLO_CHECK, NodeHelloFunc);

	//ѭ���ȴ���֤���˳�Ӧ��
	while(!mStopRun)
		sleep(60);

	return true;
}

/*********************************************************
����˵����ϵͳ���л������(����edge\iptable\node-version)
���˵����
����˵����
����ֵ  ��
*********************************************************/
 ndStatus CSuperVPNApp::RunEnvCheck()
{
#ifdef GENERAL_NODE_USER_APP
	CHttpRunEvnCKUser httpRunEnvCK(mPNode);
#else
	CHttpRunEvnCKSrv httpRunEnvCK(mPNode);
#endif
	return httpRunEnvCK.BeginCheck();
}


/*********************************************************
����˵�������캯��
���˵����
����˵����
����ֵ  ��
*********************************************************/
CSuperVPNApp::CSuperVPNApp()
{
	mStopRun = false;
#ifdef GENERAL_NODE_USER_APP
	mPNode = new CNodeUser();
#else
	mPNode = new CNodeSrv();
#endif
}

/*********************************************************
����˵������������
���˵����
����˵����
����ֵ  ��
*********************************************************/
CSuperVPNApp::~CSuperVPNApp()
{
	delete mPNode;
}

/*********************************************************
����˵����Hello�Ķ�ʱ������
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CSuperVPNApp::NodeHelloFunc(ndULong param)
{
	mPNode->NodeHello();
}

