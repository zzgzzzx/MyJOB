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
ndStatus CNodeUser::NodeInit()
{
	ndStatus ret = CNodeGeneral::NodeInit();

	if (ret != ND_SUCCESS) return ret;

	CNodeGateway nodeGateway;
    ret = nodeGateway.RouteSetting();
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeUser::NodeInit] RouteSetting error=[%d]", ret);
	}	

	return ret;
}

