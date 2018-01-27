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

	CNodeGateway nodeGateway;
    ret = nodeGateway.RouteSetting();
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::NodeEnvSet] RouteSetting error=[%d]", ret);
	}	

	return ret;
}

