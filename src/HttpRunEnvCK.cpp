/*********************************************************
模块名       : 节点http协议
文件名       : HttpUpdateCK.cpp
相关文件     : HttpUpdateCK.h
文件实现功能 : 节点Htpp协议处理
作者         :lewis
创建时间     :2017-09
**********************************************************/
#include "HttpRunEnvCK.hpp"
#include "cJSON.h"
#include "NDFunc.hpp"
#include "HttpFileDown.hpp"
#include "md5.h"

/*********************************************************
函数说明：构造函数
入参说明：Node(结点)
出参说明：无
返回值  ：无
*********************************************************/
CHttpRunEvnCK::CHttpRunEvnCK(CNodeBase *node):CHttpGeneral(node)
{
	mRunEnvCK.node.iVerCode = 0;
	mRunEnvCK.node.mDownLodURL.clear();
	mRunEnvCK.edge.mDownLodURL.clear();
	mRunEnvCK.iptable.mDownLodURL.clear();
}

/*********************************************************
函数说明：发送请求数据包
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpRunEvnCK::MakeCheckReq()
{
    char *out;
    cJSON *root, *fmt, *actions;

    //组装消息体
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "node", fmt=cJSON_CreateObject());
	cJSON_AddNumberToObject(fmt, "version",	mLocalVersion);
    cJSON_AddStringToObject(fmt, "mac", mPNode->GetNodeInform().sNodeMac.c_str());

    AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::MakeCheckReq] Make check actions");

    cJSON_AddItemToObject(root, "actions", actions = cJSON_CreateArray());

    //========================set===========================================
    cJSON_AddItemToArray(actions, fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "action", SUPER_ACTION_NODE_ENV_CHECK);
	cJSON_AddStringToObject(fmt, "arugments", "");

    out = cJSON_Print(root);
    mSendBuf = out;

    cJSON_Delete(root);
    free(out);
	
	return ND_SUCCESS;
}

/*********************************************************
函数说明：升级检测解包
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpRunEvnCK::AnalysisCheckRsp()
{
	AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::AnalysisCheckRspAndDeal] begin");

    cJSON *root;
	int iErrCode;

    root = cJSON_Parse(mRcvBuf.c_str());
    if (!root)
    {
        AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::AnalysisCheckRspAndDeal] Error before: [%s]", cJSON_GetErrorPtr());
        return ND_ERROR_INVALID_RESPONSE;
    }

	//取node信息
	cJSON *objType = cJSON_GetObjectItem(root, "node") ;
	if(objType != NULL)
	{
	    if(cJSON_GetObjectItem(objType, "version") != NULL &&
			cJSON_GetObjectItem(objType, "version")->valuestring != NULL)
	        mRunEnvCK.node.iVerCode = cJSON_GetObjectItem(objType, "nodeip")->valueuint;

	    if(cJSON_GetObjectItem(objType, "md5") != NULL &&
			cJSON_GetObjectItem(objType, "md5")->valuestring != NULL)
	        mRunEnvCK.node.sMD5 = cJSON_GetObjectItem(objType, "md5")->valuestring;	

	       
	    cJSON *URLS = cJSON_GetObjectItem(objType, "download");
	    if(URLS != NULL)
	    {
			cJSON *url = URLS->child;

			AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::AnalysisCheckRspAndDeal] Get URL");
			while(url != NULL)
			{
			    if(cJSON_GetObjectItem(url, "url") != NULL &&
			       cJSON_GetObjectItem(url, "url")->valuestring != NULL)
			    {
			        mRunEnvCK.node.mDownLodURL.push_back(cJSON_GetObjectItem(url, "url")->valuestring);
					AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::AnalysisCheckRspAndDeal] url=[%s]",cJSON_GetObjectItem(url, "url")->valuestring);
			    }
			    url = url->next;
			}	       
	    }		
	}
	//取edge
	objType = cJSON_GetObjectItem(root, "edge") ;
	if(objType != NULL)
	{
	    if(cJSON_GetObjectItem(objType, "md5") != NULL &&
			cJSON_GetObjectItem(objType, "md5")->valuestring != NULL)
	        mRunEnvCK.edge.sMD5 = cJSON_GetObjectItem(objType, "md5")->valuestring;	

	       
	    cJSON *URLS = cJSON_GetObjectItem(objType, "download");
	    if(URLS != NULL)
	    {
			cJSON *url = URLS->child;

			AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::AnalysisCheckRspAndDeal] Get URL");
			while(url != NULL)
			{
			    if(cJSON_GetObjectItem(url, "url") != NULL &&
			       cJSON_GetObjectItem(url, "url")->valuestring != NULL)
			    {
			        mRunEnvCK.edge.mDownLodURL.push_back(cJSON_GetObjectItem(url, "url")->valuestring);
					AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::AnalysisCheckRspAndDeal] url=[%s]",cJSON_GetObjectItem(url, "url")->valuestring);
			    }
			    url = url->next;
			}	       
	    }		
	}	
	//取iptable
	objType = cJSON_GetObjectItem(root, "iptable") ;
	if(objType != NULL)
	{
	    if(cJSON_GetObjectItem(objType, "md5") != NULL &&
			cJSON_GetObjectItem(objType, "md5")->valuestring != NULL)
	        mRunEnvCK.iptable.sMD5 = cJSON_GetObjectItem(objType, "md5")->valuestring;	

	       
	    cJSON *URLS = cJSON_GetObjectItem(objType, "download");
	    if(URLS != NULL)
	    {
			cJSON *url = URLS->child;

			AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::AnalysisCheckRspAndDeal] Get URL");
			while(url != NULL)
			{
			    if(cJSON_GetObjectItem(url, "url") != NULL &&
			       cJSON_GetObjectItem(url, "url")->valuestring != NULL)
			    {
			        mRunEnvCK.iptable.mDownLodURL.push_back(cJSON_GetObjectItem(url, "url")->valuestring);
					AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::AnalysisCheckRspAndDeal] url=[%s]",cJSON_GetObjectItem(url, "url")->valuestring);
			    }
			    url = url->next;
			}	       
	    }		
	}		

 	cJSON_Delete(root);

	return ND_SUCCESS;
}

/*********************************************************
函数说明：edge检测
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpRunEvnCK::EdgeCheck()
{
	//判断edge命令是否存在
	if(AfxCheckCmdExist("edge"))
		return ND_SUCCESS;

	ndStatus ret = Download("/usr/bin/edge", mRunEnvCK.edge.mDownLodURL);
	//ndStatus ret = Download("/tmp/edge", mRunEnvCK.edge.mDownLodURL);
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::EdgeCheck] Download edge command Err ret=[%d]", ret);
        return ret;
    }

	chmod("/usr/bin/edge", S_IRUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);

	return ND_SUCCESS;
}

/*********************************************************
函数说明：iptable检测
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpRunEvnCK::IPTableCheck()
{
	//判断iptable的命令是否存在
	if(AfxCheckCmdExist("iptables"))
		return ND_SUCCESS;

	ndStatus ret = Download("/usr/bin/iptables", mRunEnvCK.iptable.mDownLodURL);
	//ndStatus ret = Download("/tmp/iptables", mRunEnvCK.iptable.mDownLodURL);
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::IPTableCheck] Download iptable command Err ret=[%d]", ret);
        return ret;
    }
	
	chmod("/usr/bin/iptables", S_IRUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);

	AfxExecCmd("ln -s /usr/bin/iptables /usr/bin/iptables-restore");
	AfxExecCmd("ln -s /usr/bin/iptables /usr/bin/iptables-save");
	AfxExecCmd("ln -s /usr/bin/iptables /usr/bin/iptables-xml");
	
	return ND_SUCCESS;
}

/*********************************************************
函数说明：node检测
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpRunEvnCK::NodeCheck()
{
	//判断版本号是否需要升级
	if (mRunEnvCK.node.iVerCode <= mLocalVersion){
		AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::NodeCheck] Node Not Need Upgrade");
		return ND_SUCCESS;	
	}
	
	//下载新版本
    ndStatus ret = Download(VPN_UPGRADE_TEMP_FILE_NAME, mRunEnvCK.node.mDownLodURL);
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::NodeCheck] DownloadNewVersion Err ret=[%d]", ret);
        return ret;
    }

	//校验并升级
    ret = UpgradeAndReboot();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::NodeCheck] UpgradeAndReboot Err ret=[%d]", ret);
        return ret;
    }
	return ret;
}

/*********************************************************
函数说明：下载文件
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpRunEvnCK::Download(ndString filename, list<ndString> urls)
{
	ndStatus ret = ND_SUCCESS;
    CHttpFileDown FileDown;
    SDownloadFileReqSt downloadFileReqSt;
    downloadFileReqSt.sLocalFile = filename;
    downloadFileReqSt.fFile = NULL;
    downloadFileReqSt.iOffset = 0;
    downloadFileReqSt.iCRC = 0;
    downloadFileReqSt.iSize = 0;

	list<ndString>::iterator iterURL;
    for(iterURL=urls.begin(); iterURL!=urls.end(); iterURL++)
    {
		downloadFileReqSt.sUrl = *iterURL;
	    if (!FileDown.SetDownFileReq(downloadFileReqSt))
	    {
	        return ND_ERROR_INVALID_REQUEST;
	    }
	    ret = FileDown.BeginDownload();
	    if(ret == ND_SUCCESS)
	    {
	        AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::AnalysisCheckRsp] Download File=[%s] Success\n", downloadFileReqSt.sUrl.c_str());
	        return ND_SUCCESS;
	    }else{
	    	AfxWriteDebugLog("SuperVPN run at [CHttpRunEvnCK::AnalysisCheckRsp] Download File Err=[%s] Code=[%d]\n", downloadFileReqSt.sUrl.c_str(), ret);
	    }
    }
	
	return ret;
}

/*********************************************************
函数说明：覆盖文件并升级
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndBool getFileMD5(const ndString& filename, ndString& md5)
{
/*
    std::ifstream fin(filename.c_str(), std::ifstream::in | std::ifstream::binary);
    if (fin)
    {
    
        MD5_CTX context;
        MD5Init(&context);

        fin.seekg(0, fin.end);
        const auto fileLength = fin.tellg();
        fin.seekg(0, fin.beg);

        const int bufferLen = 8192;
        std::unique_ptr<unsigned char[]> buffer{ new unsigned char[bufferLen] {} };
        unsigned long long totalReadCount = 0;
        decltype(fin.gcount()) readCount = 0;
        // 读取文件内容，调用MD5Update()更新MD5值
        while (fin.read(reinterpret_cast<char*>(buffer.get()), bufferLen))
        {
            readCount = fin.gcount();
            totalReadCount += readCount;
            MD5Update(&context, buffer.get(), static_cast<unsigned int>(readCount));
        }
        // 处理最后一次读到的数据
        readCount = fin.gcount();
        if (readCount > 0)
        {
            totalReadCount += readCount;
            MD5Update(&context, buffer.get(), static_cast<unsigned int>(readCount));
        }
        fin.close();

        // 数据完整性检查
        if (totalReadCount != fileLength)
        {
            return false;
        }

        unsigned char digest[16];
        MD5Final(digest, &context);

        // 获取MD5
        std::ostringstream oss;
        for (int i = 0; i < 16; ++i)
        {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(digest[i]);
        }
        oss << std::ends;

        md5 = std::move(oss.str());
		return true;
    }
    else
    {
        return false;
    }
            */
    return true;
}

