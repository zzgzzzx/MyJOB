/*********************************************************
模块名       : 与服务器交互基类(Certificate\DP)
文件名       : ClientSrvBase.cpp
相关文件     : ClientSrvBase.h
文件实现功能 : 实现与服务器交互的功能
作者         :lewis
创建时间     :2017-09
**********************************************************/
#include "NodeGeneral.hpp"
#include "HttpGeneral.hpp"
#include "NDFunc.hpp"


/*********************************************************
函数说明：构造函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CNodeGeneral::CNodeGeneral()
{
	mPHttpClient = NULL;
	AfxGetEthMac("eth0", mSNodeInform.sNodeMac);	
	//mSNodeInform.sNodeMac = "aa:bb:cc:dd:ee:ff";
}

/*********************************************************
函数说明：析构函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CNodeGeneral::~CNodeGeneral()
{
	if(mPHttpClient != NULL) delete mPHttpClient;
}

/*********************************************************
函数说明：完成的事务后续处理内容
入参说明：无
出参说明：无
返回值  ：DP_SUCCESS-成功
          Others-失败
*********************************************************/
ndStatus CNodeGeneral::InitData()
{
    return ND_SUCCESS;
}

/*********************************************************
函数说明：线程处理函数
入参说明：无
出参说明：无
返回值  ：DP_SUCCESS-成功
          Others-失败
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
函数说明：根据返回的网络参数配置vpn网络
入参说明：无
出参说明：无
返回值  ：DP_SUCCESS-成功
          Others-失败
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
函数说明：结点初始化
入参说明：无
出参说明：无
返回值  ：DP_SUCCESS-成功
          Others-失败
*********************************************************/
ndStatus CNodeGeneral::NodeInit()
{
    //初始化操作
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
函数说明：结点Hello
入参说明：无
出参说明：无
返回值  ：DP_SUCCESS-成功
          Others-失败
*********************************************************/
ndStatus CNodeGeneral::NodeHello()
{
    //Hello
    mPHttpClient->NodeHello();
}

