//
// Created by lewis on 2017/9/3.
// FJ-FZ
//

#ifndef VPN_NODE_USER_H
#define VPN_NODE_USER_H

#include "NodeGateway.hpp"

/**
 * NodeUser Class
 */
class CNodeUser: public CNodeGeneral
{
private:
	//IPTable������
	int mIPTableIndex;
	//����http����
	CNodeGateway mNodeGateway;
public:
    CNodeUser();
    virtual ~CNodeUser();

	//����ʼ���������������������Ϣ
	ndStatus NodeEnvSet();
	//�����û�����������й���
	ndStatus SetPolicyRoute(SDeviceFlag deviceFlag);
};

#endif //VPN_NODE_USER_H