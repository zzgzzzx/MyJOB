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
public:
    CNodeUser();
    virtual ~CNodeUser();

	//����ʼ���������������������Ϣ
	ndStatus NodeEnvSet();
	//�����û�����������й���
	ndStatus SetEdgeAndRoute();
	//�����ķ��������������豸��ݶ�Ӧ�ĳ�����Ϣ
	ndStatus BindIdentifyService(list<SBindInform> &ltBSer);
};

#endif //VPN_NODE_USER_H