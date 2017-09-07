//
// Created by lewis on 2017/9/3.
// FJ-FZ
//

#ifndef VPN_CLINETSRVBASE_H
#define VPN_CLINETSRVBASE_H

#include "ndType.h"
#include "HttpClient.h"
#include "NodeBase.h"

/**
 * ClientBase Class
 */
class CNodeSrvBase: public CNodeBase
{
protected:
    //访问的中心服务器IP
    ndString mCenterSrvIP;
public:
    CNodeSrvBase();
    virtual ~CNodeSrvBase();

    //数据处理前的初始化操作
    virtual ndStatus InitData();

	//结点初始化，向服务器申请配置信息
	virtual ndStatus NodeInit();
	//结点Hello
	virtual ndStatus NodeHello();
};

#endif //VPN_CLINETSRVBASE_H
