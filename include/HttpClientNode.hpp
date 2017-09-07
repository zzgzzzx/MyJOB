//
// Created by lewis on 2017/3/18.
// FZ-ZEASN
//

#ifndef VPN_NODE_HTTPCLIENT_H
#define VPN_NODE_HTTPCLIENT_H

#include "ndType.h"
#include "HttpClient.h"
#include "NodeBase.h"


using namespace network;

/**
 * CHttpClientNode Class
 */
class CHttpClientNode : public CHttpClient
{
private:
	//�����Ϣ
	CNodeBase *mPNode;
	//������ip��ַ
	ndString mSrvIP;
    //���͵�����buff
    ndString mSendBuf;
    //���յ�����buff
    ndString mRcvBuf;	
	//���ݰ����Ͳ����մ���
	ndStatus PkgSendAndRecv();
protected:
    //������֤ͷ
    virtual ndString GenerateAuthHeader();
	//������֤ͷ
    virtual void AnalysisAuthHeader(ndString &head);

	//�ڵ��ʼ��
	ndStatus MakeNodeInitReq();
	ndStatus AnalysisNodeInitRsp();

	//�ڵ�Hello
	ndStatus MakeNodeHelloReq();
	ndStatus AnalysisNodeHelloRsp();

public:
    CHttpClientNode();
	CHttpClientNode(CNodeBase &node, ndString sip);

	ndStatus NodeInit();
	ndStatus NodeHello();
	
    virtual ~CHttpClientNode();

};

#endif //VPN_NODE_HTTPCLIENT_H
