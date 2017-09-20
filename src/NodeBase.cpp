/*********************************************************
ģ����       : SDK���ݴ������
�ļ���       : ClientBase.cpp
����ļ�     : ClientBase.h
�ļ�ʵ�ֹ��� : ���๦��
����         :lewis
����ʱ��     :2017-09
**********************************************************/
#include "NodeBase.hpp"

/*********************************************************
����˵�������캯��
���˵������
����˵������
����ֵ  ����
*********************************************************/
CNodeBase::CNodeBase()
{
    mErrCode = ND_SUCCESS;
    mCallBackFunc = NULL;
}

/*********************************************************
����˵������������
���˵������
����˵������
����ֵ  ����
*********************************************************/
CNodeBase::~CNodeBase()
{

}

/*********************************************************
����˵�������ýڵ������Ϣ
���˵������
����˵������
����ֵ  ����
*********************************************************/

void CNodeBase::SetNodeInform(SNodeInform &inform)
{
}


/*********************************************************
����˵������ȡ������
���˵������
����˵������
����ֵ  ����������Ϣ
*********************************************************/
ndULong CNodeBase::GetLastErrCode()
{
    return mErrCode;
}

/*********************************************************
����˵�������ô�����
���˵����code-������ֵ
����˵������
����ֵ  ����
*********************************************************/
void CNodeBase::SetLastErrCode(ndULong code)
{
    mErrCode = code;
}

/*********************************************************
����˵�������ûص�����
���˵����callbackFunction-�ص�����
����˵������
����ֵ  ����
*********************************************************/
void CNodeBase::SetAsyncMode(ndCallbackFunction callbackFunction)
{
    mCallBackFunc = callbackFunction;
}

/*********************************************************
����˵������ȡ�ص�����
���˵������
����˵������
����ֵ  ���ص�����
*********************************************************/
ndCallbackFunction CNodeBase::GetCallBackFunction()
{
    return mCallBackFunc;
}

/*********************************************************
����˵�����̴߳�����
���˵����param-�߳����ݲ���ָ��
����˵������
����ֵ  ���̷߳���ֵ
*********************************************************/
void *CNodeBase::ThreadFunction(void *param)
{
    CNodeBase *clientBase = (CNodeBase*)(param);
    if (clientBase != NULL)
    {
        TRACE("N2N run at [%s] Thread DealActionWithModel\n", __func__);
        if(clientBase->GetLastErrCode() == ND_SUCCESS){
            TRACE("N2N run at [%s] Set Last]\n", __func__);
            clientBase->SetLastErrCode(ND_SUCCESS);
        }
        ndCallbackFunction callbackFunction = clientBase->GetCallBackFunction();
        if( callbackFunction!= NULL){
            callbackFunction(clientBase);
        }
        delete clientBase;
    }
    TRACE("N2N run at [%s] Thread End\n", __func__);
    pthread_exit(0);
}
