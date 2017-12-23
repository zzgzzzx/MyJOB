/*********************************************************
模块名       : 用户节点类(Certificate\DP)
文件名       : NodeGateway.cpp
相关文件     : NodeGateway.h
文件实现功能 : 策略路由类的功能
作者         :lewis
创建时间     :2017-09
**********************************************************/
#include "NodeGateway.hpp"
#include "HttpGateway.hpp"
#include "NDFunc.hpp"

/*********************************************************
函数说明：构造函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CNodeGateway::CNodeGateway()
{
	mPHttpClient = new CHttpGateway(this);
}

/*********************************************************
函数说明：析构函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CNodeGateway::~CNodeGateway()
{
}

/*********************************************************
函数说明：请求设备策略路由信息
入参说明：无
出参说明：无
返回值  ：无
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
函数说明：设置策略路由
入参说明：无
出参说明：无
返回值  ：无
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
函数说明：设置策略路由信息
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
void CNodeGateway::SetRouteInf(list<SRouteInform> &routeInf)
{
	mRouteInf = routeInf;
}


