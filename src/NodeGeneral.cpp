/*********************************************************
ģ����       : ���������������(Certificate\DP)
�ļ���       : ClientSrvBase.cpp
����ļ�     : ClientSrvBase.h
�ļ�ʵ�ֹ��� : ʵ��������������Ĺ���
����         :lewis
����ʱ��     :2017-09
**********************************************************/
#include "NodeGeneral.hpp"
#include "HttpGeneral.hpp"
#include "NDFunc.hpp"


/*********************************************************
����˵�������캯��
���˵������
����˵������
����ֵ  ����
*********************************************************/
CNodeGeneral::CNodeGeneral()
{
	mPHttpClient = NULL;
	AfxGetEthMac("eth0", mSNodeInform.sNodeMac);	
	//mSNodeInform.sNodeMac = "aa:bb:cc:dd:ee:ff";
}

/*********************************************************
����˵������������
���˵������
����˵������
����ֵ  ����
*********************************************************/
CNodeGeneral::~CNodeGeneral()
{
	if(mPHttpClient != NULL) delete mPHttpClient;
}

/*********************************************************
����˵������ɵ����������������
���˵������
����˵������
����ֵ  ��DP_SUCCESS-�ɹ�
          Others-ʧ��
*********************************************************/
ndStatus CNodeGeneral::InitData()
{
    return ND_SUCCESS;
}

/*********************************************************
����˵�����̴߳�����
���˵������
����˵������
����ֵ  ��DP_SUCCESS-�ɹ�
          Others-ʧ��
*********************************************************/
void *CNodeGeneral::ThreadFunction(void *cmd)
{
    pthread_detach(pthread_self());

	char *ExecCMD = (char*)(cmd);
	AfxExecCmd(ExecCMD);
	free(ExecCMD);

    AfxWriteDebugLog("SuperVPN run at [%s] Thread End", __FUNCTION__);
    pthread_exit(0);
}


/*********************************************************
����˵�������ݷ��ص������������vpn����
���˵������
����˵������
����ֵ  ��DP_SUCCESS-�ɹ�
          Others-ʧ��
*********************************************************/
ndStatus CNodeGeneral::SetN2NVPNNetwork()
{
	//edge -d node0 -a 11.2.3.33 -s 255.255.0.0 -c vpntest -k vpntest -l lja1.zrdx.com:5678 -f -r
	int iNodeIndex=0;

	if (mSNodeInform.mSupperNode.size() <= 0)
	{
        AfxWriteDebugLog("SuperVPN run at [CNodeGeneral::SetN2NVPNNetwork] Reponse supper size error");
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
	        AfxWriteDebugLog("SuperVPN run at [CNodeGeneral::SetN2NVPNNetwork] pthread_create ERROR");
	        return ND_ERROR_PLATFORM_OS;
	    }
		iNodeIndex++;
    }

	return ND_SUCCESS;
}


/*********************************************************
����˵��������ʼ��
���˵������
����˵������
����ֵ  ��DP_SUCCESS-�ɹ�
          Others-ʧ��
*********************************************************/
ndStatus CNodeGeneral::NodeInit()
{
    //��ʼ������
 ReInit:
    ndStatus ret = mPHttpClient->NodeInit();
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at [CNodeGeneral::NodeInit] Node init error=[%d]", ret);
		goto ReInit;
	}

	return SetN2NVPNNetwork();
}

/*********************************************************
����˵�������Hello
���˵������
����˵������
����ֵ  ��DP_SUCCESS-�ɹ�
          Others-ʧ��
*********************************************************/
ndStatus CNodeGeneral::NodeHello()
{
    //Hello
    mPHttpClient->NodeHello();
}

