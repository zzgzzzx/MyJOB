/*********************************************************
ģ����       : �ڵ�httpЭ��
�ļ���       : HttpUpdateCK.cpp
����ļ�     : HttpUpdateCK.h
�ļ�ʵ�ֹ��� : �ڵ�HtppЭ�鴦��
����         :lewis
����ʱ��     :2017-09
**********************************************************/
#include "HttpUpdateCK.hpp"
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
CHttpUpdateCK::CHttpUpdateCK()
{
	mSrvURL = VPN_UPGRADE_SRV_URL;
	mSUpdateCK.iVerCode = 0;
	mSUpdateCK.mDownLodURL.clear();
}

/*********************************************************
����˵��������������
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpUpdateCK::MakeCheckReq()
{
	return ND_SUCCESS;
}

/*********************************************************
����˵�������������
���˵������
����˵������
����ֵ  ����
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

	//�жϰ汾���Ƿ���Ҫ����
	if (mSUpdateCK.iVerCode <= atoi(SUPER_VPN_CLIENT_VER))
		return ND_ERROR_NO_NEED_UPGRADE;
	
	return ND_SUCCESS;
}

/*********************************************************
����˵���������ļ�
���˵������
����˵������
����ֵ  ����
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
����˵���������ļ�������
���˵������
����˵������
����ֵ  ����
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
        // ��ȡ�ļ����ݣ�����MD5Update()����MD5ֵ
        while (fin.read(reinterpret_cast<char*>(buffer.get()), bufferLen))
        {
            readCount = fin.gcount();
            totalReadCount += readCount;
            MD5Update(&context, buffer.get(), static_cast<unsigned int>(readCount));
        }
        // �������һ�ζ���������
        readCount = fin.gcount();
        if (readCount > 0)
        {
            totalReadCount += readCount;
            MD5Update(&context, buffer.get(), static_cast<unsigned int>(readCount));
        }
        fin.close();

        // ���������Լ��
        if (totalReadCount != fileLength)
        {
            return false;
        }

        unsigned char digest[16];
        MD5Final(digest, &context);

        // ��ȡMD5
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
	//�����ļ���У��

	//���ýű�ʵ���ļ��������滻
	return ND_SUCCESS;
}

/*********************************************************
����˵������ʼ�������
���˵������
����˵������
����ֵ  ��trueΪ��Ҫ����,false��������
*********************************************************/
ndStatus CHttpUpdateCK::BeginUpdateCheck()
{
    //��װ���ݰ�
    ndStatus  ret = MakeCheckReq();
    if(ret != ND_SUCCESS)
	{
        AfxWriteDebugLog("SuperVPN run at[CHttpUpdateCK::BeginUpdateCheck] MakeRequest Pkg Err ret=[%d]", ret);
        return ret;
    }
	//������������ݰ�
	ret = PkgSendAndRecv(mSrvURL);
	if (ret != ND_SUCCESS)
	{
		AfxWriteDebugLog("SuperVPN run at[CHttpUpdateCK::BeginUpdateCheck] PkgSendAndRecv Err ret=[%d]", ret);
		return ret;
	}
	//�����������ݲ�����
    ret = AnalysisCheckRsp();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpUpdateCK::BeginUpdateCheck] AnalyzeResponsePkg ret=[%d]", ret);
        return ret;
    }

	//�����°汾
    ret = DownloadNewVersion();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpUpdateCK::BeginUpdateCheck] DownloadNewVersion Err ret=[%d]", ret);
        return ret;
    }

	//У�鲢����
    ret = UpgradeAndReboot();
    if(ret != ND_SUCCESS){
        AfxWriteDebugLog("SuperVPN run at[CHttpUpdateCK::BeginUpdateCheck] UpgradeAndReboot Err ret=[%d]", ret);
        return ret;
    }	
	
	return ND_SUCCESS;
}