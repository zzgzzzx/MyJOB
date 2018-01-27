//
// Created by lewis on 2017/9/3.
// FJ-FZ
//

#ifndef VPN_NODE_USER_H
#define VPN_NODE_USER_H

#include "NodeGeneral.hpp"

/**
 * NodeUser Class
 */
class CNodeUser: public CNodeGeneral
{
public:
    CNodeUser();
    virtual ~CNodeUser();

	//结点初始化，向服务器申请配置信息
	ndStatus NodeEnvSet();	
};

#endif //VPN_NODE_USER_H