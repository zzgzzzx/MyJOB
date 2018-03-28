//
// Created by lewis on 2017/3/18.
// FZ-ZEASN
//

#ifndef VPN_NODE_HTTPCLIENT_H
#define VPN_NODE_HTTPCLIENT_H

#include "NDType.hpp"
#include "HttpClient.hpp"
#include "NodeBase.hpp"


using namespace network;

/**
 * CHttpClientNode Class
 */
class CHttpGeneral : public CHttpClient
{
protected:
	//结点信息
	CNodeBase *mPNode;
	
    //生成认证头
    virtual ndString GenerateAuthHeader();
	//分析认证头
    virtual void AnalysisAuthHeader(ndString &head);

	//节点初始化
	virtual ndStatus MakeNodeInitReq();
	virtual ndStatus AnalysisNodeInitRsp();

	//节点配置
	virtual ndStatus MakeNodeEnvSetReq();
	virtual ndStatus AnalysisNodeEnvSetRsp();

	//节点Hello
	virtual ndStatus MakeNodeHelloReq();
	virtual ndStatus AnalysisNodeHelloRsp();

	//获取服务器列表
	virtual ndStatus MakeServerListReq();
	ndStatus AnalysisServerListRsp(list<SServerInfo> &mServers);

	//获取服务出口
	ndStatus MakeBindServerReq(ndString devidentify);
	ndStatus AnalysisBindServerRsp(list<SBindInform> &mServers);	

	//数据包发送并接收处理
	ndStatus PkgSendAndRecv(ndString url);	

public:
    CHttpGeneral();
	CHttpGeneral(CNodeBase *node);

	ndStatus NodeEnvSet();
	ndStatus NodeHello();
	ndStatus NodeInit();
	virtual ndStatus NodeGetIPPool();
	
	ndBool GetServerList(list<SServerInfo> &mServers);
	ndStatus GetIdentifyService(list<SBindInform> &ltBSer);
	
    virtual ~CHttpGeneral();

};

#endif //VPN_NODE_HTTPCLIENT_H
