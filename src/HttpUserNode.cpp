/*********************************************************
ģ����       : ���httpЭ��
�ļ���       : HttpUserNode.cpp
����ļ�     : HttpUserNode.h
�ļ�ʵ�ֹ��� : ���HtppЭ�鴦��
����         :lewis
����ʱ��     :2017-09
**********************************************************/
#include "HttpUserNode.hpp"
#include "cJSON.h"
#include "NDFunc.hpp"


/*********************************************************
����˵�������캯��
���˵����Node(���)
����˵������
����ֵ  ����
*********************************************************/
CHttpUserNode::CHttpUserNode(CNodeBase *node):CHttpGeneral(node)
{
	mSrvURL = VPN_CENTER_USER_URL;
}

/*********************************************************
����˵�����ڵ�Hello
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpUserNode::MakeNodeHelloReq()
{
    char *out;
    cJSON *root, *fmt, *actions;

    //��װ��Ϣ��
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "node", fmt=cJSON_CreateObject());
	cJSON_AddNumberToObject(fmt, "version",	SUPER_VPN_CLIENT_VER_NODE);
    cJSON_AddStringToObject(fmt, "mac", mPNode->GetNodeInform().sNodeMac.c_str());
    AfxWriteDebugLog("SuperVPN run at [CHttpUserNode::MakeNodeHelloReq] Make Hello actions");
    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    //========================set===========================================
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_USER_NODE_HELLO);
	cJSON_AddStringToObject(fmt, "arugments", "");

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);
	
	return ND_SUCCESS;
}

/*********************************************************
����˵�����ڵ�Hello���ش���
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpUserNode::AnalysisNodeHelloRsp()
{
	AfxWriteDebugLog("SuperVPN run at [CHttpUserNode::AnalysisNodeHelloRsp] Recv Hello actions");

	return ND_SUCCESS;
}

/*********************************************************
����˵�����ڵ��ʼ��
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpUserNode::MakeNodeInitReq()
{
    char *out;
    cJSON *root, *fmt, *actions;

    //��װ��Ϣ��
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "node", fmt=cJSON_CreateObject());
	cJSON_AddNumberToObject(fmt, "version",	SUPER_VPN_CLIENT_VER_NODE);
    cJSON_AddStringToObject(fmt, "mac", mPNode->GetNodeInform().sNodeMac.c_str());
    AfxWriteDebugLog("SuperVPN run at [CHttpUserNode::MakeNodeInitReq] Make Init actions");
    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    //========================set===========================================
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_USER_NODE_SET);
	cJSON_AddStringToObject(fmt, "arugments", "");

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);

    return ND_SUCCESS;
}
