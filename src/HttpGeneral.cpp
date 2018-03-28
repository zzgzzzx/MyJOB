/*********************************************************
模块名       : 节点http协议
文件名       : HttpClientNode.cpp
相关文件     : HttpClientNode.h
文件实现功能 : 节点Htpp协议处理
作者         :lewis
创建时间     :2017-09
**********************************************************/
#include "HttpGeneral.hpp"
#include "cJSON.h"
#include "NDFunc.hpp"

extern list<SServerInfo> gServers;
extern list<SServerInfo>::iterator gITCurServer,gITBakServer;

/*********************************************************
函数说明：构造函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CHttpGeneral::CHttpGeneral()
{
	mPNode = NULL;
}

/*********************************************************
函数说明：构造函数
入参说明：Node(结点)
出参说明：无
返回值  ：无
*********************************************************/
CHttpGeneral::CHttpGeneral(CNodeBase *node)
{
	mPNode = node;
}

/*********************************************************
函数说明：析构函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CHttpGeneral::~CHttpGeneral()
{

}

/*********************************************************
函数说明：节点初始化请求
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGeneral::MakeNodeInitReq()
{
	return ND_SUCCESS;
}

/*********************************************************
函数说明：节点初始化请求返回处理
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGeneral::AnalysisNodeInitRsp()
{	
    cJSON *root;
	int iErrCode;

    root = cJSON_Parse(mRcvBuf.c_str());
    if (!root)
    {
        AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] Error before: [%s]", cJSON_GetErrorPtr());
        return ND_ERROR_INVALID_RESPONSE;
    }

    cJSON *actionsArray = cJSON_GetObjectItem(root, "actions");
    if(actionsArray != NULL)
    {

        cJSON *actionslist = actionsArray->child;

        iErrCode = cJSON_GetObjectItem(actionslist, "error")->valueint;
        if(iErrCode != 0)
		{
			cJSON_Delete(root);
			return ND_ERROR_INVALID_RESPONSE;
        }		
        
        cJSON *replices = cJSON_GetObjectItem(root, "replies");
        if(replices != NULL)
        {
            cJSON *repliceslist = replices->child;
			mPNode->SetNodeID(cJSON_GetObjectItem(repliceslist, "nodeid")->valuestring);
        }

        cJSON_Delete(root);
    }	
	
	return ND_SUCCESS;	
}

/*********************************************************
函数说明：节点配置请求
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGeneral::MakeNodeEnvSetReq()
{
	return ND_SUCCESS;
}

/*********************************************************
函数说明：节点配置请求返回处理
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGeneral::AnalysisNodeEnvSetRsp()
{
	return ND_SUCCESS;	
}

/*********************************************************
函数说明：节点Hello
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGeneral::MakeNodeHelloReq()
{
	return ND_SUCCESS;
}

/*********************************************************
函数说明：节点Hello返回处理
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGeneral::AnalysisNodeHelloRsp()
{
	return ND_SUCCESS;
}

/*********************************************************
函数说明：组合获取服务出口
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGeneral::MakeBindServerReq(ndString devidentify)
{
    char *out;
    cJSON *root, *fmt, *actions;

    //组装消息体
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "node", fmt=cJSON_CreateObject());
	cJSON_AddNumberToObject(fmt, "version",	SUPER_VPN_CLIENT_VER_NODE);
    cJSON_AddStringToObject(fmt, "nodeid", mPNode->GetNodeInform().sNodeID.c_str());

    AfxWriteDebugLog("SuperVPN run at [CHttpSrvNode::MakeBindServerReq] Make ServerList actions");

    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    //========================set===========================================
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_NODE_GET_SERVICES);
	cJSON_AddStringToObject(fmt, "arugments", "");

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);
	
	return ND_SUCCESS;
}

/*********************************************************
函数说明：分析获取服务出口
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGeneral::AnalysisBindServerRsp(list<SBindInform> &mServers)
{
    cJSON *root;
	int iErrCode;

    root = cJSON_Parse(mRcvBuf.c_str());
    if (!root)
    {
        AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisBindServerRsp] Error before: [%s]", cJSON_GetErrorPtr());
        return ND_ERROR_INVALID_RESPONSE;
    }

    cJSON *actionsArray = cJSON_GetObjectItem(root, "actions");
    if(actionsArray != NULL)
    {

        cJSON *actionslist = actionsArray->child;

        iErrCode = cJSON_GetObjectItem(actionslist, "error")->valueint;
        if(iErrCode != 0)
		{
			cJSON_Delete(root);
			return ND_ERROR_INVALID_RESPONSE;
        }		

		mServers.clear();
		
        cJSON *replices = cJSON_GetObjectItem(root, "replies");
        if(replices != NULL)
        {
            cJSON *repliceslist = replices->child;

            cJSON *servers = cJSON_GetObjectItem(repliceslist, "services");
            if(servers != NULL)
            {
                    cJSON *serverslist = servers->child;

                    AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisBindServerRsp] Get servers");
                    SBindInform item;
                    while(serverslist != NULL)
                    {
                        if(cJSON_GetObjectItem(serverslist, "deviceflag") != NULL &&
                           cJSON_GetObjectItem(serverslist, "deviceflag")->valuestring != NULL)
                            item.sDeviceFlag = cJSON_GetObjectItem(serverslist, "deviceflag")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisBindServerRsp] server ip=[%s]",item.sServerIP.c_str());

                        if(cJSON_GetObjectItem(serverslist, "serviceip") != NULL &&
                           cJSON_GetObjectItem(serverslist, "serviceip")->valuestring != NULL)
                            item.sServiceIP = cJSON_GetObjectItem(serverslist, "serviceip")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisBindServerRsp] server port=[%s]",item.sServerPort.c_str());

                        mServers.push_back(item);
                        serverslist = serverslist->next;
                    }
			
            }
        }

        cJSON_Delete(root);
    }	
	
	return ND_SUCCESS;	
}
	
/*********************************************************
函数说明：服务器列表请求
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGeneral::MakeServerListReq()
{
	return ND_SUCCESS;
}

/*********************************************************
函数说明：服务器列表返回处理
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGeneral::AnalysisServerListRsp(list<SServerInfo> &mServers)
{
    cJSON *root;
	int iErrCode;

    root = cJSON_Parse(mRcvBuf.c_str());
    if (!root)
    {
        AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] Error before: [%s]", cJSON_GetErrorPtr());
        return ND_ERROR_INVALID_RESPONSE;
    }

    cJSON *actionsArray = cJSON_GetObjectItem(root, "actions");
    if(actionsArray != NULL)
    {

        cJSON *actionslist = actionsArray->child;

        iErrCode = cJSON_GetObjectItem(actionslist, "error")->valueint;
        if(iErrCode != 0)
		{
			cJSON_Delete(root);
			return ND_ERROR_INVALID_RESPONSE;
        }		

		mServers.clear();
		
        cJSON *replices = cJSON_GetObjectItem(root, "replies");
        if(replices != NULL)
        {
            cJSON *repliceslist = replices->child;

            cJSON *servers = cJSON_GetObjectItem(repliceslist, "servers");
            if(servers != NULL)
            {
                    cJSON *serverslist = servers->child;

                    AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisServerListRsp] Get servers");
                    SServerInfo item;
                    while(serverslist != NULL)
                    {
                        if(cJSON_GetObjectItem(serverslist, "ip") != NULL &&
                           cJSON_GetObjectItem(serverslist, "ip")->valuestring != NULL)
                            item.sServerIP = cJSON_GetObjectItem(serverslist, "ip")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisServerListRsp] server ip=[%s]",item.sServerIP.c_str());

                        if(cJSON_GetObjectItem(serverslist, "port") != NULL &&
                           cJSON_GetObjectItem(serverslist, "port")->valuestring != NULL)
                            item.sServerPort = cJSON_GetObjectItem(serverslist, "port")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisServerListRsp] server port=[%s]",item.sServerPort.c_str());

                        mServers.push_back(item);
                        serverslist = serverslist->next;
                    }
			
            }
        }

        cJSON_Delete(root);
    }	
	
	return ND_SUCCESS;	
}

/*********************************************************
函数说明：获取服务器列表
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndBool CHttpGeneral::GetServerList(list<SServerInfo> &mServers)
{
    //组装数据包
    ndStatus  ret = MakeServerListReq();
    if(ret != ND_SUCCESS)
	{
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::GetServerList] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }
	//数据包发送
	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::GetServerList] PkgSendAndRecv Err ret=[%d]", ret);
		return ret;
	}
	//数据包解析
    ret = AnalysisServerListRsp(mServers);
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::GetServerList] AnalyzeResponsePkg Err ret=[%d]", ret);
        return ret;
    }

    return ND_SUCCESS;
}

/*********************************************************
函数说明：获取服务器出口
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGeneral::GetIdentifyService(list<SBindInform> &ltBSer)
{
	ndString devidentify;
	//lewis 需增加身份标识组串
	
    //组装数据包
    ndStatus  ret = MakeBindServerReq(devidentify);
    if(ret != ND_SUCCESS)
	{
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::GetServerList] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }
	//数据包发送
	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::GetServerList] PkgSendAndRecv Err ret=[%d]", ret);
		return ret;
	}
	//数据包解析
    ret = AnalysisBindServerRsp(ltBSer);
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::GetServerList] AnalyzeResponsePkg Err ret=[%d]", ret);
        return ret;
    }

    return ND_SUCCESS;	
}

/*********************************************************
函数说明：节点初始化处理
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGeneral::NodeInit()
{
    //组装数据包
    ndStatus  ret = MakeNodeInitReq();
    if(ret != ND_SUCCESS)
	{
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::NodeEnvSet] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }
	//数据包发送
	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::NodeEnvSet] PkgSendAndRecv Err ret=[%d]", ret);
		return ret;
	}
	//数据包解析
    ret = AnalysisNodeInitRsp();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::NodeEnvSet] AnalyzeResponsePkg Err ret=[%d]", ret);
        return ret;
    }

	//进行nodeid的存储操作
	AfxWriteNodeID(mPNode->GetNodeInform().sNodeID.c_str());

    return ND_SUCCESS;
}

/*********************************************************
函数说明：节点配置处理
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGeneral::NodeEnvSet()
{
    //组装数据包
    ndStatus  ret = MakeNodeEnvSetReq();
    if(ret != ND_SUCCESS)
	{
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::NodeEnvSet] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }

	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::NodeEnvSet] PkgSendAndRecv Err ret=[%d]", ret);
		return ret;
	}

    ret = AnalysisNodeEnvSetRsp();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::NodeEnvSet] AnalyzeResponsePkg Err ret=[%d]", ret);
        return ret;
    }

    return ND_SUCCESS;
	
}


/*********************************************************
函数说明：结点Hello处理
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGeneral::NodeHello()
{
    //组装数据包
    ndStatus  ret = MakeNodeHelloReq();
    if(ret != ND_SUCCESS)
	{
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::NodeHello] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }

	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::NodeHello] PkgSendAndRecv Err ret=[%d]", ret);
		return ret;
	}

    ret = AnalysisNodeHelloRsp();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::NodeHello] AnalyzeResponsePkg Err ret=[%d]", ret);
        return ret;
    }

    return ND_SUCCESS;
}

/*********************************************************
函数说明：数据包发送与接收
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGeneral::PkgSendAndRecv(ndString url)
{
	ndString sSrvUrl;
    //判断服务器地址
    SServerInfo serverInfo = *gITCurServer;
	sSrvUrl =  "http://"+serverInfo.sServerIP+":"+serverInfo.sServerPort+url;
	
    if(sSrvUrl.empty()){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::PkgSendAndRecv] Http Server Empty Err");
        return ND_ERROR_INVALID_PARAM;
    }	

    //判断发送的数据包
    if(mSendBuf.empty()){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::PkgSendAndRecv] SendBuff Is Empty");
        return ND_ERROR_INVALID_PARAM;
    }

    AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::PkgSendAndRecv] Begin post data\n ServerURL=[%s]\n [%s]", sSrvUrl.c_str(), mSendBuf.c_str());
rePost:
    //发送服务端并接收返回
    CURLcode rtn = Post(sSrvUrl.c_str(), mSendBuf.c_str(), mRcvBuf);
    if(rtn != CURLE_OK){
        while(AfxGetNextSrvUrl(serverInfo)){
			sSrvUrl =  "http://"+serverInfo.sServerIP+":"+serverInfo.sServerPort+url;
            rtn = Post(sSrvUrl.c_str(), mSendBuf.c_str(), mRcvBuf);
            if (rtn == CURLE_OK) {
                break;
            }
        }
    }	

    if(rtn != CURLE_OK){
        AfxWriteDebugLog("SuperVPN run at[CNodeSrvBase::DealActionWithModel] Http Post Err");
        switch (rtn){
            case CURLE_COULDNT_CONNECT:
            case CURLE_OPERATION_TIMEDOUT:
            case CURLE_COULDNT_RESOLVE_HOST:
            case CURLE_SEND_ERROR:
                return ND_ERROR_SEND_FAILED;
            case CURLE_RECV_ERROR:
                return ND_ERROR_RECEIVE_FAILED;
            case CURLE_SSL_CACERT:
            case CURLE_SSL_CACERT_BADFILE:
                return ND_ERROR_LOAD_CA_CERTIFICATE;
            default:
                return ND_ERROR_INVALID_PARAM;
        }
    }

	gITBakServer = gITCurServer;

    //http服务器返回码判断
    if (GetHttpReturnCode() != 200){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::PkgSendAndRecv] Http Server Response Code=[%d]", GetHttpReturnCode());
        return ND_ERROR_INVALID_RESPONSE;
    }

    //返回数据的分析处理
    AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::PkgSendAndRecv] Begin Analyze Response\n[%s]", mRcvBuf.c_str());
    if(mRcvBuf.empty()){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::PkgSendAndRecv] Server response data is null");
        return ND_ERROR_INVALID_RESPONSE;
    }

	return ND_SUCCESS;
}

/*********************************************************
函数说明：解析http头部信息
入参说明：head-头部信息
出参说明：无
返回值  ：无
*********************************************************/
void CHttpGeneral::AnalysisAuthHeader(ndString &head)
{

}

/*********************************************************
函数说明：生成校验头
入参说明：无
出参说明：无
返回值  ：校验信息
*********************************************************/
ndString CHttpGeneral::GenerateAuthHeader()
{
    return NULL;
}