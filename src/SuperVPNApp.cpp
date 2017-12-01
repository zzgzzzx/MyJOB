/*********************************************************
模块名       : 
文件名       :BaseApp.cpp
相关文件     :BaseApp.hpp
文件实现功能 :应用工程类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#include "BaseApp.hpp"
#include "NDFunc.hpp"
#include "NodeUser.hpp"
#include "NodeSrv.hpp"
#include "HttpUpdateCK.hpp"

CNodeBase *CSuperVPNApp::mPNode;

/*********************************************************
函数说明：
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CSuperVPNApp::InitApplication(void)
{
	//系统数据初始化
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
函数说明：系统数据初始化
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CSuperVPNApp::InitSystem(void)
{
	//版本升级检测
	UpdateCheck();

	//节点初始化开始
	mPNode->NodeInit();

	//增加定时Hello
	AfxInsertCircleTimer(TIMER_ID_NODE_HELLO_CHECK, TIMER_VALUE_NODE_HELLO_CHECK, NodeHelloFunc);

	//循环等待保证不退出应用
	while(!mStopRun)
		sleep(60);

	return true;
}

/*********************************************************
函数说明：版本测试升级
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CSuperVPNApp::UpdateCheck()
{
	CHttpUpdateCK httpUpdateCK;
	if (httpUpdateCK.BeginUpdateCheck() == ND_SUCCESS)
		mStopRun = true;
}


/*********************************************************
函数说明：构造函数
入参说明：
出参说明：
返回值  ：
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
函数说明：析构函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
CSuperVPNApp::~CSuperVPNApp()
{
	delete mPNode;
}

/*********************************************************
函数说明：Hello的定时器函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CSuperVPNApp::NodeHelloFunc(ndULong param)
{
	mPNode->NodeHello();
}

