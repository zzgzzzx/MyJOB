/*********************************************************
ģ����       : ����ڵ���(Certificate\DP)
�ļ���       : NodeSrv.cpp
����ļ�     : NodeSrv.h
�ļ�ʵ�ֹ��� : ����ڵ�ʵ��
����         :lewis
����ʱ��     :2017-09
**********************************************************/
#include "NodeSrv.hpp"
#include "HttpSrvNode.hpp"
#include "NDFunc.hpp"

/*********************************************************
����˵�������캯��
���˵������
����˵������
����ֵ  ����
*********************************************************/
CNodeSrv::CNodeSrv()
{
	mPHttpClient = new CHttpSrvNode(this);
	mNoRecvHelloTime = 0;
}

/*********************************************************
����˵������������
���˵������
����˵������
����ֵ  ����
*********************************************************/
CNodeSrv::~CNodeSrv()
{
}

/*********************************************************
����˵�����ڵ�Hello
���˵������
����˵������
����ֵ  ��ND_SUCCESS-�ɹ�
          Others-ʧ��
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
����˵�����ڵ㻷������
���˵������
����˵������
����ֵ  ��DP_SUCCESS-�ɹ�
          Others-ʧ��
*********************************************************/
ndStatus CNodeSrv::NodeEnvSet()
{
    //���ò���
    ndStatus ret = mPHttpClient->NodeEnvSet();
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeSrv::NodeEnvSet] Node Evn set error=[%d]", ret);
		return ret;
	}

	return SetN2NVPNNetwork();
}


/*********************************************************
����˵�������ݷ��ص������������vpn����
���˵������
����˵������
����ֵ  ��DP_SUCCESS-�ɹ�
          Others-ʧ��
*********************************************************/
ndStatus CNodeSrv::SetN2NVPNNetwork()
{
	//edge -d node0 -a 11.2.3.33 -s 255.255.0.0 -c vpntest -k vpntest -l lja1.zrdx.com:5678 -f -r
	int iNodeIndex=0;

	if (mSNodeInform.mSupperNode.size() <= 0)
	{
        AfxWriteDebugLog("SuperVPN run at [CNodeSrv::SetN2NVPNNetwork] Reponse supper size error");
        return ND_ERROR_INVALID_RESPONSE;		
	}
	SSupperNode superNode = mSNodeInform.mSupperNode[0];

	list<SDomain>::iterator iterDomain;
    for(iterDomain=mSNodeInform.mDomainInforms.begin(); iterDomain!=mSNodeInform.mDomainInforms.end(); iterDomain++)
    {
    	SDomain domain = *iterDomain;

		char *ExecCMD = (char*)calloc(1, 1024);
		sprintf(ExecCMD, "edge -d node%d -a %s -s %s -c %s -k %s -l %s:%s -f -r",
					iNodeIndex,
					domain.lNodeIP.c_str(),
					domain.lMask.c_str(),
					domain.sDomain.c_str(),
					domain.sDomainKey.c_str(),
					superNode.sSuperNodeIP.c_str(),
					superNode.sSuperNodePort.c_str());
		
	    pthread_t id;
	    int ret = pthread_create(&id, NULL, &CNodeGeneral::ThreadFunction, (void *)ExecCMD);
	    if(ret != 0)
	    {
	        AfxWriteDebugLog("SuperVPN run at [CNodeSrv::SetN2NVPNNetwork] pthread_create ERROR");
	        return ND_ERROR_PLATFORM_OS;
	    }
		iNodeIndex++;
    }

	return ND_SUCCESS;
}



/*********************************************************
����˵�������ý���helloʱ��Ҫ���ӵ��µ���(���ڷ���ڵ�)
���˵������
����˵������
����ֵ  ����
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


