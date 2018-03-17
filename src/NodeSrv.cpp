/*********************************************************
模块名       : 服务节点类(Certificate\DP)
文件名       : NodeSrv.cpp
相关文件     : NodeSrv.h
文件实现功能 : 服务节点实现
作者         :lewis
创建时间     :2017-09
**********************************************************/
#include "NodeSrv.hpp"
#include "HttpSrvNode.hpp"
#include "NDFunc.hpp"

/*********************************************************
函数说明：构造函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CNodeSrv::CNodeSrv()
{
	mPHttpClient = new CHttpSrvNode(this);
	mNoRecvHelloTime = 0;
}

/*********************************************************
函数说明：析构函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CNodeSrv::~CNodeSrv()
{
}

/*********************************************************
函数说明：节点Hello
入参说明：无
出参说明：无
返回值  ：ND_SUCCESS-成功
          Others-失败
*********************************************************/
ndStatus CNodeSrv::NodeHello()
{
    ndStatus ret = CNodeGeneral::NodeHello();
    if(ret != ND_SUCCESS)
		mNoRecvHelloTime++;
	else
		mNoRecvHelloTime = 0;

	if(mNoRecvHelloTime >= MAX_VALUE_HELLO_CHECK_TIMES)
		ret = ND_ERROR_NOT_RECVIVE_HELLO;

	return ret;
}

/*********************************************************
函数说明：设置接收hello时需要增加的新的域(用于服务节点)
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
void CNodeSrv::DealHelloAddNewDomain(list<SDomain> &domains)
{
	SDomain domain,domainF;
	pthread_t pid;
	int ret;
	char *ExecCMD;

	list<SDomain>::iterator iterDomain;
	SSupperNode superNode = mSNodeInform.mSupperNode[0];
    for(iterDomain=domains.begin(); iterDomain!=domains.end(); iterDomain++)
    {
    	domain = *iterDomain;

		if (mSNodeInform.mDomainMap.find(domain.sDomain, domainF))
		{
			AfxWriteDebugLog("SuperVPN run at [CNodeSrv::DealHelloAddNewDomain] Hello Add New Domain Already Exists^_^");
			continue;
		}

		ExecCMD = (char*)calloc(1, 1024);
		sprintf(ExecCMD, "edge -d node%d -a %s -s %s -c %s -k %s -l %s:%s -f -r",
					mSNodeInform.mDomainInforms.size(),
					domain.lNodeIP.c_str(),
					domain.lMask.c_str(),
					domain.sDomain.c_str(),
					domain.sDomainKey.c_str(),
					superNode.sSuperNodeIP.c_str(),
					superNode.sSuperNodePort.c_str());
		
	    ret = pthread_create(&pid, NULL, &CNodeGeneral::ThreadFunction, (void *)ExecCMD);
	    if(ret != 0)
	    {
	        AfxWriteDebugLog("SuperVPN run at [CNodeSrv::DealHelloAddNewDomain] pthread_create ERROR");
	        return;
	    }
		mSNodeInform.mDomainInforms.push_back(domain);
		mSNodeInform.mDomainMap.insert(domain.sDomain, domain);
    }

}


