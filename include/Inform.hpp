/*********************************************************
模块名       : 
文件名       :Inform.hpp
相关文件     : 
文件实现功能 :信息类
作者         :zhangzx 
版本         :1.0 
**********************************************************/
#ifndef __INFORM_HPP__
#define __INFORM_HPP__

#include "Packet.hpp"

//**********************************
//信息基类                       
//**********************************
class CBaseInform
{
public:
	CBaseInform();
	virtual ~CBaseInform();
};

//**********************************
//远程服务节点信息类                       
//**********************************
class CServiceInform: public CBaseInform
{
public:
	CServiceInform();
	virtual ~CServiceInform();
};

#endif
