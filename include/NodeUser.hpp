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
	//IPTable的索引
	int mIPTableIndex;
	//网关http处理
	CNodeGateway mNodeGateway;
public:
    CNodeUser();
    virtual ~CNodeUser();

	//结点初始化，向服务器申请配置信息
	ndStatus NodeEnvSet();
	//根据用户的特征码进行关联
	ndStatus SetPolicyRoute(SDeviceFlag deviceFlag);
};

#endif //VPN_NODE_USER_H