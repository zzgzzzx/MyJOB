//
// Created by lewis on 2017/9/3.
// FJ-FZ
//

#ifndef VPN_NODE_GATEWAY_H
#define VPN_NODE_GATEWAY_H

#include "NodeGeneral.hpp"

/**
 * NodeGateway Class
 */
class CNodeGateway: public CNodeGeneral
{
private:
	//���ò���·��
	ndStatus SetPolicyRoute();

public:
	//����·�����������Ϣ
	list<SRouteInform> mRouteInf;
	
    CNodeGateway();
    virtual ~CNodeGateway();
	//�豸����·�����
	ndStatus RouteSetting();
	
	//���ò���·����Ϣ
	void SetRouteInf(list<SRouteInform> &routeInf);

	//ͨ���豸��ʶ��ѯ�������
	ndBool GetRouteInf(ndString deviceFlag, SRouteInform &sRI);
};

#endif //VPN_NODE_GATEWAY_H