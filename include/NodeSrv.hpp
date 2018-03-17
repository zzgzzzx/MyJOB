//
// Created by lewis on 2017/9/3.
// FJ-FZ
//

#ifndef VPN_NODE_SRV_H
#define VPN_NODE_SRV_H

#include "NodeGeneral.hpp"

/**
 * NodeSrv Class
 */
class CNodeSrv: public CNodeGeneral
{
private:
	ndInt8 mNoRecvHelloTime;
public:
    CNodeSrv();
    virtual ~CNodeSrv();

	//½ÚµãHello
	ndStatus NodeHello();	
	void DealHelloAddNewDomain(list<SDomain> &domains);
};

#endif //VPN_NODE_SRV_H