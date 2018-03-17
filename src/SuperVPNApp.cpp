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
#include "HttpRunEnvCKUser.hpp"
#include "HttpRunEnvCKSrv.hpp"
#include "microhttpd.h"

CNodeBase *CSuperVPNApp::gPNode;
list<SServerInfo> gServers;
list<SServerInfo>::iterator gITCurServer,gITBakServer;

/*********************************************************
函数说明：application初始化
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CSuperVPNApp::InitApplication(void)
{
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] ===============");
	#ifdef GENERAL_NODE_USER_APP
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] ver=[%d]", SUPER_VPN_CLIENT_VER_NODE);
	#else
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] ver=[%d]", SUPER_VPN_CLIENT_VER_SERVER);
	#endif
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] ===============");

	//系统数据初始化
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] SYSTEM START BEGIN INIT SYSTEM...");
	if (!InitSystem()) 
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] Init System Error......");
		cout<<"Init System Error......";
		return false;
	}

	//数据包队列循环处理
	TranslatePkt();
	
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
	//服务器列表处理
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] ServerListCheck...");
	ServerListCheck();
	
	//检查结点编号是否有获取并且本地存在
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] NodeInitCheck...");
	while(NodeInitCheck() != ND_SUCCESS)
		sleep(8);
	
	//系统运行环境检测(包括edge\iptable\node-version)
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] RunEnvCheck...");
	while(RunEnvCheck() != ND_SUCCESS)
		sleep(8);

	//节点配置请求
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] NodeEnvSet...");
	while(gPNode->NodeEnvSet() != ND_SUCCESS)
		sleep(8);

	//定时重启检测
	if(gPNode->GetNodeInform().lRestartTime > 0)
		AfxInsertSingleTimer(TIMER_ID_NODE_RESTART_CHECK, gPNode->GetNodeInform().lRestartTime, NodeRestartFunc);	

	//启用Hello服务
	if (mHelloSrv.Start()) 
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] HELLO SERVER START ERROR...");
	else
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] HELLO SERVER START WORKING...");	

#ifdef GENERAL_NODE_USER_APP
	//启动http服务	
	if (mHttpSrv.Start()) 
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] HTTP SERVER START ERROR...");
	else
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] HTTP SERVER START WORKING...");		
#endif		

	//增加定时Hello
	if(gPNode->GetNodeInform().lHelloTime <= 0)
		gPNode->GetNodeInform().lHelloTime = TIMER_VALUE_NODE_HELLO_CHECK;	
	AfxInsertCircleTimer(TIMER_ID_NODE_HELLO_CHECK, gPNode->GetNodeInform().lHelloTime, NodeHelloFunc);

	return true;
}

/*********************************************************
函数说明：节点初始化(包括nodeid)
入参说明：
出参说明：
返回值  ：
*********************************************************/
ndStatus CSuperVPNApp::NodeInitCheck()
{
	//检测结点编号的文件是否存在
	//如果存在，则读出编号，如果不存在，进行申请
	char *nodeid = AfxGetNodeID();
	if(nodeid == NULL){
		return gPNode->NodeInit();
	}

	gPNode->SetNodeID(nodeid);
	
	return ND_SUCCESS;
}

/*********************************************************
函数说明：服务器列表检测
入参说明：
出参说明：
返回值  ：
*********************************************************/
ndStatus CSuperVPNApp::ServerListCheck()
{
	ndStatus ret;
	gITCurServer = gServers.begin();
	gITBakServer = gITCurServer;
	//如果本地不存在服务列表文件，则根据默认创建
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::ServerListCheck] Get Local ServerList");
	if (AfxGetServerList(gServers))
	{
		gITCurServer = gServers.begin();
		gITBakServer = gITCurServer;
	}
	else
 	{
 		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::ServerListCheck] Get Local ServerList Error");
		return ND_ERROR_INVALID_PARAM;
	}

	//读取服务器列表
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::ServerListCheck] Get Server ServerList");
	ret = gPNode->GetServerList(gServers);
	if(ret != ND_SUCCESS) return ret;

	//更新服务器列表
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::ServerListCheck] Update Server ServerList");
	AfxUpdateServerList(gServers);
	gITCurServer = gServers.begin();
	gITBakServer = gITCurServer;
	
	return ND_SUCCESS;
}

/*********************************************************
函数说明：系统运行环境检测(包括edge\iptable\node-version)
入参说明：
出参说明：
返回值  ：
*********************************************************/
 ndStatus CSuperVPNApp::RunEnvCheck()
{
#ifdef GENERAL_NODE_USER_APP
	CHttpRunEvnCKUser httpRunEnvCK(gPNode);
#else
	CHttpRunEvnCKSrv httpRunEnvCK(gPNode);
#endif
	return httpRunEnvCK.BeginCheck();
}


