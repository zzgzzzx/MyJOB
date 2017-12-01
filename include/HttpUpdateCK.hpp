//
// Created by lewis on 2017/3/18.
// FZ-ZEASN
//

#ifndef UPDATE_CHECK_HTTPCLIENT_H
#define UPDATE_CHECK_HTTPCLIENT_H

#include "NDType.hpp"
#include "HttpGeneral.hpp"
#include "NodeBase.hpp"


/**
 * CHttpUpdateCK Class
 */
class CHttpUpdateCK : public CHttpGeneral
{
public:
	CHttpUpdateCK();	
	//开始检测
	ndStatus BeginUpdateCheck();	
	
private:
	//升级检测数据
	SUpdateCKSt mSUpdateCK;
	//check请求
	virtual ndStatus MakeCheckReq();
	//Check应答
	virtual ndStatus AnalysisCheckRsp();	
	//下载文件
	ndStatus DownloadNewVersion();
	//覆盖文件并升级
	ndStatus UpgradeAndReboot();
};

#endif //VPN_SRV_NODE_HTTPCLIENT_H
