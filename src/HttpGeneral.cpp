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
	SNodeInform sNode;
	//解析返回的数据到NdoeInform

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

			sNode.lHelloTime = cJSON_GetObjectItem(repliceslist, "hellotime")->valueint;
			sNode.lRestartTime = cJSON_GetObjectItem(repliceslist, "restarttime")->valueint;
				
            cJSON *supernode = cJSON_GetObjectItem(repliceslist, "supernode");
            if(supernode != NULL)
            {
                    cJSON *supernodelist = supernode->child;

                    AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] Get SuperNode Informs");
                    SSupperNode item;
                    while(supernodelist != NULL)
                    {
                        if(cJSON_GetObjectItem(supernodelist, "nodeip") != NULL &&
                           cJSON_GetObjectItem(supernodelist, "nodeip")->valuestring != NULL)
                            item.sSuperNodeIP = cJSON_GetObjectItem(supernodelist, "nodeip")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] supernode ip=[%s]",item.sSuperNodeIP.c_str());

                        if(cJSON_GetObjectItem(supernodelist, "nodeport") != NULL &&
                           cJSON_GetObjectItem(supernodelist, "nodeport")->valuestring != NULL)
                            item.sSuperNodePort = cJSON_GetObjectItem(supernodelist, "nodeport")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] supernode port=[%s]",item.sSuperNodePort.c_str());

                        sNode.mSupperNode.push_back(item);

                        supernodelist = supernodelist->next;
                    }
                }

            cJSON *domains = cJSON_GetObjectItem(repliceslist, "domains");
            if(domains != NULL)
            {
                    cJSON *domainslist = domains->child;

                    AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] Get Domains");
                    SDomain item;
                    while(domainslist != NULL)
                    {
                        if(cJSON_GetObjectItem(domainslist, "domainid") != NULL &&
                           cJSON_GetObjectItem(domainslist, "domainid")->valuestring != NULL)
                            item.sDomain = cJSON_GetObjectItem(domainslist, "domainid")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] domain id=[%s]",item.sDomain.c_str());

                        if(cJSON_GetObjectItem(domainslist, "ip") != NULL &&
                           cJSON_GetObjectItem(domainslist, "ip")->valuestring != NULL)
                            item.lNodeIP = cJSON_GetObjectItem(domainslist, "ip")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] domain ip=[%s]",item.lNodeIP.c_str());

                        if(cJSON_GetObjectItem(domainslist, "mask") != NULL &&
                           cJSON_GetObjectItem(domainslist, "mask")->valuestring != NULL)
                            item.lMask = cJSON_GetObjectItem(domainslist, "mask")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] domain mask=[%s]",item.lMask.c_str());

                        if(cJSON_GetObjectItem(domainslist, "key") != NULL &&
                           cJSON_GetObjectItem(domainslist, "key")->valuestring != NULL)
                            item.sDomainKey = cJSON_GetObjectItem(domainslist, "key")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisNodeEnvSetRsp] domain key=[%s]",item.sDomainKey.c_str());

                        sNode.mDomainInforms.push_back(item);

                        domainslist = domainslist->next;
                    }
			
            }
        }

        cJSON_Delete(root);
    }	

	mPNode->SetNodeInform(sNode);
	
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
    //判断服务器地址
    if(url.empty()){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::PkgSendAndRecv] Http Server Empty Err");
        return ND_ERROR_INVALID_PARAM;
    }	

    //判断发送的数据包
    if(mSendBuf.empty()){
        AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::PkgSendAndRecv] SendBuff Is Empty");
        return ND_ERROR_INVALID_PARAM;
    }

    AfxWriteDebugLog("SuperVPN run at[CHttpGeneral::PkgSendAndRecv] Begin post data\n ServerURL=[%s]\n [%s]", url.c_str(),mSendBuf.c_str());
    rePost:
    //发送服务端并接收返回
    CURLcode rtn = Post(url.c_str(), mSendBuf.c_str(), mRcvBuf);

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