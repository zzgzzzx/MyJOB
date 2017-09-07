//
// Created by lewis on 2017/9/3.
// FJ-FZ
//

#ifndef VPN_NODEBASE_H
#define VPN_NODEBASE_H

#include "ndType.h"
#include "HttpClient.h"

/**
 * NodeBase Class
 */
class CNodeBase
{
protected:
	//vpn结点的编号
	ndString mNodeID;
	//vpn结点的MAC地址
	ndString mNodeMac;
	//vpn结点的ip地址
	ndULong mNodeIP;
	//vpn结点所属的域
	ndString mDomain;
	//vpn结点所属域的key
	ndString mDomainKey;
	//vpn超级结点的IP地址
	ndString mSuperNodeIP;
	//vpn超级结点的端口号
	ndString mSuperNodePort;	
	
    //返回的错误码
    ndULong  mErrCode;

    //处理回调函数
    ndCallbackFunction mCallBackFunc;

public:
    CNodeBase();
    virtual ~CNodeBase();

	//结点初始化，向服务器申请配置信息
	virtual ndStatus NodeInit()=0;
	//结点Hello
	virtual ndStatus NodeHello()=0;
	

	//异步回调处理函数
    static void *ThreadFunction(void *param);

    //设置回调处理(设置callbackFunc)
    void SetAsyncMode(ndCallbackFunction callbackFunction);
    //获取回调
    ndCallbackFunction GetCallBackFunction();

    //获取错误码
    ndULong GetLastErrCode();
    //设置错误码
    void SetLastErrCode(ndULong code);
};

#endif //TVPN_CLINETBASE_H
