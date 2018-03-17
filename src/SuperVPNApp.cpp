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
#include "microhttpd.h"

CNodeBase *CSuperVPNApp::gPNode;
list<SServerInfo> gServers;
list<SServerInfo>::iterator gITCurServer,gITBakServer;

/*********************************************************
����˵����application��ʼ��
���˵����
����˵����
����ֵ  ��
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

	//ϵͳ���ݳ�ʼ��
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] SYSTEM START BEGIN INIT SYSTEM...");
	if (!InitSystem()) 
	{
		AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitApplication] Init System Error......");
		cout<<"Init System Error......";
		return false;
	}

	//���ݰ�����ѭ������
	TranslatePkt();
	
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
	//�������б���
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] ServerListCheck...");
	ServerListCheck();
	
	//��������Ƿ��л�ȡ���ұ��ش���
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] NodeInitCheck...");
	while(NodeInitCheck() != ND_SUCCESS)
		sleep(8);
	
	//ϵͳ���л������(����edge\iptable\node-version)
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] RunEnvCheck...");
	while(RunEnvCheck() != ND_SUCCESS)
		sleep(8);

	//�ڵ���������
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::InitSystem] NodeEnvSet...");
	while(gPNode->NodeEnvSet() != ND_SUCCESS)
		sleep(8);

	//��ʱ�������
	if(gPNode->GetNodeInform().lRestartTime > 0)
		AfxInsertSingleTimer(TIMER_ID_NODE_RESTART_CHECK, gPNode->GetNodeInform().lRestartTime, NodeRestartFunc);	

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

	//���Ӷ�ʱHello
	if(gPNode->GetNodeInform().lHelloTime <= 0)
		gPNode->GetNodeInform().lHelloTime = TIMER_VALUE_NODE_HELLO_CHECK;	
	AfxInsertCircleTimer(TIMER_ID_NODE_HELLO_CHECK, gPNode->GetNodeInform().lHelloTime, NodeHelloFunc);

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
		return gPNode->NodeInit();
	}

	gPNode->SetNodeID(nodeid);
	
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
		return ND_ERROR_INVALID_PARAM;
	}

	//��ȡ�������б�
	AfxWriteDebugLog("SuperVPN run at [CSuperVPNApp::ServerListCheck] Get Server ServerList");
	ret = gPNode->GetServerList(gServers);
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
����˵�������캯��
���˵����
����˵����
����ֵ  ��
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
����˵������������
���˵����
����˵����
����ֵ  ��
*********************************************************/
CSuperVPNApp::~CSuperVPNApp()
{
	delete gPNode;
}

/*********************************************************
����˵����Hello�Ķ�ʱ������
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CSuperVPNApp::NodeHelloFunc(ndULong param)
{
	if (gPNode->NodeHello() == ND_ERROR_NOT_RECVIVE_HELLO)
	{
		////////////////////////////////////////////////////////////////////////
		//���������Ӽ��hello��ʱ�������������hello��ʧ�ܣ�����Ҫ�������ߴ���
		////////////////////////////////////////////////////////////////////////
		FILE *pFile;
		char cmd[512]={0};
		
		if ((pFile = fopen(CLEAN_EDGE_SH_FILE_NAME, "w+")) == NULL) return;
		//�����eedge����
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
����˵�����ڵ�������ʱ������
���˵����
����˵����
����ֵ  ��
*********************************************************/
void CSuperVPNApp::NodeRestartFunc(ndULong param)
{
	//////////////////////////////////////////////////////////////////////
	//�����Ĳ�������
	//////////////////////////////////////////////////////////////////////
	FILE *pFile;
	char cmd[512]={0};
	
	if ((pFile = fopen(RESTART_SH_FILE_NAME, "w+")) == NULL) return;
	//�����eedge����
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
	//����SuperVPN
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


