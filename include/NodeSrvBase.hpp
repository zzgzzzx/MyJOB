//
// Created by lewis on 2017/9/3.
// FJ-FZ
//

#ifndef VPN_CLINETSRVBASE_H
#define VPN_CLINETSRVBASE_H

#include "ndType.h"
#include "HttpClient.h"
#include "NodeBase.h"

/**
 * ClientBase Class
 */
class CNodeSrvBase: public CNodeBase
{
protected:
    //���ʵ����ķ�����IP
    ndString mCenterSrvIP;
public:
    CNodeSrvBase();
    virtual ~CNodeSrvBase();

    //���ݴ���ǰ�ĳ�ʼ������
    virtual ndStatus InitData();

	//����ʼ���������������������Ϣ
	virtual ndStatus NodeInit();
	//���Hello
	virtual ndStatus NodeHello();
};

#endif //VPN_CLINETSRVBASE_H
