//
// Created by lewis on 2017/3/18.
// FZ-ZEASN
//

#ifndef VPN_USER_NODE_HTTPCLIENT_H
#define VPN_USER_NODE_HTTPCLIENT_H

#include "NDType.hpp"
#include "HttpGeneral.hpp"
#include "NodeBase.hpp"


/**
 * CHttpUserNode Class
 */
class CHttpUserNode : public CHttpGeneral
{
protected:
	//�ڵ��ʼ��
	virtual ndStatus MakeNodeInitReq();
};

#endif //VPN_USER_NODE_HTTPCLIENT_H
