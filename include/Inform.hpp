/*********************************************************
ģ����       : 
�ļ���       :Inform.hpp
����ļ�     : 
�ļ�ʵ�ֹ��� :��Ϣ��
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#ifndef __INFORM_HPP__
#define __INFORM_HPP__

#include "Packet.hpp"

//**********************************
//��Ϣ����                       
//**********************************
class CBaseInform
{
public:
	CBaseInform();
	virtual ~CBaseInform();
};

//**********************************
//Զ�̷���ڵ���Ϣ��                       
//**********************************
class CServiceInform: public CBaseInform
{
public:
	CServiceInform();
	virtual ~CServiceInform();
};

//**********************************
//�û��ڵ���Ϣ��                       
//**********************************
class SBindInform: public CBaseInform
{
public:
	 //vpn����ڵ��IP��ַ
	 ndString	sServiceIP;
	 //��id
	 ndString 	sDomainID;
	 //������
	 ndString 	sDomainMask;
	 //���key
	 ndString	sDomainKey;
	 //�����豸��������
	 ndString	sDeviceFlag;
	 //�����豸��IP��ַ
	 ndString	sDeviceIP;
	 //�����豸��Mac��ַ
	 ndString	sDeviceMac;	
public:
	SBindInform();
	virtual ~SBindInform();
};


#endif
