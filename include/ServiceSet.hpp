/*********************************************************
ģ����       : 
�ļ���       :ServiceSet.hpp
����ļ�     : 
�ļ�ʵ�ֹ��� :����ڵ㼯����
����         :zhangzx 
�汾         :1.0 
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
	//���캯��
	CServiceSet();
	//��������
	~CServiceSet();

	//���մ������ݰ�
	bool DealRcvPkt(CHelloPkt *pkt);

	//�Ƿ��г�ʱ�ļ��
	void CheckServiceSrv();
};

#endif
