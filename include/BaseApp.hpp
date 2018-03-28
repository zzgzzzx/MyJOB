/****************** Copyright (c) 2017 **************
模块名       : 
文件名       :CBaseApp.hpp
相关文件     : 
文件实现功能 :应用工程类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#ifndef __BASEAPP_HPP__
#define __BASEAPP_HPP__

#include "MsgQueue.hpp"
#include "TimerManager.hpp"
#include "NodeBase.hpp"
#include "Packet.hpp"
#include "HelloSrvThread.hpp"
#include "HttpSrvThread.hpp"
#include "ServiceSet.hpp"

//**********************************
//应用工程基类                       
//**********************************
class CBaseApp
{
private:
	//定时器成员
	CTimerManager mTimer;

protected:
	//应用程序初始化工作
	virtual bool InitApplication(void);
public:	
	//插入单回定时器	
	bool InsertSingleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong));
	//插入循环定时器	
	bool InsertCircleTimer(unsigned long callid,unsigned long timeout,void (*CallBack)(ndULong));

	//删除循环定时器
	bool DeleteCircleTimer(unsigned long callid);
	//删除单个定时器
	bool DeleteSingleTimer(unsigned long callid);

	//应用程序启动运行
	void Run();

	//构造函数
	CBaseApp();
	//析构函数
	virtual ~CBaseApp();
};

//**********************************
//宽带路由设备工程类                       
//**********************************
class CSuperVPNApp: public CBaseApp
{
private:
	//消息包队列
	CMsgFIFO<CPacket*> mPktQueue;	
	
	//系统数据初始化
	bool InitSystem(void);
	//服务器列表检测
	ndStatus ServerListCheck();
 	//系统运行环境检测
 	ndStatus RunEnvCheck();
	//节点初始化编号检测
	ndStatus NodeInitCheck();
	//启动http服务器
	ndStatus StartHttpd();
	//数据包队列循环处理
	void TranslatePkt(void);
	
protected:
	//应用程序初始化工作
	bool InitApplication(void);

public:
	//节点
	CNodeBase *mPNode;
	//hello服务
	CHelloSrvThread mHelloSrv;
	//http服务
	CHttpSrvThread mHttpSrv;
	//服务节点集合类
	CServiceSet mServiceSet;
	//身份识别管理类
	CIdentifySet mIdentifySet;
	
	//构造函数
	CSuperVPNApp();
	//析构函数
	~CSuperVPNApp();	

	//节点重启
	static void NodeRestartFunc(ndULong param);	

	//消息数据包入队
	void InsertPktToQueue(CPacket *pkt);	
};

#endif
