/*********************************************************
ģ����       : 
�ļ���       :IDentifySet.hpp
����ļ�     : 
�ļ�ʵ�ֹ��� :�û���ʶ������
����         :zhangzx 
�汾         :1.0 
**********************************************************/
#ifndef __IDENTIFY_SET_HPP__
#define __IDENTIFY_SET_HPP__

#include "Set.hpp"

class CIdentifySet: public CBaseSet
{
private:
	CMap<ndUInt32, ndString>mMap;

public:
	//���캯��
	CIdentifySet();
	//��������
	CIdentifySet();

	//���մ������ݰ�
	bool DealRcvPkt(CHelloPkt *pkt);

	//�Ƿ��г�ʱ�ļ��
	void CheckTimeOut();
};

#endif
