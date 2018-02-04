/*********************************************************
模块名       : 用户节点类(Certificate\DP)
文件名       : NodeUser.cpp
相关文件     : NodeUser.h
文件实现功能 : 用户节点类的功能
作者         :lewis
创建时间     :2017-09
**********************************************************/
#include "NodeUser.hpp"
#include "HttpUserNode.hpp"
#include "NDFunc.hpp"
#include "NodeGateway.hpp"

/*********************************************************
函数说明：构造函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CNodeUser::CNodeUser()
{
	mPHttpClient = new CHttpUserNode(this);
	mIPTableIndex = 0;
}

/*********************************************************
函数说明：析构函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CNodeUser::~CNodeUser()
{
}

/*********************************************************
函数说明：结点初始化，向服务器申请配置信息
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CNodeUser::NodeEnvSet()
{
	ndStatus ret = CNodeGeneral::NodeEnvSet();

	if (ret != ND_SUCCESS) return ret;
	
    ret = mNodeGateway.RouteSetting();
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::NodeEnvSet] RouteSetting error=[%d]", ret);
	}	

	return ret;
}

//根据用户的特征码进行关联
/*********************************************************
函数说明：结点初始化，向服务器申请配置信息
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CNodeUser::SetPolicyRoute(SDeviceFlag deviceFlag)
{
	SRouteInform sRI;

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


