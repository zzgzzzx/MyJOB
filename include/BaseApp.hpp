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
	//节点
	static CNodeBase *mPNode;
	//是否需要退出标识
	ndBool mStopRun;	
	//系统数据初始化
	bool InitSystem(void);
 	//系统运行环境检测
 	ndStatus RunEnvCheck();
protected:
	//应用程序初始化工作
	bool InitApplication(void);

public:
	//构造函数
	CSuperVPNApp();
	//析构函数
	~CSuperVPNApp();	
	//节点hello
	static void NodeHelloFunc(ndULong param);	
};

#endif
