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
	
    //������֤ͷ
    virtual ndString GenerateAuthHeader();
	//������֤ͷ
    virtual void AnalysisAuthHeader(ndString &head);

	//�ڵ��ʼ��
	virtual ndStatus MakeNodeInitReq();
	virtual ndStatus AnalysisNodeInitRsp();

	//�ڵ�����
	virtual ndStatus MakeNodeEnvSetReq();
	virtual ndStatus AnalysisNodeEnvSetRsp();

	//�ڵ�Hello
	virtual ndStatus MakeNodeHelloReq();
	virtual ndStatus AnalysisNodeHelloRsp();

	//��ȡ�������б�
	virtual ndStatus MakeServerListReq();
	ndStatus AnalysisServerListRsp(list<SServerInfo> &mServers);

	//��ȡ�������
	ndStatus MakeBindServerReq(ndString devidentify);
	ndStatus AnalysisBindServerRsp(list<SBindInform> &mServers);	

	//���ݰ����Ͳ����մ���
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