ndStatus CHttpRunEvnCK::UpgradeAndReboot()
{
	//下载文件的校验

	//调用脚本实现文件覆盖与替换
	return ND_SUCCESS;
}

/*********************************************************
函数说明：开始升级检测
入参说明：无
出参说明：无
返回值  ：true为需要升级,false无需升级
*********************************************************/
ndStatus CHttpRunEvnCK::BeginCheck()
{
    //组装数据包
    AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] MakeRequest Pkg");
    ndStatus  ret = MakeCheckReq();
    if(ret != ND_SUCCESS)
	{
        AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }
	//发送请求的数据包
	AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] PkgSendAndRecv");
	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] PkgSendAndRecv Err ret=[%d]", ret);
		return ret;
	}
	//解析返回数据并处理
	AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] AnalyzeResponsePkg");
    ret = AnalysisCheckRsp();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] AnalyzeResponsePkg ret=[%d]", ret);
        return ret;
    }
	//edge check
	AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] EdgeCheck");
	ret = EdgeCheck();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] EdgeCheck ret=[%d]", ret);
        return ret;
    }
	
	//iptable check
	AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] IPTableCheck");
	ret = IPTableCheck();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] IPTableCheck ret=[%d]", ret);
        return ret;
    }
	
	//node check
	AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] NodeCheck");
	ret = NodeCheck();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpRunEvnCK::BeginCheck] NodeCheck ret=[%d]", ret);
        return ret;
    }	
	
	return ND_SUCCESS;
}