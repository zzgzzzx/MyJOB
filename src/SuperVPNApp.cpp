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

CNodeBase *CSuperVPNApp::mPNode;

/*********************************************************
����˵����application��ʼ��
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
	SDeviceFlag deviceFlag;

	//���в���·������
	CNodeUser *pNodeUser = dynamic_cast<CNodeUser *>CSuperVPNApp::mPNode;
	pNodeUser->SetPolicyRoute(deviceFlag);

	//��Ӧ���ݰ�

    struct MHD_Response *response;
    response = MHD_create_response_from_buffer(strlen(pageBuffer),
        (void*)pageBuffer, MHD_RESPMEM_PERSISTENT);

    if (MHD_add_response_header(response, "Content-Type", "text/html") == MHD_NO) {
        std::cout << "MHD_add_response_header error\n";
        return MHD_NO;
    }
    if (MHD_queue_response(connection, MHD_HTTP_OK, response) == MHD_NO) {
        std::cout << "MHD_queue_response error\n";
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
	ServerListCheck();
	
	//��������Ƿ��л�ȡ���ұ��ش���
	while(NodeInitCheck() != ND_SUCCESS)
		sleep(8);
	
	//ϵͳ���л������(����edge\iptable\node-version)
	while(RunEnvCheck() != ND_SUCCESS)
		sleep(8);

	//�ڵ���������
	while(mPNode->NodeEnvSet() != ND_SUCCESS)
		sleep(8);

	//��ʱ�������
	if(mPNode->GetNodeInform().lRestartTime > 0)
		AfxInsertSingleTimer(TIMER_ID_NODE_RESTART_CHECK, mPNode->GetNodeInform().lRestartTime, NodeRestartFunc);		

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
	//������ز����ڷ����б��ļ��������Ĭ�ϴ���
	AfxGetServerList(mServers);	

	//��ȡ�������б�
	ret = mPNode->GetServerList(mServers);
	if(ret != ND_SUCCESS) return ret;

	//���·������б�
	AfxUpdateServerList(mServers);
	
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


