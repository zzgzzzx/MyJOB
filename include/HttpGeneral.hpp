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
	virtual ndStatus MakeNodeInitReq();
	virtual ndStatus AnalysisNodeInitRsp();

	//�ڵ�Hello
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
