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

/*********************************************************
函数说明：构造函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CNodeUser::CNodeUser()
{
	mPHttpClient = new CHttpUserNode(this);
	AfxGetEthMac("eth0", mSNodeInform.sNodeMac);	
}

/*********************************************************
函数说明：析构函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CNodeUser::~CNodeUser()
{
	delete mPHttpClient;
}
