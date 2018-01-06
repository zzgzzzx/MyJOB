/*********************************************************
ģ����       : �ڵ�httpЭ��
�ļ���       : HttpUpdateCK.cpp
����ļ�     : HttpUpdateCK.h
�ļ�ʵ�ֹ��� : �ڵ�HtppЭ�鴦��
����         :lewis
����ʱ��     :2017-09
**********************************************************/
#include "HttpRunEnvCKSrv.hpp"
#include "cJSON.h"
#include "NDFunc.hpp"
#include "HttpFileDown.hpp"
#include "md5.h"

/*********************************************************
����˵�������캯��
���˵����Node(���)
����˵������
����ֵ  ����
*********************************************************/
CHttpRunEvnCKSrv::CHttpRunEvnCKSrv(CNodeBase *node):CHttpRunEvnCK(node)
{
	mSrvURL = VPN_CENTER_SRV_URL;
	mLocalVersion = SUPER_VPN_CLIENT_VER_SERVER;
}

/*********************************************************
����˵����iptable���
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpRunEvnCKSrv::IPTableCheck()
{
	ndStatus ret = CHttpRunEvnCK::IPTableCheck();
	if (ret != ND_SUCCESS) return ret;

	AfxExecCmd("echo \"1\"> /proc/sys/net/ipv4/ip_forward");
	AfxExecCmd("iptables -t nat -A POSTROUTING -o ppp0 -j MASQUERADE");
	
	return ND_SUCCESS;
}

