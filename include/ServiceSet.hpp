/*********************************************************
模块名       : 
文件名       :ServiceSet.hpp
相关文件     : 
文件实现功能 :服务节点集合类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#ifndef __SERVICE_SET_HPP__
#define __SERVICE_SET_HPP__

#include "Set.hpp"
#include "HelloPkt.hpp"

class CServiceSet: public CBaseSet
{
private:
	CMap<ndUInt32, ndString>mMap;

public:
	//构造函数
	CServiceSet();
	//析构函数
	~CServiceSet();

	//接收处理数据包
	bool DealRcvPkt(CHelloPkt *pkt);

	//是否有超时的检测
	void CheckServiceSrv();
};

#endif
