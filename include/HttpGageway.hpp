//
// Created by lewis on 2017/3/18.
// FZ-ZEASN
//

#ifndef VPN_GETEWAY_HTTPCLIENT_H
#define VPN_GETEWAY_HTTPCLIENT_H

#include "NDType.hpp"
#include "HttpGeneral.hpp"
#include "NodeBase.hpp"


/**
 * CHttpGeteway Class
 */
class CHttpGateway : public CHttpGeneral
{
private:
	ndStatus MakeRouteSetting();
	ndStatus AnalysisRouteSetting();
	
public:
	CHttpGateway(CNodeBase *node);	
	//获取策略路由并设置
	ndStatus GetRouteSetting();
};

#endif //VPN_GETEWAY_HTTPCLIENT_H