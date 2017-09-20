//
// Created by lewis on 2017/9/3.
// FJ-FZ
//

#ifndef VPN_CLINETSRVBASE_H
#define VPN_CLINETSRVBASE_H

#include "NDType.hpp"
#include "HttpClient.hpp"
#include "NodeBase.hpp"

/**
 * ClientBase Class
 */
class CNodeGeneral: public CNodeBase
{
protected:
    //���ʵ����ķ�����IP
    ndString mCenterSrvIP;
public:
    CNodeGeneral();
    virtual CNodeGeneral();

    //���ݴ���ǰ�ĳ�ʼ������
    virtual ndStatus InitData();

	//����ʼ���������������������Ϣ
	virtual ndStatus NodeInit();
	//���Hello
	virtual ndStatus NodeHello();
};

#endif //VPN_CLINETSRVBASE_H
