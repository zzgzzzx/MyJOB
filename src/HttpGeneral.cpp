/*********************************************************
ģ����       : �ڵ�httpЭ��
�ļ���       : HttpClientNode.cpp
����ļ�     : HttpClientNode.h
�ļ�ʵ�ֹ��� : �ڵ�HtppЭ�鴦��
����         :lewis
����ʱ��     :2017-09
**********************************************************/
#include "HttpGeneral.hpp"

/*********************************************************
����˵�������캯��
���˵������
����˵������
����ֵ  ����
*********************************************************/
CHttpGeneral::CHttpGeneral()
{
	mPNode = NULL;
}

/*********************************************************
����˵�������캯��
���˵����Node(���)
����˵������
����ֵ  ����
*********************************************************/
CHttpGeneral::CHttpGeneral(CNodeBase *node)
{
	mPNode = node;
}

/*********************************************************
����˵������������
���˵������
����˵������
����ֵ  ����
*********************************************************/
CHttpGeneral::~CHttpGeneral()
{

}

/*********************************************************
����˵�����ڵ��ʼ��
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::MakeNodeInitReq()
{
	return ND_SUCCESS;
}

/*********************************************************
����˵�����ڵ��ʼ�����ش���
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::AnalysisNodeInitRsp()
{
	SNodeInform sNode;
	//�������ص����ݵ�NdoeInform

	mPNode->SetNodeInform(sNode);
	
	return ND_SUCCESS;	
}

/*********************************************************
����˵�����ڵ�Hello
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::MakeNodeHelloReq()
{
	return ND_SUCCESS;
}

/*********************************************************
����˵�����ڵ�Hello���ش���
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::AnalysisNodeHelloRsp()
{
	return ND_SUCCESS;
}
	

/*********************************************************
����˵��������ʼ�����״���
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::NodeInit()
{
    //��װ���ݰ�
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
����˵�������Hello����
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::NodeHello()
{
    //��װ���ݰ�
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
����˵�������ݰ����������
���˵������
����˵������
����ֵ  ����
*********************************************************/
ndStatus CHttpGeneral::PkgSendAndRecv(ndString url)
{
    //�жϷ�������ַ
    if(url.empty()){
        TRACE("N2N run at[%s] Http Server Empty Err\n", __func__);
        return ND_ERROR_INVALID_PARAM;
    }	

    //�жϷ��͵����ݰ�
    if(mSendBuf.empty()){
        TRACE("N2N run at[%s] SendBuff Is Empty\n", __func__);
        return ND_ERROR_INVALID_PARAM;
    }

    TRACE("N2N run at[%s] Begin Post Action\n ServerURL=[%s]\n Data=[%s]\n", __func__, url.c_str(),mSendBuf.c_str());
    rePost:
    //���ͷ���˲����շ���
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

    //http�������������ж�
    if (GetHttpReturnCode() != 200){
        TRACE("N2N run at[%s] Http Server Response Code=[%d]\n", __func__, GetHttpReturnCode());
        return ND_ERROR_INVALID_RESPONSE;
    }

    //�������ݵķ�������
    TRACE("N2N run at[%s] Begin Analyze Response\n Action Data=[%s]\n", __func__, mRcvBuf.c_str());
    if(mRcvBuf.empty()){
        TRACE("N2N run at[%sl] Response Action Data Err\n", __func__);
        return ND_ERROR_INVALID_RESPONSE;
    }
}

/*********************************************************
����˵��������httpͷ����Ϣ
���˵����head-ͷ����Ϣ
����˵������
����ֵ  ����
*********************************************************/
void CHttpGeneral::AnalysisAuthHeader(ndString &head)
{

}

/*********************************************************
����˵��������У��ͷ
���˵������
����˵������
����ֵ  ��У����Ϣ
*********************************************************/
ndString CHttpGeneral::GenerateAuthHeader()
{
    return NULL;
}