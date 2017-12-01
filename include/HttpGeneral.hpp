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
	//�����Ϣ
	CNodeBase *mPNode;	
	//�����URL
	ndString mSrvURL;
    //���͵�����buff
    ndString mSendBuf;
    //���յ�����buff
    ndString mRcvBuf;	
	
    //������֤ͷ
    virtual ndString GenerateAuthHeader();
	//������֤ͷ
    virtual void AnalysisAuthHeader(ndString &head);

	//�ڵ��ʼ��
	virtual ndStatus MakeNodeInitReq();
	virtual ndStatus AnalysisNodeInitRsp();

	//�ڵ�Hello
	virtual ndStatus MakeNodeHelloReq();
	virtual ndStatus AnalysisNodeHelloRsp();

	//���ݰ����Ͳ����մ���
	ndStatus PkgSendAndRecv(ndString url);	

public:
    CHttpGeneral();
	CHttpGeneral(CNodeBase *node);

	ndStatus NodeInit();
	ndStatus NodeHello();
	
    virtual ~CHttpGeneral();

};

#endif //VPN_NODE_HTTPCLIENT_H
