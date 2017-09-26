/*********************************************************
模块名       : 节点http协议
文件名       : HttpClientNode.cpp
相关文件     : HttpClientNode.h
文件实现功能 : 节点Htpp协议处理
作者         :lewis
创建时间     :2017-09
**********************************************************/
#include "HttpGeneral.hpp"

/*********************************************************
函数说明：构造函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CHttpGeneral::CHttpGeneral()
{
	mPNode = NULL;
}

/*********************************************************
函数说明：构造函数
入参说明：Node(结点)
出参说明：无
返回值  ：无
*********************************************************/
CHttpGeneral::CHttpGeneral(CNodeBase *node)
{
	mPNode = node;
}

/*********************************************************
函数说明：析构函数
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
CHttpGeneral::~CHttpGeneral()
{

}

/*********************************************************
函数说明：节点初始化
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGeneral::MakeNodeInitReq()
{
	return ND_SUCCESS;
}

/*********************************************************
函数说明：节点初始化返回处理
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGeneral::AnalysisNodeInitRsp()
{
	SNodeInform sNode;
	//解析返回的数据到NdoeInform

	mPNode->SetNodeInform(sNode);
	
	return ND_SUCCESS;	
}

/*********************************************************
函数说明：节点Hello
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGeneral::MakeNodeHelloReq()
{
	return ND_SUCCESS;
}

/*********************************************************
函数说明：节点Hello返回处理
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGeneral::AnalysisNodeHelloRsp()
{
	return ND_SUCCESS;
}
	

/*********************************************************
函数说明：结点初始化交易处理
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGeneral::NodeInit()
{
    //组装数据包
    ndStatus  ret = MakeNodeInitReq();
    if(ret != ND_SUCCESS)
	{
        TRACE("N2N run at[%s] MakeRequest Pkg Err ret=[%d]\n", __func__, ret);
        return ret;
    }

	ret = PkgSendAndRecv(VPN_CENTER_URL);
	if (ret != ND_SUCCESS)
	{
		TRACE("N2N run at[%s] PkgSendAndRecv Err ret=[%d]\n", __func__, ret);
		return ret;
	}

    ret = AnalysisNodeInitRsp();
    if(ret != ND_SUCCESS){
        TRACE("N2N run at[%s] AnalyzeResponsePkg Err ret-[%d]\n", __func__, ret);
        return ret;
    }

    return ND_SUCCESS;
	
}


/*********************************************************
函数说明：结点Hello处理
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGeneral::NodeHello()
{
    //组装数据包
    ndStatus  ret = MakeNodeHelloReq();
    if(ret != ND_SUCCESS)
	{
        TRACE("N2N run at[%s] MakeRequest Pkg Err ret=[%d]\n", __func__, ret);
        return ret;
    }

	ret = PkgSendAndRecv(VPN_CENTER_URL);
	if (ret != ND_SUCCESS)
	{
		TRACE("N2N run at[%s] PkgSendAndRecv Err ret=[%d]\n", __func__, ret);
		return ret;
	}

    ret = AnalysisNodeHelloRsp();
    if(ret != ND_SUCCESS){
        TRACE("N2N run at[%s] AnalyzeResponsePkg Err ret-[%d]\n", __func__, ret);
        return ret;
    }

    return ND_SUCCESS;
}

/*********************************************************
函数说明：数据包发送与接收
入参说明：无
出参说明：无
返回值  ：无
*********************************************************/
ndStatus CHttpGeneral::PkgSendAndRecv(ndString url)
{
    //判断服务器地址
    if(url.empty()){
        TRACE("N2N run at[%s] Http Server Empty Err\n", __func__);
        return ND_ERROR_INVALID_PARAM;
    }	

    //判断发送的数据包
    if(mSendBuf.empty()){
        TRACE("N2N run at[%s] SendBuff Is Empty\n", __func__);
        return ND_ERROR_INVALID_PARAM;
    }

    TRACE("N2N run at[%s] Begin Post Action\n ServerURL=[%s]\n Data=[%s]\n", __func__, url.c_str(),mSendBuf.c_str());
    rePost:
    //发送服务端并接收返回
    CURLcode rtn = Post(url.c_str(), mSendBuf.c_str(), mRcvBuf);

    if(rtn != CURLE_OK){
        TRACE("N2N run at[CNodeSrvBase::DealActionWithModel] Http Post Err\n");
        switch (rtn){
            case CURLE_COULDNT_CONNECT:
            case CURLE_OPERATION_TIMEDOUT:
            case CURLE_COULDNT_RESOLVE_HOST:
            case CURLE_SEND_ERROR:
                return ND_ERROR_SEND_FAILED;
            case CURLE_RECV_ERROR:
                return ND_ERROR_RECEIVE_FAILED;
            case CURLE_SSL_CACERT:
            case CURLE_SSL_CACERT_BADFILE:
                return ND_ERROR_LOAD_CA_CERTIFICATE;
            default:
                return ND_ERROR_INVALID_PARAM;
        }
    }

    //http服务器返回码判断
    if (GetHttpReturnCode() != 200){
        TRACE("N2N run at[%s] Http Server Response Code=[%d]\n", __func__, GetHttpReturnCode());
        return ND_ERROR_INVALID_RESPONSE;
    }

    //返回数据的分析处理
    TRACE("N2N run at[%s] Begin Analyze Response\n Action Data=[%s]\n", __func__, mRcvBuf.c_str());
    if(mRcvBuf.empty()){
        TRACE("N2N run at[%sl] Response Action Data Err\n", __func__);
        return ND_ERROR_INVALID_RESPONSE;
    }
}

/*********************************************************
函数说明：解析http头部信息
入参说明：head-头部信息
出参说明：无
返回值  ：无
*********************************************************/
void CHttpGeneral::AnalysisAuthHeader(ndString &head)
{

}

/*********************************************************
函数说明：生成校验头
入参说明：无
出参说明：无
返回值  ：校验信息
*********************************************************/
ndString CHttpGeneral::GenerateAuthHeader()
{
    return NULL;
}