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
private:
	//结点信息
	CNodeBase *mPNode;
	//服务器ip地址
	ndString mSrvIP;
    //发送的数据buff
    ndString mSendBuf;
    //接收的数据buff
    ndString mRcvBuf;	
	//数据包发送并接收处理
	ndStatus PkgSendAndRecv();
protected:
    //生成认证头
    virtual ndString GenerateAuthHeader();
	//分析认证头
    virtual void AnalysisAuthHeader(ndString &head);

	//节点初始化
	virtual ndStatus MakeNodeInitReq();
	virtual ndStatus AnalysisNodeInitRsp();

	//节点Hello
	virtual ndStatus MakeNodeHelloReq();
	virtual ndStatus AnalysisNodeHelloRsp();

public:
    CHttpGeneral();
	CHttpGeneral(CNodeBase &node, ndString sip);

	ndStatus NodeInit();
	ndStatus NodeHello();
	
    virtual ~CHttpClientNode();

};

#endif //VPN_NODE_HTTPCLIENT_H
