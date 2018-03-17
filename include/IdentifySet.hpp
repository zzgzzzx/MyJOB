/*********************************************************
模块名       : 
文件名       :IDentifySet.hpp
相关文件     : 
文件实现功能 :用户标识集合类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#ifndef __IDENTIFY_SET_HPP__
#define __IDENTIFY_SET_HPP__

#include "Set.hpp"

class CIdentifySet: public CBaseSet
{
private:
	CMap<ndUInt32, ndString>mMap;

public:
	//构造函数
	CIdentifySet();
	//析构函数
	CIdentifySet();

	//接收处理数据包
	bool DealRcvPkt(CHelloPkt *pkt);

	//是否有超时的检测
	void CheckTimeOut();
};

#endif
