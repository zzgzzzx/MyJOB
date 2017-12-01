/*********************************************************
模块名       : 节点http协议
文件名       : HttpUpdateCK.cpp
相关文件     : HttpUpdateCK.h
文件实现功能 : 节点Htpp协议处理
作者         :lewis
创建时间     :2017-09
**********************************************************/
#include "HttpUpdateCK.hpp"
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
CHttpUpdateCK::CHttpUpdateCK()
{
	mSrvURL = VPN_UPGRADE_SRV_URL;
	mSUpdateCK.iVerCode = 0;
	mSUpdateCK.mDownLodURL.clear();
}

/*********************************************************
函数说明：升级检测组包
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpUpdateCK::MakeCheckReq()
{
	return ND_SUCCESS;
}

/*********************************************************
函数说明：升级检测解包
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpUpdateCK::AnalysisCheckRsp()
{
	AfxWriteDebugLog("SuperVPN run at [CHttpUpdateCK::AnalysisCheckRspAndDeal] begin");

    cJSON *root;
	int iErrCode;

    root = cJSON_Parse(mRcvBuf.c_str());
    if (!root)
    {
        AfxWriteDebugLog("SuperVPN run at [CHttpUpdateCK::AnalysisCheckRspAndDeal] Error before: [%s]", cJSON_GetErrorPtr());
        return ND_ERROR_INVALID_RESPONSE;
    }

    if(cJSON_GetObjectItem(root, "version") != NULL &&
		cJSON_GetObjectItem(root, "version")->valuestring != NULL)
        mSUpdateCK.iVerCode = cJSON_GetObjectItem(root, "nodeip")->valueuint;

    if(cJSON_GetObjectItem(root, "md5") != NULL &&
		cJSON_GetObjectItem(root, "md5")->valuestring != NULL)
        mSUpdateCK.sMD5 = cJSON_GetObjectItem(root, "md5")->valuestring;	

       
    cJSON *URLS = cJSON_GetObjectItem(root, "download");
    if(URLS != NULL)
    {
		cJSON *url = URLS->child;

		AfxWriteDebugLog("SuperVPN run at [CHttpUpdateCK::AnalysisCheckRspAndDeal] Get URL");
		while(url != NULL)
		{
		    if(cJSON_GetObjectItem(url, "url") != NULL &&
		       cJSON_GetObjectItem(url, "url")->valuestring != NULL)
		    {
		        mSUpdateCK.mDownLodURL.push_back(cJSON_GetObjectItem(url, "url")->valuestring);
				AfxWriteDebugLog("SuperVPN run at [CHttpUpdateCK::AnalysisCheckRspAndDeal] url=[%s]",cJSON_GetObjectItem(url, "url")->valuestring);
		    }
		    url = url->next;
		}
        cJSON_Delete(root);
    }	

	//判断版本号是否需要升级
	if (mSUpdateCK.iVerCode <= atoi(SUPER_VPN_CLIENT_VER))
		return ND_ERROR_NO_NEED_UPGRADE;
	
	return ND_SUCCESS;
}

/*********************************************************
函数说明：下载文件
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpUpdateCK::DownloadNewVersion()
{
	ndStatus ret = ND_SUCCESS;
    CHttpFileDown FileDown;
    SDownloadFileReqSt downloadFileReqSt;
    downloadFileReqSt.sLocalFile = VPN_UPGRADE_TEMP_FILE_NAME;
    downloadFileReqSt.fFile = NULL;
    downloadFileReqSt.iOffset = 0;
    downloadFileReqSt.iCRC = 0;
    downloadFileReqSt.iSize = 0;

	list<ndString>::iterator iterURL;
    for(iterURL=mSUpdateCK.mDownLodURL.begin(); iterURL!=mSUpdateCK.mDownLodURL.end(); iterURL++)
    {
		downloadFileReqSt.sUrl = *iterURL;
	    if (!FileDown.SetDownFileReq(downloadFileReqSt))
	    {
	        return ND_ERROR_INVALID_REQUEST;
	    }
	    ret = FileDown.BeginDownload();
	    if(ret == ND_SUCCESS)
	    {
	        TRACE("SuperVPN run at [CHttpUpdateCK::AnalysisCheckRsp] Download Upgrade File Success\n");
	        return ND_SUCCESS;
	    }else{
	    	TRACE("SuperVPN run at [CHttpUpdateCK::AnalysisCheckRsp] Download Upgrade File Err=[%s] Code=[%d]\n", downloadFileReqSt.sUrl.c_str(), ret);
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

ndStatus CHttpUpdateCK::UpgradeAndReboot()
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
ndStatus CHttpUpdateCK::BeginUpdateCheck()
{
    //组装数据包
    ndStatus  ret = MakeCheckReq();
    if(ret != ND_SUCCESS)
	{
        AfxWriteDebugLog("SuperVPN run at[CHttpUpdateCK::BeginUpdateCheck] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }
	//发送请求的数据包
	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at[CHttpUpdateCK::BeginUpdateCheck] PkgSendAndRecv Err ret=[%d]", ret);
		return ret;
	}
	//解析返回数据并处理
    ret = AnalysisCheckRsp();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpUpdateCK::BeginUpdateCheck] AnalyzeResponsePkg ret=[%d]", ret);
        return ret;
    }

	//下载新版本
    ret = DownloadNewVersion();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpUpdateCK::BeginUpdateCheck] DownloadNewVersion Err ret=[%d]", ret);
        return ret;
    }

	//校验并升级
    ret = UpgradeAndReboot();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpUpdateCK::BeginUpdateCheck] UpgradeAndReboot Err ret=[%d]", ret);
        return ret;
    }	
	
	return ND_SUCCESS;
}