/*********************************************************
函数说明：构造函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
CSuperVPNApp::CSuperVPNApp()
{
#ifdef GENERAL_NODE_USER_APP
	gPNode = new CNodeUser();
#else
	gPNode = new CNodeSrv();
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
	delete gPNode;
}

/*********************************************************
函数说明：Hello的定时器函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CSuperVPNApp::NodeHelloFunc(ndULong param)
{
	if (gPNode->NodeHello() == ND_ERROR_NOT_RECVIVE_HELLO)
	{
		////////////////////////////////////////////////////////////////////////
		//这里需增加检测hello的时候，如果超过几次hello都失败，则需要进行下线处理
		////////////////////////////////////////////////////////////////////////
		FILE *pFile;
		char cmd[512]={0};
		
		if ((pFile = fopen(CLEAN_EDGE_SH_FILE_NAME, "w+")) == NULL) return;
		//结束錯edge进行
		sprintf(cmd, "PROCESS=`ps |grep edge|grep -v grep|grep -v PPID|awk '{ print $1}'`\n");
		fputs(cmd, pFile);
		sprintf(cmd, "for i in $PROCESS\n");
		fputs(cmd, pFile);
		sprintf(cmd, "do\n");
		fputs(cmd, pFile);
		sprintf(cmd, "\techo \"Kill the $1 process [ $i ]\"\n");
		fputs(cmd, pFile);
		sprintf(cmd, "\tkill -9 $i\n");
		fputs(cmd, pFile);
		sprintf(cmd, "done\n");
		fputs(cmd, pFile);
		fclose(pFile);

		AfxWriteDebugLog("SuperVPN run at[CSuperVPNApp::NodeHelloFunc] Exec chmod 777 restart_SH_FILE");	
		sprintf(cmd, "chmod 777 %s", CLEAN_EDGE_SH_FILE_NAME);
		AfxExecCmd(cmd);
		sprintf(cmd, "./%s", CLEAN_EDGE_SH_FILE_NAME);
		AfxExecCmdNotWait(cmd);	
	}
}

/*********************************************************
函数说明：节点重启定时器函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CSuperVPNApp::NodeRestartFunc(ndULong param)
{
	//////////////////////////////////////////////////////////////////////
	//重启的操作处理
	//////////////////////////////////////////////////////////////////////
	FILE *pFile;
	char cmd[512]={0};
	
	if ((pFile = fopen(RESTART_SH_FILE_NAME, "w+")) == NULL) return;
	//结束錯edge进行
	sprintf(cmd, "PROCESS=`ps |grep edge|grep -v grep|grep -v PPID|awk '{ print $1}'`\n");
	fputs(cmd, pFile);
	sprintf(cmd, "for i in $PROCESS\n");
	fputs(cmd, pFile);
	sprintf(cmd, "do\n");
	fputs(cmd, pFile);
	sprintf(cmd, "\techo \"Kill the $1 process [ $i ]\"\n");
	fputs(cmd, pFile);
	sprintf(cmd, "\tkill -9 $i\n");
	fputs(cmd, pFile);
	sprintf(cmd, "done\n");
	fputs(cmd, pFile);	
	//结束SuperVPN
	sprintf(cmd, "PROCESS=`ps |grep %s|grep -v grep|grep -v PPID|awk '{ print $1}'`\n", VPN_EXE_FILE_NAME);
	fputs(cmd, pFile);
	sprintf(cmd, "for i in $PROCESS\n");
	fputs(cmd, pFile);
	sprintf(cmd, "do\n");
	fputs(cmd, pFile);
	sprintf(cmd, "\techo \"Kill the $1 process [ $i ]\"\n");
	fputs(cmd, pFile);
	sprintf(cmd, "\tkill -9 $i\n");
	fputs(cmd, pFile);
	sprintf(cmd, "done\n");
	fputs(cmd, pFile);
	
	sprintf(cmd, "./%s&\n", VPN_EXE_FILE_NAME);
	fputs(cmd, pFile);	
	fclose(pFile);

	AfxWriteDebugLog("SuperVPN run at[CSuperVPNApp::NodeRestartFunc] Exec chmod 777 restart_SH_FILE");	
	sprintf(cmd, "chmod 777 %s", RESTART_SH_FILE_NAME);
	AfxExecCmd(cmd);
	sprintf(cmd, "./%s", RESTART_SH_FILE_NAME);
	if (AfxExecCmdNotWait(cmd)) exit(0);
}

/*********************************************************
函数说明：消息数据包入队
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CSuperVPNApp::InsertPktToQueue(CPacket *pkt)
{
	//AfxWriteDebugLog("===>INSERT PKT TO SYSTEM MSG QUEUE...");
	mPktQueue.PutMsg(pkt);
}

/*********************************************************
函数说明：数据包队列循环处理
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CSuperVPNApp::TranslatePkt(void)
{
	CPacket *pkt;

	while(true)
	{
		//数据包读取
		pkt = mPktQueue.GetMsg();		
		if (NULL == pkt) continue;
		//数据包处理
		AfxWriteDebugLog("===>CSuperVPNApp Recv Pkt Begin Dispose...");
		pkt->DisposePkt();		
		//数据包释放
		delete pkt;
	}
}


