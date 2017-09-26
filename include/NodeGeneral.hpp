//
// Created by lewis on 2017/9/3.
// FJ-FZ
//

#ifndef VPN_NODE_GENERAL_H
#define VPN_NODE_GENERAL_H

#include "NodeBase.hpp"
#include "HttpGeneral.hpp"

/**
 * NodeGeneral Class
 */
class CNodeGeneral: public CNodeBase
{
protected:
	//ͨѶ�Ŀͻ���
	CHttpGeneral *mPHttpClient;
public:
    CNodeGeneral();
    virtual ~CNodeGeneral();

    //���ݴ���ǰ�ĳ�ʼ������
    virtual ndStatus InitData();

	//����ʼ���������������������Ϣ
	virtual ndStatus NodeInit();
	//���Hello
	virtual ndStatus NodeHello();
};

#endif //VPN_NODE_GENERAL_H