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
 * CHttpRunEvnCK Class
 */
class CHttpRunEvnCK : public CHttpGeneral
{
public:
	CHttpRunEvnCK(CNodeBase *node);	
	//开始检测
	ndStatus BeginCheck();	
	
private:
	//检测数据
	SRunEnvCKSt mRunEnvCK;
	//check请求
	virtual ndStatus MakeCheckReq();
	//Check应答
	virtual ndStatus AnalysisCheckRsp();	
	//edge检测
	ndStatus EdgeCheck();
	//iptable检测
	ndStatus IPTableCheck();
	//node检测
	ndStatus NodeCheck();
	
	//下载文件
	ndStatus Download(ndString filename, list<ndString> urls);
	//覆盖文件并升级
	ndStatus UpgradeAndReboot();
};

#endif //VPN_SRV_NODE_HTTPCLIENT_H
