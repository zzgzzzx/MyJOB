//
// Created by lewis on 2017/9/3.
// FJ-FZ
//

#ifndef VPN_NODEBASE_H
#define VPN_NODEBASE_H

#include "NDType.hpp"
#include "HttpClient.hpp"

/**
 * NodeBase Class
 */
class CNodeBase
{
protected:
	//�ڵ����������Ϣ
	SNodeInform mSNodeInform;
	
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

	void SetNodeInform(SNodeInform &inform);

    //��ȡ������
    ndULong GetLastErrCode();
    //���ô�����
    void SetLastErrCode(ndULong code);
};

#endif //TVPN_CLINETBASE_H
