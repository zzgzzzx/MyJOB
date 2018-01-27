/*********************************************************
模块名       : 结点http协议
文件名       : HttpGateway.cpp
相关文件     : HttpGateway.hpp
文件实现功能 : 结点Htpp协议处理
作者         :lewis
创建时间     :2017-09
**********************************************************/
#include "HttpGateway.hpp"
#include "cJSON.h"
#include "NDFunc.hpp"
#include "NodeGateway.hpp"



/*********************************************************
函数说明：构造函数
入参说明：Node(结点)
出参说明：无
返回值  ：无
*********************************************************/
CHttpGateway::CHttpGateway(CNodeBase *node):CHttpGeneral(node)
{
	mSrvURL = VPN_CENTER_GW_URL;
}

/*********************************************************
函数说明：策略路由信息请求
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGateway::MakeRouteSetting()
{
    char *out;
    cJSON *root, *fmt, *actions;

    //组装消息体
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "node", fmt=cJSON_CreateObject());
	cJSON_AddNumberToObject(fmt, "version",	SUPER_VPN_CLIENT_VER_NODE);    
    cJSON_AddStringToObject(fmt, "nodeid", mPNode->GetNodeInform().sNodeID.c_str());
    AfxWriteDebugLog("SuperVPN run at [CHttpGateway::MakeRouteSetting] Make RouteSetting actions");
    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    //========================set===========================================
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_USER_NODE_ROUTE);
	cJSON_AddStringToObject(fmt, "arugments", "");

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);
	
	return ND_SUCCESS;
}

/*********************************************************
函数说明：策略路由返回解析
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGateway::AnalysisRouteSetting()
{
    cJSON *root;
	int iErrCode;
	list<SRouteInform> routeInfo;
	
	AfxWriteDebugLog("SuperVPN run at [CHttpGateway::AnalysisRouteSetting] Recv Route actions");

    root = cJSON_Parse(mRcvBuf.c_str());
    if (!root)
    {
        AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisRouteSetting] Error before: [%s]", cJSON_GetErrorPtr());
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

            cJSON *services = cJSON_GetObjectItem(repliceslist, "services");
            if(services != NULL)
            {
                    cJSON *serviceslist = services->child;

                    AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisRouteSetting] Get SuperNode Informs");
                    SRouteInform item;
                    while(serviceslist != NULL)
                    {
                        if(cJSON_GetObjectItem(serviceslist, "devicesip") != NULL &&
                           cJSON_GetObjectItem(serviceslist, "devicesip")->valuestring != NULL)
                            item.sDeviceIP = cJSON_GetObjectItem(serviceslist, "devicesip")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisRouteSetting] deviceip=[%s]",item.sDeviceIP.c_str());

                        if(cJSON_GetObjectItem(serviceslist, "serviceip") != NULL &&
                           cJSON_GetObjectItem(serviceslist, "serviceip")->valuestring != NULL)
                            item.sServiceIP = cJSON_GetObjectItem(serviceslist, "serviceip")->valuestring;
						AfxWriteDebugLog("SuperVPN run at [CHttpGeneral::AnalysisRouteSetting] ServiceIP=[%s]",item.sServiceIP.c_str());

                        routeInfo.push_back(item);
                        serviceslist = serviceslist->next;
                    }
                }
        }

        cJSON_Delete(root);
    }		

	CNodeGateway *mPGete = dynamic_cast<CNodeGateway *>(mPNode);
	mPGete->SetRouteInf(routeInfo);

	return ND_SUCCESS;
}

/*********************************************************
函数说明：GetRouteSetting
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGateway::GetRouteSetting()
{
    //组装数据包
    ndStatus  ret = MakeRouteSetting();
    if(ret != ND_SUCCESS)
	{
        AfxWriteDebugLog("SuperVPN run at[CHttpGateway::GetRouteSetting] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }

	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at[CHttpGateway::GetRouteSetting] PkgSendAndRecv Err ret=[%d]", ret);
		return ret;
	}

    ret = AnalysisRouteSetting();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpGateway::GetRouteSetting] AnalyzeResponsePkg Err ret=[%d]", ret);
        return ret;
    }

    return ND_SUCCESS;;
}
