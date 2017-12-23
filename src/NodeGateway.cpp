/*********************************************************
ģ����       : �û��ڵ���(Certificate\DP)
�ļ���       : NodeGateway.cpp
����ļ�     : NodeGateway.h
�ļ�ʵ�ֹ��� : ����·����Ĺ���
����         :lewis
����ʱ��     :2017-09
**********************************************************/
#include "NodeGateway.hpp"
#include "HttpGateway.hpp"
#include "NDFunc.hpp"

/*********************************************************
����˵�������캯��
���˵������
����˵������
����ֵ  ����
*********************************************************/
CNodeGateway::CNodeGateway()
{
	mPHttpClient = new CHttpGateway(this);
}

/*********************************************************
����˵������������
���˵������
����˵������
����ֵ  ����
*********************************************************/
CNodeGateway::~CNodeGateway()
{
}

/*********************************************************
����˵���������豸����·����Ϣ
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CNodeGateway::RouteSetting()
{
	CHttpGateway *mHttpGate = dynamic_cast<CHttpGateway *>(mPHttpClient);

 ReInit:
    ndStatus ret = mHttpGate->GetRouteSetting();
	
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeGateway::RouteSetting] error=[%d]", ret);
		goto ReInit;
	}

	return SetPolicyRoute();	

}

/*********************************************************
����˵�������ò���·��
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CNodeGateway::SetPolicyRoute()
{
	//iptables -t mangle -A PREROUTING -s 10.11.12.11 -j MARK --set-mark 100
	//ip rule add fwmark 100 table 100
	//ip route add 0/0 via 11.2.3.1 table 100

	int iTableIndex=1;

	if (mRouteInf.size() <= 0)
	{
        AfxWriteDebugLog("SuperVPN run at [CNodeGateway::SetPolicyRoute] RouteInf size error");
        return ND_ERROR_INVALID_RESPONSE;		
	}

	char ExecCMD[1024] = {0};

	list<SRouteInform>::iterator iterRoute;
    for(iterRoute=mRouteInf.begin(); iterRoute!=mRouteInf.end(); iterRoute++)
    {
    	SRouteInform route = *iterRoute;
		
 		sprintf(ExecCMD, "iptables -t mangle -A PREROUTING -s %s -j MARK --set-mark %d", route.sDeviceIP.c_str(), iTableIndex);
		AfxWriteDebugLog("SuperVPN run at [CNodeGateway::SetPolicyRoute] ExecCmd=[%s]", ExecCMD);
		AfxExecCmd(ExecCMD);

 		sprintf(ExecCMD, "ip rule add fwmark %d table %d", iTableIndex, iTableIndex);
		AfxWriteDebugLog("SuperVPN run at [CNodeGateway::SetPolicyRoute] ExecCmd=[%s]", ExecCMD);
		AfxExecCmd(ExecCMD);

 		sprintf(ExecCMD, "ip route add 0/0 via %s table %d", route.sServiceIP.c_str(), iTableIndex);
		AfxWriteDebugLog("SuperVPN run at [CNodeGateway::SetPolicyRoute] ExecCmd=[%s]", ExecCMD);
		AfxExecCmd(ExecCMD);

		iTableIndex++;
    }
			

	return ND_SUCCESS;
}


/*********************************************************
����˵�������ò���·����Ϣ
���˵������
����˵������
����ֵ  ����
*********************************************************/
void CNodeGateway::SetRouteInf(list<SRouteInform> &routeInf)
{
	mRouteInf = routeInf;
}


