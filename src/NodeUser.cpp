/*********************************************************
ģ����       : �û��ڵ���(Certificate\DP)
�ļ���       : NodeUser.cpp
����ļ�     : NodeUser.h
�ļ�ʵ�ֹ��� : �û��ڵ���Ĺ���
����         :lewis
����ʱ��     :2017-09
**********************************************************/
#include "NodeUser.hpp"
#include "HttpUserNode.hpp"
#include "NDFunc.hpp"
#include "NodeGateway.hpp"

/*********************************************************
����˵�������캯��
���˵������
����˵������
����ֵ  ����
*********************************************************/
CNodeUser::CNodeUser()
{
	mPHttpClient = new CHttpUserNode(this);
	mIPTableIndex = 0;
}

/*********************************************************
����˵������������
���˵������
����˵������
����ֵ  ����
*********************************************************/
CNodeUser::~CNodeUser()
{
}

/*********************************************************
����˵��������ʼ���������������������Ϣ
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CNodeUser::NodeEnvSet()
{
	//=============================================================================
    //���ò���(��ȡ���ؿ��õ�ip��ַ����Ϣ\supernode��Ϣ)
    //=============================================================================
    ndStatus ret = mPHttpClient->NodeEnvSet();
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::NodeEnvSet] Node Evn set error=[%d]", ret);
		return ret;
	}

	//=============================================================================
	//��ȡ�����豸MAC�����ID��Ӧ�Ĺ�ϵ(���������ݿ��ȡ),���������ݵĳ�ʼ��
	//=============================================================================
	if (!AfxGetIdentifySet()->InitIdentifyFromGW())
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::NodeEnvSet] ReadMacIdentifyFromGW");
		return ND_ERROR_INVALID_PARAM;	
	}

	return ND_SUCCESS;
}

/*********************************************************
����˵���������û�����������й���
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CNodeUser::SetEdgeAndRoute(list<SBindService> ltBSer)
{
	//lewis
	SBindService sRI;
	SDeviceFlag deviceFlag;

	if (!mNodeGateway.GetRouteInf(deviceFlag.sDeviceFlag, sRI))
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::SetPolicyRoute] GetRouteInf Not Found");
		return ND_ERROR_INVALID_PARAM;
	}

	char ExecCMD[1024] = {0};		
	sprintf(ExecCMD, "iptables -t mangle -A PREROUTING -s %s -j MARK --set-mark %d", deviceFlag.sDeviceIP.c_str(), mIPTableIndex);
	//AfxWriteDebugLog("SuperVPN run at [CNodeGateway::SetPolicyRoute] ExecCmd=[%s]", ExecCMD);
	AfxExecCmd(ExecCMD);

		sprintf(ExecCMD, "ip rule add fwmark %d table %d", mIPTableIndex, mIPTableIndex);
	//AfxWriteDebugLog("SuperVPN run at [CNodeGateway::SetPolicyRoute] ExecCmd=[%s]", ExecCMD);
	AfxExecCmd(ExecCMD);

		sprintf(ExecCMD, "ip route add 0/0 via %s table %d", sRI.sServiceIP.c_str(), mIPTableIndex);
	//AfxWriteDebugLog("SuperVPN run at [CNodeGateway::SetPolicyRoute] ExecCmd=[%s]", ExecCMD);
	AfxExecCmd(ExecCMD);

	mIPTableIndex++;

	return ND_SUCCESS;
}

/*********************************************************
����˵���������ķ��������������豸��ݶ�Ӧ�ĳ�����Ϣ
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CNodeUser::BindIdentifyService(list<SBindInform> &ltBSer)
{
	//��ȡ�������ڵİ���Ϣ	
    ndStatus ret = mPHttpClient->GetIdentifyService(ltBSer);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::GetIdentifyService] Get Services error=[%d]", ret);
		return ret;
	}

	//=============================================================================
	//�������ϻ�ȡmac��ip�Ķ�Ӧ��ϵ������vpnͨ��
	//=============================================================================	
	AfxGetIdentifySet()->ReadARP(ltBSer);

	//=============================================================================
	//����vpnͨ�����¶�Դ��ַ����·��
	//=============================================================================	
	SetEdgeAndRoute(ltBSer);
}


