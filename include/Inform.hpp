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

#endif
