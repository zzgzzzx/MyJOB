//
// Created by lewis on 2017/9/3.
// FJ-FZ
//

#ifndef VPN_NODEBASE_H
#define VPN_NODEBASE_H

#include "ndType.h"
#include "HttpClient.h"

/**
 * NodeBase Class
 */
class CNodeBase
{
protected:
	//vpn���ı��
	ndString mNodeID;
	//vpn����MAC��ַ
	ndString mNodeMac;
	//vpn����ip��ַ
	ndULong mNodeIP;
	//vpn�����������
	ndString mDomain;
	//vpn����������key
	ndString mDomainKey;
	//vpn��������IP��ַ
	ndString mSuperNodeIP;
	//vpn�������Ķ˿ں�
	ndString mSuperNodePort;	
	
    //���صĴ�����
    ndULong  mErrCode;

    //����ص�����
    ndCallbackFunction mCallBackFunc;

public:
    CNodeBase();
    virtual ~CNodeBase();

	//����ʼ���������������������Ϣ
	virtual ndStatus NodeInit()=0;
	//���Hello
	virtual ndStatus NodeHello()=0;
	

	//�첽�ص�������
    static void *ThreadFunction(void *param);

    //���ûص�����(����callbackFunc)
    void SetAsyncMode(ndCallbackFunction callbackFunction);
    //��ȡ�ص�
    ndCallbackFunction GetCallBackFunction();

    //��ȡ������
    ndULong GetLastErrCode();
    //���ô�����
    void SetLastErrCode(ndULong code);
};

#endif //TVPN_CLINETBASE_H
