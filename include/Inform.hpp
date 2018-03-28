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

//**********************************
//用户节点信息类                       
//**********************************
class SBindInform: public CBaseInform
{
public:
	 //vpn服务节点的IP地址
	 ndString	sServiceIP;
	 //域id
	 ndString 	sDomainID;
	 //域掩码
	 ndString 	sDomainMask;
	 //域的key
	 ndString	sDomainKey;
	 //下游设备的特征码
	 ndString	sDeviceFlag;
	 //下游设备的IP地址
	 ndString	sDeviceIP;
	 //下游设备的Mac地址
	 ndString	sDeviceMac;	
public:
	SBindInform();
	virtual ~SBindInform();
};


#endif
