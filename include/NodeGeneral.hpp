//
// Created by lewis on 2017/9/3.
// FJ-FZ
//

#ifndef VPN_NODE_GENERAL_H
#define VPN_NODE_GENERAL_H

#include "NodeBase.hpp"
#include "HttpGeneral.hpp"

/**
 * NodeGeneral Class
 */
class CNodeGeneral: public CNodeBase
{
private :
	//���ݷ��ص������������vpn����
	ndStatus SetN2NVPNNetwork();
	//�̵߳Ĵ���
	static void *ThreadFunction(void *param);	
protected:
	//ͨѶ�Ŀͻ���
	CHttpGeneral *mPHttpClient;
public:
    CNodeGeneral();
    virtual ~CNodeGeneral();

    //���ݴ���ǰ�ĳ�ʼ������
    virtual ndStatus InitData();
	//�ڵ��ʼ��
	virtual ndStatus NodeInit();

	//�ڵ������������������Ϣ
	virtual ndStatus NodeEnvSet();
	//�ڵ�Hello
	virtual ndStatus NodeHello();
	//��ȡ�������б�
	virtual ndBool GetServerList(list<SServerInfo> &mServers);
};

#endif //VPN_NODE_GENERAL_H