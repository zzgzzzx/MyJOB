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

CNodeBase *CSuperVPNApp::mPNode;

/*********************************************************
函数说明：application初始化
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
函数说明：http服务接收数据处理
入参说明：
出参说明：
返回值  ：
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

	//接收HTTP数据包
	SDeviceFlag deviceFlag;

	//进行策略路由设置
	CNodeUser *pNodeUser = dynamic_cast<CNodeUser *>CSuperVPNApp::mPNode;
	pNodeUser->SetPolicyRoute(deviceFlag);

	//回应数据包

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
函数说明：启动http服务器
入参说明：
出参说明：
返回值  ：
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

	//循环等待保证不退出应用
	while(!mStopRun)
		sleep(60);

    MHD_stop_daemon(daemon);
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
	ServerListCheck();
	
	//检查结点编号是否有获取并且本地存在
	while(NodeInitCheck() != ND_SUCCESS)
		sleep(8);
	
	//系统运行环境检测(包括edge\iptable\node-version)
	while(RunEnvCheck() != ND_SUCCESS)
		sleep(8);

	//节点配置请求
	while(mPNode->NodeEnvSet() != ND_SUCCESS)
		sleep(8);

	//定时重启检测
	if(mPNode->GetNodeInform().lRestartTime > 0)
		AfxInsertSingleTimer(TIMER_ID_NODE_RESTART_CHECK, mPNode->GetNodeInform().lRestartTime, NodeRestartFunc);		

	//增加定时Hello
	AfxInsertCircleTimer(TIMER_ID_NODE_HELLO_CHECK, TIMER_VALUE_NODE_HELLO_CHECK, NodeHelloFunc);

#ifdef GENERAL_NODE_USER_APP
	//启动http服务器
	StartHttpd();
#else
	//循环等待保证不退出应用
	while(!mStopRun)
		sleep(60);
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
	//如果本地不存在服务列表文件，则根据默认创建
	AfxGetServerList(mServers);	

	//读取服务器列表
	ret = mPNode->GetServerList(mServers);
	if(ret != ND_SUCCESS) return ret;

	//更新服务器列表
	AfxUpdateServerList(mServers);
	
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
	CHttpRunEvnCKUser httpRunEnvCK(mPNode);
#else
	CHttpRunEvnCKSrv httpRunEnvCK(mPNode);
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
	////////////////////////////////////////////////////////////////////////
	//这里需增加检测hello的时候，如果超过几次hello都失败，则需要进行下线处理
	////////////////////////////////////////////////////////////////////////
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
}


