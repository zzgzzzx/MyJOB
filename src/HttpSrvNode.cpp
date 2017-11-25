/*********************************************************
ģ����       : �ڵ�httpЭ��
�ļ���       : HttpSrvNode.cpp
����ļ�     : HttpSrvNode.h
�ļ�ʵ�ֹ��� : �ڵ�HtppЭ�鴦��
����         :lewis
����ʱ��     :2017-09
**********************************************************/
#include "HttpSrvNode.hpp"
#include "cJSON.h"
#include "NDFunc.hpp"

/*********************************************************
����˵�������캯��
���˵����Node(���)
����˵������
����ֵ  ����
*********************************************************/
CHttpSrvNode::CHttpSrvNode(CNodeBase *node):CHttpGeneral(node)
{
	mSrvURL = VPN_CENTER_SRV_URL;
}

/*********************************************************
����˵�����ڵ�Hello
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpSrvNode::MakeNodeHelloReq()
{
    char *out;
    cJSON *root, *fmt, *actions;

    //��װ��Ϣ��
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "node", fmt=cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "version",	SUPER_VPN_CLIENT_VER);
    cJSON_AddStringToObject(fmt, "mac", mPNode->GetNodeInform().sNodeMac.c_str());

    AfxWriteDebugLog("SuperVPN run at [CHttpSrvNode::MakeNodeHelloReq] Make Hello actions");

    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    //========================set===========================================
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_SERVER_NODE_HELLO);
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
ndStatus CHttpSrvNode::AnalysisNodeHelloRsp()
{
	AfxWriteDebugLog("SuperVPN run at [CHttpSrvNode::AnalysisNodeHelloRsp] Recv Hello actions");
	return ND_SUCCESS;
}

/*********************************************************
����˵�����ڵ��ʼ��
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpSrvNode::MakeNodeInitReq()
{
    char *out;
    cJSON *root, *fmt, *actions;

    //��װ��Ϣ��
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "node", fmt=cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "version",	SUPER_VPN_CLIENT_VER);
    cJSON_AddStringToObject(fmt, "mac", mPNode->GetNodeInform().sNodeMac.c_str());

    AfxWriteDebugLog("SuperVPN run at [CHttpSrvNode::MakeNodeInitReq] Make Init actions");

    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    //========================set===========================================
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_SERVER_NODE_SET);
	cJSON_AddStringToObject(fmt, "arugments", "");

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);

    return ND_SUCCESS;
}