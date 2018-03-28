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
	//=============================================================================
    //配置操作(获取网关可用的ip地址段信息\supernode信息)
    //=============================================================================
    ndStatus ret = mPHttpClient->NodeEnvSet();
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::NodeEnvSet] Node Evn set error=[%d]", ret);
		return ret;
	}

	//=============================================================================
	//获取下游设备MAC与身份ID对应的关系(从网关数据库获取),并进行数据的初始化
	//=============================================================================
	if (!AfxGetIdentifySet()->InitIdentifyFromGW())
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::NodeEnvSet] ReadMacIdentifyFromGW");
		return ND_ERROR_INVALID_PARAM;	
	}

	return ND_SUCCESS;
}

/*********************************************************
函数说明：根据用户的特征码进行关联
入参说明：无
出参说明：无
返回值  ：无
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
函数说明：向中心服务器请求下游设备身份对应的出口信息
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CNodeUser::BindIdentifyService(list<SBindInform> &ltBSer)
{
	//获取身份与出口的绑定信息	
    ndStatus ret = mPHttpClient->GetIdentifyService(ltBSer);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::GetIdentifyService] Get Services error=[%d]", ret);
		return ret;
	}

	//=============================================================================
	//在网关上获取mac与ip的对应关系，配置vpn通道
	//=============================================================================	
	AfxGetIdentifySet()->ReadARP(ltBSer);

	//=============================================================================
	//配置vpn通道与下端源地址策略路由
	//=============================================================================	
	SetEdgeAndRoute(ltBSer);
}


