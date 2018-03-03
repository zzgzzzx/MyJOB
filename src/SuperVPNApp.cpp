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
	return true;
}

/*********************************************************
����˵����http����������ݴ���
���˵����
����˵����
����ֵ  ��
*********************************************************/
int  connectionHandler(
    void *cls,
    struct MHD_Connection *connection,
    const char *url,
    const char *method,
    const char *version,
    const char *upload_data,
    size_t *upload_data_size,
    void **con_cls)
{
    const char* pageBufferOK = "<html><body>Hello, I'm lgxZJ!</body></html>";
	const char* pageBufferERR = "<html><body>Hello, I'm lgxZJ!</body></html>";

	//����HTTP���ݰ�
	struct sockaddr *clientaddr = (struct sockaddr *)MHD_get_connection_info(connection, MHD_CONNECTION_INFO_CLIENT_ADDRESS);
	const char *value = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "value");
	
	SDeviceFlag deviceFlag;
	struct sockaddr_in *addr_in;
	addr_in= (struct sockaddr_in *)clientaddr;
	deviceFlag.sDeviceIP = inet_ntoa(addr_in->sin_addr);
	AfxWriteDebugLog( "MHD Request Client address=[%s]", deviceFlag.sDeviceIP.c_str());
	AfxWriteDebugLog( "MHD Request URL=[%s]", value);
	AfxWriteDebugLog( "MHD Request sDeviceFlag=[%s]", deviceFlag.sDeviceFlag.c_str());

	//���в���·������
	CNodeUser *pNodeUser = dynamic_cast<CNodeUser *>(CSuperVPNApp::gPNode);
	pNodeUser->SetPolicyRoute(deviceFlag);

	//��Ӧ���ݰ�
    struct MHD_Response *response;
    response = MHD_create_response_from_buffer(strlen(pageBufferOK),
        (void*)pageBufferOK, MHD_RESPMEM_PERSISTENT);

    if (MHD_add_response_header(response, "Content-Type", "text/html") == MHD_NO) {
        AfxWriteDebugLog( "MHD_add_response_header error");
        return MHD_NO;
    }
    if (MHD_queue_response(connection, MHD_HTTP_OK, response) == MHD_NO) {
        AfxWriteDebugLog("MHD_queue_response error");
        return MHD_NO;
    }
    MHD_destroy_response(response);

    return MHD_YES;
}

/*********************************************************
����˵��������http������
���˵����
����˵����
����ֵ  ��
*********************************************************/
ndStatus CSuperVPNApp::StartHttpd()
{
    const int port = 8888;

    struct MHD_Daemon* daemon = 
        MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, port
        , NULL, NULL, connectionHandler, NULL, MHD_OPTION_END);
    if (daemon == NULL) {
        AfxWriteDebugLog("Cannot start httpd server!");
        return -1;
    }

	//ѭ���ȴ���֤���˳�Ӧ��
	while(!mStopRun)
		sleep(60);

    MHD_stop_daemon(daemon);
    
    return ND_SUCCESS;
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

	//���Ӷ�ʱHello
	AfxInsertCircleTimer(TIMER_ID_NODE_HELLO_CHECK, TIMER_VALUE_NODE_HELLO_CHECK, NodeHelloFunc);

#ifdef GENERAL_NODE_USER_APP
	//����http������
	StartHttpd();
#else
	//ѭ���ȴ���֤���˳�Ӧ��
	while(!mStopRun)
		sleep(60);
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
	mStopRun = false;
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
	gPNode->NodeHello();
	////////////////////////////////////////////////////////////////////////
	//���������Ӽ��hello��ʱ�������������hello��ʧ�ܣ�����Ҫ�������ߴ���
	////////////////////////////////////////////////////////////////////////
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
}


