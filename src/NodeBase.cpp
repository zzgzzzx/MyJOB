/*********************************************************
模块名       : SDK数据处理基类
文件名       : ClientBase.cpp
相关文件     : ClientBase.h
文件实现功能 : 基类功能
作者         :lewis
创建时间     :2017-09
**********************************************************/
#include "NodeBase.hpp"

/*********************************************************
函数说明：构造函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CNodeBase::CNodeBase()
{
    mErrCode = ND_SUCCESS;
    mCallBackFunc = NULL;
}

/*********************************************************
函数说明：析构函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CNodeBase::~CNodeBase()
{

}

/*********************************************************
函数说明：设置节点参数信息
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/

void CNodeBase::SetNodeInform(SNodeInform &inform)
{
}


/*********************************************************
函数说明：获取错误码
入参说明：无
出参说明：无
返回值  ：错误码信息
*********************************************************/
ndULong CNodeBase::GetLastErrCode()
{
    return mErrCode;
}

/*********************************************************
函数说明：设置错误码
入参说明：code-错误码值
出参说明：无
返回值  ：无
*********************************************************/
void CNodeBase::SetLastErrCode(ndULong code)
{
    mErrCode = code;
}

/*********************************************************
函数说明：设置回调处理
入参说明：callbackFunction-回调函数
出参说明：无
返回值  ：无
*********************************************************/
void CNodeBase::SetAsyncMode(ndCallbackFunction callbackFunction)
{
    mCallBackFunc = callbackFunction;
}

/*********************************************************
函数说明：获取回调函数
入参说明：无
出参说明：无
返回值  ：回调函数
*********************************************************/
ndCallbackFunction CNodeBase::GetCallBackFunction()
{
    return mCallBackFunc;
}

/*********************************************************
函数说明：线程处理函数
入参说明：param-线程数据参数指针
出参说明：无
返回值  ：线程返回值
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
